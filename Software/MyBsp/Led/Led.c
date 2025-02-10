/**
  *@ FileName: Led.c
  *@ Author: CzrTuringB
  *@ Brief: LED
  *@ Time:  Jan 13, 2025
  */
/* Includes" "------------------------------------------------------------------*/
#include "Led.h"
#include "LedConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Shell/Shell.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
typedef struct
{
	Bool			isPwm;				  //是否需要进行PWM计算
	GPIO_TypeDef*   ledPort;			  //LED的GPIO端口
	uint16_t 	    ledPin;				  //LED的GPIO引脚
	uint8_t   		ledBright;		      //LED灯的亮度(0~10)
}LedStruct,*pLedStruct;
//-variable
xTaskHandle LedTaskHandle;				  //LED任务句柄
xSemaphoreHandle LedMutexHandle;		  //LED资源锁句柄
xQueueHandle LedHandleQueue;			  //LED事件队列句柄
#if SoftWarePwm
xTimerHandle LedTimer;					  //LED软件定时器句柄
#endif
static LedStruct led[LedCount];			  //LED资源数组
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: BspState LedInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t index)
  *@ Author: CzrTuringB
  *@ Brief: 初始化LED结构体
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState LedInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t index)
{
	//1、初始化LED就结构体数组，并关闭LED
	led[index].ledPort = GPIOx;
	led[index].ledPin = GPIO_Pin;
	led[index].ledBright = 0;
	led[index].isPwm = False;
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, LedStateOff);
    return BspOk;
}
/**
  *@ FunctionName: BspState LedUpdate(uint8_t index, uint8_t bright)
  *@ Author: CzrTuringB
  *@ Brief: Led更新函数
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState LedUpdate(uint8_t index, uint8_t bright)
{
	//1、形参有效性判断
	if(index >= LedCount || bright > 10)
		return BspError;
    if (xSemaphoreTake(LedMutexHandle, portMAX_DELAY) == pdPASS)
    {
    	//2、更新结构体数组参数
    	led[index].ledBright = bright;
    	//3、亮度为0或10，直接打开关闭即可不用进行定时器计算
        if (bright == 0)
        {
        	led[index].isPwm = False;
            HAL_GPIO_WritePin(led[index].ledPort, led[index].ledPin, LedStateOff);
        }
        else if (bright == 10)
        {
        	led[index].isPwm = False;
            HAL_GPIO_WritePin(led[index].ledPort, led[index].ledPin, LedStateOn);
        }
        else
        {
        	led[index].isPwm = True;
        }
        xSemaphoreGive(LedMutexHandle);
        return BspOk;
    }
	return BspError;
}
/**
  *@ FunctionName: BspState LedPost(uint8_t index, uint8_t bright)
  *@ Author: CzrTuringB
  *@ Brief: 向LED任务投递一个信号，以更新LED的状态
  *@ Time: Jan 20, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、只适用于任务，不适用于中断
  */
BspState LedPost(uint8_t index, uint8_t bright)
{
	//1、形参有效性判断
	if(index >= LedCount || bright > 10)
		return BspError;
	//2、准备队列数据
	LedHandle led;
	led.index = index;
	led.bright = bright;
	//3、发送队列
	BaseType_t result = xQueueSend(LedHandleQueue, &led, portMAX_DELAY);
	//4、返回结果
    // 4、返回结果
    if (result != pdPASS)
        return BspError;
    else
        return BspOk;
}
/**
  *@ FunctionName: BspState LedPostFromISR(uint8_t index, uint8_t bright,BaseType_t xHigherPriorityTaskWoken)
  *@ Author: CzrTuringB
  *@ Brief: 在中断中向LED任务投递一个信号以更新LED状态
  *@ Time: Jan 20, 2025
  *@ Requirement：
  *@ Notes:
  *@     1、只适用于中断上下文
  *@     2、确保队列大小足够，避免数据丢失
  */
BspState LedPostFromISR(uint8_t index, uint8_t bright,BaseType_t xHigherPriorityTaskWoken)
{
    // 1、形参有效性判断
    if (index >= LedCount || bright > 10)
        return BspError;
    // 2、准备队列数据
    LedHandle led;
    led.index = index;
    led.bright = bright;
    // 3、发送队列（使用 FromISR 版本）
    BaseType_t result = xQueueSendFromISR(LedHandleQueue, &led, &xHigherPriorityTaskWoken);
    // 4、返回结果
    if (result != pdPASS)
        return BspError;
    else
        return BspOk;
}
/**
  *@ FunctionName: void LedTimerCallback(TimerHandle_t xTimer)
  *@ Author: CzrTuringB
  *@ Brief: LED亮度控制的软件定时器回调函数
  *@ Time: Jan 22, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、软件定时器的函数是在守护者进程任务中调用的，需要注意资源保护
  */
#if SoftWarePwm
void LedTimerCallback(TimerHandle_t xTimer)
{
	static uint8_t ledCounter = 0;
    ledCounter++;
    if (xSemaphoreTake(LedMutexHandle, portMAX_DELAY) == pdPASS)
    {
    	for (uint8_t i = 0; i < LedCount; i++)
    	{
    		if(led[i].isPwm == True)
    		{
    			uint16_t period = (LedResolution / 10) * led[i].ledBright;
    			if(ledCounter == period)
    			{
    				HAL_GPIO_WritePin(led[i].ledPort, led[i].ledPin, LedStateOff);
    			}
    		}
    	}
    	if(ledCounter == 20)
    	{
    		ledCounter = 0;
    		for (uint8_t i = 0; i < LedCount; i++)
    		{
    			if(led[i].isPwm == True)
    			{
    				HAL_GPIO_WritePin(led[i].ledPort, led[i].ledPin, LedStateOn);
    			}
    		}
    	}
    }
    xSemaphoreGive(LedMutexHandle);
}
#endif
/**
  *@ FunctionName: void LedApp(void)
  *@ Author: CzrTuringB
  *@ Brief: LED的FreeRTOS任务函数
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
void LedApp(void)
{
	//1、初始化LED
	LedInit(GPIOG, GPIO_PIN_2, 0);		//绿色LED
	LedInit(GPIOB, GPIO_PIN_5, 1);		//蓝色LED
	LedInit(GPIOA, GPIO_PIN_14,2);		//蓝色LED
    //2、创建互斥锁
    LedMutexHandle = xSemaphoreCreateMutex();
	//3、创建一个LED事件队列
	LedHandleQueue = xQueueCreate(LedQueueSize,sizeof(LedHandle));
	LedHandle handle;
	//4、初始化定时器
#if SoftWarePwm
	LedTimer = xTimerCreate("LED", pdMS_TO_TICKS(1), pdTRUE, NULL, LedTimerCallback);
    if (LedTimer != NULL)	xTimerStart(LedTimer, 0);
#endif
	//5、接收LED事件消息并进行处理
	while(1)
	{
		if(xQueueReceive(LedHandleQueue, &handle, portMAX_DELAY) == pdPASS)
		{
			LedUpdate(handle.index, handle.bright);
		}
	}
}

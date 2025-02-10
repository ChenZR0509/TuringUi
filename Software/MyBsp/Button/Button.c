/**
  *@ FileName: Button.c
  *@ Author: CzrTuringB
  *@ Brief: 按键
  *@ Time:  Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
/* Includes" "------------------------------------------------------------------*/
#include "Button.h"
#include "ButtonConfig.h"
#include "Led/Led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
typedef enum						//按键状态定义
{
    Idle,   						//空闲
    PreeDetected,  					//按键按下
    ReleaseDetected, 				//按键释放
    ShortPress,     				//短按
    LongPress,     					//长按
	LongPressEnd, 					//长按结束
	DoublePress,    				//双击
	Wait,							//等待
}ButtonState;
//-struct
typedef struct
{
	GPIO_TypeDef* buttonPort;		//按键的GPIO端口
	uint16_t buttonPin;				//按键的GPIO引脚
	ButtonHandle handle;			//按键的事件处理函数
	ButtonState state;				//按键的状态
	TickType_t pressStartTime;      //记录按键按下时间
	TickType_t releaseTime;			//记录按键释放时间
}ButtonStruct,*pButtonStruct;
//-variable
//如果是多个按键更改数组大小就行
static ButtonStruct button[ButtonCount];
ButtonHandle* buttonHandle[ButtonCount];
xQueueHandle buttonQueue;
xTaskHandle ButtonTaskHandle;
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
#if buttonTest
void ClickTest(void)
{
	//单击功能实现函数
	LedPost(0, 10);
}
void DoubleClickTest(void)
{
	//双击功能实现函数
	LedPost(0, 0);
}
void LongClickTest(void)
{
	//长按功能实现函数：单双击功能切换
	ButtonDispatch dispatch;
	dispatch.id = 0;
	dispatch.type = Click;
	dispatch.functionPointer = DoubleClickTest;
	xQueueSendToBack(buttonQueue,&dispatch,0);
	dispatch.type = DoubleClick;
	dispatch.functionPointer = ClickTest;
	xQueueSendToBack(buttonQueue,&dispatch,0);
}
#endif
/**
  *@ FunctionName: BspState ButtonInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
  *@ Author: CzrTuringB
  *@ Brief: 初始化按键结构体
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ButtonInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t index)
{
	button[index].buttonPort = GPIOx;
	button[index].buttonPin = GPIO_Pin;
	button[index].handle.ClickHandle = NULL;
	button[index].handle.DoubleClickHandle = NULL;
	button[index].handle.LongClickHandle = NULL;
	button[index].pressStartTime = 0;
	button[index].releaseTime = 0;
	button[index].state = Idle;
	return BspOk;
}
/**
  *@ FunctionName: BspState ButtonHandleInit(void)
  *@ Author: CzrTuringB
  *@ Brief: 按键处理函数初始化
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ButtonHandleInit(void)
{
	//1、注册按键与按键处理函数的对应关系
	for(uint8_t i=0; i<ButtonCount; i++)
	{
		buttonHandle[i] = &(button->handle);
	}
	//2、创建分发事件队列
	buttonQueue = xQueueCreate(QueueLength,sizeof(ButtonDispatch));
	//3、按键功能测试
	#if buttonTest
	button[0].handle.ClickHandle = ClickTest;
	button[0].handle.DoubleClickHandle = DoubleClickTest;
	button[0].handle.LongClickHandle = LongClickTest;
	#endif
	//4、返回函数运行状态信息
	if(buttonQueue == NULL)
		return BspError;
	else
		return BspOk;
}
/**
  *@ FunctionName: void ButtonTask(uint8_t index)
  *@ Author: CzrTuringB
  *@ Brief: 读取按键状态
  *@ Time: Sep 18, 2024
  *@ Requirement：
  *@ Notes：
  */
void ButtonTask(uint8_t index)
{
	//1、读取按键状态
	GPIO_PinState ButtonStatus = HAL_GPIO_ReadPin(button[index].buttonPort, button[index].buttonPin);
	//2、获取当前系统运行时间
	TickType_t currentTime = xTaskGetTickCount();
	//3、根据按键状态进行判断
	switch (button[index].state)
	{
		//上次状态为空闲
		case Idle:
			//检测到按键按下
			if (ButtonStatus == ButtonPress)
			{
				//设置状态为按键按下
				button[index].state = PreeDetected;
				//记录按键按下时间
				button[index].pressStartTime = currentTime;	//记录按下时间
			}
			break;

		//上次状态为按键按下
		case PreeDetected:
			//检测到按键松开
			if (ButtonStatus == ButtonIdle)
			{
				//设置按键状态为按键释放
				button[index].state = ReleaseDetected;
				//记录按键释放时间
				button[index].releaseTime = currentTime;
			}
			//检测到按键按下且按下时间超过长按时间阈值
			else if (currentTime - button[index].pressStartTime > LongPressTime)
			{
				//设置按键状态为长按
				button[index].state = LongPress;
			}
			break;

		case LongPress:
			if (ButtonStatus == ButtonIdle)
			{
				button[index].state = LongPressEnd;
			}
			break;

		//上次状态为释放
		case ReleaseDetected:
			//如果按键再次按下且两次按下时间间隔小于双击阈值
			if ((ButtonStatus == ButtonPress) && (currentTime - button[index].releaseTime < ShortClickTime))
			{
				//设置状态为双击
				button[index].state = DoublePress;
			}
			//判断按键按下时长防止毛刺 在判断当前是否在双击范围内
			else if ((button[index].releaseTime - button[index].pressStartTime > PressTime) && (currentTime - button[index].releaseTime > ShortClickTime))
			{
				button[index].state = ShortPress;
			}
			break;

		//单击情况下应用代码：
		case ShortPress:
			//功能代码开始
			if(button[index].handle.ClickHandle)	button[index].handle.ClickHandle();
			//功能代码结束
			button[index].state = Wait;
			break;

		//双击情况下应用代码：
		case DoublePress:
			//功能代码开始
			if(button[index].handle.DoubleClickHandle)	button[index].handle.DoubleClickHandle();
			//功能代码结束
			button[index].state = Wait;
			break;

		//长按情况下应用代码：
		case LongPressEnd:
			//功能代码开始
			if(button[index].handle.LongClickHandle)	button[index].handle.LongClickHandle();
			//功能代码结束
			//执行完代码后设置为长按结束
			button[index].state = Wait;
			break;

		//防止按键检测一直触发
		case Wait:
			if(ButtonStatus == ButtonIdle)
			{
				button[index].state = Idle;
			}
			break;
	}
}
/**
  *@ FunctionName: void ButtonDispatchFunction(void)
  *@ Author: CzrTuringB
  *@ Brief: 给按键分发处理函数
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
void ButtonDispatchFunction(void)
{
	ButtonDispatch dispatch;
	//从按键事件队列中提取消息并根据消息注册功能实现函数
	if(xQueueReceive(buttonQueue, &dispatch, 10) == pdPASS)
	{
		switch(dispatch.type)
		{
			case Click:
				button[dispatch.id].handle.ClickHandle = dispatch.functionPointer;
				break;
			case DoubleClick:
				button[dispatch.id].handle.DoubleClickHandle = dispatch.functionPointer;
				break;
			case LongClick:
				button[dispatch.id].handle.LongClickHandle = dispatch.functionPointer;
				break;
            default:
                break;
		}
	}
}
/**
  *@ FunctionName: BspState ButtonPostFunction(uint8_t	id, ButtonHandleType type, void* functionPointer)
  *@ Author: CzrTuringB
  *@ Brief: 给按键分发处理函数
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ButtonPostFunction(uint8_t	id, ButtonHandleType type, void* functionPointer)
{
    //1、形参有效性判断
    if (id >= ButtonCount || functionPointer == NULL || (uint8_t)type > 2)
    {
    	return BspError;
    }
	//2、创建消息结构体
	ButtonDispatch dispatch;
	dispatch.id = id;
	dispatch.type = type;
	dispatch.functionPointer = functionPointer;
	//3、发送消息
	BaseType_t result = xQueueSend(buttonQueue, &dispatch, portMAX_DELAY);
    //4、返回结果
    if (result != pdPASS)
        return BspError;
    else
        return BspOk;
}
/**
  *@ FunctionName: BspState ButtonPostFunctionFromISR(uint8_t id, ButtonHandleType type, void* functionPointer, BaseType_t xHigherPriorityTaskWoken)
  *@ Author: CzrTuringB
  *@ Brief: 给按键分发处理函数
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ButtonPostFunctionFromISR(uint8_t id, ButtonHandleType type, void* functionPointer, BaseType_t xHigherPriorityTaskWoken)
{
    //1、形参有效性判断
    if (id >= ButtonCount || functionPointer == NULL || (uint8_t)type > 2)
    {
    	return BspError;
    }
	//2、创建消息结构体
	ButtonDispatch dispatch;
	dispatch.id = id;
	dispatch.type = type;
	dispatch.functionPointer = functionPointer;
	//3、发送消息
	BaseType_t result = xQueueSendFromISR(buttonQueue, &dispatch, &xHigherPriorityTaskWoken);
    //4、返回结果
    if (result != pdPASS)
        return BspError;
    else
        return BspOk;
}
/**
  *@ FunctionName: void ButtonApp(void)
  *@ Author: CzrTuringB
  *@ Brief: 按键任务的实现
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
void ButtonApp(void)
{
	//1、初始化按键
	ButtonInit(GPIOB, GPIO_PIN_13, 0);
	ButtonInit(GPIOH, GPIO_PIN_7, 1);
	//2、初始化按键处理器
	ButtonHandleInit();
	while (1)
	{
		//1、其他任务是否更改按键的执行功能
		ButtonDispatchFunction();
		//2、检测按键是否按下并执行相应的功能
		for(uint8_t i=0;i<ButtonCount;i++)
		{
			ButtonTask(i);
		}
	}
}

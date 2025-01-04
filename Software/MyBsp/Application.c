/**
  *@ FileName: Application.c
  *@ Author: CzrTuringB
  *@ Brief: 实时操作系统的应用文件
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "Application.h"
#include "Oled/TuringUi/Application/UiInit.h"
#include "Oled/TuringUi/Basic/UiPlot.h"
/* Data ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/
void MyRtosInit(void)
{
	//创建LED应用任务
	xTaskHandle LedTaskHandle;
	xTaskCreate((void*)LedApp, "LED", 128 * 4, NULL, 25, &LedTaskHandle);
	//创建Button应用任务
	xTaskHandle ButtonTaskHandle;
	xTaskCreate((void*)ButtonApp, "Button", 128 * 4, NULL, 25 , &ButtonTaskHandle);
	//创建Oled应用任务
	xTaskHandle OledTaskHandle;
	xTaskCreate((void*)OledApp, "Oled", 256 * 4, NULL, 24, &OledTaskHandle);
	//创建Debug应用任务
	xTaskHandle DebugTaskHandle;
	xTaskCreate((void*)DebugApp, "Debug", 128 * 4, NULL, 24, &DebugTaskHandle);
}

void LedApp(void)
{
	LedEvent = xEventGroupCreate();
	EventBits_t EventFlag = 0x00;
	while(1)
	{
		//任务进入阻塞状态等待事件标志位发生变化
		xEventGroupWaitBits(LedEvent,0xFFFFFF,pdFALSE,pdFALSE,pdMS_TO_TICKS(10));
		EventFlag = xEventGroupGetBits(LedEvent);
		switch(EventFlag)
		{
			case LedOnEvent:
				LedOn(LedPort, LedPin);
				xEventGroupClearBits(LedEvent, LedOnEvent);
				break;
			case LedOffEvent:
				LedOff(LedPort, LedPin);
				xEventGroupClearBits(LedEvent, LedOffEvent);
				break;
			case LedToggleEvent:
				LedToggle(LedPort, LedPin);
				xEventGroupClearBits(LedEvent, LedToggleEvent);
				break;
		}
	}
}

void ButtonApp(void)
{
	uint16_t SHORT_CLICK_THRESHOLD = 500;  // 这个是第一次松开时间和第二次按下时间的判断时长
	uint16_t LONG_PRESS_THRESHOLD = 1000;   // 定义长按的时间阈值（以FreeRTOS时基为单位）
	uint16_t PRESS_Time = 100;             // 判断毛刺时长

	ButtonState State = IDLE;  //按键状态
	TickType_t pressStartTime = 0;        //记录按下时间
	TickType_t ReleaseTime = 0;

	while (1)
	{
		//读取按键状态
		GPIO_PinState ButtonStatus = ButtonRead(ButtonPort, ButtonPin);
		//获取当前系统运行时间
		TickType_t currentTime = xTaskGetTickCount();
		//根据按键状态进行判断
		switch (State)
		{
			//上次状态为空闲
			case IDLE:
				//检测到按键按下
				if (ButtonStatus == ButtonPress)
				{
					//设置状态为按键按下
					State = PRESS_DETECTED;
					//记录按键按下时间
					pressStartTime = currentTime;	//记录按下时间
				}
				break;

			//上次状态为按键按下
			case PRESS_DETECTED:
				//检测到按键松开
				if (ButtonStatus == ButtonIdle)
				{
					//设置按键状态为按键释放
					State = RELEASE_DETECTED;
					//记录按键释放时间
					ReleaseTime = currentTime;
				}
				//检测到按键按下且按下时间超过长按时间阈值
				else if (currentTime - pressStartTime > LONG_PRESS_THRESHOLD)
				{
					//设置按键状态为长按
					State = LONG_PRESS;
				}
				break;

			case LONG_PRESS:
				if (ButtonStatus == ButtonIdle)
				{
					State = LONG_PRESS_END;
				}
				break;

			//上次状态为释放
			case RELEASE_DETECTED:
				//如果按键再次按下且两次按下时间间隔小于双击阈值
				if ((ButtonStatus == ButtonPress) && (currentTime - ReleaseTime < SHORT_CLICK_THRESHOLD))
				{
					//设置状态为双击
					State = DOUBLE_PRESS;
				}
				//判断按键按下时长防止毛刺 在判断当前是否在双击范围内
				else if ((ReleaseTime - pressStartTime > PRESS_Time) && (currentTime - ReleaseTime > SHORT_CLICK_THRESHOLD))
				{
					State = SHORT_PRESS;
				}
				break;

			//单击情况下应用代码：
			case SHORT_PRESS:
				//功能代码开始
				xEventGroupSetBits(LedEvent, LedOnEvent);
				//功能代码结束
				State = WAIT;
				break;

			//双击情况下应用代码：
			case DOUBLE_PRESS:
				//功能代码开始
				xEventGroupSetBits(LedEvent, LedOffEvent);
				//功能代码结束
				State = WAIT;
				break;

			//长按情况下应用代码：
			case LONG_PRESS_END:
				//功能代码开始
				xEventGroupSetBits(LedEvent, LedToggleEvent);
				//功能代码结束
				//执行完代码后设置为长按结束
				State = WAIT;
				break;

			//防止按键检测一直触发
			case WAIT:
				if(ButtonStatus == ButtonIdle)
				{
					State = IDLE;
				}
				break;
		}
		vTaskDelay(10);
	}
}

void OledApp(void)
{
	//TuringUi初始化
	UiInit();
	while(1)
	{
		//UI事件管理器：依据事件发生与否，执行相应的代码
		//UI页面管理器：如果切换页面，则初始化新的页面
		//UI界面渲染器：依据UI界面上的UI元素，进行绘制
	}
}

void DebugApp(void)
{
	uint8_t Cmd[20] = {};
	//开启Debug的输出和接收
	DebugInit();
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(5000));
		if(DebugScanf(Cmd) != BspError)
		{
			if (strncmp("left", (char*)Cmd,2) == 0)
			{
				xEventGroupSetBits(LedEvent, LedOnEvent);
			}
			if (strncmp("right", (char*)Cmd,4) == 0)
			{
				xEventGroupSetBits(LedEvent, LedOffEvent);
			}
			if (strncmp("click", (char*)Cmd,5) == 0)
			{
				xEventGroupSetBits(LedEvent, LedToggleEvent);
			}
		}
	}
}

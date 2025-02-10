/**
  *@ FileName：UiEvent.c
  *@ Author：CzrTuringB
  *@ Brief：Ui事件管理器
  *@ Time：Jan 15, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "UiEvent.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
#define EventQueueSize 5
//-enum
//-struct
//-variable
QueueHandle_t eventQueue;
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: BspState PostUiEvent(pUiEvent event)
  *@ Author: CzrTuringB
  *@ Brief: 事件的发送
  *@ Time: Jan 10, 2025
  *@ Requirement：
  *@ Notes:S
  */
BspState PostUiEvent(pUiEvent event)
{
	BaseType_t result = xQueueSend(eventQueue, event, portMAX_DELAY);
	return result == pdPASS ? BspOk : BspError;
}
/**
  *@ FunctionName: void UiEventHandle(void)
  *@ Author: CzrTuringB
  *@ Brief: TuringUi的事件管理任务函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiEventHandle(void)
{
	//1、创建事件队列
	eventQueue = xQueueCreate(EventQueueSize, sizeof(UiEvent));
	//2、创建一个Ui事件用于接收队列的数据
	UiEvent event;
	while(1)
	{
		//接收其他任务发送的事件
        if (xQueueReceive(eventQueue, &event, portMAX_DELAY) == pdTRUE)
        {
        	//执行页面的事件处理函数
        	uiPage->handleEvent(&event);
        	//每执行完一个事件，释放事件数据
        	vPortFree(event.data);
        }
	}
}

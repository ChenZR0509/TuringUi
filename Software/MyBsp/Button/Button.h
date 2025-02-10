/**
  *@ FileName: Button.h
  *@ Author: CzrTuringB
  *@ Brief: 按键
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
#ifndef __BUTTON_H
#define __BUTTON_H
/* Includes" "------------------------------------------------------------------*/
#include <BspCore.h>
#include "gpio.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
#define ButtonCount 2										//按键数量(移植更改项)
//-enum
typedef enum
{
    Click,
	DoubleClick,
	LongClick,
}ButtonHandleType;											//按键事件类型
//-typedef(类型重命名)
typedef void (*ButtonClick)(void);
typedef void (*ButtonDoubleClick)(void);
typedef void (*ButtonLongClick)(void);
//-struct
typedef struct
{
	ButtonClick ClickHandle;  	 							//单击处理函数
	ButtonDoubleClick DoubleClickHandle;  	 				//双击处理函数
	ButtonLongClick LongClickHandle;  	 					//长按处理函数
}ButtonHandle;
typedef struct
{
	uint8_t	id;												//指明按键
	ButtonHandleType type;									//替换类型
	void* functionPointer;									//处理函数指针
}ButtonDispatch;
//-variable
extern xQueueHandle buttonQueue;
extern xTaskHandle ButtonTaskHandle;
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
void ButtonApp(void);
#endif

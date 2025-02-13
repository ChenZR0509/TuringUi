/**
  *@ FileName: Led.h
  *@ Author: CzrTuringB
  *@ Brief: LED
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
#ifndef __LED_H
#define __LED_H

/* C Style Start------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes" "------------------------------------------------------------------*/
#include "gpio.h"
#include <BspCore.h>
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
typedef struct LedHandle				 //LED控制信息结构体
{
	uint8_t 		index;				 //LED灯的索引
	uint8_t   		bright;				 //LED灯的亮度(0~10)
}LedHandle,*pLedHandle;
//-variable
extern xTaskHandle LedTaskHandle;
extern xQueueHandle LedHandleQueue;
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
BspState LedPost(uint8_t index, uint8_t bright);
BspState LedPostFromISR(uint8_t index, uint8_t bright,BaseType_t xHigherPriorityTaskWoken);
void LedApp(void);
#ifdef __cplusplus
}
#endif
/* C Style End------------------------------------------------------------------*/
#endif

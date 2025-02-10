/**
  *@ FileName：ShellUsart.h
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 23, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __SHELLUART_H
#define __SHELLUART_H
/* C Style Start------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes" "------------------------------------------------------------------*/
#include "usart.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
//-variable
extern xQueueHandle UartOutputQueue;		//串口消息发送队列
extern xQueueHandle UartInputQueue;			//消息串口接收队列
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
void ShellUartApp(void);
#ifdef __cplusplus
}
#endif
/* C Style End------------------------------------------------------------------*/
#endif

/**
  *@ FileName：Oled.h
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 14, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __OLED_H
#define __OLED_H
/* Includes" "------------------------------------------------------------------*/
#include "TuringUi/Basic/UiCore.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
//-variable
extern xTaskHandle OledTaskHandle;
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
void OledApp(void);
#endif

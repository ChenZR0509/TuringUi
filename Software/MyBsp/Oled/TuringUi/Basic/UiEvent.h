/**
  *@ FileName：UiEvent.h
  *@ Author：CzrTuringB
  *@ Brief：Ui事件管理器
  *@ Time：Jan 15, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __UIEVENT_H
#define __UIEVENT_H
/* Includes" "------------------------------------------------------------------*/
#include "UiCore.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
BspState PostUiEvent(pUiEvent event);
void UiEventHandle(void);
#endif

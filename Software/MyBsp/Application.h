/**
  *@ FileName: Application.c
  *@ Author: CzrTuringB
  *@ Brief: 实时操作系统的应用文件
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
#ifndef __APPLICATION_H
#define __APPLICATION_H
/* Includes ------------------------------------------------------------------*/

//FreeRtos
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "event_groups.h"
#include "task.h"
#include "string.h"

//Peripheral
#include "Led/Led.h"
#include "Button/Button.h"
#include "CH340/CH340.h"
/* Data ------------------------------------------------------------------*/

/* Functions------------------------------------------------------------------*/
/**
  *@ FunctionName: void MyRtosInit(void);
  *@ Author: CzrTuringB
  *@ Brief: 实时操作系统的初始化函数
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
void MyRtosInit(void);
/**
  *@ FunctionName: void LedApp(void)
  *@ Author: CzrTuringB
  *@ Brief: LED应用任务函数
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
void LedApp(void);
/**
  *@ FunctionName: void ButtonApp(void)
  *@ Author: CzrTuringB
  *@ Brief: Button应用任务函数
  *@ 	1、实现了单个按键的单击、双击、长按等功能；
  *@ 	2、按键功能触发通过事件组传递出去；
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
void ButtonApp(void);
/**
  *@ FunctionName: void OledApp(void)
  *@ Author: CzrTuringB
  *@ Brief: Oled的应用任务
  *@ Time: Sep 29, 2024
  *@ Requirement：
  */
void OledApp(void);
/**
  *@ FunctionName: void DebugApp(void)
  *@ Author: CzrTuringB
  *@ Brief: Debug应用任务函数，用于接收上位机的指令
  *@ Time: Sep 21, 2024
  *@ Requirement：
  */
void DebugApp(void);
#endif

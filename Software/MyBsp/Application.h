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
#include "semphr.h"
#include "event_groups.h"
#include "task.h"
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
#endif

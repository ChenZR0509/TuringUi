/**
  *@ FileName：BspCore.h
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 23, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __BSPCORE_H
#define __BSPCORE_H
/* C Style Start------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes" "------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* Includes< >------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
#define haveIdleApi	0
//-enum
//状态枚举值
typedef enum
{
	BspError = 0,
	BspOk	 = 1,
}BspState;
typedef enum
{
	Enable = 0,
 	Disable = 1,
}EnDis;
typedef enum
{
 	False = 0,
 	True = 1,
}Bool;
//-typedef(类型重命名)
//-数据结构节点初始化函数
typedef void (*InitFunc)(void* data, void* params);
//-struct
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
//-弱函数
void ShellUartTxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken);
#if haveIdleApi
void ShellUartRxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken);
#else
void ShellUartRxCallback(void *huart);
#endif
#ifdef __cplusplus
}
#endif
/* C Style End------------------------------------------------------------------*/
#endif

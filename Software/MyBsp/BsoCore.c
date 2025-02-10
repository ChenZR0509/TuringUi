/**
  *@ FileName：BspCore.c
  *@ Author：CzrTuringB
  *@ Brief：板级支持包的内核文件
  *@ Time：Jan 23, 2025
  *@ Requirement：
  *@ Notes：
  *@ 	1、实现各种外设功能实现的中断服务回调函数
  */
/* Includes" "------------------------------------------------------------------*/
#include "BspCore.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: void ShellUartTxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken)
  *@ Author: CzrTuringB
  *@ Brief: ShellUart的中断回调函数
  *@ Time: Jan 23, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、弱符号函数，在使用时重写这个函数即可
  */
__attribute__((weak)) void ShellUartTxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken)
{
	(void)huart;
}
#if haveIdleApi
/**
  *@ FunctionName: void ShellUartRxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken)
  *@ Author: CzrTuringB
  *@ Brief: ShellUart的中断回调函数
  *@ Time: Jan 23, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、弱符号函数，在使用时重写这个函数即可
  */
__attribute__((weak)) void ShellUartRxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken)
{
	(void)huart;
}
#else
/**
  *@ FunctionName: void ShellUartRxCallback(void *huart)
  *@ Author: CzrTuringB
  *@ Brief: ShellUart的中断回调函数
  *@ Time: Jan 23, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、弱符号函数，在使用时重写这个函数即可
  */
__attribute__((weak)) void ShellUartRxCallback(void *huart)
{
	(void)huart;
}
#endif

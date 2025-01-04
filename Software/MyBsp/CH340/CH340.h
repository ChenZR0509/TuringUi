/**
  *@ FileName: CH340.c
  *@ Author: CzrTuringB
  *@ Brief: 用于实现USB转串口通信
  *@ Time: Sep 21, 2024
  *@ Requirement：
  */
#ifndef __CH340_H
#define __CH340_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "../DataStructure/Queue.h"
/* Data ------------------------------------------------------------------*/
//设置通信串口
#define UartDebugHandle huart1
//选择传输方式
//#define UART_DEBUG_TRANSMIT()  HAL_UART_Transmit(&UartDebugHandle, OutputBuff, OutputBuffIndex, 0xFFFF)
#define UART_DEBUG_TRANSMIT()  HAL_UART_Transmit_DMA(&UartDebugHandle, OutputBuff, OutputBuffIndex)
//#define UART_DEBUG_TRANSMIT()  HAL_UART_Transmit_IT(&UartDebugHandle, OutputBuff, OutputBuffIndex)
/* Functions------------------------------------------------------------------*/
/**
  *@ FunctionName: void DebugInit(void)
  *@ Author: CzrTuringB
  *@ Brief:	初始化Debug
  *@ Time: Sep 21, 2024
  *@ Requirement：
  */
void DebugInit(void);
/**
  *@ FunctionName: DebugPrintf(const char *format, ...)
  *@ Author: CzrTuringB
  *@ Brief:	可以像使用Printf函数那样使用这个函数
  *@ Time: Sep 21, 2024
  *@ Requirement：
  */
void DebugPrintf(const char *format, ...);
/**
  *@ FunctionName: BspState DebugScanfOne(uint8_t* ch)
  *@ Author: CzrTuringB
  *@ Brief: 从输入缓冲区中读取出一个字节的数据
  *@ Time: Sep 23, 2024
  *@ Requirement：
  */
BspState DebugScanfOne(uint8_t* ch);
/**
  *@ FunctionName: BspState DebugScanf(uint8_t* str)
  *@ Author: CzrTuringB
  *@ Brief: 从输入缓冲区中读取上位机输入的完整数据
  *@ Time: Sep 23, 2024
  *@ Requirement：
  */
BspState DebugScanf(uint8_t* str);
/**
  *@ FunctionName: void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
  *@ Author: CzrTuringB
  *@ Brief:	重写的IDLE中断回调函数
  *@ Time: Sep 21, 2024
  *@ Requirement：
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
#endif

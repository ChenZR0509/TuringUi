/**
  *@ FileName：ShellUsart.c
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 23, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "../Shell.h"
#include "ShellUart.h"
#include "queue.h"
/* Includes< >------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
#define UartShellHandle huart4					//通信串口设置
#define UartShellDmaHandle hdma_uart4_rx		//串口DMA设置
#define InputBufferSize  20						//接收缓冲数组大小
#define OutputBufferSize MaxMessageSize			//发送缓冲数组大小(要与最小消息字符数一样)
#define OutputQueueSize  10						//消息队列大小
//-enum
//-struct
//-variable
extern DMA_HandleTypeDef hdma_uart4_rx;
static uint8_t InputBuffer[InputBufferSize];	//接收缓冲数组
static uint8_t OutputBuffer[OutputBufferSize];	//发送缓冲数组
static SemaphoreHandle_t DmaIdle = NULL;		//DMA锁
xQueueHandle UartInputQueue = NULL;				//串口消息接收队列
xQueueHandle UartOutputQueue = NULL;			//串口消息发送队列
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: BspState ShellUsartInit(void)
  *@ Author: CzrTuringB
  *@ Brief: 初始化串口传输
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ShellUartInit(void)
{
	//1、创建串口消息发送队列
	UartOutputQueue = xQueueCreate(OutputQueueSize,sizeof(ShellMessage));
	if(UartOutputQueue == NULL) return BspError;
	//2、创建DMA空闲信号量，并发送以表明当前DMA处于空闲状态
	DmaIdle = xSemaphoreCreateBinary();
	if (DmaIdle == NULL) return BspError;
	xSemaphoreGive(DmaIdle);
	//3、开启串口DMA+IDLE接收
#if	haveIdleApi
	HAL_UARTEx_ReceiveToIdle_DMA(&UartShellHandle, InputBuffer, InputBufferSize);
#else
	HAL_UART_Receive_DMA(&UartShellHandle, InputBuffer, InputBufferSize);
	__HAL_UART_CLEAR_IDLEFLAG(&UartShellHandle);
	__HAL_UART_ENABLE_IT(&UartShellHandle, UART_IT_IDLE);
#endif
	return BspOk;
}
/**
  *@ FunctionName: BspState ShellUsartPrintf(const uint8_t *pData, uint16_t Size)
  *@ Author: CzrTuringB
  *@ Brief: 串口发送数据
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ShellUartPrintf(const uint8_t *pData, uint16_t Size)
{
	if(xSemaphoreTake(DmaIdle,portMAX_DELAY) == pdPASS)
	{
		memcpy(OutputBuffer, pData, Size);
		while((&UartShellHandle)->gState != HAL_UART_STATE_READY);
		HAL_UART_Transmit_DMA(&UartShellHandle, OutputBuffer, Size);
	}
	return BspOk;
}
/**
  *@ FunctionName: void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
  *@ Author: CzrTuringB
  *@ Brief: Uart的发送DMA回调函数
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ 	1、重写的BspCore里面的弱函数
  *@ Notes:
  *@ 	1、在执行这个回调函数的时候会发送一个信号量来表明此时的DMA发送通道是空闲的，可以让其继续发送数据
  */
void ShellUartTxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken)
{
	if((UART_HandleTypeDef*) huart == &UartShellHandle)
	{
		xSemaphoreGiveFromISR(DmaIdle, xHigherPriorityTaskWoken);
	}
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
void ShellUartRxCallback(void *huart, BaseType_t* xHigherPriorityTaskWoken)
{
	if((UART_HandleTypeDef*) huart == &UartShellHandle)
	{
		if(huart->RxEventType != HAL_UART_RXEVENT_HT)
		{
			uint8_t nullChar = '\0';
			//禁用 DMA 接收
			HAL_UART_DMAStop(&UartShellHandle);
			//清除 DMA 相关标志
			__HAL_UART_CLEAR_IDLEFLAG(&UartShellHandle);
			//发生IDLE中断后，将UART接收到的数据写入输入缓冲区
			for(uint8_t i=0;i<Size;i++)
			{
				xQueueSendFromISR(UartInputQueue, &InputBuffer[i], xHigherPriorityTaskWoken);
			}
			xQueueSendFromISR(UartInputQueue, &nullChar, xHigherPriorityTaskWoken);
			//开启串口DMA+IDLE接收
			HAL_UARTEx_ReceiveToIdle_DMA(&UartShellHandle, InputBuffer, InputBufferSize);
		}
	}
}
#else
/**
  *@ FunctionName: void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
  *@ Author: CzrTuringB
  *@ Brief: Uart的接收DMA回调函数
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ 	1、重写的BspCore里面的弱函数
  *@ 	2、在stm32mp1xx_it.c的UART_IRQHandler函数中调用
  *@ Notes:
  */
void ShellUartRxCallback(void *huart)
{
	//1、创建任务切换标志位
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	//2、是否是Shell串口的中断
	if((UART_HandleTypeDef*) huart == &UartShellHandle)
	{
		//是否是空闲中断
		if(__HAL_UART_GET_FLAG(&UartShellHandle, UART_FLAG_IDLE) != RESET)
		{
			//清除空闲中断标志位
			__HAL_UART_CLEAR_IDLEFLAG(&UartShellHandle);
			//禁用 DMA 接收
			HAL_UART_DMAStop(&UartShellHandle);
			uint8_t nullChar = '\0';
			//清除 DMA 相关标志
			__HAL_UART_CLEAR_IDLEFLAG(&UartShellHandle);
			//发生IDLE中断后，将UART接收到的数据写入输入缓冲区
			uint8_t Size  = InputBufferSize - __HAL_DMA_GET_COUNTER(&UartShellDmaHandle);
			for(uint8_t i=0;i<Size;i++)
			{
				xQueueSendFromISR(UartInputQueue, &InputBuffer[i], &xHigherPriorityTaskWoken);
			}
			xQueueSendFromISR(UartInputQueue, &nullChar, &xHigherPriorityTaskWoken);
			//开启DMA传输、使能空闲中断
			HAL_UART_Receive_DMA(&UartShellHandle, InputBuffer, InputBufferSize);
			__HAL_UART_CLEAR_IDLEFLAG(&UartShellHandle);
			__HAL_UART_ENABLE_IT(&UartShellHandle, UART_IT_IDLE);
		}
	}
	//3、通知调度器是否进行任务切换
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif
/**
  *@ FunctionName: void ShellUsartApp(void)
  *@ Author: CzrTuringB
  *@ Brief: Shell串口任务函数，实现串口的收发
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
void ShellUartApp(void)
{
	ShellUartInit();
	ShellPrintf(UartShell,"Welcom to CzrTuringB's STM32 Uart Shell!!!\r\n");
	while(1)
	{
		ShellMessage message;
		if(xQueueReceive(UartOutputQueue, &message, portMAX_DELAY) == pdPASS)
		{
			ShellUartPrintf(message.messageBuffer, message.index);
		}
	}
}

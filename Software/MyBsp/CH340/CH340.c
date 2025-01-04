/**
  *@ FileName: CH340.c
  *@ Author: CzrTuringB
  *@ Brief:	用于实现USB转串口通信
  *@ Time: Sep 21, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "CH340.h"
/* Data ------------------------------------------------------------------*/
//配置输出缓冲数组
#define OutputBuffSize  500
static uint8_t OutputBuff[OutputBuffSize];
static uint32_t OutputBuffIndex;
//配置输入环形缓冲
#define InputBufferSize  80
static uint8_t InputBuffer[InputBufferSize];
#define InputTempBufferSize 20
static uint8_t InputTempBuffer[InputTempBufferSize];
Queue InputQueue;
/* 重定向printf至串口输出------------------------------------------------------------------*/
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE  int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE  int fputc(int ch, FILE *stream)
#endif
PUTCHAR_PROTOTYPE
{
	OutputBuff[OutputBuffIndex] = (uint8_t) ch;
	OutputBuffIndex ++;
	return ch;
}
/* Functions ------------------------------------------------------------------*/
void DebugInit(void)
{
	//初始化输入缓冲区
	QueueInit(&InputQueue, sizeof(uint8_t), InputBufferSize, InputBuffer);
	//开启串口DMA+IDLE接收
	HAL_UARTEx_ReceiveToIdle_DMA(&UartDebugHandle, InputTempBuffer, InputTempBufferSize);
	//打印输出调试欢迎信息
	DebugPrintf("Welcom to CzrTuringB's STM32 Uart Debug!!!\r\n");
}
void DebugPrintf(const char *format, ...)
{
	/* 等待串口状态 */
	while((&UartDebugHandle)->gState != HAL_UART_STATE_READY) ;
	/* 初始缓存下标 */
	OutputBuffIndex = 0;
	/* 调用vprintf解析格式化字符串 */
	va_list args_list;
	va_start(args_list, format);
	vprintf(format, args_list);
	va_end(args_list);
	UART_DEBUG_TRANSMIT();
}
BspState DebugScanfOne(uint8_t* ch)
{
	return QueueOutElement(&InputQueue, (void*)ch);
}
BspState DebugScanf(uint8_t* str)
{
	BspState state;
	uint8_t ch;
	if(InputQueue.count != 0)
	{
		for(uint8_t i=0;i<InputBufferSize;i++)
		{
			state = QueueOutElement(&InputQueue, (void*)&ch);
			if(state == BspOk && ch != '\0')
			{
				*str = ch;
				str++;
			}
			else if(ch == '\0')
			{
				break;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		state = BspError;
	}
	return state;
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	//判断是否为完全传输空闲中断
	if(huart->RxEventType != HAL_UART_RXEVENT_HT)
	{
		//发生IDLE中断后，将UART接收到的数据写入输入缓冲区
		for(uint8_t i=0;i<Size;i++)
		{
			QueueInElement(&InputQueue, (void*)&InputTempBuffer[i]);
		}
		QueueInElement(&InputQueue, (void*)'\0');
		//开启串口DMA+IDLE接收
		HAL_UARTEx_ReceiveToIdle_DMA(&UartDebugHandle, InputTempBuffer, InputTempBufferSize);
	}
}

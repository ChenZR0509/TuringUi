/**
  *@ FileName：Shell.h
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 13, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __SHELL_H
#define __SHELL_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes" "------------------------------------------------------------------*/
#include "BspCore.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
#define MaxMessageSize 50					//一次发送最大消息量
//-enum
typedef enum
{
	UartShell  = 0x01,						//Uart串口
	UsbShell   = 0x02,						//Usb虚拟串口
	OledShell  = 0x04,						//OledShell
}MessageReceiver;
//-struct
typedef struct
{
    MessageReceiver receiver;  				//消息接收者
    uint8_t index;							//消息索引
    uint8_t messageBuffer[MaxMessageSize];	//消息缓冲区
}ShellMessage,*pShellMessage;
//-variable
extern xTaskHandle ShellTaskHandle;
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
pShellMessage MessageCreate(uint8_t receiver);
BspState MessageDestory(pShellMessage message);
BspState MessageReset(pShellMessage message, uint8_t receiver);
BspState ShellPrintf(uint8_t receiver, const char *format, ...);
BspState ShellNormalPrintf(pShellMessage message, const char *format, ...);
void ShellApp(void);
#ifdef __cplusplus
}
#endif
/* C Style End------------------------------------------------------------------*/
#endif

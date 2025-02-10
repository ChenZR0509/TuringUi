/**
  *@ FileName: Application.c
  *@ Author: CzrTuringB
  *@ Brief: 实时操作系统的应用文件
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "Application.h"
//Peripheral
#include "Led/Led.h"
#include "Shell/Shell.h"
#include "Button/Button.h"
#include "Oled/Oled.h"
/* Data ------------------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
void MyRtosInit(void)
{
	vTraceEnable(TRC_START_AWAIT_HOST);
	xTaskCreate((void*)ShellApp, "Shell", 64 * 4, NULL, 24, &ShellTaskHandle);
	xTaskCreate((void*)LedApp, "LED", 64 * 4, NULL, 24, &LedTaskHandle);
	xTaskCreate((void*)ButtonApp, "Button", 64 * 4, NULL, 24 , &ButtonTaskHandle);
	xTaskCreate((void*)OledApp, "Oled", 64 * 4, NULL, 24, &OledTaskHandle);
}

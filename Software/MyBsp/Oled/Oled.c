/**
  *@ FileName：Oled.c
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 14, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "Oled.h"
#include "TuringUi/Application/UiInit.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
xTaskHandle OledTaskHandle;
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: void OledApp(void)
  *@ Author: CzrTuringB
  *@ Brief: Oled任务函数
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
void OledApp(void)
{
	//TuringUi初始化
	UiInit();
	//删除当前任务
	vTaskDelete(NULL);
	while(1)
	{
		;
	}
}

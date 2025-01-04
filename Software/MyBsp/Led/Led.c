/**
  *@ FileName: Led.c
  *@ Author: CzrTuringB
  *@ Brief: 开发板上LED灯的板级支持包
  *@ Time: Sep 15, 2024
  *@ Requirement：
  *@	1、在调用这个文件的函数前确保已经初始化GPIO配置
  */
/* Includes ------------------------------------------------------------------*/
#include "Led.h"
/* Data ------------------------------------------------------------------*/
EventGroupHandle_t LedEvent;
/* Functions ------------------------------------------------------------------*/
void LedOn(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, LedStateOn);
}

void LedOff(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, LedStateOff);
}

void LedToggle(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

GPIO_PinState LedRead(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	GPIO_PinState Temp;
	Temp = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	return Temp;
}

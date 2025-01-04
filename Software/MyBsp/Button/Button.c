/**
  *@ FileName: Button.c
  *@ Author: CzrTuringB
  *@ Brief: 开发板上Button的板级支持包
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "Button.h"
/* Data ------------------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
GPIO_PinState ButtonRead(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	GPIO_PinState temp;
	temp = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	return temp;
}

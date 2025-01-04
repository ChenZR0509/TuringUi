/**
  *@ FileName: Led.h
  *@ Author: CzrTuringB
  *@ Brief: 开发板上LED灯的板级支持包
  *@ Time: Sep 18, 2024
  *@ Requirement：
  *@	1、根据原理图定义LED所对应的端口和引脚
  *@	2、根据原理图设置LED的开关状态对应的输出电平
  */
#ifndef __LED_H
#define __LED_H
/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* Data ------------------------------------------------------------------*/
//LED的端口引脚
#define LedPort GPIOD
#define LedPin  GPIO_PIN_12
//LED的输出状态
#define LedStateOn GPIO_PIN_RESET
#define LedStateOff GPIO_PIN_SET
//事件标志
#define LedOnEvent 0x01
#define LedOffEvent (0x01<<1)
#define LedToggleEvent (0x01<<2)
//extern
extern EventGroupHandle_t LedEvent;
/* Functions------------------------------------------------------------------*/
/**
  *@ FunctionName: void LedOn(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
  *@ Author: CzrTuringB
  *@ Brief: 打开LED灯
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
void LedOn(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
/**
  *@ FunctionName: void LedOff(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
  *@ Author: CzrTuringB
  *@ Brief: 关闭LED灯
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
void LedOff(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
/**
  *@ FunctionName: void LedToggle(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
  *@ Author: CzrTuringB
  *@ Brief: 翻转LED灯的状态
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
void LedToggle(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
/**
  *@ FunctionName: GPIO_PinState LedRead(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
  *@ Author: CzrTuringB
  *@ Brief:读取LED灯的当前状态
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
GPIO_PinState LedRead(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
#endif

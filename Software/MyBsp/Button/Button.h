/**
  *@ FileName: Button.h
  *@ Author: CzrTuringB
  *@ Brief: 开发板上Button的板级支持包
  *@ Time: Sep 18, 2024
  *@ Requirement：
  *@ 	1、根据原理图配置Button的端口引脚
  *@ 	2、根据原理图配置Button的按下与否对应的输入电平
  */
#ifndef __BUTTON_H
#define __BUTTON_H
/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* Data ------------------------------------------------------------------*/
//Button的端口引脚
#define ButtonPort GPIOA
#define ButtonPin  GPIO_PIN_0
//Button按下与否对应的引脚输入电平
#define ButtonPress GPIO_PIN_SET
#define ButtonIdle GPIO_PIN_RESET
//Button的状态
 typedef enum
 {
    IDLE,   //空闲
    PRESS_DETECTED,  //按键按下
    RELEASE_DETECTED, //按键释放
    SHORT_PRESS,     //短按
    LONG_PRESS,     //长按
	LONG_PRESS_END, //长按结束
	DOUBLE_PRESS,    //双击
	WAIT,	//等待
 }ButtonState;
/* Functions------------------------------------------------------------------*/
/**
  *@ FunctionName:
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Sep 18, 2024
  *@ Requirement：
  */
GPIO_PinState ButtonRead(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
#endif

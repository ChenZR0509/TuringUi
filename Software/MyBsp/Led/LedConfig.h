/**
  *@ FileName：LedConfig.h
  *@ Author：CzrTuringB
  *@ Brief：LED的配置文件
  *@ Time：Jan 22, 2025
  *@ Requirement：
  *@ 	1、GPIO输出电平设置为将LED灯关闭的电平(硬件电路设计时，最好让所有的LED都统一)
  *@ 	2、输出模式设置为推挽输出
  *@ 	3、无上下拉
  *@ 	4、输出速度为低速
  *@ Notes:
  *@ 	1、采用FreeRTOS软件定时器实现，过多的LED或频繁的改动LED的亮度会增加CPU负担
  *@ 	2、可以直接采用硬件定时器输出PWM波形，但是对于硬件定时器资源稀缺的控制器，建议还是使用软件定时器实现
  */
#ifndef __LEDCONFIG_H
#define __LEDCONFIG_H
/* C Style Start------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes" "------------------------------------------------------------------*/
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-参数定义
#define SoftWarePwm		1				  //软件定时器实现亮度控制
#define HardWarePwm		0				  //硬件定时器实现亮度控制

#define LedCount	 	3				  //LED数量
#define LedQueueSize	6			  	  //LED队列长度(最好为LED数量的1.5倍)
#define LedResolution	20				  //LED亮度分辨率(最好为10的整数倍)

#define LedStateOff  	GPIO_PIN_SET	  //LED灭时的电平
#define LedStateOn 	 	GPIO_PIN_RESET	  //LED亮时的电平
 //-enum
//-typedef(类型重命名)
//-struct
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/* C Style End------------------------------------------------------------------*/
#endif

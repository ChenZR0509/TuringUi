/**
  *@ FileName：
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 23, 2025
  *@ Requirement：
  *@ 	1、设置相应的引脚为输入模式，浮空输入
  *@ 	2、依据原理图设置按键不同状态对应的电平
  *@ Notes：
  */
#ifndef __BUTTONCONFIG_H
#define __BUTTONCONFIG_H
/* C Style Start------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes" "------------------------------------------------------------------*/
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
#define buttonTest 1				  //按键功能测试

#define ButtonPress GPIO_PIN_SET	  //按键按下时的电平
#define ButtonIdle  GPIO_PIN_RESET	  //按键未按下时的电平

#define ShortClickTime 500			  //这个是第一次松开时间和第二次按下时间的判断时长
#define LongPressTime 1000  		  //定义长按的时间阈值
#define PressTime 100             	  //判断毛刺时长

#define QueueLength 3				  //按键功能更换队列的长度(不用太大)
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

/**
  *@ FileName: Ssd1306.h
  *@ Author: CzrTuringB
  *@ Brief:	IIC接口的SSD1306头文件
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ Note:
  *@ 	1、PinCom表示引脚行，一共64个
  *@ 	2、RamRow表示显存行，一共64个
  *@ 	3、Row表示显示行，一共64个
  *@ 	4、Segment表示显示列
  *@ 	5、RamClo表示显存列
  */
#ifndef SSD1306_H_
#define SSD1306_H_
/* Includes" "------------------------------------------------------------------*/
#include "i2c.h"
#include "crc.h"
#include "BspCore.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
#define ScreenWidth		128					//屏幕宽度
#define ScreenHeight	64					//屏幕高度
#define PageNumber		8					//页数量
//-enum
//-typedef(类型重命名)
typedef uint8_t (*SSDBuffer)[ScreenWidth];  //SSDBuffer为指向128列数组的指针类型
//-struct
typedef struct
{
	uint8_t disBuffer[PageNumber][ScreenWidth];		//屏幕显示缓冲数组
	uint8_t maskBuffer[PageNumber][ScreenWidth];	//蒙版
	Bool dataValid; 								//数据有效性判断变量
}SsdPublicConfig;
//-variable
extern SsdPublicConfig* uiDevice;
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
void SSDInit(void);
void SSDUpdateScreen(void);
void SSDClean(void);
#endif

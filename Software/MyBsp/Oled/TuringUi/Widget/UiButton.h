/**
  *@ FileName: UiButton.h
  *@ Author: CzrTuringB
  *@ Brief: Button开关
  *@ Time: Dec 20, 2024
  *@ Requirement：
  */
#ifndef __UIButton_H
#define __UIButton_H
/* Includes ------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
#include "../Basic/UiAnimation.h"
/* Data ------------------------------------------------------------------*/
typedef struct UiButtonStruct
{
	Bool  isAniInit;		 //是否进行了动画初始化
	UiElement* base;         //UI元素基结构体
	EnDis* trigger; 	 	 //表示是否触发
	uint8_t* size;	 		 //表示绘图尺寸
	Point2D p;		         //表示绘图位置
	UiStyle uiStyle;         //表示Ui样式
	AnimSet* anis;			 //标签的动画集合信息
	uint16_t aniSpeed;		 //动画播放速度
}UiButtonStruct, *pUiButtonStruct;
/* Functions------------------------------------------------------------------*/
UiButtonStruct* UiButtonCreate(void);
void UiButtonInit(UiButtonStruct* button, EnDis* trigger, uint8_t* size, Point2D p, UiStyle uiStyle);
void UiButtonsDestory(UiElement* ele);
void UiButtonUiStyle1(void* widget);
#endif

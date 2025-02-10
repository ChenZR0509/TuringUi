/**
  *@ FileName: UiUiLabel.h
  *@ Author: CzrTuringB
  *@ Brief: 标签部件
  *@ Time: Date
  *@ Requirement：
  */
#ifndef __UIUiLabel_H
#define __UIUiLabel_H
/* Includes ------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
#include "../Basic/UiAnimation.h"
/* Data ------------------------------------------------------------------*/
typedef struct UiLabelStruct
{
	Bool  isAniInit;		//是否进行了动画初始化
	pUiElement base;	 	//UI元素基结构体
	uint8_t numChar;		//字符个数
	char* str;	 			//标签文本内容
	FontSize* size;			//字体样式
	UiStyle	uiStyle;		//设置UI样式
	Point2D   p;			//UI元素位置信息：UI元素的左下角坐标点
	uint8_t length;			//字符长度
	uint8_t disLength;		//显示区长度
	uint8_t disHeight;		//显示区高度
	AnimSet* anis;			//标签的动画集合信息
	uint16_t aniSpeed;		//动画播放速度
}UiLabelStruct,*pUiLabelStruct;
/* Functions------------------------------------------------------------------*/
UiLabelStruct* UiLabelCreate(void);
void UiLabelInit(UiLabelStruct* label, FontSize* size, Point2D p,const char* str, UiStyle uiStyle);
void UiLabelGetSize(UiLabelStruct* label);
void UiLabelSetSize(UiLabelStruct* label,uint8_t length,uint8_t height);
void UiLabelAnimInit(UiLabelStruct* label,uint8_t length, uint8_t height, uint16_t speed, void (*animation)(UiElement*));
void UiLabelAnimation(UiElement* ele);
void UiLabelUiStyle1(void* widget);
void UiLabelUiStyle2(void* widget);
#endif

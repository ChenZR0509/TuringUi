/**
  *@ FileName：UiBorder.h
  *@ Author：CzrTuringB
  *@ Brief：边框
  *@ Time：Jan 16, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __UIBORDER_H
#define __UIBORDER_H
/* Includes" "------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
#include "../Basic/UiAnimation.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
typedef struct UiBorderStruct
{
	Bool  isAniInit;			//是否进行了动画初始化
	Point2D   p;				//UI元素位置信息：UI元素的左下角坐标点
	UiElement* base;			//UI元素基结构体
	uint8_t	width;				//边框长度
	uint8_t	height;				//边框高度
	uint8_t radius;				//圆角半径
	UiStyle	uiStyle;			//设置UI样式
	FillMode* mode;				//填充模式
	AnimSet* anis;				//标签的动画集合信息
	uint16_t aniSpeed;			//动画播放速度
}UiBorderStruct,*pUiBorderStruct;
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
UiBorderStruct* UiBorderCreate(void);
void UiBorderInit(UiBorderStruct* border, Point2D p, uint8_t width, uint8_t height, uint8_t radius, FillMode* mode, UiStyle uiStyle);
void UiBorderAnimInit(UiBorderStruct* border, uint8_t count, uint16_t speed, void (*animation)(UiElement*));
void UiBorderAnimation(UiElement* ele);
void UiBorderUiStyle1(void* widget);
void UiBorderUiStyle2(void* widget);
void UiBorderUiStyle3(void* widget);
void UiBorderUiStyle4(void* widget);
#endif

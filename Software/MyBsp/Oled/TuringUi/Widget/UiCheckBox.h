/**
  *@ FileName: UiCheckBox.h
  *@ Author: CzrTuringB
  *@ Brief: 复选框
  *@ Time: Dec 17, 2024
  *@ Requirement：
  *@ 	1、style1对应的是<v> <x>分别表示确认和取消
  *@ 	2、style2对应的是矩形填充表示勾选，无填充表示是不勾选
  *@ 	3、style3对应的是圆形填充表示勾选，无填充表示是不勾选
  */
#ifndef __UICHECKBOX_H
#define __UICHECKBOX_H
/* Includes ------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
/* Define ------------------------------------------------------------------*/
/* Data ------------------------------------------------------------------*/
typedef struct UiCheckBoxStruct
{
	pUiElement base; //UI元素基结构体
	EnDis* check; 	 //表示是否勾选
	uint8_t* size;	 //表示绘图尺寸
	Point2D p;		 //表示绘图位置
	UiStyle uiStyle; //表示Ui样式
}UiCheckBoxStruct,*pUiCheckBoxStruct;
/* Functions------------------------------------------------------------------*/
UiCheckBoxStruct* UiCheckBoxCreate(void);
void UiCheckBoxInit(UiCheckBoxStruct* box, EnDis* check, uint8_t* size, Point2D p, UiStyle uiStyle);
void UiCheckBoxUiStyle1(void* widget);
void UiCheckBoxUiStyle2(void* widget);
void UiCheckBoxUiStyle3(void* widget);
#endif

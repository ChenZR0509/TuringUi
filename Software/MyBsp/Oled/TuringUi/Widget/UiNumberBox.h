/**
  *@ FileName: UiNumberBox.h
  *@ Author: CzrTuringB
  *@ Brief: 数字选择框
  *@ Time: Oct 6, 2024
  *@ Requirement：
  *@ 	1、style0对应的是只有数字的UI样式
  *@ 	2、style1对应的是小括号包裹数字的UI样式
  *@ 	3、style2对应的是中括号包裹数字的UI样式
  */
#ifndef __UINUMBERBOX_H
#define __UINUMBERBOX_H
/* Includes ------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
/* Data ------------------------------------------------------------------*/
typedef struct UiNumberBoxStruct UiNumberBoxStruct;
typedef struct UiNumberBoxStruct* pUiNumberBoxStruct;
typedef void (*ValueType)(UiNumberBoxStruct*);
struct UiNumberBoxStruct
{
	Point2D   p;				//UI元素位置信息：UI元素的左下角坐标点
	UiElement* base;			//UI元素基结构体
	char str[10];				//存储转换后的字符串
	void* data;    				//指向实际数据的指针
	uint8_t	strLength;			//数字复选框字符串长度
	uint8_t	strHeight;			//数字复选框字符串高度
	FontSize* size;				//字体样式
	ValueType	setValue;		//设置存储数值
	UiStyle		uiStyle;		//设置UI样式
};
/* Function ------------------------------------------------------------------*/
UiNumberBoxStruct* UiNumberBoxCreate(void);
void UiNumberBoxInit(UiNumberBoxStruct* box, FontSize* size, Point2D p, void (*SetValue)(UiNumberBoxStruct*), UiStyle uiStyle, void* data);
void UiNumBoxSetUint8(UiNumberBoxStruct* box);
void UiNumBoxSetint8(UiNumberBoxStruct* box);
void UiNumBoxSetFloat(UiNumberBoxStruct* box);
void UiNumBoxUiStyle1(void* widget);
void UiNumBoxUiStyle2(void* widget);
void UiNumBoxUiStyle3(void* widget);
void UiNumBoxGetSize(UiNumberBoxStruct* box);
#endif

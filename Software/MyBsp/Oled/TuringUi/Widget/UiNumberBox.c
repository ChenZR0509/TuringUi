/**
 *@ FileName: UiUiNumberBox.c
 *@ Author: CzrTuringB
 *@ Brief: 数字选择框
 *@ Time: Oct 6, 2024
 *@ Requirement：
 */
/* Includes ------------------------------------------------------------------*/
#include "UiNumberBox.h"
#include <stdio.h>
#include <string.h>
/* Data ------------------------------------------------------------------*/
void UiNumberBoxHandle(UiElement* ele, void* value);
void UiNumberBoxRender(UiElement* ele);
void UiNumberBoxDestory(UiElement* ele);
/* Function ------------------------------------------------------------------*/
/**
  *@ FunctionName: UiNumberBoxStruct* UiNumberBoxCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个数字复选框
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
UiNumberBoxStruct* UiNumberBoxCreate(void)
{
	UiNumberBoxStruct* box = pvPortMalloc(sizeof(UiNumberBoxStruct));
	if(box == NULL)	return NULL;
	memset(box, 0, sizeof(UiNumberBoxStruct));
	return box;
}
/**
  *@ FunctionName: void UiNumberBoxInit(UiNumberBoxStruct* box, FontSize* size, Point2D p, ValueType setValue, UiStyle uiStyle, void* data)
  *@ Author: CzrTuringB
  *@ Brief: 初始化数字复选框，设置其ui样式、尺寸、坐标点
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiNumberBoxInit(UiNumberBoxStruct* box, FontSize* size, Point2D p, ValueType setValue, UiStyle uiStyle, void* data)
{
	box->base = UiElementCreate();
	UiElementInit(box->base, (void*)box, UiNumberBoxRender, UiNumberBoxHandle, UiNumberBoxDestory);
	box->p = p;
	box->size = size;
	box->data = data;
	box->setValue = setValue;
	box->uiStyle = uiStyle;
	box->setValue(box);
	box->uiStyle(box);
	UiNumBoxGetSize(box);
}
/**
  *@ FunctionName: void UiNumberBoxDestory(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 释放动态分配的内存
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiNumberBoxDestory(UiElement* ele)
{
	UiNumberBoxStruct* box = (UiNumberBoxStruct*)(ele->widget);
	vPortFree(box);
	UiElementDestroy(ele);
}
/**
  *@ FunctionName: void UiNumberBoxHandle(UiElement* ele, void* value)
  *@ Author: CzrTuringB
  *@ Brief: 数字复选框的处理函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  *@ 	1、设置数字复选框的数值
  *@ 	2、更新数字复选框的字符串
  */
void UiNumberBoxHandle(UiElement* ele, void* value)
{
	UiNumberBoxStruct* box = (UiNumberBoxStruct*)(ele->widget);
	box->setValue(box);
	box->uiStyle((void*)box);
	UiNumBoxGetSize(box);
}
/**
  *@ FunctionName: void UiNumberBoxRender(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 数字复选框渲染函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiNumberBoxRender(UiElement* ele)
{
	UiNumberBoxStruct* box = (UiNumberBoxStruct*)(ele->widget);
	PlotFillRectangle(box->p, box->strLength, box->strHeight, uiDevice->disBuffer, Fill0);
	PlotString(box->p, box->str, *box->size, uiDevice->disBuffer);
}
/**
  *@ FunctionName: void NumBoxSetUint8(UiNumberBoxStruct* box)
  *@ Author: CzrTuringB
  *@ Brief: 设置数字复选框存储无符号8位整形
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxSetUint8(UiNumberBoxStruct* box)
{
	snprintf(box->str, sizeof(box->str), "%d", *(uint8_t *)box->data);
}
/**
  *@ FunctionName: void NumBoxSetint8(UiNumberBoxStruct* box)
  *@ Author: CzrTuringB
  *@ Brief: 设置数字复选框存储有符号8位整形
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxSetint8(UiNumberBoxStruct* box)
{
	snprintf(box->str, sizeof(box->str), "%d", *(int8_t *)box->data);
}
/**
  *@ FunctionName: void NumBoxSetFloat(UiNumberBoxStruct* box)
  *@ Author: CzrTuringB
  *@ Brief: 设置数字复选框存储浮点数
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxSetFloat(UiNumberBoxStruct* box)
{
	snprintf(box->str, sizeof(box->str), "%.2f", *(float *)box->data);
}
/**
  *@ FunctionName: void NumBoxUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式只有数字
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxUiStyle1(void* widget)
{
	UiNumberBoxStruct* box = (UiNumberBoxStruct*)widget;
	char str[12];
	sprintf(str, "%s", box->str);
	memset(box->str, 0, sizeof(box->str));
	strcpy(box->str, str);
}
/**
  *@ FunctionName: void NumBoxUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式小括号+数字
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxUiStyle2(void* widget)
{
	UiNumberBoxStruct* box = (UiNumberBoxStruct*)widget;
	char str[12];
	sprintf(str, "(%s)", box->str);
	memset(box->str, 0, sizeof(box->str));
	strcpy(box->str, str);
}
/**
  *@ FunctionName: void NumBoxUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式中小括号+数字
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxUiStyle3(void* widget)
{
	UiNumberBoxStruct* box = (UiNumberBoxStruct*)widget;
	char str[12];
	sprintf(str, "[%s]", box->str);
	memset(box->str, 0, sizeof(box->str));
	strcpy(box->str, str);
}
/**
  *@ FunctionName: void NumBoxGetSize(UiNumberBoxStruct* box)
  *@ Author: CzrTuringB
  *@ Brief: 获取字符串所占像素宽度,高度
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiNumBoxGetSize(UiNumberBoxStruct* box)
{
	if(*box->size == C6x8)
	{
		box->strHeight = 8;
		box->strLength = 6*strlen(box->str);
	}
	if(*box->size == C8x16)
	{
		box->strHeight = 16;
		box->strLength = 8*strlen(box->str);
	}
}

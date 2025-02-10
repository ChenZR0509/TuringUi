/**
  *@ FileName: UiCheckBox.c
  *@ Author: CzrTuringB
  *@ Brief: 复选框
  *@ Time: Dec 17, 2024
  *@ Requirement：
  *@ 	1、复选框可以实现按钮的功能
  *@ 	2、复选框勾选时并不作用，尽在按下当前页面或窗口的确认或取消按键后作用
  */
/* Includes ------------------------------------------------------------------*/
#include "UiCheckBox.h"
/* Data ------------------------------------------------------------------*/
void UiCheckBoxDestory(UiElement* ele);
void UiCheckBoxHandle(UiElement* ele, void* value);
void UiCheckBoxRender(UiElement* ele);
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: NumberBoxStruct* UiCheckBoxCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个复选框
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
UiCheckBoxStruct* UiCheckBoxCreate(void)
{
	UiCheckBoxStruct* box = pvPortMalloc(sizeof(UiCheckBoxStruct));
	if(box == NULL)	return NULL;
	memset(box, 0, sizeof(UiCheckBoxStruct));
	return box;
}
/**
  *@ FunctionName: void UiCheckBoxInit(UiCheckBoxStruct* box, EnDis* check, uint8_t* size, Point2D p, UiStyle uiStyle)
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiCheckBoxInit(UiCheckBoxStruct* box, EnDis* check, uint8_t* size, Point2D p, UiStyle uiStyle)
{
	box->base = UiElementCreate();
	UiElementInit(box->base, (void*)box, UiCheckBoxRender, UiCheckBoxHandle, UiCheckBoxDestory);
	//对于复选框而言，没有相关事件进行处理
	box->base->isHandle = False;
	box->check = check;
	box->p = p;
	box->size = size;
	box->uiStyle = uiStyle;
}
/**
  *@ FunctionName: void UiCheckBoxDestory(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 释放动态分配的内存
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiCheckBoxDestory(UiElement* ele)
{
	UiCheckBoxStruct* box = (UiCheckBoxStruct*)(ele->widget);
	vPortFree(box);
	UiElementDestroy(ele);
}
/**
  *@ FunctionName: void UiCheckBoxHandle(UiElement* ele, void* value)
  *@ Author: CzrTuringB
  *@ Brief: 复选框的处理函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiCheckBoxHandle(UiElement* ele, void* value)
{
	//UiCheckBoxStruct* box = (UiCheckBoxStruct*)(ele->widget);
	return;
}
/**
  *@ FunctionName: void UiCheckBoxRender(void* self)
  *@ Author: CzrTuringB
  *@ Brief: 复选框渲染函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiCheckBoxRender(UiElement* ele)
{
	UiCheckBoxStruct* box = (UiCheckBoxStruct*)(ele->widget);
	//绘制复选框
	box->uiStyle((void*)box);
}
/**
  *@ FunctionName: void UiCheckBoxUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: 字符串样式复选框
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ 	1、<v>表示使能
  *@ 	2、<x>表示失能
  *@ 	3、UiCheckBoxStruct中size存储字体样式
  */
void UiCheckBoxUiStyle1(void* widget)
{
	UiCheckBoxStruct* box = (UiCheckBoxStruct*)widget;
	if(*(box->check) == Enable)
	{
		if(*(box->size) == 0) PlotFillRectangle(box->p, 24, 16, uiDevice->disBuffer, Fill0);
		if(*(box->size) == 1) PlotFillRectangle(box->p, 18, 8, uiDevice->disBuffer, Fill0);
		PlotString(box->p, "<V>", (FontSize)*(box->size), uiDevice->disBuffer);
	}
	else
	{
		if(*(box->size) == 0) PlotFillRectangle(box->p, 24, 16, uiDevice->disBuffer, Fill0);
		if(*(box->size) == 1) PlotFillRectangle(box->p, 18, 8, uiDevice->disBuffer, Fill0);
		PlotString(box->p, "<X>", (FontSize)*(box->size), uiDevice->disBuffer);
	}
}
/**
  *@ FunctionName: void UiCheckBoxUiStyle2(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: 矩形复选框
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ 	1、填充表示使能，不填充表示失能
  *@ 	2、UiCheckBoxStruct中size存储矩形边长
  *@ 	3、UiCheckBoxStruct中p存储矩形左下角坐标
  */
void UiCheckBoxUiStyle2(void* widget)
{
	UiCheckBoxStruct* box = (UiCheckBoxStruct*)widget;
	PlotFillRectangle(box->p, *box->size, *box->size, uiDevice->disBuffer, Fill0);
	PlotRectangle(box->p, *box->size, *box->size, uiDevice->disBuffer, FillF);
	Point2D p1 = box->p;
	p1.x += *box->size/4;
	p1.y += *box->size/4;
	if(*(box->check) == Enable)
	{
		PlotFillRectangle(p1, *box->size/2, *box->size/2, uiDevice->disBuffer, FillF);
	}
	else
	{
		PlotFillRectangle(p1, *box->size/2, *box->size/2, uiDevice->disBuffer, Fill0);
	}
}
/**
  *@ FunctionName: void UiCheckBoxUiStyle3(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: 圆形复选框
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ 	1、填充表示使能，不填充表示失能
  *@ 	2、UiCheckBoxStruct中size存储圆形半径
  *@ 	3、UiCheckBoxStruct中p存储圆心坐标
  */
void UiCheckBoxUiStyle3(void* widget)
{
	UiCheckBoxStruct* box = (UiCheckBoxStruct*)widget;
	Point2D pTemp = box->p;
	pTemp.x -= *(box->size);
	pTemp.y -= *(box->size);
	PlotFillRectangle(pTemp, 2*(*box->size), 2*(*box->size), uiDevice->disBuffer, Fill0);
	PlotCircle(box->p, *box->size, uiDevice->disBuffer, FillF);
	if(*(box->check) == Enable)
	{
		PlotFillCircle(box->p, *box->size/2, uiDevice->disBuffer, FillF);
	}
	else
	{
		PlotFillCircle(box->p, *box->size/2, uiDevice->disBuffer, Fill0);
	}
}

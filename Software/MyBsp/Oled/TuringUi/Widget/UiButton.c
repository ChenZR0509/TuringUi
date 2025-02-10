/**
  *@ FileName: UiButton.c
  *@ Author: CzrTuringB
  *@ Brief: Button开关
  *@ Time: Dec 20, 2024
  *@ Requirement：
  *@ 	1、开关
  */
/* Includes ------------------------------------------------------------------*/
#include "UiButton.h"
/* Data ------------------------------------------------------------------*/
void UiButtonsDestory(UiElement* ele);
void UiButtonHandle(UiElement* ele, void* value);
void UiButtonRender(UiElement* ele);
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: UiButtonStruct* UiButtonCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个按钮
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
UiButtonStruct* UiButtonCreate(void)
{
	UiButtonStruct* button = pvPortMalloc(sizeof(UiButtonStruct));
	if(button == NULL)	return NULL;
	memset(button, 0, sizeof(UiButtonStruct));
	return button;
}
/**
  *@ FunctionName: void UiButtonInit(UiButtonStruct* button, EnDis* trigger, uint8_t* size, Point2D p, UiStyle uiStyle)
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiButtonInit(UiButtonStruct* button, EnDis* trigger, uint8_t* size, Point2D p, UiStyle uiStyle)
{
	button->base = UiElementCreate();
	//UiElementInit(button->base, (void*)box, UiButtonRender, UiButtonHandle, UiButtonDestory);
	button->p = p;
	button->size = size;
	button->trigger = trigger;
	button->uiStyle = uiStyle;
}
/**
  *@ FunctionName: void UiButtonsDestory(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 释放动态分配的内存
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiButtonsDestory(UiElement* ele)
{
	UiButtonStruct* button = (UiButtonStruct*)(ele->widget);
	if(button->isAniInit == True)
	{
		//释放动作因子
		vPortFree(button->anis->anie->Attribute);
		//释放动画类型
		vPortFree(button->anis->anie->anit);
		//释放运动范围
		vPortFree(button->anis->anie->scale);
		//释放动画元素
		vPortFree(button->anis->anie);
		//释放动画集合
		vPortFree(button->anis);
	}
	vPortFree(button);
	UiElementDestroy(ele);
}
/**
  *@ FunctionName: void UiButtonHandle(UiElement* ele, void* value)
  *@ Author: CzrTuringB
  *@ Brief: 按键处理函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  *@ 	1、改变字符串内容
  *@ 	2、适合动态标签渲染方式
  */
void UiButtonHandle(UiElement* ele, void* value)
{
	//UiButtonStruct* button = (UiButtonStruct*)(ele->widget);
}
/**
  *@ FunctionName: void UiLabelRender(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 按键渲染函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiButtonRender(UiElement* ele)
{
	UiButtonStruct* button = (UiButtonStruct*)(ele->widget);
	//绘制标签
	button->uiStyle((void*)button);
}
/**
  *@ FunctionName: void UiLabelUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: 按键样式1(无动画)
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、要求尺寸设置为8或8的倍数
  */
void UiButtonUiStyle1(void* widget)
{
	UiButtonStruct* button = (UiButtonStruct*)widget;
	PlotFillRectangle(button->p, *button->size/8*13, *button->size, uiDevice->disBuffer, Fill0);
	PlotArcRectangle(button->p, *button->size/8*13, *button->size, *button->size/2, uiDevice->disBuffer, FillF);
	if(*(button->trigger) == Enable)
	{
		PlotFillCircle((Point2D){button->p.x + *button->size * 9 / 8, button->p.y + *button->size/2}, *button->size/2 - 2, uiDevice->disBuffer, FillF);
	}
	else
	{
		PlotCircle((Point2D){button->p.x + *button->size/2, button->p.y + *button->size/2}, *button->size/2 - 2, uiDevice->disBuffer, FillF);
	}
}

/**
  *@ FileName: UiUiLabel.c
  *@ Author: CzrTuringB
  *@ Brief: 标签部件
  *@ Time:  Jan 7, 2025
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiLabel.h"
/* Data ------------------------------------------------------------------*/
void UiLabelDestory(UiElement* ele);
void UiLabelHandle(UiElement* ele, void* value);
void UiLabelRender(UiElement* ele);
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: UiLabelStruct* UiLabelCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个字符标签
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
UiLabelStruct* UiLabelCreate(void)
{
	UiLabelStruct* label = pvPortMalloc(sizeof(UiLabelStruct));
	if(label == NULL)	return NULL;
	memset(label, 0, sizeof(UiLabelStruct));
	return label;
}
/**
  *@ FunctionName: void UiLabelInit(UiLabelStruct* label, FontSize* size, Point2D p,const char* str, UiStyle uiStyle)
  *@ Author: CzrTuringB
  *@ Brief: 初始化标签结构体
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiLabelInit(UiLabelStruct* label, FontSize* size, Point2D p, const char* str, UiStyle uiStyle)
{
	label->base = UiElementCreate();
	UiElementInit(label->base, (void*)label, UiLabelRender, UiLabelHandle, UiLabelDestory);
	label->p = p;
	label->size = size;
	label->aniSpeed = 0;
	label->uiStyle = uiStyle;
	label->numChar = strlen(str);
	label->isAniInit = False;
	label->str = pvPortMalloc(label->numChar + 1);
	strcpy(label->str, str);
	UiLabelGetSize(label);
	label->isAniInit = False;
}
/**
  *@ FunctionName: void UiLabelDestory(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 释放动态分配的内存
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiLabelDestory(UiElement* ele)
{
	UiLabelStruct* label = (UiLabelStruct*)(ele->widget);
	if(label->isAniInit == True)
	{
		//释放动作因子
		vPortFree(label->anis->anie->Attribute);
		//释放动画类型
		vPortFree(label->anis->anie->anit);
		//释放运动范围
		vPortFree(label->anis->anie->scale);
		//释放动画元素
		vPortFree(label->anis->anie);
		//释放动画集合
		vPortFree(label->anis);
	}
	vPortFree(label->str);
	vPortFree(label);
	UiElementDestroy(ele);
}
/**
  *@ FunctionName: void UiLabelHandle(UiElement* ele, void* value)
  *@ Author: CzrTuringB
  *@ Brief: 标签的处理函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  *@ 	1、改变字符串内容
  *@ 	2、适合动态标签渲染方式
  */
void UiLabelHandle(UiElement* ele, void* value)
{
	UiLabelStruct* label = (UiLabelStruct*)(ele->widget);
	PlotFillRectangle(label->p, label->disLength, label->disHeight, uiDevice->disBuffer, Fill0);
	vPortFree(label->str);
	label->numChar = strlen((char*)value);
	label->str = pvPortMalloc(label->numChar + 1);
	strcpy(label->str, (char*)value);
	if(label->uiStyle == UiLabelUiStyle1)
	UiLabelGetSize(label);
}
/**
  *@ FunctionName: void UiLabelRender(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 标签的渲染函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiLabelRender(UiElement* ele)
{
	UiLabelStruct* label = (UiLabelStruct*)(ele->widget);
	//绘制标签
	label->uiStyle((void*)label);
}
/**
  *@ FunctionName: void UiLabelAnimInit(UiLabelStruct* label,uint8_t length, uint8_t height, uint16_t speed, void (*animation)(UiElement*))
  *@ Author: CzrTuringB
  *@ Brief: 标签动画的初始化
  *@ Time: Jan 10, 2025
  *@ Requirement：
  *@ 	1、配置其为线性循环的字符滚动动画
  */
void UiLabelAnimInit(UiLabelStruct* label,uint8_t length, uint8_t height, uint16_t speed, void (*animation)(UiElement*))
{
	label->isAniInit = True;
	label->aniSpeed = speed;
	label->base->animation = animation;
	UiLabelGetSize(label);
	UiLabelSetSize(label, length, height);
	float* attribute = pvPortMalloc(sizeof(float));			//创建运动因子
	if(attribute == NULL)	return;
	AnimType *anit = pvPortMalloc(sizeof(AnimType));		//创建动画类型
	if(anit == NULL)	return;
	AnimTypeInit(anit, aEasingInOut);						//动画类型设置为线性
	MoveScale *scale = pvPortMalloc(sizeof(MoveScale));		//创建运动范围
	if(scale == NULL)	return;
	scale->sPosition = -label->disLength - (label->length / label->numChar);
	scale->ePosition = label->length - label->disLength + (label->length / label->numChar);
	AnimElement *anie = pvPortMalloc(sizeof(AnimElement));	//创建动画元素
	if(anie == NULL)	return;
	AnimElementInit(anie, anit, scale, attribute);			//初始化动画元素
	label->anis = pvPortMalloc(sizeof(AnimSet));			//创建动画集合
	if(label->anis == NULL)	return;
	//计算动画持续时间
	uint16_t time = (label->numChar + 2) * label->aniSpeed;
	AnimSetInit(label->anis, anie, 1, pdMS_TO_TICKS(time));
	label->base->isAniFinish = &(label->anis->isFinish);
}
/**
  *@ FunctionName: void UiLabelAnimation(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 标签动画更新函数
  *@ Time: Jan 10, 2025
  *@ Requirement：
  *@ 	2、动画计算
  */
void UiLabelAnimation(UiElement* ele)
{
	UiLabelStruct* label = (UiLabelStruct*)(ele->widget);
	if(label->isAniInit == False)	return;
	//判断显示字符是否小于显示区域长度
	if(label->length <= label->disLength)
	{
		*label->anis->anie->Attribute = 0;
		return;
	}
	//动画计算
	Animation(label->anis);
	//循环播放
	AnimOneWayLoop(label->anis);
}
/**
  *@ FunctionName: void UiLabelGetSize(UiLabelStruct* label)
  *@ Author: CzrTuringB
  *@ Brief: 获取字符串所占像素宽度(适配于静态UI样式)
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiLabelGetSize(UiLabelStruct* label)
{
	if(*label->size == C6x8)
	{
		label->length = 6 * label->numChar;
		label->disLength = 6 * label->numChar;
		label->disHeight = 8;
	}
	if(*label->size == C8x16)
	{
		label->length = 8 * label->numChar;
		label->disLength = 8 * label->numChar;
		label->disHeight = 16;
	}
}
/**
  *@ FunctionName:void UiLabelSetSize(UiLabelStruct* label,uint8_t length,uint8_t height)
  *@ Author: CzrTuringB
  *@ Brief: 设置标签显示区，适配于动态标签方式
  *@ Time: Jan 9, 2025
  *@ Requirement：
  */
void UiLabelSetSize(UiLabelStruct* label,uint8_t length,uint8_t height)
{
	label->disHeight = height;
	label->disLength = length;
}
/**
  *@ FunctionName: void UiLabelUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式静态标签
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiLabelUiStyle1(void* widget)
{
	UiLabelStruct* label = (UiLabelStruct*)widget;
	PlotFillRectangle(label->p, label->disLength, label->disHeight, uiDevice->disBuffer, Fill0);
	PlotString(label->p, label->str, *label->size, uiDevice->disBuffer);
}
/**
  *@ FunctionName: void NumBoxUiStyle2(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式动态标签
  *@ Time: Jan 7, 2025
  *@ Requirement：
  */
void UiLabelUiStyle2(void* widget)
{
	UiLabelStruct* label = (UiLabelStruct*)widget;
	//将字符串填充至蒙版
	Point2D p = label->p;
	p.x -= *label->anis->anie->Attribute;
	PlotString(p, label->str, *label->size, uiDevice->maskBuffer);
	//清除标签显示区域
	p.x = label->p.x + label->disLength;
	p.y = label->p.y + label->disHeight;
	PlotFillRectangle(label->p, label->disLength, label->disHeight, uiDevice->disBuffer, Fill0);
	//蒙版覆盖
	PlotAreaMaskCover(label->p, p, OrCover);
	//清除蒙版
	PlotCleanBuffer(uiDevice->maskBuffer);
}

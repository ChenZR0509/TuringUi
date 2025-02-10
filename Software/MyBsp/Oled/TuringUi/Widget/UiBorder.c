/**
  *@ FileName：UiBorder.c
  *@ Author：CzrTuringB
  *@ Brief：边框
  *@ Time：Jan 16, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "UiBorder.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
void UiBorderHandle(UiElement* ele, void* value);
void UiBorderRender(UiElement* ele);
void UiBorderDestory(UiElement* ele);
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: UiBorderStruct* UiBorderCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个边框
  *@ Time: Jan 16, 2025
  *@ Requirement：
  *@ Notes:
  */
UiBorderStruct* UiBorderCreate(void)
{
	UiBorderStruct* border = pvPortMalloc(sizeof(UiBorderStruct));
	if(border == NULL)	return NULL;
	memset(border, 0, sizeof(UiBorderStruct));
	return border;
}
/**
  *@ FunctionName: void BorderInit(UiBorderStruct* border, Point2D p, uint8_t width, uint8_t height, uint8_t radius, FillMode mode,UiStyle uiStyle)
  *@ Author: CzrTuringB
  *@ Brief: 边框初始化
  *@ Time: Jan 16, 2025
  *@ Requirement：
  *@ Notes:
  */
void UiBorderInit(UiBorderStruct* border, Point2D p, uint8_t width, uint8_t height, uint8_t radius, FillMode* mode, UiStyle uiStyle)
{
	border->base = UiElementCreate();
	UiElementInit(border->base, (void*)border, UiBorderRender, UiBorderHandle, UiBorderDestory);
	border->uiStyle = uiStyle;
	border->p = p;
	border->width = width;
	border->height = height;
	border->radius = radius;
	border->mode = mode;
	border->isAniInit = False;
	border->aniSpeed = 0;
	border->anis = NULL;
}
/**
  *@ FunctionName: void BorderDestory(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 释放动态分配的内存
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiBorderDestory(UiElement* ele)
{
	UiBorderStruct* border = (UiBorderStruct*)(ele->widget);
	if(border->isAniInit == True)
	{
		//释放动作因子
		vPortFree(border->anis->anie->Attribute);
		//释放动画类型
		vPortFree(border->anis->anie->anit);
		//释放运动范围
		vPortFree(border->anis->anie->scale);
		//释放动画元素
		vPortFree(border->anis->anie);
		//释放动画集合
		vPortFree(border->anis);
	}
	vPortFree(border);
	UiElementDestroy(ele);
}
/**
  *@ FunctionName: void UiBorderHandle(UiElement* ele, void* value)
  *@ Author: CzrTuringB
  *@ Brief: 边框的处理函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、改变填充模式
  *@ 	2、如果配置成了动画渲染方式就不使用边界处理函数
  */
void UiBorderHandle(UiElement* ele, void* value)
{
	UiBorderStruct* border = (UiBorderStruct*)(ele->widget);
	border->mode = (FillMode*)value;
}
/**
  *@ FunctionName: void UiBorderAnimInit(LabelStruct* label,uint8_t length, uint8_t height, uint16_t speed, void (*animation)(UiElement*))
  *@ Author: CzrTuringB
  *@ Brief: 边框动画的初始化
  *@ Time: Jan 10, 2025
  *@ Requirement：
  */
void UiBorderAnimInit(UiBorderStruct* border, uint8_t count, uint16_t speed, void (*animation)(UiElement*))
{
	border->isAniInit = True;
	border->aniSpeed = speed;
	border->base->animation = animation;
	float* attribute = pvPortMalloc(sizeof(float));			//创建运动因子
	if(attribute == NULL)	return;
	AnimType *anit = pvPortMalloc(sizeof(AnimType));		//创建动画类型
	if(anit == NULL)	return;
	AnimTypeInit(anit,aEasingInOut);						//动画类型设置为缓入缓出
	MoveScale *scale = pvPortMalloc(sizeof(MoveScale));		//创建运动范围
	if(scale == NULL)	return;
	scale->sPosition = 0;
	scale->ePosition = count - 1;
	AnimElement *anie = pvPortMalloc(sizeof(AnimElement));	//创建动画元素
	if(anie == NULL)	return;
	AnimElementInit(anie, anit, scale, attribute);			//初始化动画元素
	border->anis = pvPortMalloc(sizeof(AnimSet));			//创建动画集合
	if(border->anis == NULL)	return;
	AnimSetInit(border->anis, anie, 1, pdMS_TO_TICKS(count * border->aniSpeed));
	border->base->isAniFinish = &(border->anis->isFinish);
}
/**
  *@ FunctionName: void UiBorderAnimation(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 边框动画更新函数
  *@ Time: Jan 10, 2025
  *@ Requirement：
  *@ 	2、动画计算
  */
void UiBorderAnimation(UiElement* ele)
{
	UiBorderStruct* border = (UiBorderStruct*)(ele->widget);
	if(border->isAniInit == False)	return;
	//动画计算
	Animation(border->anis);
	//循环播放
	AnimOneWayLoop(border->anis);
}
/**
  *@ FunctionName: void UiBorderRender(UiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 边框的渲染函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  *@ Notes:
  */
void UiBorderRender(UiElement* ele)
{
	UiBorderStruct* border = (UiBorderStruct*)(ele->widget);
	//绘制标签
	border->uiStyle((void*)border);
}
/**
  *@ FunctionName: void UiBorderUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式静态边框【圆角矩形】
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ Notes：
  */
void UiBorderUiStyle1(void* widget)
{
	UiBorderStruct* border = (UiBorderStruct*)widget;
	PlotArcRectangle(border->p, border->width, border->height, border->radius, uiDevice->disBuffer, Fill0);
	PlotArcRectangle(border->p, border->width, border->height, border->radius, uiDevice->disBuffer, *border->mode);
}
/**
  *@ FunctionName: void UiBorderUiStyle1(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式动态边框【圆角矩形】
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ Notes：
  */
void UiBorderUiStyle2(void* widget)
{
	UiBorderStruct* border = (UiBorderStruct*)widget;
	FillMode mode;
	mode = border->mode[(uint8_t)*border->anis->anie->Attribute];
	PlotArcRectangle(border->p, border->width, border->height, border->radius, uiDevice->disBuffer, Fill0);
	PlotArcRectangle(border->p, border->width, border->height, border->radius, uiDevice->disBuffer, mode);
}
/**
  *@ FunctionName: void UiBorderUiStyle3(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式静态边框【矩形】
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ Notes：
  */
void UiBorderUiStyle3(void* widget)
{
	UiBorderStruct* border = (UiBorderStruct*)widget;
	PlotRectangle(border->p, border->width, border->height, uiDevice->disBuffer, Fill0);
	PlotRectangle(border->p, border->width, border->height, uiDevice->disBuffer, *border->mode);
}
/**
  *@ FunctionName: void UiBorderUiStyle4(void* widget)
  *@ Author: CzrTuringB
  *@ Brief: Ui样式动态边框【矩形】
  *@ Time: Jan 7, 2025
  *@ Requirement：
  *@ Notes：
  */
void UiBorderUiStyle4(void* widget)
{
	UiBorderStruct* border = (UiBorderStruct*)widget;
	FillMode mode;
	mode = border->mode[(uint8_t)*border->anis->anie->Attribute];
	PlotRectangle(border->p, border->width, border->height, uiDevice->disBuffer, Fill0);
	PlotRectangle(border->p, border->width, border->height, uiDevice->disBuffer, mode);
}

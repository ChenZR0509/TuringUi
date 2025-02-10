/**
  *@ FileName: UiRootPage.c
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes:
  */
/* Includes ------------------------------------------------------------------*/
#include "UiRootPage.h"
#include "../Widget/UiNumberBox.h"
#include "../Widget/UiCheckBox.h"
#include "../Widget/UiLabel.h"
#include "../Widget/UiBorder.h"
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-varible
//-declare
void RootPageEventHandle(UiEvent* event);
/* Functions ----------------------------------------------------------------*/
/**
  *@ FunctionName:  void RootPageInit(void* data, void* params)
  *@ Author: CzrTuringB
  *@ Brief: 根页面初始化
  *@ Time: Jan 10, 2025
  *@ Requirement：
  */
void RootPageInit(void* data, void* params)
{
	//1、创建Ui页面
	uiPage = UiPageCreate();
	//2、动态创建样式参数
	FontSize* menuFontSize = pvPortMalloc(sizeof(FontSize));	//菜单字体
	if(menuFontSize == NULL) return;
	*menuFontSize = C8x16;
	FillMode* pageBoderMode = pvPortMalloc(sizeof(FillMode));	//页面边框填充模式
	if(pageBoderMode == NULL) return;
	*pageBoderMode = FillF;
	FillMode* overViewBoderMode = pvPortMalloc(sizeof(FillMode));	//页面边框填充模式
	if(overViewBoderMode == NULL) return;
	*overViewBoderMode = FillEBD7;
	//2、创建并初始化UI元素
	UiBorderStruct* pageBoder = UiBorderCreate();//创建页面外边框
	UiBorderInit(pageBoder, (Point2D){0,0}, 127, 63, 8, pageBoderMode, UiBorderUiStyle1);

	UiBorderStruct* overViewBoder = UiBorderCreate();//创建页面内边框
	UiBorderInit(overViewBoder, (Point2D){67,3}, 57, 57, 5, overViewBoderMode, UiBorderUiStyle1);

	UiLabelStruct* menuLabel = UiLabelCreate();//创建菜单标签
	UiLabelInit(menuLabel, menuFontSize, (Point2D){13,44}, "Terminal", UiLabelUiStyle2);
	UiLabelAnimInit(menuLabel, 40, 16, 500, UiLabelAnimation);
	//3、初始化Ui页面
	UiPageInit(uiPage, 3, (pnTreeNode)data, RootPageEventHandle);
	UiPageAddElements(&(pageBoder->base));
	UiPageAddElements(&(overViewBoder->base));
	UiPageAddElements(&(menuLabel->base));
	//将需要动画计算的UI元素包含到动画队列中
	UiAnimationAddElements(&(menuLabel->base));
	//将需要渲染绘制的UI元素包含到渲染队列中
	UiRenderAddElements(&(pageBoder->base));
	UiRenderAddElements(&(overViewBoder->base));
	UiRenderAddElements(&(menuLabel->base));
}
/**
  *@ FunctionName: void RootPageEventHandle(UiEvent* event)
  *@ Author: CzrTuringB
  *@ Brief: 页面事件处理器
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void RootPageEventHandle(UiEvent* event)
{
;
}
/**
  *@ FunctionName: void RootPageDestory(void)
  *@ Author: CzrTuringB
  *@ Brief: 页面注销函数：删除页面中包含的所有动态创建的UI部件
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void RootPageDestory(void)
{
	UiElement* ele = NULL;
	while(uxQueueMessagesWaiting(uiPage->elements))
	{
		xQueueReceive(uiPage->elements, &ele, portMAX_DELAY);
		if(ele != NULL)	ele->destory(ele);
	}
	UiPageDestroy(uiPage);
}

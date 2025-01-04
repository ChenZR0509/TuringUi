/**
  *@ FileName: UiCore.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
#ifndef __UICORE_H
#define __UICORE_H
/* Includes ------------------------------------------------------------------*/
#include "BspState.h"
#include "../Device/Ssd1306.h"
#include "DataStructure/nTree.h"
/* Define ------------------------------------------------------------------*/
#define CenterX (ScreenWidth/2)
#define CenterY (ScreenHeight/2)
/* Data ------------------------------------------------------------------*/
//二维坐标点
typedef struct
{
    int8_t x, y;
}Point2D;
//三维坐标点
typedef struct
{
    float x, y, z;
}Point3D;
//UiStyle
typedef enum
{
	//不同的UI部件，指定的UI类型不同
	style0,
	style1,
	style2,
	style3,
	style4,
}UiStyle;
//ui元素
typedef struct
{
	Bool isRender;								 	     // UI元素是否需要渲染
	Bool isEnabled;									     // UI元素是否响应事件
    void (*render)(void* self);           		 	 	 // 渲染函数
    void (*handleEvent)(void* self, uint8_t event);  	 // 事件处理函数
}UiElement;
//ui页面
typedef struct
{
	pTreeNode base;				 // Ui页面的基类(指向索引树)
	uint8_t index;				 // Ui页面的索引(用于子节点数组的索引)
    UiElement** elements;   	 // 存储页面的所有UI元素
    uint8_t eleCount;      		 // 当前页面包含的UI元素数量
}UiPage;
/* Extern ------------------------------------------------------------------*/
extern UiPage* uiPage;
/* Functions------------------------------------------------------------------*/
uint32_t UiGetRandom(uint32_t min, uint32_t max);
void UiElementInit(UiElement ele, void (*render)(void*), void (*handle)(void*, void*));
UiPage* UiPageCreate(void);
void UiPageInit(UiPage* page, UiElement** elements, uint8_t count, pTreeNode node);
void UiPageDestroy(UiPage* page);
void UiPageRender(UiPage* page);
void UiPageEventHandle(UiPage* page, uint8_t event);
pTreeNode UiPageEnter(UiPage* page);
pTreeNode UiPageQuit(UiPage* page);
#endif

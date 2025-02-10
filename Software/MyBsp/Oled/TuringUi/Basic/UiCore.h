/**
  *@ FileName：UiCore.h
  *@ Author：CzrTuringB
  *@ Brief：TuringUi的内核实现
  *@ Time：Jan 15, 2025
  *@ Requirement：
  *@ Notes：
  */
#ifndef __UICORE_H
#define __UICORE_H
/* Includes" "------------------------------------------------------------------*/
#include "BspCore.h"
#include "Shell/Shell.h"
#include "../Device/Ssd1306.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "DataStructure/nTree.h"
#include "DataStructure/List.h"
/* Includes< >------------------------------------------------------------------*/
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
#define CenterX (ScreenWidth/2)			//屏幕中心点坐标
#define CenterY (ScreenHeight/2)		//屏幕中心点坐标
//-enum
typedef enum EventType
{
	EventButton,		   //按键事件
    EventTerminal,		   //终端事件
}EventType;
//-typedef(类型重命名)
typedef void (*UiStyle)(void*);			//UI样式函数指针，用以支持不同的Ui样式渲染
typedef struct UiElement UiElement;
typedef struct UiElement* pUiElement;
typedef int8_t UiType;    				//我的屏幕是128*64的所以我选择为int_8类型(注意更换屏幕尺寸时需要更改，且最好选择有符号整型)
typedef uint8_t uUiType;				//用于遍历的无符号类型
//-struct
typedef struct
{
	UiType x, y;
}Point2D;								//二维坐标点
typedef struct
{
    float x, y, z;
}Point3D;
//-SVG图像相关
typedef struct SVG
{
	char command;					//命令字符
	uint8_t num;					//数据量
	float data[6];					//数据数组的指针
}SVG,*pSVG;
//三维坐标点
typedef struct UiEvent
{
	EventType	type;					//存储事件类型
	uint8_t 	id;						//存储事件ID，用于分辨不同外设的同类型事件，如多个按键的单击事件；
	void* 		data;					//存储事件携带的数据
}UiEvent,*pUiEvent;
struct UiElement						//Ui元素
{
	Bool isHandle;											// UI元素是否需要处理事件
	Bool* isAniFinish;										// 动画是否完成
	void* widget;										 	// 指向UI部件
    void (*render)(pUiElement ele);           		 	 	// 渲染函数
    void (*animation)(pUiElement ele);           		 	// 动画函数
    void (*handleEvent)(pUiElement ele, void* value);  	 	// 事件处理函数
    void (*destory)(pUiElement ele);						// 析构函数
};
typedef struct UiPage
{
	pnTreeNode base;				 		     			// Ui页面的基类(指向索引树)
	uint8_t index;				 				 			// Ui页面的索引(用于子节点数组的索引)
    uint8_t eleCount;      		 							// 当前页面包含的UI元素数量
    xQueueHandle elements;   					 		 	// 存储页面的所有UI元素(队列元素存储的是Ui元素的指针)
    xSemaphoreHandle elesAniMutex;							// 动画队列的锁
    pList elesAnimation;						 	 		// 当前页面的UI元素动画链表(链表节点存储的是Ui元素的指针)
    xQueueHandle elesRender;								// 当前页面的UI元素渲染队列(队列元素存储的是Ui元素的指针)
    void (*handleEvent)(pUiEvent event);  	 	 			// 事件处理函数
}UiPage,*pUiPage;
//-variable
extern pUiPage uiPage;
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
//-随机数计算
uint32_t UiGetRandom(uint32_t min, uint32_t max);
//-Ui元素的对外接口
pUiElement UiElementCreate(void);
BspState UiElementInit(pUiElement ele, void* widget, void (*render)(pUiElement), void (*handle)(pUiElement, void*), void (*destory)(pUiElement));
void UiElementDestroy(pUiElement ele);
//-Ui页面的对外接口
pUiPage UiPageCreate(void);
BspState UiPageInit(pUiPage page, uint8_t count, pnTreeNode node, void (*handleEvent)(pUiEvent));
void UiPageDestroy(pUiPage page);
BspState UiPageAddElements(pUiElement* ele);
BspState UiRenderAddElements(pUiElement* ele);
BspState UiRenderTakeElements(pUiElement* ele);
uint8_t GetUiRenderEleCounts(void);
BspState UiAnimationAddElements(pUiElement* ele);
pnTreeNode UiPageEnter(pUiPage page);
pnTreeNode UiPageQuit(pUiPage page);
#endif

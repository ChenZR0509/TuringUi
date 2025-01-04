/**
  *@ FileName: UiCore.c
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiCore.h"
#include "rng.h"
/* Data ------------------------------------------------------------------*/
UiPage* uiPage;
/* Functions ------------------------------------------------------------------*/
//-随机数的生成
/**
  *@ FunctionName: uint32_t UiGetRandom(uint32_t min, uint32_t max)
  *@ Author: CzrTuringB
  *@ Brief: 在指定范围内生成随机数
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
uint32_t UiGetRandom(uint32_t min, uint32_t max)
{
    if (min > max)  return 0;
    uint32_t randomValue;
    // 获取随机数
    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomValue) != HAL_OK) return 0;
    // 映射到 [min, max] 范围
    return (randomValue % (max - min + 1)) + min;
}
//-Ui元素的实现
/**
  *@ FunctionName: void UiElementInit(UiElement ele, void (*render)(void*), void (*handle)(void*, void*))
  *@ Author: CzrTuringB
  *@ Brief: 初始化UI元素
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ 	1、render即为UI元素渲染函数的指针
  *@ 	2、handle即为UI元素事件处理函数的指针
  */
void UiElementInit(UiElement ele, void (*render)(void*), void (*handle)(void*, void*))
{
	ele.isRender = True;
	ele.render = render;
	//ele.handleEvent = handle;
}
//-Ui页面的实现
/**
  *@ FunctionName: UiPage* UiPageCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
UiPage* UiPageCreate(void)
{
    //动态分配 UiPage 内存
    UiPage* page = pvPortMalloc(sizeof(UiPage));
    if (page == NULL)
    {
        //如果分配失败，返回 NULL
        return NULL;
    }
    //返回创建的UI页面指针
    return page;
}
/**
  *@ FunctionName: void UiPageInit(UiPage* page, UiElement** elements, uint8_t count, pTreeNode node)
  *@ Author: CzrTuringB
  *@ Brief: 初始化UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ 	1、elements即为ui元素指针数组
  *@ 	2、count即为页面包含的ui元素的个数
  *@ 	3、node即为页面所在菜单索引树的节点
  */
void UiPageInit(UiPage* page, UiElement** elements, uint8_t count, pTreeNode node)
{
    page->elements = elements;  	// 设置传入的元素数组
    page->eleCount = count;     	// 设置当前元素数量
    page->index = 0;				// 下一级页面的索引
    page->base = node;
}
/**
  *@ FunctionName: void UiPageDestroy(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 销毁UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void UiPageDestroy(UiPage* page)
{
    if (page != NULL)
    {
        //释放元素数组的内存
        if (page->elements != NULL)
        {
            vPortFree(page->elements);
        }
        //释放UiPage自身的内存
        vPortFree(page);
    }
}
/**
  *@ FunctionName: void UiPageRender(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 页面渲染器
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void UiPageRender(UiPage* page)
{
	//遍历页面上的每个UI元素并执行渲染工作
    for (uint8_t i = 0; i < page->eleCount; i++)
    {
    	if(page->elements[i]->isRender == True)
    	{
    		page->elements[i]->render(page->elements[i]);
    	}
    }
}
/**
  *@ FunctionName: void UiPageEventHandle(UiPage* page, uint8_t event)
  *@ Author: CzrTuringB
  *@ Brief: 页面事件处理器
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void UiPageEventHandle(UiPage* page, uint8_t event)
{
	//遍历页面上的每个UI元素并执行相关事件动作
	for (uint8_t i = 0; i < page->eleCount; i++)
    {
        page->elements[i]->handleEvent(page->elements[i], event);
    }
}
/**
  *@ FunctionName: pTreeNode UiPageEnter(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 进入下一级页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
pTreeNode UiPageEnter(UiPage* page)
{
	pTreeNode node;
	if(page->base->children == NULL)	return NULL;
	node = page->base->children[page->index];
	UiPageDestroy(page);
	return node;
}
/**
  *@ FunctionName: pTreeNode UiPageQuit(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 返回上一级页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
pTreeNode UiPageQuit(UiPage* page)
{
	pTreeNode node;
	if(page->base->parent == NULL)	return NULL;
	node = page->base->parent;
	UiPageDestroy(page);
	return node;
}

/**
  *@ FileName：UiCore.c
  *@ Author：CzrTuringB
  *@ Brief：TuringUi的内核实现
  *@ Time：Jan 15, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "UiCore.h"
#include "rng.h"
#include "string.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
pUiPage uiPage;				//ui页面(全局变量)指向程序运行期间当前时刻渲染的页面指针
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
//-随机数的生成
/**
  *@ FunctionName: uint32_t UiGetRandom(uint32_t min, uint32_t max)
  *@ Author: CzrTuringB
  *@ Brief: 在指定范围内生成随机数
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes：
  */
uint32_t UiGetRandom(uint32_t min, uint32_t max)
{
	//1、检查形参有效性
    if (min > max)  return 0;
    uint32_t randomValue;
    //2、获取随机数
    if (HAL_RNG_GenerateRandomNumber(&hrng2, &randomValue) != HAL_OK) return 0;
    //3、映射到 [min, max] 范围
    return (randomValue % (max - min + 1)) + min;
}
//-Ui元素
/**
  *@ FunctionName: pUiElement UiElementCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 创建一个UI元素
  *@ Time: Jan 9, 2025
  *@ Requirement：
  *@ Notes：
  */
pUiElement UiElementCreate(void)
{
	//1、动态创建一个Ui元素
	UiElement* ele = pvPortMalloc(sizeof(UiElement));
	if(ele == NULL)	return NULL;
	memset(ele, 0, sizeof(UiElement));
	return ele;
}
/**
  *@ FunctionName: BspState UiElementInit(pUiElement ele, void* widget, void (*render)(UiElement*), void (*handle)(UiElement*, void*), void (*destory)(pUiElement))
  *@ Author: CzrTuringB
  *@ Brief: 初始化UI元素
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes：
  *@ 	1、ele即为Ui元素指针
  *@ 	2、widget即为此Ui元素对应的Ui部件的指针
  *@ 	3、render即为Ui元素对应的Ui部件渲染函数的指针
  *@ 	4、handle即为Ui元素对应的Ui部件事件处理函数的指针
  *@ 	5、destory即为Ui元素对应的Ui部件析构函数的指针
  */
BspState UiElementInit(pUiElement ele, void* widget, void (*render)(UiElement*), void (*handle)(UiElement*, void*), void (*destory)(pUiElement))
{
	//1、检查参数有效性
	if(!ele || !widget || !render || !handle || !destory) return BspError;
	//2、配置Ui元素参数
	ele->widget = widget;
	ele->isHandle = True;
	ele->render = render;
	ele->handleEvent = handle;
	ele->destory = destory;
	ele->isAniFinish = NULL;
	ele->animation = NULL;
	//3、返回状态信息
	return BspOk;
}
/**
  *@ FunctionName: void UiElementDestroy(pUiElement ele)
  *@ Author: CzrTuringB
  *@ Brief: 销毁UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes:
  */
void UiElementDestroy(pUiElement ele)
{
    if (!ele) vPortFree(ele);
}
//-Ui页面的实现
/**
  *@ FunctionName: pUiPage UiPageCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes:
  */
pUiPage UiPageCreate(void)
{
    //1、动态分配 UiPage 内存
    UiPage* page = pvPortMalloc(sizeof(UiPage));
    if (page == NULL) return NULL;
    memset(page,0,sizeof(UiPage));
    return page;
}
/**
  *@ FunctionName: BspState UiPageInit(pUiPage page, uint8_t count, pnTreeNode node, void (*handleEvent)(pUiEvent))
  *@ Author: CzrTuringB
  *@ Brief: 初始化UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、page即为Ui页面指针，在图灵Ui中只能传入uiPage
  *@ 	2、count即为当前页面包含的Ui元素数
  *@ 	3、node即为当前页面在菜单索引树的节点指针
  *@ 	4、handleEvent即为当前页面的事件处理函数(每个不同的页面具有不同的事件处理函数)
  */
BspState UiPageInit(pUiPage page, uint8_t count, pnTreeNode node, void (*handleEvent)(pUiEvent))
{
	//1、检查参数有效性
	if(!page || !handleEvent) return BspError;
	//2、配置Ui页面属性
    page->eleCount = count;
    page->index = 0;
    page->base = node;
    page->handleEvent = handleEvent;
    //2、创建Ui页面元素队列
    page->elements = xQueueCreate(count,sizeof(UiElement*));
    if(!page->elements) return BspError;
    //3、创建Ui页面动画链表及其锁
    page->elesAniMutex = xSemaphoreCreateMutex();
    if(!page->elesAniMutex) return BspError;
    page->elesAnimation = ListCreate();
    if(!page->elesAnimation) return BspError;
    //创建一块内存用于存储后续需要渲染处理的元素
    page->elesRender = xQueueCreate(count,sizeof(UiElement*));
    if(!page->elements) return BspError;
    return BspOk;
}
/**
  *@ FunctionName: void UiPageDestroy(pUiPage page)
  *@ Author: CzrTuringB
  *@ Brief: 销毁UI页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes:
  */
void UiPageDestroy(pUiPage page)
{
    if (!page)
    {
    	if(!page->elements) vQueueDelete(page->elements);
    	if(!page->elesRender) vQueueDelete(page->elesRender);
    	if(!page->elesAnimation) ListDestroy(page->elesAnimation);
    	if(!page->elesAniMutex) vSemaphoreDelete(page->elesAniMutex);
        vPortFree(page);
    }
}
/**
  *@ FunctionName: BspState UiPageAddElements(pUiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 向Ui页面中添加Ui元素
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState UiPageAddElements(pUiElement* ele)
{
	BaseType_t result = xQueueSend(uiPage->elements,ele,portMAX_DELAY);
	return result == pdPASS ? BspOk : BspError;
}
/**
  *@ FunctionName: BspState UiRenderAddElements(pUiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 向Ui渲染队列中添加Ui元素
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState UiRenderAddElements(pUiElement* ele)
{
	BaseType_t result = xQueueSend(uiPage->elesRender,ele,portMAX_DELAY);
	return result == pdPASS ? BspOk : BspError;
}
/**
  *@ FunctionName: uint8_t GetUiRenderEleCounts(void)
  *@ Author: CzrTuringB
  *@ Brief: 读取Ui渲染队列中的元素个数
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  */
uint8_t GetUiRenderEleCounts(void)
{
	uint8_t count = uxQueueMessagesWaiting(uiPage->elesRender);
	return count;
}
/**
  *@ FunctionName: BaseType_t result = xQueueReceive(uiPage->elesRender, ele, portMAX_DELAY);
  *@ Author: CzrTuringB
  *@ Brief: 从Ui渲染队列中取出一个Ui元素
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState UiRenderTakeElements(pUiElement* ele)
{
	BaseType_t result = xQueueReceive(uiPage->elesRender, ele, portMAX_DELAY);
	return result == pdPASS ? BspOk : BspError;
}
/**
  *@ FunctionName: BspState UiAnimationAddElements(pUiElement* ele)
  *@ Author: CzrTuringB
  *@ Brief: 向Ui动画链表中添加Ui元素
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState UiAnimationAddElements(pUiElement* ele)
{
	//1、创建一个链表节点
	pListNode node = ListNodeCreate(ele, sizeof(pUiElement), NULL);
	if(node == NULL)	return BspError;
	//2、在链表头节点插入
	BspState result = ListInsert(uiPage->elesAnimation, node, NULL);
	//3、返回状态
	return result;
}
/**
  *@ FunctionName: pnTreeNode UiPageEnter(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 进入下一级页面
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes:
  */
pnTreeNode UiPageEnter(pUiPage page)
{
	pnTreeNode node;
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
pnTreeNode UiPageQuit(pUiPage page)
{
	pnTreeNode node;
	if(page->base->parent == NULL)	return NULL;
	node = page->base->parent;
	UiPageDestroy(page);
	return node;
}
/* Functions ------------------------------------------------------------------*/

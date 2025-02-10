/**
  *@ FileName：UiRender.c
  *@ Author：CzrTuringB
  *@ Brief：Ui渲染
  *@ Time：Jan 15, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "UiRender.h"
#include "UiPlot.h"
#include "UiAnimation.h"
#include "UiPictureLibrary.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: void AnimationCalculate(void)
  *@ Author: CzrTuringB
  *@ Brief: 动画计算
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、链表是我自己的实现的，所以要注意线程保护
  */
void AnimationCalculate(void)
{
	xSemaphoreTake(uiPage->elesAniMutex, portMAX_DELAY);
	pListNode currentNode = uiPage->elesAnimation->frontNode;
	pListNode nextNode;
	while (currentNode != NULL)
	{
	    pUiElement ele = *((pUiElement*)currentNode->data); 	//获取当前节点的数据
	    nextNode = currentNode->next; 						 	//保存当前节点的下一个节点指针
	    ele->animation(ele);									//调用UI部件的动画处理函数进行计算
	    UiRenderAddElements(&ele);								//送入渲染队列
	    if(*(ele->isAniFinish) == True) 						//动画完成
	    {
	        ListRemove(uiPage->elesAnimation, currentNode);		//从动画链表中移除
	    }
	    currentNode = nextNode; 							  	// 移动到下一个节点
	}
	xSemaphoreGive(uiPage->elesAniMutex);
}
/**
  *@ FunctionName: void RenderCalculate(void)
  *@ Author: CzrTuringB
  *@ Brief: 渲染计算
  *@ Time: Jan 15, 2025
  *@ Requirement：
  *@ Notes:
  */
void RenderCalculate(void)
{
	pUiElement ele;
	while(GetUiRenderEleCounts())
	{
		UiRenderTakeElements(&ele);
		ele->render(ele);
	}
	SSDUpdateScreen();
}
/**
  *@ FunctionName: void UiEventHandle(void)
  *@ Author: CzrTuringB
  *@ Brief: TuringUi的事件管理任务函数
  *@ Time: Jan 6, 2025
  *@ Requirement：
  */
void UiRenderHandle(void)
{
	while(1)
	{
		RenderCalculate();
		AnimationCalculate();
	}
}

/**
  *@ FileName: UiNavigation.c
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiNavigation.h"
#include "UiRootPage.h"
/* Data ------------------------------------------------------------------*/
pTreeNode rootNode;
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: void MenuTreeInit(void)
  *@ Author: CzrTuringB
  *@ Brief: 初始化菜单页面索引树
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ 	1、这个页面索引树贯穿整个程序生命周期,所以不用销毁分配的空间
  *@ 	2、在程序编写时就要确定好整个UI框架的目录结构，不要再程序运行期间添加节点
  */
void MenuTreeInit(void)
{
	//根页面节点的创建
	rootNode = CreateNode("rootPage", sizeof("rootPage"), RootPageInit);
	//向跟页面添加子节点
//	for(uint8_t i=0; i<rootPage->num; i++)
//	{
//		AddChild(rootPage, menu1[i]);
//	}
}

/**
  *@ FileName: UiRootPage.c
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiRootPage.h"
#include "../Widget/UiNumberBox.h"
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-varible
/* Functions ----------------------------------------------------------------*/
void RootPageInit(void* data, void* params)
{
	//创建Ui页面
	uiPage = UiPageCreate();
	//初始化Ui页面
	//UiPageInit(uiPage, elements, 3, (pTreeNode)data);
}

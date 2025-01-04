/**
  *@ FileName: nTree.h
  *@ Author: CzrTuringB
  *@ Brief: N叉树
  *@ Time: Sep 22, 2024
  *@ Requirement：
  */
#ifndef __NTREE_H
#define __NTREE_H
/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BspState.h"
#include "FreeRTOS.h"
#include "task.h"
/* Extern ------------------------------------------------------------------*/
/**
  *@ FunctionName: void (*InitFunc)(void* data, void* params)
  *@ Author: CzrTuringB
  *@ Brief: 初始化函数指针，可以是任意数据结构的初始化
  *@ Time: Jan 3, 2025
  *@ Requirement：
  *@ 	1、data指针：指向的是目标数据的指针
  *@ 	2、params指针：指向的是存储初始化数据的指针
  *@ 	3、初始化函数将params指向的数据复制到data中
  */
typedef void (*InitFunc)(void* data, void* params);
/* Data ------------------------------------------------------------------*/
//定义树节点
typedef struct TreeNode
{
    void* data;							//指向树节点数据
    InitFunc initFunc;                  //存储初始化函数的指针
    size_t num;							//树节点个数
    struct TreeNode** children; 		//存储一个指向结构体数组的指针
    struct TreeNode* parent;			//存储父节点指针
}TreeNode, *pTreeNode;
/* Functions------------------------------------------------------------------*/
pTreeNode CreateNode(void* data, size_t dataSize, InitFunc initFunc);
void AddChild(pTreeNode parent, pTreeNode child);
void FreeTree(pTreeNode node);
#endif

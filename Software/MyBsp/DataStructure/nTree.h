/**
  *@ FileName: nTree.h
  *@ Author: CzrTuringB
  *@ Brief: N叉树
  *@ Time: Sep 22, 2024
  *@ Requirement：
  *@ Notes：
  */
#ifndef __NTREE_H
#define __NTREE_H
/* Includes" "------------------------------------------------------------------*/
#include <BspCore.h>
#include <stdlib.h>
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
typedef struct nTreeNode
{
    void* data;							//指向树节点数据
    InitFunc initFunc;                  //存储初始化函数的指针(一个树节点可以存储任意类型的数据，某些数据需要进行初始化)
    size_t num;							//树子节点个数
    struct nTreeNode** children; 		//存储一个指向结构体数组的指针
    struct nTreeNode* parent;			//存储父节点指针
}nTreeNode, *pnTreeNode;
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
pnTreeNode NTreeNodeCreate(void* data, size_t dataSize, InitFunc initFunc);
BspState NTreeNodeInit(pnTreeNode node, void* data, size_t dataSize, InitFunc initFunc);
BspState AddNTreeChild(pnTreeNode parent, pnTreeNode child);
BspState FreeTree(pnTreeNode node);
#endif

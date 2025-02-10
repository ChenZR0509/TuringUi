/**
  *@ FileName: nTree.c
  *@ Author: CzrTuringB
  *@ Brief: N叉树
  *@ Time: Sep 22, 2024
  *@ Requirement：
  *@ 	1、FreeRTOS Heap4内存管理方式
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "nTree.h"
#include "FreeRTOS.h"
/* Includes< >------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: pnTreeNode CreateNTreeNode(void* data, size_t dataSize, InitFunc initFunc)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个节点，并完成节点的初始化
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes：
  */
pnTreeNode NTreeNodeCreate(void* data, size_t dataSize, InitFunc initFunc)
{
    //1、分配节点内存
    pnTreeNode node = (pnTreeNode)pvPortMalloc(sizeof(nTreeNode));
    if (!node)
    {
        //内存分配失败，输出调试信息
        return NULL;
    }
    //2、分配数据内存并复制数据
    node->data = pvPortMalloc(dataSize);
    if (!node->data)
    {
    	//内存分配失败，输出调试信息
        vPortFree(node);
        return NULL;
    }
    memcpy(node->data, data, dataSize);
    //3、初始化子节点信息
    node->num = 0;
    node->children = NULL;
    node->parent = NULL;
    //4、初始化节点的初始化函数指针
    node->initFunc = initFunc;
    //5、返回节点指针
    return node;
}
/**
  *@ FunctionName: BspState NTreeNodeInit(pnTreeNode node, void* data, size_t dataSize, InitFunc initFunc)
  *@ Author: CzrTuringB
  *@ Brief: 树节点的初始化
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  *@	1、适用于静态节点的初始化
  */
BspState NTreeNodeInit(pnTreeNode node, void* data, size_t dataSize, InitFunc initFunc)
{
	//1、如果形参指针为空，返回或输出打印信息
	if (!node || !data || !initFunc) return BspError;
	//2、分配数据内存并复制数据
	node->data = pvPortMalloc(dataSize);
	if (!node->data)
	{
		//内存分配失败，输出调试信息
		vPortFree(node);
		return BspError;
	 }
	memcpy(node->data, data, dataSize);
	//3、初始化子节点信息
	node->num = 0;
	node->children = NULL;
	node->parent = NULL;
	node->initFunc = initFunc;
	return BspOk;
}
/**
  *@ FunctionName: BspState AddChild(pTreeNode parent, pTreeNode child)
  *@ Author: CzrTuringB
  *@ Brief: 添加子节点
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes：
  */
BspState AddNTreeChild(pnTreeNode parent, pnTreeNode child)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (!parent || !child) return BspError;
    //2、调整子节点数组大小
    parent->children = (pnTreeNode*)pvPortMalloc(sizeof(pnTreeNode) * (parent->num + 1));
    if (!parent->children)
    {
        //内存分配失败，输出打印信息
    	return BspError;
    }
    //3、添加新子节点
    parent->children[parent->num++] = child;
    child->parent = parent;
    return BspOk;
}
/**
  *@ FunctionName: BspState freeTree(pTreeNode node)
  *@ Author: CzrTuringB
  *@ Brief: 释放树
  *@ Time: Jan 2, 2025
  *@ Requirement：
  *@ Notes：
  */
BspState FreeTree(pnTreeNode node)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (!node) return BspError;
    //2、递归释放所有子节点
    for (size_t i = 0; i < node->num; i++)
    {
        FreeTree(node->children[i]);
    }
    //释放子节点数组和节点数据
    vPortFree(node->children);
    vPortFree(node->data);
    vPortFree(node);
    return BspOk;
}

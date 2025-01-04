/**
  *@ FileName: nTree.c
  *@ Author: CzrTuringB
  *@ Brief: N叉树
  *@ Time: Sep 22, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "nTree.h"
/* Data ------------------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: pTreeNode CreateNode(void* data, size_t dataSize, InitFunc initFunc)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个节点
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
pTreeNode CreateNode(void* data, size_t dataSize, InitFunc initFunc)
{
    //分配节点内存
    pTreeNode node = (pTreeNode)pvPortMalloc(sizeof(TreeNode));
    if (!node)
    {
        //内存分配失败，输出调试信息
        return NULL;
    }
    //分配数据内存并复制数据
    node->data = pvPortMalloc(dataSize);
    if (!node->data)
    {
    	//内存分配失败，输出调试信息
        vPortFree(node);
        return NULL;
    }
    memcpy(node->data, data, dataSize);
    //初始化子节点信息
    node->num = 0;
    node->children = NULL;
    node->parent = NULL;
    //初始化节点的初始化函数指针
    node->initFunc = initFunc;
    //返回节点指针
    return node;
}
/**
  *@ FunctionName: void AddChild(pTreeNode parent, pTreeNode child)
  *@ Author: CzrTuringB
  *@ Brief: 添加子节点
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void AddChild(pTreeNode parent, pTreeNode child)
{
	//如果形参指针为空，返回或输出打印信息
    if (!parent || !child) return;
    //调整子节点数组大小
    parent->children = (pTreeNode*)pvPortMalloc(sizeof(pTreeNode) * (parent->num + 1));
    if (!parent->children)
    {
        //内存分配失败，输出打印信息
        return;
    }
    // 添加新子节点
    parent->children[parent->num++] = child;
    child->parent = parent;
}
/**
  *@ FunctionName: void freeTree(pTreeNode node)
  *@ Author: CzrTuringB
  *@ Brief: 释放树
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void FreeTree(pTreeNode node)
{
    if (!node) return;
    //递归释放所有子节点
    for (size_t i = 0; i < node->num; i++)
    {
        FreeTree(node->children[i]);
    }
    //释放子节点数组和节点数据
    vPortFree(node->children);
    vPortFree(node->data);
    vPortFree(node);
}

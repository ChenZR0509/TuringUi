/**
  *@ FileName: List.c
  *@ Author: CzrTuringB
  *@ Brief: 链表
  *@ Time:  Jan 11, 2025
  *@ Requirement：
  *@ Notes:
  */
/* Includes" "------------------------------------------------------------------*/
#include "List.h"
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
  *@ FunctionName: pListNode ListNodeCreate(void* data, size_t dataSize, InitFunc initFunc)
  *@ Author: CzrTuringB
  *@ Brief: 创建链表节点，并初始化
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、动态创建一个节点，并对节点的相关属性进行初始化
  *@ 	2、依据数据类型动态分配一块内存用于存储节点数据据，将形参void* data指向的数据复制到节点的data属性
  */
pListNode ListNodeCreate(void* data, size_t dataSize, InitFunc initFunc)
{
	//1、分配节点内存，若分配失败则返回空地址
	pListNode node = (pListNode )pvPortMalloc(sizeof(ListNode));
    if (!node)	return NULL;
    //2、初始化链表节点
	node->data = pvPortMalloc(dataSize);
	if (!node->data)
	{
		//内存分配失败，输出调试信息
		vPortFree(node);
		return BspError;
	 }
	memcpy(node->data, data, dataSize);
    //3、初始化子节点信息
	node->initFunc = initFunc;
	node->next = NULL;
	node->prev = NULL;
	//4、返回节点指针
	return node;
}
/**
  *@ FunctionName: BspState ListNodeInit(pListNode node, void* data, size_t dataSize, InitFunc initFunc)
  *@ Author: CzrTuringB
  *@ Brief: 节点初始化
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、依据数据类型动态分配一块内存用于存储节点数据据，将形参void* data指向的数据复制到节点的data属性
  */
BspState ListNodeInit(pListNode node, void* data, size_t dataSize, InitFunc initFunc)
{
	//1、如果形参指针为空，返回或输出打印信息
	if(!node || !data || !initFunc) return BspError;
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
	node->initFunc = initFunc;
	node->next = NULL;
	node->prev = NULL;
	return BspOk;
}
/**
  *@ FunctionName: BspState ListNodeDestory(pListNode node)
  *@ Author: CzrTuringB
  *@ Brief: 销毁节点
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、删除节点的数据内存
  *@ 	2、删除节点内存
  */
BspState ListNodeDestory(pListNode node)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (node == NULL) return BspError;
    //2、释放节点的数据内存
    if (node->data != NULL)
    {
        vPortFree(node->data);
        node->data = NULL;
    }
    //3、释放节点本身的内存
    vPortFree(node);
    node = NULL;
	return BspOk;
}
/**
  *@ FunctionName: pList ListCreate(void)
  *@ Author: CzrTuringB
  *@ Brief: 创建链表
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes：
  */
pList ListCreate(void)
{
	//1、分配节点内存，若分配失败则返回空地址
	pList list = (pList)pvPortMalloc(sizeof(List));
    if (!list)	return NULL;
    //2、重置节点内存数据
	list->frontNode = NULL;
	list->tailNode = NULL;
	list->count = 0;
	return list;
}
/**
  *@ FunctionName: BspState ListInit(pList list)
  *@ Author: CzrTuringB
  *@ Brief: 链表初始化
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ListInit(pList list)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (list == NULL) return BspError;
    //2、初始化节点信息
	list->frontNode = NULL;
	list->tailNode = NULL;
	list->count = 0;
	return BspOk;
}
/**
  *@ FunctionName: BspState ListDestroy(pList list)
  *@ Author: CzrTuringB
  *@ Brief: 析构链表，释放链表及其所有节点的内存
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ListDestroy(pList list)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (!list)	return BspError;
    //2、遍历链表的所有节点，并删除
    pListNode currentNode = list->frontNode;
    while (currentNode)
    {
        pListNode nextNode = currentNode->next;
        ListNodeDestory(currentNode);
        currentNode = nextNode;
    }
    //3、释放链表结构体内存
    vPortFree(list);
    return BspOk;
}
/**
  *@ FunctionName: BspState ListInsert(pList list, pListNode newNode, pListNode prevNode)
  *@ Author: CzrTuringB
  *@ Brief: 在链表的任意位置插入一个节点
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes：
  */
BspState ListInsert(pList list, pListNode newNode, pListNode prevNode)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (!list || !newNode) return BspError;
    //2、判断是否插入到链表的头节点
    if (prevNode == NULL)
    {
        newNode->next = list->frontNode;
        newNode->prev = NULL;
        if (list->frontNode)
            list->frontNode->prev = newNode;
        else
            list->tailNode = newNode;
        list->frontNode = newNode;
    }
    //3、中间节点的插入
    else
    {
        newNode->next = prevNode->next;
        newNode->prev = prevNode;
        if (prevNode->next)
            prevNode->next->prev = newNode;
        else
            list->tailNode = newNode;
        prevNode->next = newNode;
    }
    //4、增加链表节点计数值
    list->count += 1;
    return BspOk;
}
/**
  *@ FunctionName: BspState ListRemove(pList list, pListNode node)
  *@ Author: CzrTuringB
  *@ Brief: 删除链表中指定的节点
  *@ Time: Jan 11, 2025
  *@ Requirement：
  *@ Notes：
  */
BspState ListRemove(pList list, pListNode node)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (!list || !node) return BspError;
    //2、是否为头节点
    if (node == list->frontNode)
    {
        list->frontNode = node->next;
        if (list->frontNode)
            list->frontNode->prev = NULL;
        else
            list->tailNode = NULL;
    }
    //3、是否为尾节点
    else if (node == list->tailNode)
    {
        list->tailNode = node->prev;
        if (list->tailNode) // 如果链表非空，更新新尾节点的 next
            list->tailNode->next = NULL;
        else // 如果链表为空，更新头节点
            list->frontNode = NULL;
    }
    //4、是否为中间节点
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    //5、释放节点内存
    if (node->data) vPortFree(node->data); // 释放数据内存
    vPortFree(node); 					   // 释放节点本身
    //6、更新链表节点计数值
    list->count -= 1;
    return BspOk;
}

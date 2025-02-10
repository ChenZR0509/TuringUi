/**
  *@ FileName: List.H
  *@ Author: CzrTuringB
  *@ Brief: 链表
  *@ Time: Date
  *@ Requirement：
  *@ Notes:
  */
#ifndef __LIST_H
#define __LIST_H
/* Includes" "------------------------------------------------------------------*/
#include <BspCore.h>
#include <stdlib.h>
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
//定义链表节点结构
typedef struct ListNode
{
    void* data;							//指向单向链表节点数据
    struct ListNode* next;    			//指向下一个节点
    struct ListNode* prev;    			//指向下一个节点
    InitFunc initFunc;                  //存储初始化函数的指针
}ListNode, *pListNode;
//定义链表
typedef struct
{
	pListNode frontNode;				//存储链表的头节点
	pListNode tailNode;					//存储链表的尾节点
	size_t count;						//存储链表包含的节点数
}List, *pList;
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
pListNode ListNodeCreate(void* data, size_t dataSize, InitFunc initFunc);
BspState ListNodeInit(pListNode node, void* data, size_t dataSize, InitFunc initFunc);
BspState ListNodeDestory(pListNode node);
pList ListCreate(void);
BspState ListInit(pList list);
BspState ListDestroy(pList list);
BspState ListInsert(pList list, pListNode newNode, pListNode prevNode);
BspState ListRemove(pList list, pListNode node);
#endif

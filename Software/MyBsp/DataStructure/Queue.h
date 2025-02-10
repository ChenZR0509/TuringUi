/**
  *@ FileName: Queue.h
  *@ Author: CzrTuringB
  *@ Brief: 队列的实现
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
#ifndef __QUEUE_H
#define __QUEUE_H
/* Includes" "------------------------------------------------------------------*/
#include <BspCore.h>
#include <stdlib.h>
/* Functions------------------------------------------------------------------*/
/* Data(对外接口)-----------------------------------------------------*/
//-define
//-enum
//-typedef(类型重命名)
//-struct
//定义队列结构
typedef struct Queue
{
	    size_t elementSize;  	// 单个元素的大小
	    size_t capacity;       	// 队列容量（最大元素数）
	    size_t head;            // 队列头索引
	    size_t tail;            // 队列尾索引
	    size_t count;          	// 队列当前元素数
	    void* pdQueue;		 	// 存储数据的数组指针
}Queue,*pQueue;
//-variable
/* Functions Declare(对外接口函数)------------------------------------------------------------------*/
pQueue QueueCreate(size_t dataSize, size_t capacity);
BspState QueueInit(pQueue sQueue, size_t size, size_t capacity, void* pdQueue);
BspState QueueDestory(pQueue sQueue);
BspState QueueInElement(pQueue sQueue, const void *data);
BspState QueueOutElement(pQueue sQueue, void *data);
BspState QueueGetElementAt(pQueue sQueue, size_t index, void* data);
BspState QueueDeleteElementAt(pQueue sQueue, size_t index);
#endif

/**
  *@ FileName: Queue.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Sep 22, 2024
  *@ Requirement：
  */
#ifndef __QUEUE_H
#define __QUEUE_H
/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "BspState.h"
/* Data ------------------------------------------------------------------*/
typedef struct Queue
{
	    size_t elementSize;  // 单个元素的大小
	    size_t capacity;     // 队列容量（最大元素数）
	    size_t head;         // 队列头指针
	    size_t tail;         // 队列尾指针
	    size_t count;        // 队列当前元素数
	    void* pdQueue;		 // 存储数据的数组指针
}Queue,*pQueue;
/* Functions------------------------------------------------------------------*/
/**
  *@ FunctionName: BspState QueueInit(pQueue sQueue, size_t size, size_t capacity, void* pdQueue)
  *@ Author: CzrTuringB
  *@ Brief: 初始化队列相关配置
  *@ Time: Dec 23, 2024
  *@ Requirement：
  */
BspState QueueInit(pQueue sQueue, size_t size, size_t capacity, void* pdQueue);
/**
  *@ FunctionName: BspState QueueInElement(pQueue sQueue, const void *data)
  *@ Author: CzrTuringB
  *@ Brief: 将一个数据送入队列中
  *@ Time: Dec 23, 2024
  *@ Requirement：
  */
BspState QueueInElement(pQueue sQueue, const void *data);
/**
  *@ FunctionName: BspState QueueOutElement(pQueue sQueue, const void *data)
  *@ Author: CzrTuringB
  *@ Brief: 将一个数据从队列中读出
  *@ Time: Dec 23, 2024
  *@ Requirement：
  */
BspState QueueOutElement(pQueue sQueue, void *data);
#endif

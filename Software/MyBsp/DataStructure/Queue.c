/**
  *@ FileName: QUEUE.c
  *@ Author: CzrTuringB
  *@ Brief: 环形队列
  *@ Time: Sep 22, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "Queue.h"
/* Data ------------------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
BspState QueueInit(pQueue sQueue, size_t size, size_t capacity, void* pdQueue)
{
    if (sQueue == NULL || pdQueue == NULL || capacity == 0)
    {
        return BspError;  // 无效参数
    }
    sQueue->elementSize = size;
    sQueue->capacity = capacity;
    sQueue->count = 0;
    sQueue->head = 0;
    sQueue->tail = 0;
    sQueue->pdQueue = pdQueue;
    return BspOk;
}
BspState QueueInElement(pQueue sQueue, const void *data)
{
	if(sQueue->count == sQueue->capacity)
	{
		//队列满了
		return BspError;
	}
	//计算数据应存入队列的所在位置
    void *dest = (char *)sQueue->pdQueue + (sQueue->tail * sQueue->elementSize);
    memcpy(dest, data, sQueue->elementSize);
    //使用取模运算，实现环形队列的效果
    sQueue->tail = (sQueue->tail + 1) % sQueue->capacity;
    sQueue->count++;
    return BspOk;
}
BspState QueueOutElement(pQueue sQueue, void *data)
{
	if(sQueue->count == 0)
	{
		//队列为空
		return BspError;
	}
	//计算应从队列的那个位置取出数据
    void *src = (char *)sQueue->pdQueue + (sQueue->head * sQueue->elementSize);
    memcpy(data, src, sQueue->elementSize);
    //使用取模运输算，实现环形队列的效果
    sQueue->head = (sQueue->head + 1) % sQueue->capacity;
    sQueue->count--;
    return BspOk;
}

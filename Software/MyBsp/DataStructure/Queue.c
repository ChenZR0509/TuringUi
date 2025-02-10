/**
  *@ FileName: Queue.c
  *@ Author: CzrTuringB
  *@ Brief:  队列的实现
  *@ Time:  Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
/* Includes" "------------------------------------------------------------------*/
#include "Queue.h"
#include "FreeRTOS.h"
/* Includes< >------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <string.h>
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: pQueue QueueCreate(size_t dataSize, size_t capacity)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个队列
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
pQueue QueueCreate(size_t dataSize, size_t capacity)
{
	//1、分配队列内存，若分配失败则返回空地址
	pQueue queue = (pQueue)pvPortMalloc(sizeof(Queue));
    if (!queue)	return NULL;
    //2、初始化链表节点
    queue->count = 0;
    queue->elementSize = dataSize;
    queue->capacity = capacity;
    queue->head = 0;
    queue->tail = 0;
    //2、分配存储队列数据的数组内存，若分配失败则返回空地址
    queue->pdQueue = pvPortMalloc(capacity * dataSize);
    if(!queue->pdQueue)
    {
		vPortFree(queue);
		return NULL;
    }
	memset(queue->pdQueue,0,capacity * dataSize);
	//3、返回节点指针
	return queue;
}
/**
  *@ FunctionName: BspState QueueInit(pQueue sQueue, size_t size, size_t capacity, void* pdQueue)
  *@ Author: CzrTuringB
  *@ Brief: 队列的初始化
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState QueueInit(pQueue sQueue, size_t size, size_t capacity, void* pdQueue)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (sQueue == NULL || pdQueue == NULL || capacity == 0)
    {
        return BspError;  // 无效参数
    }
    //2、初始化队列信息
    sQueue->elementSize = size;
    sQueue->capacity = capacity;
    sQueue->count = 0;
    sQueue->head = 0;
    sQueue->tail = 0;
    sQueue->pdQueue = pdQueue;
    return BspOk;
}
/**
  *@ FunctionName: BspState QueueDestory(pQueue sQueue)
  *@ Author: CzrTuringB
  *@ Brief: 删除动态创建的队列
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState QueueDestory(pQueue sQueue)
{
	//1、如果形参指针为空，返回或输出打印信息
    if (sQueue == NULL)
    {
        return BspError;  // 无效参数
    }
    //2、释放数组的内存
    if (sQueue->pdQueue != NULL)
    {
        vPortFree(sQueue->pdQueue);
        sQueue->pdQueue = NULL;
    }
    //3、释放队列自身的内存
    vPortFree(sQueue);
	return BspOk;
}
/**
  *@ FunctionName: BspState QueueInElement(pQueue sQueue, const void *data)
  *@ Author: CzrTuringB
  *@ Brief: 添加一个数据
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState QueueInElement(pQueue sQueue, const void *data)
{
	//1、如果形参指针为空或队列满了，返回或输出打印信息
	if(sQueue->count == sQueue->capacity || sQueue == NULL)
	{
		return BspError;
	}
	//2、计算数据应存入队列的所在位置
    void *dest = (char *)sQueue->pdQueue + (sQueue->tail * sQueue->elementSize);
    memcpy(dest, data, sQueue->elementSize);
    //3、使用取模运算，实现环形队列的效果
    sQueue->tail = (sQueue->tail + 1) % sQueue->capacity;
    //4、更新队列计数值
    sQueue->count++;
    return BspOk;
}
/**
  *@ FunctionName: BspState QueueOutElement(pQueue sQueue, void *data)
  *@ Author: CzrTuringB
  *@ Brief: 取出一个数据
  *@ Time: Jan 13, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState QueueOutElement(pQueue sQueue, void *data)
{
	//1、如果形参指针为空或队列为空，返回或输出打印信息
	if(sQueue->count == 0 || sQueue == NULL)
	{
		return BspError;
	}
	//2、计算应从队列的那个位置取出数据
    void *src = (char *)sQueue->pdQueue + (sQueue->head * sQueue->elementSize);
    memcpy(data, src, sQueue->elementSize);
    //3、使用取模运输算，实现环形队列的效果
    sQueue->head = (sQueue->head + 1) % sQueue->capacity;
    //4、更新队列计数值
    sQueue->count--;
    return BspOk;
}
/**
  *@ FunctionName: BspState QueueGetElementAt(pQueue sQueue, size_t index, void* data)
  *@ Author: CzrTuringB
  *@ Brief: 从队列中取出一个指定位置的数据，但是并不更改队列信息
  *@ Time: Jan 9, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState QueueGetElementAt(pQueue sQueue, size_t index, void* data)
{
	//1、判断形参有效性
    if (sQueue == NULL || data == NULL || index >= sQueue->count)
    {
        return BspError;
    }
    //2、计算指定位置的数据在队列中的实际位置
    size_t actualIndex = (sQueue->head + index) % sQueue->capacity;
    void* src = (char*)sQueue->pdQueue + (actualIndex * sQueue->elementSize);
    //3、复制数据到用户提供的缓冲区
    memcpy(data, src, sQueue->elementSize);
    return BspOk;
}
/**
  *@ FunctionName: BspState QueueDeleteElementAt(pQueue sQueue, size_t index)
  *@ Author: CzrTuringB
  *@ Brief: 从队列中删除一个指定位置的数据
  *@ Time: Jan 10, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState QueueDeleteElementAt(pQueue sQueue, size_t index)
{
	//1、判断形参有效性
    if (sQueue == NULL || index >= sQueue->count)
    {
        return BspError;
    }
    //2、删除位置的实际索引
    size_t actualIndex = (sQueue->head + index) % sQueue->capacity;
    //3、如果删除的是队列中间的元素，将后续元素前移
    if (index < sQueue->count - 1)
    {
        size_t nextIndex = (actualIndex + 1) % sQueue->capacity;
        for (size_t i = 0; i < sQueue->count - index - 1; i++)
        {
            size_t fromIndex = (nextIndex + i) % sQueue->capacity;
            size_t toIndex = (actualIndex + i) % sQueue->capacity;
            void* src = (char*)sQueue->pdQueue + (fromIndex * sQueue->elementSize);
            void* dest = (char*)sQueue->pdQueue + (toIndex * sQueue->elementSize);
            memcpy(dest, src, sQueue->elementSize);
        }
    }
    //4、更新队尾指针（简化逻辑）
    sQueue->tail = (sQueue->tail + sQueue->capacity - 1) % sQueue->capacity;
    //5、更新队列元素计数
    sQueue->count--;
    return BspOk;
}

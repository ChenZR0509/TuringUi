/**
  *@ FileName: UiNumberBox.h
  *@ Author: CzrTuringB
  *@ Brief: 数字选择框
  *@ Time: Oct 6, 2024
  *@ Requirement：
  */
#ifndef __UINUMBERBOX_H
#define __UINUMBERBOX_H
/* Includes ------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
/* Data ------------------------------------------------------------------*/
typedef enum
{
	uint8,
	int8,
	float32
}DataType;
typedef struct
{
	Point2D   p;				//UI元素位置信息
	UiElement base;				//UI元素基结构体
	DataType type;  			//表示所存储的数据类型
	char str[10];				//存储转换后的字符串
	void *data;    				//指向实际数据的指针
	UiStyle style;				//UI样式
	FontSize size;				//字体样式
}NumberBoxStruct;
/* Function ------------------------------------------------------------------*/
#endif

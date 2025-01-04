/**
  *@ FileName: UiCheckBox.h
  *@ Author: CzrTuringB
  *@ Brief: 复选框
  *@ Time: Dec 17, 2024
  *@ Requirement：
  */
#ifndef __UICHECKBOX_H
#define __UICHECKBOX_H
/* Includes ------------------------------------------------------------------*/
#include "../Basic/UiPlot.h"
#include "BspState.h"
/* Define ------------------------------------------------------------------*/
/* Data ------------------------------------------------------------------*/
typedef enum
{
	//默认Ui：<v> <x>
	MarkCheck,
	//矩形填充选择：填充表示使能，无填充表示是失能
	FillRecCheck,
	//圆形填充选择：填充表示使能，无填充表示是失能
	FillCirCheck,
}CheckBoxStyle;
typedef struct
{
	//表示是否勾选
	EnDis check;
	//设置样式
	uint8_t* style;
	//设置尺寸：指定高度就好
	uint8_t* size;
}CheckBoxStruct;
/* Functions------------------------------------------------------------------*/
/**
  *@ FunctionName: void CheckBoxInit(CheckBoxStruct* box,CheckBoxStyle* style,EnDis check,uint8_t* size)
  *@ Author: CzrTuringB
  *@ Brief: 初始化复选框
  *@ Time: Dec 17, 2024
  *@ Requirement：
  */
void CheckBoxInit(CheckBoxStruct* box,CheckBoxStyle* style,EnDis check,uint8_t* size);
/**
  *@ FunctionName: void CheckBoxDeInit(CheckBoxStruct* box)
  *@ Author: CzrTuringB
  *@ Brief: 解初始化复选框
  *@ Time: Dec 17, 2024
  *@ Requirement：
  */
void CheckBoxDeInit(CheckBoxStruct* box);
/**
  *@ FunctionName: void CheckBoxDraw(uint8_t x, uint8_t y,CheckBoxStruct* box)
  *@ Author: CzrTuringB
  *@ Brief: 复选框的绘制
  *@ Time: Dec 17, 2024
  *@ Requirement：
  */
void CheckBoxDraw(uint8_t x, uint8_t y,CheckBoxStruct* box);
#endif

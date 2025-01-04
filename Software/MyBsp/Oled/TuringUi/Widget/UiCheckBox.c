/**
  *@ FileName: UiCheckBox.c
  *@ Author: CzrTuringB
  *@ Brief: 复选框
  *@ Time: Dec 17, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiCheckBox.h"
/* Data ------------------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
void CheckBoxInit(CheckBoxStruct* box,CheckBoxStyle* style,EnDis check,uint8_t* size)
{
	box->check = check;
	box->style = style;
	box->size = size;
}
void CheckBoxDeInit(CheckBoxStruct* box)
{
	//待添加代码;
}

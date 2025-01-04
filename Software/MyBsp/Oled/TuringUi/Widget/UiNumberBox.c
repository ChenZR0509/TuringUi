/**
 *@ FileName: UiNumberBox.c
 *@ Author: CzrTuringB
 *@ Brief: 数字选择框
 *@ Time: Oct 6, 2024
 *@ Requirement：
 */
/* Includes ------------------------------------------------------------------*/
#include "UiNumberBox.h"
#include "FreeRTOS.h"
/* Function ------------------------------------------------------------------*/
NumberBoxStruct* NumberBoxCreate(void* value, DataType type)
{
	NumberBoxStruct* box = pvPortMalloc(sizeof(NumberBoxStruct));
	if(box == NULL)	return NULL;
	box->type = (uint8_t)type;
	switch(box->type)
	{
		case uint8:
			box->data = pvPortMalloc(sizeof(uint8_t));
			*(uint8_t *)box->data = *(uint8_t *)value;
			snprintf(box->str, sizeof(box->str), "%d", *(uint8_t *)box->data);
			break;
		case int8:
			box->data = pvPortMalloc(sizeof(int8_t));
			*(int8_t *)box->data = *(int8_t *)value;
			snprintf(box->str, sizeof(box->str), "%d", *(int8_t *)box->data);
			break;
		case float32:
			box->data = pvPortMalloc(sizeof(float));
			*(float *)box->data = *(float *)value;
			snprintf(box->str, sizeof(box->str), "%.2f", *(float *)box->data);
			break;
		default:
			vPortFree(box->data);
			break;
	}
	return box;
}
void NumberBoxInit(NumberBoxStruct* box, UiStyle style, FontSize size, Point2D p)
{
	box->p = p;
	box->size = size;
	box->style = style;
	char str[12];
	if(box->style == style0)
	{
		sprintf(str, "%s", box->str);
		strcpy(box->str, str);
	}
	if(box->style == style1)
	{
		sprintf(str, "(%s)", box->str);
		strcpy(box->str, str);
	}
	if(box->style == style2)
	{
		sprintf(str, "[%s]", box->str);
		strcpy(box->str, str);
	}
}
void NumberBoxDeInit(NumberBoxStruct* box)
{
	vPortFree(box->data);
	vPortFree(box);
}
void NumberBoxHandle(void* self, uint8_t event)
{
	//NumberBoxStruct* box = (NumberBoxStruct*)self;
}
void NumberBoxRender(void* self)
{
	NumberBoxStruct* box = (NumberBoxStruct*)self;
	PlotString(box->p, box->str, box->size, uiDevice->disBuffer);
}

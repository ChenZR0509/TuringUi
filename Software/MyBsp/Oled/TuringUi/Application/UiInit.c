/**
  *@ FileName: UiInit.c
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiInit.h"
#include "UiEvent.h"
#include "../Basic/UiAnimation.h"
#include "../Basic/UiPlot.h"
/* Data ------------------------------------------------------------------*/
void StartupAnimation(void);
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: BspState UiInit(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 初始化UI框架
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
BspState UiInit()
{
	vTaskDelay(pdMS_TO_TICKS(50));		//延时50ms，避免重新上电导致OLED初始化失败
	SSDInit();							//初始化SSD1306
	//执行开机动画
	StartupAnimation();
	//初始化UI菜单
	MenuTreeInit();
	return BspOk;
}
/**
  *@ FunctionName: void StartupAnimation(void)
  *@ Author: CzrTuringB
  *@ Brief: TuringUI框架的开机动画
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void StartupAnimation(void)
{
	float attribute[3];								//创建动作因子
	uint8_t fillMode[11] = {8,3,5,2,6,3,4,2,7,3,1};	//外边框填充模式的变化规律
	AnimType anit;									//创建动画类型
	AnimTypeInit(&anit,aEasingIn);					//动画类型设置为缓入
	MoveScale scale[3] = {{0,60},{0,160},{0,10}};	//设置动画的起始位置和结束位置：内圆弧旋转角度、外圆弧旋转角度，边框动作模式
	AnimElement anie[3];
	for(uint8_t i=0;i<3;i++)
	{
		//初始化动画元素
		AnimElementInit(&anie[i], &anit, &scale[i], &attribute[i]);
	}
	AnimSet anis;
	AnimSetInit(&anis, &anie[0], 3, pdMS_TO_TICKS(1500));//设置动画时间为1.5s
	//缓入运行动画
	while(anis.isFinish != True)
	{
		PlotCleanBuffer(uiDevice->disBuffer);
		PlotString((Point2D){46,28}, "Turing", C6x8, uiDevice->disBuffer);
		PlotArcRectangle((Point2D){0,0}, 127, 63, 8, uiDevice->disBuffer, (FillMode)fillMode[(uint8_t)attribute[2]]);
		PlotRingWithRoundedEnds((Point2D){64,32}, 22, 20, uiDevice->disBuffer, 0+attribute[1], 150+attribute[1], FillB);
		PlotRingWithRoundedEnds((Point2D){64,32}, 22, 20, uiDevice->disBuffer, 180+attribute[1], 330+attribute[1], FillB);
		PlotRingWithRoundedEnds((Point2D){64,32}, 28, 24, uiDevice->disBuffer, 0+attribute[0], 90+attribute[0], Fill5);
		PlotRingWithRoundedEnds((Point2D){64,32}, 28, 24, uiDevice->disBuffer, 120+attribute[0], 210+attribute[0], Fill5);
		PlotRingWithRoundedEnds((Point2D){64,32}, 28, 24, uiDevice->disBuffer, 240+attribute[0], 330+attribute[0], Fill5);
		Animation(&anis);
		SSDUpdateScreen();
	}
	//缓出运行动画
	AnimTypeInit(&anit,aEasingOut);
	for(uint8_t i=0;i<3;i++)
	{
		float temp = scale[i].ePosition;
		scale[i].ePosition = scale[i].sPosition;
		scale[i].sPosition = temp;
	}
	anis.isFinish = False;
	while(anis.isFinish != True)
	{
		PlotCleanBuffer(uiDevice->disBuffer);
		PlotString((Point2D){46,28}, "Turing", C6x8, uiDevice->disBuffer);
		PlotArcRectangle((Point2D){0,0}, 127, 63, 8, uiDevice->disBuffer, (FillMode)fillMode[(uint8_t)attribute[2]]);
		PlotRingWithRoundedEnds((Point2D){64,32}, 22, 20, uiDevice->disBuffer, 0+attribute[1], 150+attribute[1], FillB);
		PlotRingWithRoundedEnds((Point2D){64,32}, 22, 20, uiDevice->disBuffer, 180+attribute[1], 330+attribute[1], FillB);
		PlotRingWithRoundedEnds((Point2D){64,32}, 28, 24, uiDevice->disBuffer, 0+attribute[0], 90+attribute[0], Fill5);
		PlotRingWithRoundedEnds((Point2D){64,32}, 28, 24, uiDevice->disBuffer, 120+attribute[0], 210+attribute[0], Fill5);
		PlotRingWithRoundedEnds((Point2D){64,32}, 28, 24, uiDevice->disBuffer, 240+attribute[0], 330+attribute[0], Fill5);
		Animation(&anis);
		SSDUpdateScreen();
	}
	//保留外框清空屏幕
	PlotCleanBuffer(uiDevice->disBuffer);
	PlotArcRectangle((Point2D){0,0}, 127, 63, 8, uiDevice->disBuffer, Fill1);
	SSDUpdateScreen();
}

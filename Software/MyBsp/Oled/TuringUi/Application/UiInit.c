/**
  *@ FileName：
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 15, 2025
  *@ Requirement：
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "UiInit.h"
//-开机动画设计
#include "../Basic/UiPlot.h"
#include "../Basic/UiAnimation.h"
//-根页面
#include "UiNavigation.h"
#include "UiRootPage.h"
#include "../Basic/UiEvent.h"
#include "../Basic/UiRender.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
//-enum
//-struct
//-variable
/* Functions Declare------------------------------------------------------------------*/
void StartupAnimation(void);
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: BspState UiInit(UiPage* page)
  *@ Author: CzrTuringB
  *@ Brief: 初始化UI框架
  *@ Time: Oct 30, 2024
  *@ Requirement:
  *@ Notes:
  */
BspState UiInit()
{
	//1、延时50ms，避免重新上电导致OLED初始化失败
	vTaskDelay(pdMS_TO_TICKS(50));
	//2、初始化SSD1306
	SSDInit();
	//3、创建开机动画任务
	xTaskHandle StartupAnimationHandle;
	xTaskCreate((void*)StartupAnimation, "StartupAnimation", 128 * 4, NULL, 24, &StartupAnimationHandle);
	TickType_t time = xTaskGetTickCount();
	//4、初始化UI菜单树
	MenuTreeInit();
	//5、初始化根页面
	RootPageInit((void*)rootNode, (void*)NULL);
	//6、(等前面的初始化函数执行完)开机动画播放时间设置为4s
	TickType_t nowTime = xTaskGetTickCount();
	if(nowTime - time < pdMS_TO_TICKS(4000)) vTaskDelayUntil(&time, pdMS_TO_TICKS(4000));
	vTaskDelete(StartupAnimationHandle);
	PlotFillScreen(uiDevice->disBuffer, Fill0);
	SSDUpdateScreen();
	//7、初始化事件管理器
	xTaskHandle UiEventTaskHandle = NULL;
	xTaskCreate((void*)UiEventHandle, "UiEvent", 128 * 4, NULL, 24, &UiEventTaskHandle);
	//8、初始化屏幕渲染器
	xTaskHandle UiRenderTaskHandle = NULL;
	xTaskCreate((void*)UiRenderHandle, "UiRender", 256 * 4, NULL, 24, &UiRenderTaskHandle);
	return BspOk;
}
/**
  *@ FunctionName: void StartupAnimation(void)
  *@ Author: CzrTuringB
  *@ Brief: 开机动画
  *@ Time: Oct 30, 2024
  *@ Requirement:
  *@ Notes:
  */
void StartupAnimation(void)
{
	float attribute[3];									//创建动作因子
	uint8_t fillMode[11] = {3,9,11,2,12,9,10,2,13,9,4};	//外边框填充模式的变化规律
	AnimType anit;										//创建动画类型
	AnimTypeInit(&anit,aEasingInOut);					//动画类型设置为缓入换出
	MoveScale scale[3] = {{0,60},{0,160},{0,10}};		//设置动画的起始位置和结束位置：内圆弧旋转角度、外圆弧旋转角度，边框动作模式
	AnimElement anie[3];
	for(uint8_t i=0;i<3;i++)
	{
		AnimElementInit(&anie[i], &anit, &scale[i], &attribute[i]);
	}
	AnimSet anis;
	AnimSetInit(&anis, &anie[0], 3, pdMS_TO_TICKS(1500));//设置动画时间为1.5s
	while(1)
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
		AnimBidirectionalLoop(&anis, 3);
	}
}

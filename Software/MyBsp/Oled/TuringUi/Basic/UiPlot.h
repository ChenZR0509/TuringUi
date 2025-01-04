/**
  *@ FileName: UiPlot.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
#ifndef __UIPLOT_H
#define __UIPLOT_H
/* Includes ------------------------------------------------------------------*/
#include "UiCore.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
/* Data(对外接口)-----------------------------------------------------*/
//-enum
typedef enum
{
	C8x16,
	C6x8,
}FontSize;
typedef enum
{
	StartArrow,
	EndArrow,
	StaEndArrow,
}ArrowMode;
/*
 * @灰度阶定义：
 * 		|2|3|
 * 		|0|1|
 * @0：全填充0
 * @1：全填充1
 * @5：
 * 		|1|0|
 * 		|0|1|
 * @A：
 * 		|0|1|
 * 		|1|0|
 * @B：
 * 		|0|1|
 * 		|1|1|
 * @7：
 * 		|1|0|
 * 		|1|1|
 * @E：
 * 		|1|1|
 * 		|0|1|
 * @D：
 * 		|1|1|
 * 		|1|0|
 * @EBD7:
 * 		|1|1|1|0|
 * 		|1|0|1|1|
 * 		|1|1|0|1|
 * 		|0|1|1|1|
 * */
typedef enum
{
	Fill0,	//填充0：灰度为0%
	Fill1,	//填充1：灰度为100%
	Fill5,	//交替填充0、1：灰度为50%
	FillA,	//交替填充1、0：灰度为50%
	FillB,  	//灰度为75%
	Fill7,		//灰度为75%
	FillE,		//灰度为75%
	FillD,		//灰度为75%
	FillEBD7,	//灰度为75%：四种75%灰度交替填充
}FillMode;
//-struct(结构体对外接口)
typedef struct
{
	Point2D controlPoints[4];
	uint8_t segment;
}BezierLine;
/* Functions(对外接口函数)------------------------------------------------------------------*/
void PlotPoint(Point2D p,SSDBuffer buffer,FillMode mode);
void PlotLine(Point2D p0, Point2D p1, SSDBuffer buffer,FillMode mode);
void PlotDashedLine(Point2D p0, Point2D p1, SSDBuffer buffer, uint8_t dashLength, uint8_t gapLength, FillMode mode);
void PlotArrow(Point2D p0, Point2D p1, SSDBuffer buffer, ArrowMode aMode, FillMode fMode);
void PlotCircle(Point2D pc, uint8_t r, SSDBuffer buffer, FillMode mode);
void PlotFillCircle(Point2D pc, uint8_t r, SSDBuffer buffer, FillMode mode);
void PlotArc(Point2D pc, uint8_t r, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode);
void PlotFillArc(Point2D pc, uint8_t r, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode, uint8_t Boundry);
void PlotRingWithRoundedEnds(Point2D pc, uint8_t outerRadius, uint8_t innerRadius, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode);
void PlotTriangle(Point2D* p, SSDBuffer buffer, FillMode mode);
void PlotFillTriangle(Point2D* p, SSDBuffer buffer, FillMode mode);
void PlotRectangle(Point2D p0, uint8_t width, uint8_t height, SSDBuffer buffer, FillMode mode);
void PlotQuadrilateral(Point2D* p, SSDBuffer buffer, FillMode mode);
void PlotFillRectangle(Point2D p0, uint8_t width, uint8_t height, SSDBuffer buffer, FillMode mode);
void PlotFillQuadrilateral(Point2D* p, SSDBuffer buffer, FillMode mode);
void PlotArcRectangle(Point2D p0, uint8_t width, uint8_t height, uint8_t r, SSDBuffer buffer, FillMode mode);
void PlotPolygon(Point2D pc, uint8_t radius, uint8_t sides, SSDBuffer buffer, FillMode mode);
void PlotWave(Point2D p0, uint8_t length, uint8_t amplitude, uint8_t frequency, SSDBuffer buffer, FillMode mode);
void PlotEllipse(Point2D p0, Point2D p1, Point2D p2, Point2D p3, SSDBuffer buffer, FillMode mode, Bool isDashed);
void PlotChar(Point2D p0, uint8_t chr, FontSize charSize, SSDBuffer buffer);
void PlotString(Point2D p0, const char* str, FontSize charSize, SSDBuffer buffer);
void PlotBMP(Point2D p0,uint8_t width,uint8_t height,const uint8_t* picture, SSDBuffer buffer);
void Plot2Axes(Point2D pc, uint8_t xLength, uint8_t yLength, float angle, SSDBuffer buffer, FillMode mode);
void PlotRoseCurve(Point2D pc, float a, float k, uint16_t steps, SSDBuffer buffer, FillMode mode);
void PlotHeartCurve(Point2D pc, uint16_t steps, float scale, SSDBuffer buffer, FillMode mode);
void PlotButterflyCurve(Point2D pc, uint16_t steps, float scale, SSDBuffer buffer, FillMode mode);
void PlotStarCurve(Point2D pc, uint16_t steps, float a, float b, float k, float scale, SSDBuffer buffer, FillMode mode);
void PlotFillPolygon(Point2D pc, SSDBuffer buffer, FillMode mode, uint8_t Boundary);
void BezierLineInit(BezierLine *line, Point2D *controlPoints, uint8_t segment);
void PlotBezierLine(BezierLine *line, SSDBuffer buffer, FillMode mode);
void PlotBezierArrowLine(BezierLine *line, SSDBuffer buffer, FillMode mode);
void PlotFillBlock(uint8_t bx, uint8_t by, SSDBuffer buffer, FillMode mode);
void PlotFillScreen(SSDBuffer buffer, FillMode mode);
void PlotCleanBlock(uint8_t bx, uint8_t by, SSDBuffer buffer);
void PlotCleanBuffer(SSDBuffer buffer);
void PlotCleanScreen();
#endif

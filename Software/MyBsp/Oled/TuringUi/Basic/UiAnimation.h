/**
  *@ FileName: UiAnimation.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 6, 2024
  *@ Requirement：
  *@ Notes:
  *@ 	1、CSS工具：https://cubic-bezier.tupulin.com/#cubic-bezier(0.50,-1.00,0.50,2.00)
  */
#ifndef __UIANIMATION_H
#define __UIANIMATION_H
/* Includes ------------------------------------------------------------------*/
#include "UiCore.h"
#include "UiPlot.h"
/* Define ------------------------------------------------------------------*/
#define aLinear (Point3D){1.00, 1.00, 0.00}, (Point3D){0.00, 0.00, 0.00}			//线性动画
#define aEasing (Point3D){0.25, 0.10, 0.00}, (Point3D){0.25, 1.00, 0.00}			//缓动动画
#define aEasingIn (Point3D){0.42, 0.00, 0.00}, (Point3D){1.00, 1.00, 0.00}			//缓入动画
#define aEasingOut (Point3D){0.00, 0.00, 0.00}, (Point3D){0.58, 1.00, 0.00}			//缓出动画
#define aEasingInOut (Point3D){0.42, 0.00, 0.00}, (Point3D){0.58, 1.00, 0.00}		//缓入缓出动画
#define aChargeUp (Point3D){0.00, -0.32, 0.00}, (Point3D){1.00, -0.32, 0.00}		//蓄力动画
#define aInertia  (Point3D){0.00, 1.34, 0.00}, (Point3D){1.00, 1.34, 0.00}			//惯性动画
#define aSwing    (Point3D){0.50, -1.00, 0.00}, (Point3D){0.50, 2.00, 0.00}			//摇摆动画
/* Data ------------------------------------------------------------------*/
typedef struct
{
    float xAngle;
    float yAngle;
    float zAngle;
    UiType xc;
    UiType yc;
    UiType zc;
}RotateValue;
typedef struct
{
	UiType xTra;
	UiType yTra;
	UiType zTra;
}TranslateValue;
//-动画相关
typedef struct
{
	Point3D p1;				    //贝塞尔控制点1
	Point3D p2;				    //贝塞尔控制点2
}AnimType;
typedef struct
{
	float  sPosition;			//起始位置
	float  ePosition;			//结束位置
}MoveScale;
typedef struct
{
    AnimType* anit;				//动画类型
    MoveScale* scale;			//运动范围
 	float*  Attribute;			//运动属性
}AnimElement;
typedef struct
{
	uint32_t startTime;     	  	//动画开始时间: 若其值为零则表示动画尚未开始或已经结束
	uint32_t  duration;			 	//动画持续时间
	Bool isFinish;					//动画是否播放完成
	float time;						//贝塞尔时间参数 = 动画已经播放时间/动画持续时间
    uint8_t num;				 	//动画元素的数量
    AnimElement* anie;			    //动画元素数组
}AnimSet;
/* Function ------------------------------------------------------------------*/
void OrtProject(Point3D p3, Point2D* p2, float scale, Point2D pc);
void PreProject(Point3D p3, Point2D* p2, float scale, uint8_t depth, Point2D pc);
void Scale2D(Point2D p0, Point2D* p1, float xScale, float yScale);
void Scale3D(Point3D p0, Point3D* p1, float xScale, float yScale, float zScale);
void Shear2D(Point2D p0, Point2D* p1, float xShear, float yShear);
void Shear3D(Point3D p0, Point3D* p1, float xyShear, float xzShear, float yxShear, float yzShear,float zxShear, float zyShear);
void TranslatePoint2D(Point2D* p2, TranslateValue tra);
void TranslatePoint3D(Point3D* p3, TranslateValue tra);
void RotatePoint2D(Point2D* p2, RotateValue rot);
void RotatePoint3D(Point3D* p3,RotateValue rot);
void RotateRegion(Point2D p0, uint8_t width, uint8_t height, RotateValue rot);
/* 动画 ------------------------------------------------------------------*/
void AnimTypeInit(AnimType* anit, Point3D p1, Point3D p2);
void MoveScaleInit(MoveScale* scale, float sp, float ep);
void AnimElementInit(AnimElement* anie,AnimType* anit,MoveScale* scale,float* attribute);
void AnimSetInit(AnimSet* anis,AnimElement* anie,uint8_t num,uint32_t  duration);
void Animation(AnimSet* anis);
void AnimBidirectionalLoop(AnimSet* anis,uint8_t num);
void AnimOneWayLoop(AnimSet* anis);
/* 矢量图绘制 ------------------------------------------------------------------*/
void ParseSvg(const SVG* path,Point2D p0,UiType width, UiType height, RotateValue rot, uint8_t num, uint16_t segment, SSDBuffer buffer ,FillMode mode);
#endif

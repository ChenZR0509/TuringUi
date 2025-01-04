/**
  *@ FileName: UiAnimation.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 6, 2024
  *@ Requirement：
  */
#ifndef __UIANIMATION_H
#define __UIANIMATION_H
/* Includes ------------------------------------------------------------------*/
#include "UiCore.h"
/* Define ------------------------------------------------------------------*/
#define aLinear (Point3D){1.00, 1.00, 0.00}, (Point3D){0.00, 0.00, 0.00}
#define aEasing (Point3D){0.25, 0.10, 0.00}, (Point3D){0.25, 1.00, 0.00}
#define aEasingIn (Point3D){0.42, 0.00, 0.00}, (Point3D){1.00, 1.00, 0.00}
#define aEasingOut (Point3D){0.00, 0.00, 0.00}, (Point3D){0.58, 1.00, 0.00}
#define aEasingInOut (Point3D){0.42, 0.00, 0.00}, (Point3D){0.58, 1.00, 0.00}
/* Data ------------------------------------------------------------------*/
typedef struct
{
    float xAngle;
    float yAngle;
    float zAngle;
    int8_t xc;
    int8_t yc;
    int8_t zc;
}RotateValue;
typedef struct
{
    int16_t xTra;
    int16_t yTra;
    int16_t zTra;
}TranslateValue;
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
    AnimElement* anie;			     //动画元素数组
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
/* 动画 ------------------------------------------------------------------*/
void AnimTypeInit(AnimType* anit, Point3D p1, Point3D p2);
void MoveScaleInit(MoveScale* scale, float sp, float ep);
void AnimElementInit(AnimElement* anie,AnimType* anit,MoveScale* scale,float* attribute);
void AnimSetInit(AnimSet* anis,AnimElement* anie,uint8_t num,uint32_t  duration);
void Animation(AnimSet* anis);
#endif

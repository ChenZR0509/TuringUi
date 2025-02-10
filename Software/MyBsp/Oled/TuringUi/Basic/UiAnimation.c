/**
 *@ FileName: UiAnimation.c
 *@ Author: CzrTuringB
 *@ Brief:
 *@ Time: Oct 6, 2024
 *@ Requirement：
 */
/* Includes ------------------------------------------------------------------*/
#include "UiAnimation.h"
#include "UiPlot.h"
#include <stdio.h>
#include <math.h>
/* 运动转换算法 ------------------------------------------------------------------*/
/**
  *@ FunctionName: void OrtProject(Point3D* p3, Point2D* p2, float scale, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 正交投影变换
  *@ Time: Dec 18, 2024
  *@ Requirement：
  *@ 	1、xc和yc是正交投影的中心
  */
void OrtProject(Point3D p3, Point2D* p2, float scale, Point2D pc)
{
	p2->x = (UiType)(pc.x + p3.x * scale);
	p2->y = (UiType)(pc.y - p3.y * scale);
}
/**
  *@ FunctionName: void PreProject(Point3D* p3, Point2D* p2, float scale, uint8_t depth, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 透视投影变换
  *@ Time: Dec 18, 2024
  *@ Requirement：
  *@ 	1、xc和yc是透视投影的中心
  */
void PreProject(Point3D p3, Point2D* p2, float scale, uint8_t depth, Point2D pc)
{
	if(p3.z + depth != 0)
	{
		p2->x = (UiType)(pc.x + p3.x * scale / (p3.z + depth));
		p2->y = (UiType)(pc.y - p3.y * scale / (p3.z + depth));
	}
	else
	{
		p2->x = (UiType)(pc.x);
		p2->y = (UiType)(pc.y);
	}
}
/**
  *@ FunctionName: void Scale2D(Point2D p0, Point2D* p1, float xScale, float yScale)
  *@ Author: CzrTuringB
  *@ Brief: 二维比例缩放
  *@ Time: Jan 3, 2025
  *@ Requirement：
  */
void Scale2D(Point2D p0, Point2D* p1, float xScale, float yScale)
{
    p1->x = p0.x * xScale;
    p1->y = p0.y * yScale;
}
/**
  *@ FunctionName: void Scale3D(Point3D p0, Point3D* p1, float xScale, float yScale, float zScale)
  *@ Author: CzrTuringB
  *@ Brief: 三维比例缩放
  *@ Time: Jan 3, 2025
  *@ Requirement：
  */
void Scale3D(Point3D p0, Point3D* p1, float xScale, float yScale, float zScale)
{
    p1->x = p0.x * xScale;
    p1->y = p0.y * yScale;
    p1->z = p0.z * zScale;
}
/**
  *@ FunctionName: void Shear2D(Point2D p0, Point2D* p1, float xShear, float yShear)
  *@ Author: CzrTuringB
  *@ Brief: 二维错切
  *@ Time: Jan 3, 2025
  *@ Requirement：
  */
void Shear2D(Point2D p0, Point2D* p1, float xShear, float yShear)
{
    p1->x = p0.x + xShear * p0.y;
    p1->y = p0.y + yShear * p0.x;
}
/**
  *@ FunctionName: void Shear3D(Point3D p0, Point3D* p1, float xyShear, float xzShear, float yxShear, float yzShear,float zxShear, float zyShear)
  *@ Author: CzrTuringB
  *@ Brief: 三维错切
  *@ Time: Jan 3, 2025
  *@ Requirement：
  */
void Shear3D(Point3D p0, Point3D* p1, float xyShear, float xzShear, float yxShear, float yzShear,float zxShear, float zyShear)
{
    p1->x = p0.x + xyShear * p0.y + xzShear * p0.z;
    p1->y = p0.y + yxShear * p0.x + yzShear * p0.z;
    p1->z = p0.z + zxShear * p0.x + zyShear * p0.y;
}
/**
  *@ FunctionName: void TranslatePoint2D(Point2D* p2, TranslateValue tra)
  *@ Author: CzrTuringB
  *@ Brief: 二维点的平移
  *@ Time: Dec 18, 2024
  *@ Requirement：
  *@ 	1、xTra和yTra表示平移距离
  */
void TranslatePoint2D(Point2D* p2, TranslateValue tra)
{
	p2->x += tra.xTra;
	p2->y += tra.yTra;
}
/**
  *@ FunctionName: void TranslatePoint3D(Point3D* p3, TranslateValue tra);
  *@ Author: CzrTuringB
  *@ Brief: 三维点的平移
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void TranslatePoint3D(Point3D* p3, TranslateValue tra)
{
	p3->x += tra.xTra;
	p3->y += tra.yTra;
	p3->z += tra.zTra;
}
/**
  *@ FunctionName: void RotatePoint2D(Point2D* p2, RotateValue rot)
  *@ Author: CzrTuringB
  *@ Brief: 二维点的旋转
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void RotatePoint2D(Point2D* p2, RotateValue rot)
{
	//计算三角函数
    float rad = rot.zAngle * M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    //将点平移到旋转中心
    UiType tx = p2->x - rot.xc;
    UiType ty = p2->y - rot.yc;

    //旋转计算
    float rx = tx * cosA - ty * sinA;
    float ry = tx * sinA + ty * cosA;

    //平移回原位置
    p2->x = rot.xc + (UiType)rx;
    p2->y = rot.yc + (UiType)ry;
}
/**
  *@ FunctionName: void RotatePoint3D(Point3D* p3,RotateValue rot)
  *@ Author: CzrTuringB
  *@ Brief: 三维点的旋转
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void RotatePoint3D(Point3D* p3,RotateValue rot)
{
    // 将角度转换为弧度
    float xRad = rot.xAngle * 3.14159265 / 180.0f;
    float yRad = rot.yAngle * 3.14159265 / 180.0f;
    float zRad = rot.zAngle * 3.14159265 / 180.0f;
    // 平移到旋转中心
    float px = p3->x - rot.xc;
    float py = p3->y - rot.yc;
    float pz = p3->z - rot.zc;
    // 旋转绕 X 轴
    float cosX = cos(xRad);
    float sinX = sin(xRad);
    float tempY = py * cosX - pz * sinX;
    float tempZ = py * sinX + pz * cosX;
    py = tempY;
    pz = tempZ;
    // 旋转绕 Y 轴
    float cosY = cos(yRad);
    float sinY = sin(yRad);
    float tempX = px * cosY + pz * sinY;
    tempZ = -px * sinY + pz * cosY;
    px = tempX;
    pz = tempZ;
    // 旋转绕 Z 轴
    float cosZ = cos(zRad);
    float sinZ = sin(zRad);
    tempX = px * cosZ - py * sinZ;
    tempY = px * sinZ + py * cosZ;
    px = tempX;
    py = tempY;
    // 平移回原位置
    p3->x = px + rot.xc;
    p3->y = py + rot.yc;
    p3->z = pz + rot.zc;
}
/* 动画算法 ------------------------------------------------------------------*/
/**
  *@ FunctionName: float BezierCurve(float t, float v0, float v1, float v2, float v3)
  *@ Author: CzrTuringB
  *@ Brief: 计算三次贝塞尔曲线值
  *@ Time: Dec 20, 2024
  *@ Requirement：
  */
float BezierCurve(float t, float v0, float v1, float v2, float v3)
{
    float u = 1 - t;
    return (u * u * u * v0 + 3 * u * u * t * v1 + 3 * u * t * t * v2 + t * t * t * v3);
}
/**
  *@ FunctionName: float SolveBezierT(float x, float x1, float x2, float epsilon)
  *@ Author: CzrTuringB
  *@ Brief: 依据曲线横坐标值计算插值参数t
  *@ Time: Dec 20, 2024
  *@ Requirement：
  *@ 	1、epsilon为曲线的精度参数
  */
float SolveBezierT(float x, float x1, float x2, float epsilon)
{
    // 使用二分法求解贝塞尔曲线的参数 t
    float x0 = 0.0f, x3 = 1.0f, t = 0.5f;

    while (x3 - x0 > epsilon)
    {
        float xt = BezierCurve(t, 0.0f, x1, x2, 1.0f); // 计算当前 t 对应的 x 值
        if (fabs(xt - x) < epsilon)
        {
            return t; // 找到符合条件的 t
        }
        if (xt < x)
        {
            x0 = t; // 调整下限
        }
        else
        {
            x3 = t; // 调整上限
        }
        t = (x0 + x3) * 0.5f; // 更新 t 为中间点
    }
    return t; // 返回最终逼近的 t
}
/**
  *@ FunctionName: void AnimTypeInit(AnimType* anit, Point3D p1, Point3D p2)
  *@ Author: CzrTuringB
  *@ Brief: 初始化动画类型结构体
  *@ Time: Dec 28, 2024
  *@ Requirement：
  */
void AnimTypeInit(AnimType* anit, Point3D p1, Point3D p2)
{
	anit->p1 = p1;
	anit->p2 = p2;
}
/**
  *@ FunctionName: void MoveScaleInit(MoveScale* scale, float sp, float ep)
  *@ Author: CzrTuringB
  *@ Brief: 初始化运动范围
  *@ Time: Dec 28, 2024
  *@ Requirement：
  */
void MoveScaleInit(MoveScale* scale, float sp, float ep)
{
	scale->ePosition = ep;
	scale->sPosition = sp;
}
/**
  *@ FunctionName: void AnimElementInit(AnimElement* anie,AnimType* anit,MoveScale* scale,float* attribute)
  *@ Author: CzrTuringB
  *@ Brief: 初始化动画元素结构体的相关参数
  *@ Time: Dec 28, 2024
  *@ Requirement：
  */
void AnimElementInit(AnimElement* anie,AnimType* anit,MoveScale* scale,float* attribute)
{
	anie->Attribute = attribute;
	anie->anit = anit;
	anie->scale = scale;
}
/**
  *@ FunctionName: void AnimSetInit(AnimSet* anis,AnimElement* anie,uint8_t num,uint32_t  duration)
  *@ Author: CzrTuringB
  *@ Brief: 初始化动画集合结构体的相关参数
  *@ Time: Dec 28, 2024
  *@ Requirement：
  */
void AnimSetInit(AnimSet* anis,AnimElement* anie,uint8_t num,uint32_t  duration)
{
	anis->duration =duration;
	anis->num = num;
	anis->startTime = 0;
	anis->time = 0.0;
	anis->anie = anie;
	anis->isFinish = False;
}
/**
  *@ FunctionName: void Animation(AnimSet* anis)
  *@ Author: CzrTuringB
  *@ Brief: 动画集合运动属性计算函数
  *@ Time: Dec 28, 2024
  *@ Requirement：
  */
void Animation(AnimSet* anis)
{
	//1、获取动画开始时的系统滴答计数
	if(anis->startTime == 0 && anis->isFinish == False)
	{
		anis->startTime = xTaskGetTickCount();
		for(uint8_t i=0;i<anis->num;i++)
		{
			*(anis->anie[i].Attribute) = anis->anie[i].scale->sPosition;
		}
		return;
	}
    //2、获取当前时间、计算已过时间
    uint32_t currentTime = xTaskGetTickCount();
    uint32_t elapsedTime = currentTime - anis->startTime;
    //3、动画处理
    if (elapsedTime >= anis->duration)
    {
        //动画完成
    	anis->isFinish = True;
    	anis->startTime = 0;
    	anis->time = 0.0f;
		for(uint8_t i=0;i<anis->num;i++)
		{
			*(anis->anie[i].Attribute) = anis->anie[i].scale->ePosition;
		}
    }
    else
    {
        //动画还未完成
    	anis->time = (float)elapsedTime / (float)anis->duration;
		for(uint8_t i=0;i<anis->num;i++)
		{
	        //使用贝塞尔曲线控制动画的进度
	        float t = SolveBezierT(anis->time, anis->anie[i].anit->p1.x, anis->anie[i].anit->p2.x, 0.001f);
	        float y = BezierCurve(t, 0.0f, anis->anie[i].anit->p1.y, anis->anie[i].anit->p2.y, 1.0f);
	        //根据贝塞尔曲线进度计算当前动画位置
	        *(anis->anie[i].Attribute) = anis->anie[i].scale->sPosition + (anis->anie[i].scale->ePosition - anis->anie[i].scale->sPosition) * y;
		}
    }
}
/**
  *@ FunctionName: void AnimationRoundTrip(AnimSet* anis,uint8_t num)
  *@ Author: CzrTuringB
  *@ Brief: 将动画设置为往返循环模式
  *@ Time: Jan 10, 2025
  *@ Requirement：
  */
void AnimBidirectionalLoop(AnimSet* anis,uint8_t num)
{
	if(anis->isFinish == True)
	{
		for(uint8_t i=0; i<num; i++)
		{
			float temp = anis->anie[i].scale->sPosition;
			anis->anie[i].scale->sPosition = anis->anie[i].scale->ePosition;
			anis->anie[i].scale->ePosition = temp;
		}
		anis->isFinish = False;
	}
}
/**
  *@ FunctionName: void AnimOneWayLoop(AnimSet* anis)
  *@ Author: CzrTuringB
  *@ Brief: 将动画设置为单项循环模式
  *@ Time: Jan 10, 2025
  *@ Requirement：
  */
void AnimOneWayLoop(AnimSet* anis)
{
	anis->isFinish = False;
}

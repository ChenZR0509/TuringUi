/**
  *@ FileName: UiPlot3D.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Dec 17, 2024
  *@ Requirement：
  *@ 	1、定义z轴方向：垂直屏幕向外方向为z轴方向
  *@ 	2、多面体参数配置流程：
  *@ 		a.初始化投影缩放比例
  *@ 		b.初始化投影中心
  *@ 		c.创建半边
  *@ 		d.创建顶点
  *@ 		f.创建面
  *@ 		g.初始化半边的其余参数:配对、下一个半边、前一个半边
  *@ 		h.计算空间最长距离用于配置视点
  */
#ifndef __UIPLOT3D_H
#define __UIPLOT3D_H
/* Includes ------------------------------------------------------------------*/
#include "UiCore.h"
#include "UiPlot.h"
#include "UiAnimation.h"
/* Define ------------------------------------------------------------------*/
typedef enum
{
	Hidden = 0,		//不带隐藏边绘制
	DashedHidden,	//隐藏边为虚线绘制
	NoHidden		//带隐藏边绘制
}Plot3Mode;
/* Data ------------------------------------------------------------------*/
typedef struct Vertex Vertex;
typedef struct HalfEdge HalfEdge;
typedef struct Face Face;
struct HalfEdge
{
    Vertex* vStart;    // 半边开始的顶点
    Vertex* vEnd;      // 半边指向的顶点
    HalfEdge* pair;    // 相对的半边
    HalfEdge* next;    // 同一面上的下一个半边
    HalfEdge* prev;    // 同一面上的上一个半边
    Face* face;        // 半边所属的面
};
struct Vertex
{
    Point3D p;       		 // 顶点坐标
    Point3D p3;			     // 顶点运动后坐标
    Point2D p2;				 // 顶点投影后坐标
    HalfEdge* halfEdge;  	 // 存储任意一个与顶点关联的半边
};
struct Face
{
    Point3D dirVector;		  // 平面的法向量
    HalfEdge* halfEdge;       // 面的某一个半边
};
typedef struct MoveValue
{
	RotateValue rot;		  //旋转参数
	float	scale;			  //投影缩放因子
	Point2D	pc;				  //投影中心，更改其值可以实现平移
}MoveValue;
/*
 * @圆柱体参数设计要点：
 * 	1、共有八个顶点：上下个四个顶点投影也是依靠这四个顶点来绘制椭圆的
 * 	2、八条半边：半边的定义要遵循右手定则，大拇指为平面法向量指向圆柱外部
 * 	3、两个平面：由于其两个平面互相平行，所以圆柱体的半边可以不用配对
 * */
typedef struct
{
	Vertex vertices[8];		//顶点数组
	HalfEdge edges[8];		//半边数组
	Face	faces[2];		//面数组
	float maxDistence;		//圆柱空间最长距离
	float r;				//圆柱半径
	float h;				//圆柱高
	MoveValue move;			//运动变换参数
}Cylinder;
typedef struct
{
	Vertex vertices[5];		//顶点数组
	HalfEdge edges[4];		//半边数组
	Face	faces[1];		//面数组
	float maxDistence;		//圆锥空间最长距离
	float r;				//圆锥半径
	float h;				//圆锥高
	MoveValue move;			//运动变换参数
}Cone;
typedef struct
{
	Vertex vertices[8];		//顶点数组
	HalfEdge edges[24];		//半边数组
	Face	faces[6];		//面数组
	float maxDistence;		//六面体空间最长距离
	MoveValue move;			//运动变换参数
}Hexahedron;
typedef struct
{
	Point3D vertices3[3];		//原始三维坐标点
	Point2D vertices2[3];		//二维坐标点
	Point2D pc;					//投影中心点
}Axes;
/* Functions(对外接口函数)------------------------------------------------------------------*/
void CrossProduct(Point3D *a, Point3D *b, Point3D *result);
void DotProduct(Point3D *a, Point3D *b,float* result);
void Subtract(Point3D* a, Point3D* b, Point3D* result);
/* 圆柱------------------------------------------------------------------*/
void CylinderInit(Cylinder *cyl, float r, float h, float scale, Point2D pc);
void UpdateCylinder(Cylinder* cyl);
void PlotCylinder(Cylinder* cyl, SSDBuffer buffer, FillMode mode, Plot3Mode pMode);
/* 圆锥------------------------------------------------------------------*/
void ConeInit(Cone *con, float r, float h, float scale, Point2D pc);
void UpdateCone(Cone* con);
void PlotCone(Cone* con, SSDBuffer buffer, FillMode mode, Plot3Mode pMode);
/* 六面体------------------------------------------------------------------*/
void HexahedronInit(Hexahedron *cube, Point3D* p, float scale, Point2D pc);
void CubeInit(Hexahedron *cube, float scale, Point2D pc, float length);
void CuboidInit(Hexahedron *cube, float scale, Point2D pc, float length, float width, float Height);
void UpdateHexahedron(Hexahedron* cube);
void PlotHexahedron(Hexahedron* cube, SSDBuffer buffer, FillMode mode, Plot3Mode pMode);
/* 三维坐标轴------------------------------------------------------------------*/
void Plot3Axes(Point2D pc,uint8_t length,RotateValue rot,SSDBuffer buffer,FillMode mode);
#endif


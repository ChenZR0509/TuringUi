/**
 *@ FileName: UiPlot3D.c
 *@ Author: CzrTuringB
 *@ Brief:
 *@ Time: Dec 17, 2024
 *@ Requirement：
 *@ 	1、定义z轴方向：垂直屏幕向外方向为z轴方向
 */
/* Includes ------------------------------------------------------------------*/
#include "UiPlot3D.h"
#include <stdlib.h>
#include <math.h>
/* Data ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: void CrossProduct(Point3D *a, Point3D *b, Point3D *result)
  *@ Author: CzrTuringB
  *@ Brief: 计算三维向量叉乘
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void CrossProduct(Point3D *a, Point3D *b, Point3D *result)
{
	result->x = a->y * b->z - a->z * b->y;
	result->y = a->z * b->x - a->x * b->z;
	result->z = a->x * b->y - a->y * b->x;
}
/**
  *@ FunctionName: void DotProduct(Point3D *a, Point3D *b,float* result)
  *@ Author: CzrTuringB
  *@ Brief: 计算三维向量点乘
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void DotProduct(Point3D *a, Point3D *b,float* result)
{
	*result=a->x * b->x + a->y * b->y + a->z * b->z;
}
/**
  *@ FunctionName: void Subtract(Point3D* a, Point3D* b, Point3D* result)
  *@ Author: CzrTuringB
  *@ Brief: 计算向量的和，或者两点所构成的向量表示
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void Subtract(Point3D* a, Point3D* b, Point3D* result)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}
/**
  *@ FunctionName: Bool IsEdgeOnPlane(HalfEdge *edge, Point3D *planeNormal)
  *@ Author: CzrTuringB
  *@ Brief: 判断半边是否处于平面上
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
Bool IsEdgeOnPlane(HalfEdge *edge, Point3D *planeNormal)
{
	// 计算半边向量
	Point3D edgeVector;
	edgeVector.x = edge->vEnd->p.x - edge->vStart->p.x;
	edgeVector.y = edge->vEnd->p.y - edge->vStart->p.y;
	edgeVector.z = edge->vEnd->p.z - edge->vStart->p.z;
	//计算半边向量与平面法向量的点乘
    float dotProduct;
	DotProduct(&edgeVector, planeNormal, &dotProduct);
	const float epsolon = 1e-6;
	if(fabs(dotProduct) < epsolon)
	{
		return True;
	}
	else
	{
		return False;
	}
}
/**
  *@ FunctionName: Bool IsFrontFace(Face* face, Point3D viewPoint)
  *@ Author: CzrTuringB
  *@ Brief: 判断平面是否位于视角的前方
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
Bool IsFrontFace(Face* face, Point3D viewPoint)
{
    //获取面上的两个半边
    HalfEdge* e1 = face->halfEdge;
    HalfEdge* e2 = e1->next;
    //计算半边向量
    Point3D AB, AC;
    Subtract(&e1->vEnd->p3, &e1->vStart->p3, &AB);  // 向量 AB
    Subtract(&e2->vEnd->p3, &e2->vStart->p3, &AC);  // 向量 AC
    //计算面法向量（叉积）
    Point3D N;
    CrossProduct(&AB, &AC, &N);
    //计算视点到面上一个点的向量
    Point3D AV;
    Subtract(&viewPoint, &e1->vStart->p, &AV);
    //计算法向量与视点向量的点积
    float dotProduct;
	DotProduct(&N, &AV, &dotProduct);
    //如果点积小于0，则视点在面前
    return (dotProduct < 0) ? True : False;
}
/**
  *@ FunctionName: void CreateVertex(Vertex* ver, Point3D p, HalfEdge *edgeArray, uint8_t numEdges, float scale, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 创建顶点
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void CreateVertex(Vertex* ver, Point3D p, HalfEdge *edgeArray, uint8_t numEdges, float scale, Point2D pc)
{
	//1、设置顶点的三维坐标
	ver->p = p;
	ver->p3 = p;
	//2、计算顶点的二维坐标
	OrtProject(p, &ver->p2, scale, pc);
	//3、遍历半边数组，查找与顶点相关的半边
	for (uint8_t i = 0; i < numEdges; i++)
	{
		HalfEdge *edge = &edgeArray[i];
		// 检查半边的起始顶点或结束顶点是否是当前顶点
		if (edge->vStart == ver || edge->vEnd == ver)
		{
			// 一旦找到一个与顶点相关的半边，设置顶点的halfEdge
			ver->halfEdge = edge;
			break;  // 找到一个相关半边后就可以停止遍历
		}
	}
}
/**
  *@ FunctionName: void SetFaceRecursive(HalfEdge *edge, Face *face, Point3D *planeNormal,HalfEdge *edgeArray, uint8_t numEdges)
  *@ Author: CzrTuringB
  *@ Brief: 递归法设置半边结构的平面参数
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void SetFaceRecursive(HalfEdge *edge, Face *face, Point3D *planeNormal,
		HalfEdge *edgeArray, uint8_t numEdges)
{
	// 设置当前半边的面属性
	if (edge->face != NULL)
		return;
	edge->face = face;
	// 遍历所有半边，查找与当前半边相邻且符合条件的半边
	for (uint8_t i = 0; i < numEdges; i++)
	{
		HalfEdge *e1 = &edgeArray[i];
		// 如果半边的起点与当前半边的终点一致，并且在平面上，且未设置面属性
		if (e1->vStart == edge->vEnd && e1->vEnd != edge->vStart && IsEdgeOnPlane(e1, planeNormal) == True)
		{
			//递归设置相邻半边的面属性
			SetFaceRecursive(e1, face, planeNormal, edgeArray, numEdges);
		}
	}
}
/**
  *@ FunctionName: void CreateFace(Face *face, HalfEdge *edge, Point3D vec, HalfEdge *edgeArray,uint8_t numEdges)
  *@ Author: CzrTuringB
  *@ Brief: 创建平面
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void CreateFace(Face *face, HalfEdge *edge, Point3D vec, HalfEdge *edgeArray,
		uint8_t numEdges)
{
	// 设置面和第一个半边的面属性
	face->halfEdge = edge;
	face->dirVector = vec;
	// 使用递归设置其他相关半边的面属性
	SetFaceRecursive(edge, face, &vec, edgeArray, numEdges);
}
/**
  *@ FunctionName: void CreateHalfEdge(HalfEdge *edge, Vertex *vStart, Vertex *vEnd)
  *@ Author: CzrTuringB
  *@ Brief: 创建半边
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void CreateHalfEdge(HalfEdge *edge, Vertex *vStart, Vertex *vEnd)
{
	edge->vStart = vStart;
	edge->vEnd = vEnd;
	edge->pair = NULL;
	edge->next = NULL;
	edge->prev = NULL;
	edge->face = NULL;
}
/**
  *@ FunctionName: void SetHalfEdgePairs(HalfEdge *edgeArray, uint8_t numEdges)
  *@ Author: CzrTuringB
  *@ Brief: 设置半边的配对属性
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void SetHalfEdgePairs(HalfEdge *edgeArray, uint8_t numEdges)
{
    for (uint8_t i = 0; i < numEdges; i++)
    {
        HalfEdge *e1 = &edgeArray[i];
        if (e1->pair != NULL) continue; // 已设置配对关系，跳过

        for (uint8_t j = i + 1; j < numEdges; j++) // 避免重复判断
        {
            HalfEdge *e2 = &edgeArray[j];
            if (e2->pair == NULL &&
                e1->vStart == e2->vEnd &&
                e1->vEnd == e2->vStart)
            {
                e1->pair = e2;
                e2->pair = e1;
                break; // 配对完成，跳出内循环
            }
        }
    }
}
/**
  *@ FunctionName: void SetHalfEdgeNextPrev(HalfEdge *edgeArray, uint8_t numEdges)
  *@ Author: CzrTuringB
  *@ Brief: 设置半边的next属性和prev属性
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void SetHalfEdgeNextPrev(HalfEdge *edgeArray, uint8_t numEdges)
{
    for (uint8_t i = 0; i < numEdges; i++)
    {
        HalfEdge *e1 = &edgeArray[i];
        HalfEdge *startEdge = e1;
        HalfEdge *currentEdge = e1;

        do {
            // 找到与 currentEdge 的 vEnd 匹配的半边
            for (uint8_t j = 0; j < numEdges; j++)
            {
                HalfEdge *candidate = &edgeArray[j];
                if (candidate == currentEdge || candidate->face != currentEdge->face)
                    continue; // 跳过非同面半边

                if (currentEdge->vEnd == candidate->vStart && currentEdge->next == NULL)
                {
                    currentEdge->next = candidate;
                    candidate->prev = currentEdge;
                    break; // 找到后跳出内循环
                }
            }
            currentEdge = currentEdge->next;
        } while (currentEdge != NULL && currentEdge != startEdge);
    }
}
/**
  *@ FunctionName: void SetHalfEdgeRelations(HalfEdge *edgeArray, uint8_t numEdges)
  *@ Author: CzrTuringB
  *@ Brief: 设置半边结构体相关参数
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void SetHalfEdgeRelations(HalfEdge *edgeArray, uint8_t numEdges)
{
    SetHalfEdgePairs(edgeArray, numEdges);
    SetHalfEdgeNextPrev(edgeArray, numEdges);
}
/* 圆柱------------------------------------------------------------------*/
/**
  *@ FunctionName: void CylinderInit(Cylinder *cyl, float r, float h, float scale, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 初始化圆柱体相关参数
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void CylinderInit(Cylinder *cyl, float r, float h, float scale, Point2D pc)
{
	//1、初始化运动转换参数
	cyl->move.scale = scale;
	cyl->move.pc = pc;
	cyl->move.rot = (RotateValue){0,0,0,0,0,0};
	//2、创建半边
	CreateHalfEdge(&cyl->edges[0], &cyl->vertices[0], &cyl->vertices[1]);
	CreateHalfEdge(&cyl->edges[1], &cyl->vertices[1], &cyl->vertices[2]);
	CreateHalfEdge(&cyl->edges[2], &cyl->vertices[2], &cyl->vertices[3]);
	CreateHalfEdge(&cyl->edges[3], &cyl->vertices[3], &cyl->vertices[0]);
	CreateHalfEdge(&cyl->edges[4], &cyl->vertices[4], &cyl->vertices[5]);
	CreateHalfEdge(&cyl->edges[5], &cyl->vertices[5], &cyl->vertices[6]);
	CreateHalfEdge(&cyl->edges[6], &cyl->vertices[6], &cyl->vertices[7]);
	CreateHalfEdge(&cyl->edges[7], &cyl->vertices[7], &cyl->vertices[4]);
	//3、创建顶点
	CreateVertex(&cyl->vertices[0], (Point3D){ -r, 0.5 * h, 0 }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[1], (Point3D){ 0, 0.5 * h, r }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[2], (Point3D){ r, 0.5 * h, 0 }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[3], (Point3D){ 0, 0.5 * h, -r }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[4], (Point3D){ -r, -0.5 * h, 0 }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[5], (Point3D){ 0, -0.5 * h, -r }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[6], (Point3D){ r, -0.5 * h, 0 }, cyl->edges, 8, scale, pc);
	CreateVertex(&cyl->vertices[7], (Point3D){ 0, -0.5 * h, r }, cyl->edges, 8, scale, pc);
	//4、创建面，半边形参方向遵循右手定则向外
	CreateFace(&cyl->faces[0], &cyl->edges[0], (Point3D){ 0, 1, 0 }, cyl->edges, 8);
	CreateFace(&cyl->faces[1], &cyl->edges[7], (Point3D){ 0, -1, 0 }, cyl->edges,8);
	//5、初始化半边的其余参数
	SetHalfEdgeRelations(&cyl->edges[0], 8);
	//6、计算圆柱空间最长距离
	cyl->r = r;
	cyl->h = h;
	float xD = 2*r;
	float yD = h;
	cyl->maxDistence = sqrt(xD*xD+yD*yD);
}
/**
  *@ FunctionName: void UpdateCylinder(Cylinder* cyl)
  *@ Author: CzrTuringB
  *@ Brief: 更新圆柱运动参数
  *@ Time: Dec 26, 2024
  *@ Requirement：
  *@ 	1、圆柱要避免绕y轴旋转
  */
void UpdateCylinder(Cylinder* cyl)
{
    for (uint8_t i = 0; i < 8; i++)
    {
    	cyl->vertices[i].p3 = cyl->vertices[i].p;
        RotatePoint3D(&cyl->vertices[i].p3, cyl->move.rot);
        OrtProject(cyl->vertices[i].p3, &cyl->vertices[i].p2, cyl->move.scale, cyl->move.pc);
    }
}
/**
  *@ FunctionName: void PlotCylinder(Cylinder* cyl, SSDBuffer buffer, FillMode mode, Plot3Mode pMode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制圆柱体
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void PlotCylinder(Cylinder* cyl, SSDBuffer buffer, FillMode mode, Plot3Mode pMode)
{
	//特殊情况处理：只看到一个圆形
	if(cyl->vertices[0].p2.x == cyl->vertices[4].p2.x)
	{
		if(cyl->vertices[0].p2.y == cyl->vertices[4].p2.y)
		{
			PlotCircle(cyl->move.pc,cyl->move.scale*cyl->r, buffer, mode);
			return;
		}
	}
	//特殊情况处理：只看到一个矩形
	if(cyl->vertices[1].p2.x == cyl->vertices[3].p2.x)
	{
		if(cyl->vertices[1].p2.y == cyl->vertices[3].p2.y)
		{
			Point2D p[4] = {cyl->vertices[0].p2,cyl->vertices[2].p2,cyl->vertices[6].p2,cyl->vertices[4].p2};
			PlotQuadrilateral(p, buffer, FillF);
			return;
		}
	}
    //1、设置视点
    Point3D viewPoint = {0, 0, cyl->maxDistence};
    //2、绘制椭圆轮廓
	PlotEllipse(cyl->vertices[0].p2, cyl->vertices[1].p2, cyl->vertices[2].p2, cyl->vertices[3].p2, buffer, mode, False);
	PlotEllipse(cyl->vertices[4].p2, cyl->vertices[5].p2, cyl->vertices[6].p2, cyl->vertices[7].p2, buffer, mode, False);
	if(pMode != NoHidden)
	{
		//3、清除上下椭圆可能被遮挡的部分
		Point2D p[4] = {cyl->vertices[0].p2,cyl->vertices[2].p2,cyl->vertices[6].p2,cyl->vertices[4].p2};
		PlotFillQuadrilateral(p, buffer, Fill0);
		//遍历面判断其是否被遮挡
		for(uint8_t i=0;i<2;i++)
		{
			if(IsFrontFace(&cyl->faces[i], viewPoint) == True)
			{
				PlotEllipse(cyl->vertices[i*4].p2, cyl->vertices[i*4+1].p2, cyl->vertices[i*4+2].p2, cyl->vertices[i*4+3].p2, buffer, mode, False);
			}
			else if(pMode == DashedHidden)
			{
				PlotEllipse(cyl->vertices[i*4].p2, cyl->vertices[i*4+1].p2, cyl->vertices[i*4+2].p2, cyl->vertices[i*4+3].p2, buffer, mode, True);
			}
		}
	}
	//绘制侧边直线
	PlotLine(cyl->vertices[0].p2, cyl->vertices[4].p2, buffer, mode);
	PlotLine(cyl->vertices[2].p2, cyl->vertices[6].p2, buffer, mode);
}
/* 圆锥------------------------------------------------------------------*/
/**
  *@ FunctionName: void ConeInit(Cone *con, float r, float h, float scale, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 初始化圆锥
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void ConeInit(Cone *con, float r, float h, float scale, Point2D pc)
{
	//1、初始化运动转换参数
	con->move.scale = scale;
	con->move.pc = pc;
	con->move.rot = (RotateValue){0,0,0,0,0,0};
	//2、创建半边
	CreateHalfEdge(&con->edges[0], &con->vertices[0], &con->vertices[1]);
	CreateHalfEdge(&con->edges[1], &con->vertices[1], &con->vertices[2]);
	CreateHalfEdge(&con->edges[2], &con->vertices[2], &con->vertices[3]);
	CreateHalfEdge(&con->edges[3], &con->vertices[3], &con->vertices[0]);
	//3、创建顶点
	CreateVertex(&con->vertices[0], (Point3D){ -r, 0, 0 }, con->edges, 4, scale, pc);
	CreateVertex(&con->vertices[1], (Point3D){ 0, 0, -r }, con->edges, 4, scale, pc);
	CreateVertex(&con->vertices[2], (Point3D){ r, 0, 0 }, con->edges, 4, scale, pc);
	CreateVertex(&con->vertices[3], (Point3D){ 0, 0, r }, con->edges, 4, scale, pc);
	CreateVertex(&con->vertices[4], (Point3D){ 0, -h, 0 }, NULL, 0, scale, pc);
	//4、创建面，半边形参方向遵循右手定则向外
	CreateFace(&con->faces[0], &con->edges[0], (Point3D){ 0, -1, 0 }, con->edges, 4);
	//5、初始化半边的其余参数
	SetHalfEdgeRelations(&con->edges[0], 4);
	//6、计算圆锥空间最长距离
	float xD = 2*r;
	float yD = h;
	con->r = r;
	con->h = h;
	con->maxDistence = sqrt(xD*xD+yD*yD);
}
/**
  *@ FunctionName: void ConeInit(Cone *con, float r, float h, float scale, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 更新圆锥运动参数
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void UpdateCone(Cone* con)
{
    for (uint8_t i = 0; i < 5; i++)
    {
    	con->vertices[i].p3 = con->vertices[i].p;
        RotatePoint3D(&con->vertices[i].p3, con->move.rot);
        OrtProject(con->vertices[i].p3, &con->vertices[i].p2, con->move.scale, con->move.pc);
    }
}
/**
  *@ FunctionName: void PlotCone(Cone* con, SSDBuffer buffer, FillMode mode, Plot3Mode pMode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制圆锥
  *@ Time: Dec 26, 2024
  *@ Requirement：
  *@ 	1、圆锥要避免绕Y轴旋转
  */
void PlotCone(Cone* con, SSDBuffer buffer, FillMode mode, Plot3Mode pMode)
{
	//特殊情况处理：只看到一个圆形
	if(con->vertices[4].p2.x == con->move.pc.x)
	{
		if(con->vertices[4].p2.y == con->move.pc.y)
		{
			PlotCircle(con->move.pc,con->move.scale*con->r, buffer, mode);
			return;
		}
	}
	//特殊情况处理：只看到一个三角形
	if(con->vertices[1].p2.x == con->vertices[3].p2.x)
	{
		if(con->vertices[1].p2.y == con->vertices[3].p2.y)
		{
			Point2D p[3] = {con->vertices[0].p2,con->vertices[2].p2,con->vertices[4].p2};
			PlotTriangle(p, buffer, FillF);
			return;
		}
	}
    //1、设置视点
    Point3D viewPoint = {0, 0, con->maxDistence};
    //2、绘制椭圆轮廓
	PlotEllipse(con->vertices[0].p2, con->vertices[1].p2, con->vertices[2].p2, con->vertices[3].p2, buffer, mode, False);
	if(pMode != NoHidden)
	{
		//3、清除上下椭圆可能被遮挡的部分
		Point2D p[3] = {con->vertices[0].p2,con->vertices[2].p2,con->vertices[4].p2};
		PlotFillTriangle(p, buffer, Fill0);
		//遍历面判断其是否被遮挡
		if(IsFrontFace(&con->faces[0], viewPoint) == True)
		{
			PlotEllipse(con->vertices[0].p2, con->vertices[1].p2, con->vertices[2].p2, con->vertices[3].p2, buffer, mode, False);
		}
		else if(pMode == DashedHidden)
		{
			PlotEllipse(con->vertices[0].p2, con->vertices[1].p2, con->vertices[2].p2, con->vertices[3].p2, buffer, mode, True);
		}
	}
	//绘制侧边直线
	PlotLine(con->vertices[4].p2, con->vertices[0].p2, buffer, mode);
	PlotLine(con->vertices[4].p2, con->vertices[2].p2, buffer, mode);
}
/* 六面体------------------------------------------------------------------*/
/**
  *@ FunctionName: void HexahedronInit(Hexahedron *cube, Point3D* p, float scale, Point2D pc)
  *@ Author: CzrTuringB
  *@ Brief: 初始化六面体配置结构体
  *@ Time: Dec 18, 2024
  *@ Requirement：
  *@ 	1、投影中心的选取应避免与屏幕边缘发生干涉
  *@ 	2、   v0---------v3
   @  	  	  /|        / |
   @	    v1-|-------v2 |
   @	 	 | |       |  |
   @	 	 | v4------|--v7
   @	 	 |/        | /
   @		v5----------v6
   @	3、三维坐标系原点为六面体的中心
  *@    4、x轴向右，y轴向上，z轴垂直于屏幕向外
  */
void HexahedronInit(Hexahedron *cube, Point3D* p, float scale, Point2D pc)
{
	//1、初始化运动转换参数
	cube->move.scale = scale;
	cube->move.pc = pc;
	cube->move.rot = (RotateValue){0,0,0,0,0,0};
	//2、创建半边
	CreateHalfEdge(&cube->edges[0], &cube->vertices[0], &cube->vertices[1]);
	CreateHalfEdge(&cube->edges[1], &cube->vertices[1], &cube->vertices[2]);
	CreateHalfEdge(&cube->edges[2], &cube->vertices[2], &cube->vertices[3]);
	CreateHalfEdge(&cube->edges[3], &cube->vertices[3], &cube->vertices[0]);

	CreateHalfEdge(&cube->edges[4], &cube->vertices[1], &cube->vertices[5]);
	CreateHalfEdge(&cube->edges[5], &cube->vertices[5], &cube->vertices[6]);
	CreateHalfEdge(&cube->edges[6], &cube->vertices[6], &cube->vertices[2]);
	CreateHalfEdge(&cube->edges[7], &cube->vertices[2], &cube->vertices[1]);

	CreateHalfEdge(&cube->edges[8], &cube->vertices[4], &cube->vertices[7]);
	CreateHalfEdge(&cube->edges[9], &cube->vertices[7], &cube->vertices[6]);
	CreateHalfEdge(&cube->edges[10], &cube->vertices[6], &cube->vertices[5]);
	CreateHalfEdge(&cube->edges[11], &cube->vertices[5], &cube->vertices[4]);

	CreateHalfEdge(&cube->edges[12], &cube->vertices[0], &cube->vertices[3]);
	CreateHalfEdge(&cube->edges[13], &cube->vertices[3], &cube->vertices[7]);
	CreateHalfEdge(&cube->edges[14], &cube->vertices[7], &cube->vertices[4]);
	CreateHalfEdge(&cube->edges[15], &cube->vertices[4], &cube->vertices[0]);

	CreateHalfEdge(&cube->edges[16], &cube->vertices[3], &cube->vertices[2]);
	CreateHalfEdge(&cube->edges[17], &cube->vertices[2], &cube->vertices[6]);
	CreateHalfEdge(&cube->edges[18], &cube->vertices[6], &cube->vertices[7]);
	CreateHalfEdge(&cube->edges[19], &cube->vertices[7], &cube->vertices[3]);

	CreateHalfEdge(&cube->edges[20], &cube->vertices[1], &cube->vertices[0]);
	CreateHalfEdge(&cube->edges[21], &cube->vertices[0], &cube->vertices[4]);
	CreateHalfEdge(&cube->edges[22], &cube->vertices[4], &cube->vertices[5]);
	CreateHalfEdge(&cube->edges[23], &cube->vertices[5], &cube->vertices[1]);
	//3、创建顶点
	for(uint8_t i=0; i<8;i++)
	{
		CreateVertex(&cube->vertices[i], p[i], cube->edges, 24, scale, pc);
	}
	//4、创建面，半边形参方向遵循右手定则向外
	CreateFace(&cube->faces[0], &cube->edges[0], (Point3D){ 0, 1, 0 }, cube->edges, 24);
	CreateFace(&cube->faces[1], &cube->edges[4], (Point3D){ 0, 0, 1 }, cube->edges,24);
	CreateFace(&cube->faces[2], &cube->edges[8], (Point3D){ 0, -1, 0 }, cube->edges, 24);
	CreateFace(&cube->faces[3], &cube->edges[12], (Point3D){ 0, 0, -1 }, cube->edges,24);
	CreateFace(&cube->faces[4], &cube->edges[16], (Point3D){ 1, 0, 0 }, cube->edges, 24);
	CreateFace(&cube->faces[5], &cube->edges[20], (Point3D){ -1, 0, 0 }, cube->edges,24);
	//5、初始化半边的其余参数
	SetHalfEdgeRelations(cube->edges, 24);
	//6、计算六面体空间最长距离
	float xD = cube->vertices[0].p3.x - cube->vertices[6].p3.x;
	float yD = cube->vertices[0].p3.y - cube->vertices[6].p3.y;
	float zD = cube->vertices[0].p3.z - cube->vertices[6].p3.z;
	cube->maxDistence = sqrt(xD*xD+yD*yD+zD*zD);
}
/**
  *@ FunctionName: void CubeInit(Hexahedron *cube, float scale, Point2D pc, float length)
  *@ Author: CzrTuringB
  *@ Brief: 初始化正方体配置结构体
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void CubeInit(Hexahedron *cube, float scale, Point2D pc, float length)
{
	Point3D p[8] =
	{
			(Point3D){-0.5*length, 0.5*length, -0.5*length},
			(Point3D){-0.5*length, 0.5*length, 0.5*length},
			(Point3D){0.5*length, 0.5*length, 0.5*length},
			(Point3D){0.5*length, 0.5*length, -0.5*length},
			(Point3D){-0.5*length, -0.5*length, -0.5*length},
			(Point3D){-0.5*length, -0.5*length, 0.5*length},
			(Point3D){0.5*length, -0.5*length, 0.5*length},
			(Point3D){0.5*length, -0.5*length, -0.5*length},
	};
	HexahedronInit(cube, p, scale, pc);
}
/**
  *@ FunctionName: void CuboidInit(Hexahedron *cube, float scale, Point2D pc, float length, float width, float Height)
  *@ Author: CzrTuringB
  *@ Brief: 初始化长方体配置结构体
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void CuboidInit(Hexahedron *cube, float scale, Point2D pc, float length, float width, float Height)
{
	Point3D p[8] =
	{
			(Point3D){-0.5*length, 0.5*Height, -0.5*width},
			(Point3D){-0.5*length, 0.5*Height, 0.5*width},
			(Point3D){0.5*length, 0.5*Height, 0.5*width},
			(Point3D){0.5*length, 0.5*Height, -0.5*width},
			(Point3D){-0.5*length, -0.5*Height, -0.5*width},
			(Point3D){-0.5*length, -0.5*Height, 0.5*width},
			(Point3D){0.5*length, -0.5*Height, 0.5*width},
			(Point3D){0.5*length, -0.5*Height, -0.5*width},
	};
	HexahedronInit(cube, p, scale, pc);
}
/**
  *@ FunctionName: void UpdateHexahedron(Hexahedron* cube)
  *@ Author: CzrTuringB
  *@ Brief: 更新六面体参数
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void UpdateHexahedron(Hexahedron* cube)
{
    for (uint8_t i = 0; i < 8; i++)
    {
    	cube->vertices[i].p3 = cube->vertices[i].p;
        RotatePoint3D(&cube->vertices[i].p3, cube->move.rot);
        OrtProject(cube->vertices[i].p3, &cube->vertices[i].p2, cube->move.scale, cube->move.pc);
    }
}
/**
  *@ FunctionName: void PlotHexahedron(Hexahedron* cube, SSDBuffer buffer, FillMode mode, Plot3Mode pMode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制六面体
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void PlotHexahedron(Hexahedron* cube, SSDBuffer buffer, FillMode mode, Plot3Mode pMode)
{
    //1、设置视点
    Point3D viewPoint = {0, 0, cube->maxDistence};
    Bool isDashed = False;
    //2、判断不可视面
    for (uint8_t i = 0; i < 6; i++)
    {
    	if(pMode != NoHidden)
    	{
    		//判断平面是否在视角的后面
    		if(IsFrontFace(&cube->faces[i], viewPoint) == False)
    		{
    			if(pMode == Hidden)	continue;
    			else	isDashed = True;
    		}
    	}
    	//遍历当前面的半边然后绘制线条
    	HalfEdge* temp;
    	temp = cube->faces[i].halfEdge;
    	do
    	{
    	    // 获取起始点和结束点的投影坐标
    	    Point2D start = temp->vStart->p2;  // 投影后的 2D 坐标
    	    Point2D end = temp->vEnd->p2;
    	    if(isDashed == True)
    	    {
    	    	PlotDashedLine(start, end, buffer, 1, 3, mode);
    	    }
    	    else
    	    {
    	    	PlotLine(start, end, buffer, mode);
    	    }
    	    temp = temp->next;
    	}while (temp != cube->faces[i].halfEdge);
    	isDashed = False;
    }
}
/* 三维坐标轴------------------------------------------------------------------*/
/**
  *@ FunctionName: void Plot3Axes(Point2D pc,uint8_t length,RotateValue rot,SSDBuffer buffer,FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制三维坐标轴
  *@ Time: Dec 27, 2024
  *@ Requirement：
  */
void Plot3Axes(Point2D pc,uint8_t length,RotateValue rot,SSDBuffer buffer,FillMode mode)
{
	Axes worldAxes;
	worldAxes.pc = pc;
	worldAxes.vertices3[0] = (Point3D){0,0,length};
	worldAxes.vertices3[1] = (Point3D){0,length,0};
	worldAxes.vertices3[2] = (Point3D){length,0,0};
	for(uint8_t i = 0; i<3; i++)
	{

		RotatePoint3D(&worldAxes.vertices3[i], rot);
		OrtProject(worldAxes.vertices3[i], &worldAxes.vertices2[i], 1.0, pc);
	}
	PlotLine(pc,worldAxes.vertices2[0],buffer, mode);
	PlotLine(pc,worldAxes.vertices2[1],buffer, mode);
	PlotLine(pc,worldAxes.vertices2[2],buffer, mode);
	PlotArrow(pc,worldAxes.vertices2[0], buffer, mode, EndArrow);
	PlotArrow(pc,worldAxes.vertices2[1], buffer, mode, EndArrow);
	PlotArrow(pc,worldAxes.vertices2[2], buffer, mode, EndArrow);
}

/**
  *@ FileName: UiCore.c
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "UiPlot.h"
#include "UiAnimation.h"
#include "UiFontLibrary.h"
#include "UiPictureLibrary.h"
/* Data ------------------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: Bool EdgeDetect(Point2D p)
  *@ Author: CzrTuringB
  *@ Brief: 待绘制点有效性检测
  *@ Time: Dec 24, 2024
  *@ Requirement：
  *@ 	1、超出屏幕范围返回True;
  *@ 	2、没有超出屏幕范围返回False;
  */
Bool EdgeDetect(Point2D p)
{
	if(p.x < 0 || p.y < 0) return True;
	if(p.x >= ScreenWidth || p.y > ScreenWidth) return True;
	return False;
}
/**
  *@ FunctionName: void PlotPoint(Point2D p,SSDBuffer buffer,FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 根据P点x,y坐标绘制点
  *@ Time: Oct 30, 2024
  *@ Requirement：
  *@ 	1、坐标系：
  *@ 		y63
  *@		 |
  *@		 |
  *@		 |
  *@	   y0/x0-------------y127
  *@	2、buffer为一个指向屏幕显示缓冲区或蒙版缓冲区的二维数组的地址
  */
void PlotPoint(Point2D p,SSDBuffer buffer,FillMode mode)
{
	//超出屏幕边界
	if(EdgeDetect(p) == True)	return;
	uint8_t index = p.y / 8;
	uint8_t shift = p.y % 8;
	switch(mode)
	{
		case Fill0:
			buffer[index][p.x] &= (~((1 << shift)));
			break;
		case Fill1:
			buffer[index][p.x] |= (1 << shift);
			break;
		case Fill5:
			if(p.x%2 == 0)	buffer[index][p.x] |= (0xAA & (1 << shift));
			else			buffer[index][p.x] |= (0x55 & (1 << shift));
			break;
		case FillA:
			if(p.x%2 == 0)	buffer[index][p.x] |= (0x55 & (1 << shift));
			else			buffer[index][p.x] |= (0xAA & (1 << shift));
			break;
		case FillE:
			if(p.x%2 != 0)	buffer[index][p.x] |= ((1 << shift));
			else			buffer[index][p.x] |= (0xAA & (1 << shift));
			break;
		case FillB:
			if(p.x%2 != 0)	buffer[index][p.x] |= (1 << shift);
			else			buffer[index][p.x] |= (0x55 & (1 << shift));
			break;
		case FillD:
			if(p.x%2 == 0)	buffer[index][p.x] |= ((1 << shift));
			else			buffer[index][p.x] |= (0xAA & (1 << shift));
			break;
		case Fill7:
			if(p.x%2 == 0)	buffer[index][p.x] |= (1 << shift);
			else			buffer[index][p.x] |= (0xAA & (1 << shift));
			break;
		case FillEBD7:
			if(p.x%4 == 0)	buffer[index][p.x] |= (0xEE & (1 << shift));
			if(p.x%4 == 1)	buffer[index][p.x] |= (0xBB & (1 << shift));
			if(p.x%4 == 2)	buffer[index][p.x] |= (0xDD & (1 << shift));
			if(p.x%4 == 3)	buffer[index][p.x] |= (0x77 & (1 << shift));
			break;
	}
}
/**
  *@ FunctionName: uint8_t GetPoint(uint8_t x, uint8_t y)
  *@ Author: CzrTuringB
  *@ Brief: 读取显存内容
  *@ Time: Dec 17, 2024
  *@ Requirement：
  *@ 	1、如果返回值非零则为1
  *@ 	2、如果返回值为零则为0
  */
uint8_t GetPoint(Point2D p,SSDBuffer buffer)
{
	//超出屏幕边界
	if(EdgeDetect(p) == True)	return 0;
	uint8_t index = p.y / 8;
	uint8_t shift = p.y % 8;
	return (buffer[index][p.x] & (1 << shift)) ? 1 : 0;
}
/**
  *@ FunctionName: void PlotLine(Point2D p0, Point2D p1, SSDBuffer buffer,FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 根据起始点\结束点坐标坐标绘制直线
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
void PlotLine(Point2D p0, Point2D p1, SSDBuffer buffer,FillMode mode)
{
	//超出屏幕边界
	if(EdgeDetect(p0) == True || EdgeDetect(p1) == True)	return;
	//直线在x和y方向上的变化
    int8_t dx = abs(p1.x - p0.x);
    int8_t dy = abs(p1.y - p0.y);
    //步进方向
    int8_t sx = (p0.x < p1.x) ? 1 : -1;
    int8_t sy = (p0.y < p1.y) ? 1 : -1;
    //绘制直线、
    if (dx > dy)
    {
        //当 dx > dy 时，x 轴是主轴
    	int8_t err = dx / 2;
        while (p0.x != p1.x)
        {
        	PlotPoint(p0,buffer,mode);
            err -= dy;
            if (err < 0)
            {
            	p0.y += sy;
                err += dx;
            }
            p0.x += sx;
        }
    }
    else
    {
        //当 dy > dx 时，y 轴是主轴
    	int8_t err = dy / 2;
        while (p0.y != p1.y)
        {
        	PlotPoint(p0,buffer,mode);
            err -= dx;
            if (err < 0)
            {
            	p0.x += sx;
                err += dy;
            }
            p0.y += sy;
        }
    }
    //绘制终点
    PlotPoint(p1,buffer,mode);
}
/**
  *@ FunctionName: void PlotDashedLine(Point2D p0, Point2D p1, SSDBuffer buffer, uint8_t dashLength, uint8_t gapLength, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 根据起始点\结束点坐标坐标绘制虚线
  *@ Time: Oct 30, 2024
  *@ Requirement：
  *@ 	1、dashLength表示虚线线段长度
  *@ 	2、gapLength表示虚线间隔长度
  */
void PlotDashedLine(Point2D p0, Point2D p1, SSDBuffer buffer, uint8_t dashLength, uint8_t gapLength, FillMode mode)
{
	//超出屏幕边界
	if(EdgeDetect(p0) == True || EdgeDetect(p1) == True)	return;
    // 计算直线在 x 和 y 方向上的变化
    int16_t dx = abs(p1.x - p0.x);
    int16_t dy = abs(p1.y - p0.y);

    // 确定步进方向
    int8_t sx = (p0.x < p1.x) ? 1 : -1;
    int8_t sy = (p0.y < p1.y) ? 1 : -1;

    // 初始化误差项
    int16_t err = (dx > dy ? dx : -dy) / 2;
    int16_t err2;

    // 初始化虚线绘制状态
    uint8_t segmentLength = dashLength + gapLength;  // 总周期长度
    uint8_t stepCount = 0;                           // 当前步数计数

    // 循环绘制虚线
    while (1)
    {
        // 判断当前步是否在虚线段内
        if (stepCount < dashLength)
        {
            PlotPoint(p0,buffer,mode); // 绘制点
        }

        // 更新步数计数
        stepCount = (stepCount + 1) % segmentLength;

        // 到达终点时退出
        if (p0.x == p1.x && p0.y == p1.y)
            break;

        // 更新误差和坐标
        err2 = err;
        if (err2 > -dx)
        {
            err -= dy;
            p0.x += sx;
        }
        if (err2 < dy)
        {
            err += dx;
            p0.y += sy;
        }
    }
}
/**
  *@ FunctionName: void PlotArrow(Point2D p0, Point2D p1, SSDBuffer buffer, ArrowMode aMode, FillMode fMode)
  *@ Author: CzrTuringB
  *@ Brief: 根据起始点\结束点坐标坐标绘制箭头
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
void PlotArrow(Point2D p0, Point2D p1, SSDBuffer buffer, ArrowMode aMode, FillMode fMode)
{
    // 如果起点或终点超出屏幕边界，直接返回
    if (EdgeDetect(p0) == True || EdgeDetect(p1) == True) return;

    // 内部函数：绘制箭头
    void DrawArrow(Point2D base, Point2D tip)
    {
        // 计算箭头方向的角度（弧度制）
        float angle = atan2(tip.y - base.y, tip.x - base.x);

        // 计算箭头的两条边端点
        int8_t x1 = tip.x + (int8_t)(3 * cos(angle + M_PI - 0.5)); // 箭头左边
        int8_t y1 = tip.y + (int8_t)(3 * sin(angle + M_PI - 0.5));
        int8_t x2 = tip.x + (int8_t)(3 * cos(angle + M_PI + 0.5)); // 箭头右边
        int8_t y2 = tip.y + (int8_t)(3 * sin(angle + M_PI + 0.5));

        // 绘制箭头的两条边
        PlotLine((Point2D){x1, y1}, tip, buffer, fMode);
        PlotLine((Point2D){x2, y2}, tip, buffer, fMode);
    }

    // 根据箭头模式绘制箭头
    switch (aMode)
    {
        case StartArrow:
            // 在起点绘制箭头
            DrawArrow(p1, p0);
            break;
        case EndArrow:
            // 在终点绘制箭头
            DrawArrow(p0, p1);
            break;
        case StaEndArrow:
            // 在起点和终点都绘制箭头
            DrawArrow(p1, p0);
            DrawArrow(p0, p1);
            break;
        default:
            // 如果是无效模式，不绘制箭头
            break;
    }
}
/**
  *@ FunctionName: void PlotCircle(Point2D pc, uint8_t r, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 根据起始点坐标、半径绘制圆
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
void PlotCircle(Point2D pc, uint8_t r, SSDBuffer buffer, FillMode mode)
{
	uint8_t x = 0, y = r;
	int8_t d = 1 - r;
    // 使用八分之一圆对称绘制
    while (x <= y)
    {
        // 绘制八个对称点
    	PlotPoint((Point2D){pc.x + x, pc.y + y}, buffer, mode);
    	PlotPoint((Point2D){pc.x - x, pc.y + y}, buffer, mode);
    	PlotPoint((Point2D){pc.x + x, pc.y - y}, buffer, mode);
    	PlotPoint((Point2D){pc.x - x, pc.y - y}, buffer, mode);
    	PlotPoint((Point2D){pc.x + y, pc.y + x}, buffer, mode);
    	PlotPoint((Point2D){pc.x - y, pc.y + x}, buffer, mode);
    	PlotPoint((Point2D){pc.x + y, pc.y - x}, buffer, mode);
    	PlotPoint((Point2D){pc.x - y, pc.y - x}, buffer, mode);
        // 更新参数，确定下一个点
        if (d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
/**
  *@ FunctionName: void PlotFillCircle(Point2D pc, uint8_t r, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制填充圆
  *@ Time: Dec 20, 2024
  *@ Requirement：
  */
void PlotFillCircle(Point2D pc, uint8_t r, SSDBuffer buffer, FillMode mode)
{
	uint8_t x = 0, y = r;
	int8_t d = 1 - r;

    // 使用八分之一圆对称绘制
    while (x <= y)
    {
    	PlotLine((Point2D){pc.x - x, pc.y + y}, (Point2D){pc.x + x, pc.y + y}, buffer, mode);
    	PlotLine((Point2D){pc.x - x, pc.y - y}, (Point2D){pc.x + x, pc.y - y}, buffer, mode);
    	PlotLine((Point2D){pc.x - y, pc.y + x}, (Point2D){pc.x + y, pc.y + x}, buffer, mode);
    	PlotLine((Point2D){pc.x - y, pc.y - x}, (Point2D){pc.x + y, pc.y - x}, buffer, mode);
        // 更新参数，确定下一个点
        if (d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
/**
  *@ FunctionName: int8_t inArcRange(int8_t angle, int8_t startAngle, int8_t endAngle)
  *@ Author: CzrTuringB
  *@ Brief: 判断角度是否在范围内
  *@ Time: Oct 30, 2024
  *@ Requirement：
  */
int8_t inArcRange(int16_t angle, int16_t startAngle, int16_t endAngle)
{
    // 将角度限制在 0~360
    angle = (angle + 360) % 360;
    startAngle = (startAngle + 360) % 360;
    endAngle = (endAngle + 360) % 360;
    // 判断当前角度是否在指定范围内
    if (startAngle < endAngle)
    {
        return angle >= startAngle && angle <= endAngle;
    }
    else
    {
        return angle >= startAngle || angle <= endAngle;  // 跨0度情况
    }
}
/**
  *@ FunctionName: void PlotArc(Point2D pc, uint8_t r, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 根据起始点坐标、半径、起始角度和终止角度绘制圆弧
  *@ Time: Oct 30, 2024
  *@ Requirement：
  *@ 	1、采用角度制表示角度
  */
void PlotArc(Point2D pc, uint8_t r, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode)
{
	uint8_t x = 0, y = r;
	int16_t d = 1 - r;

    while (x <= y)
    {
        // 计算当前点的八个对称点的角度
        int angles[8] =
        {
            (int16_t)(atan2(y, x) * 180 / M_PI),
            (int16_t)(atan2(y, -x) * 180 / M_PI),
            (int16_t)(atan2(-y, x) * 180 / M_PI),
            (int16_t)(atan2(-y, -x) * 180 / M_PI),
            (int16_t)(atan2(x, y) * 180 / M_PI),
            (int16_t)(atan2(x, -y) * 180 / M_PI),
            (int16_t)(atan2(-x, y) * 180 / M_PI),
            (int16_t)(atan2(-x, -y) * 180 / M_PI)
        };
        // 绘制每个符合角度条件的对称点
        if (inArcRange(angles[0], startAngle, endAngle)) PlotPoint((Point2D){pc.x + x, pc.y + y}, buffer, mode);
        if (inArcRange(angles[1], startAngle, endAngle)) PlotPoint((Point2D){pc.x - x, pc.y + y}, buffer, mode);
        if (inArcRange(angles[2], startAngle, endAngle)) PlotPoint((Point2D){pc.x + x, pc.y - y}, buffer, mode);
        if (inArcRange(angles[3], startAngle, endAngle)) PlotPoint((Point2D){pc.x - x, pc.y - y}, buffer, mode);
        if (inArcRange(angles[4], startAngle, endAngle)) PlotPoint((Point2D){pc.x + y, pc.y + x}, buffer, mode);
        if (inArcRange(angles[5], startAngle, endAngle)) PlotPoint((Point2D){pc.x - y, pc.y + x}, buffer, mode);
        if (inArcRange(angles[6], startAngle, endAngle)) PlotPoint((Point2D){pc.x + y, pc.y - x}, buffer, mode);
        if (inArcRange(angles[7], startAngle, endAngle)) PlotPoint((Point2D){pc.x - y, pc.y - x}, buffer, mode);

        // 更新决策参数
        if (d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
/**
  *@ FunctionName: void PlotFillArc(Point2D pc, uint8_t r, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode, uint8_t Boundry)
  *@ Author: CzrTuringB
  *@ Brief: 填充圆弧
  *@ Time: Dec 17, 2024
  *@ Requirement：
  */
void PlotFillArc(Point2D pc, uint8_t r, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode, uint8_t Boundry)
{
	uint8_t x, y;
	if(Boundry == 1)
	{
		//绘制圆弧
		PlotArc(pc, r, buffer, startAngle, endAngle, Fill1);
	    //绘制圆边
		PlotLine(pc, (Point2D){pc.x+r*cos(startAngle/180.0*M_PI), pc.y+r*sin(startAngle/180.0*M_PI)}, buffer, Fill1);
		PlotLine(pc, (Point2D){pc.x+r*cos(endAngle/180.0*M_PI), pc.y+r*sin(endAngle/180.0*M_PI)}, buffer, Fill1);
	}
	else
	{
		//绘制圆弧
		PlotArc(pc, r, buffer, startAngle, endAngle, Fill0);
	    //绘制圆边
		PlotLine(pc, (Point2D){pc.x+r*cos(startAngle/180.0*M_PI), pc.y+r*sin(startAngle/180.0*M_PI)}, buffer, Fill0);
		PlotLine(pc, (Point2D){pc.x+r*cos(endAngle/180.0*M_PI), pc.y+r*sin(endAngle/180.0*M_PI)}, buffer, Fill0);
	}
    //计算递归原点
    x = pc.x + 0.5*r*cos((startAngle+endAngle)/360.0*M_PI);
    y = pc.y + 0.5*r*sin((startAngle+endAngle)/360.0*M_PI);
    //递归填充
    PlotFillPolygon((Point2D){x, y}, buffer, mode, Boundry);
}
void PlotRingWithRoundedEnds(Point2D pc, uint8_t outerRadius, uint8_t innerRadius, SSDBuffer buffer, int16_t startAngle, int16_t endAngle, FillMode mode)
{
    uint8_t ringThickness = outerRadius - innerRadius;
    uint8_t halfThickness = ringThickness / 2;
    // 绘制外圆弧
    PlotArc(pc, outerRadius, buffer, startAngle, endAngle, mode);
    // 绘制内圆弧
    PlotArc(pc, innerRadius, buffer, startAngle, endAngle, mode);
    // 起始半圆圆心计算
    Point2D startCapCenter = {
        .x = pc.x + (int16_t)((outerRadius - halfThickness) * cos(startAngle * M_PI / 180)),
        .y = pc.y + (int16_t)((outerRadius - halfThickness) * sin(startAngle * M_PI / 180))
    };
    // 结束半圆圆心计算
    Point2D endCapCenter = {
        .x = pc.x + (int16_t)((outerRadius - halfThickness) * cos(endAngle * M_PI / 180)),
        .y = pc.y + (int16_t)((outerRadius - halfThickness) * sin(endAngle * M_PI / 180))
    };
    // 绘制起始半圆
    PlotArc(startCapCenter, halfThickness, buffer, startAngle-180, startAngle, mode);
    // 绘制结束半圆
    PlotArc(endCapCenter, halfThickness, buffer, endAngle, endAngle+180, mode);
}
/**
  *@ FunctionName: void PlotTriangle(Point2D* p, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制三角形
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void PlotTriangle(Point2D* p, SSDBuffer buffer, FillMode mode)
{
	PlotLine(p[0], p[1], buffer, mode);
	PlotLine(p[1], p[2], buffer, mode);
	PlotLine(p[2], p[0], buffer, mode);
}
/**
  *@ FunctionName: void PlotFillTriangle(Point2D* p, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制填充三角形
  *@ Time: Dec 26, 2024
  *@ Requirement：
  */
void PlotFillTriangle(Point2D* p, SSDBuffer buffer, FillMode mode)
{
    // 获取边界框
    int8_t minX = p[0].x, maxX = p[0].x, minY = p[0].y, maxY = p[0].y;
    for (uint8_t i = 1; i < 3; i++)  // 三角形只有三个顶点
    {
        if (p[i].x < minX) minX = p[i].x;
        if (p[i].x > maxX) maxX = p[i].x;
        if (p[i].y < minY) minY = p[i].y;
        if (p[i].y > maxY) maxY = p[i].y;
    }

    // 扫描线填充
    for (int8_t y = minY; y <= maxY; y++)
    {
        // 存储交点
        int8_t intersections[6], n = 0;  // 三角形最多3条边
        // 计算与每条边的交点
        for (uint8_t i = 0; i < 3; i++)
        {
            Point2D p1 = p[i];
            Point2D p2 = p[(i + 1) % 3];

            // 检查扫描线是否穿过当前边
            if (y >= fmin(p1.y, p2.y) && y <= fmax(p1.y, p2.y))
            {
                // 如果边与扫描线重合（水平边），跳过
                if (p1.y == p2.y)
                    continue;

                // 计算交点 x 坐标，避免浮动误差
                int16_t x = p1.x + (int16_t)(y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);

                // 仅在 x 坐标在 minX 和 maxX 范围内时才记录交点
                if (x >= minX && x <= maxX)
                {
                    intersections[n++] = x;
                }
            }
        }

        // 去重交点，避免重复交点的影响
        for (uint8_t i = 0; i < n; i++) {
            for (uint8_t j = i + 1; j < n; j++)
            {
                if (intersections[i] == intersections[j])
                {
                    // 将重复的交点移到数组的末尾并减少数量
                    for (uint8_t k = j; k < n - 1; k++)
                    {
                        intersections[k] = intersections[k + 1];
                    }
                    n--; // 更新交点数量
                    j--; // 检查下一个元素
                }
            }
        }

        // 排序交点，确保从左到右
        for (uint8_t i = 0; i < n - 1; i++)
        {
            for (uint8_t j = i + 1; j < n; j++)
            {
                if (intersections[i] > intersections[j])
                {
                    uint8_t temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }

        // 在交点之间填充区域
        for (uint8_t i = 0; i < n - 1; i += 2)
        {
            // 填充交点之间的区域
            for (uint8_t x = intersections[i]; x <= intersections[i + 1]; x++)
            {
                PlotPoint((Point2D){x, y}, buffer, mode);
            }
        }
    }
}
/**
  *@ FunctionName: void PlotRectangle(Point2D p0, uint8_t width, uint8_t height, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制矩形
  *@ Time: Oct 30, 2024
  *@ Requirement：
  *@ 	1、起始点为矩形左下角
  */
void PlotRectangle(Point2D p0, uint8_t width, uint8_t height, SSDBuffer buffer, FillMode mode)
{
	PlotLine(p0, (Point2D){p0.x + width, p0.y}, buffer, mode);
	PlotLine((Point2D){p0.x + width , p0.y}, (Point2D){p0.x + width, p0.y + height}, buffer, mode);
	PlotLine((Point2D){p0.x + width, p0.y + height}, (Point2D){p0.x, p0.y + height}, buffer, mode);
	PlotLine((Point2D){p0.x, p0.y + height}, p0, buffer, mode);
}
/**
  *@ FunctionName: void PlotQuadrilateral(Point2D* p, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制四边形
  *@ Time: Dec 25, 2024
  *@ Requirement：
  *@ 	1、四个点坐标要按照逆时针或顺时针方向来给
  */
void PlotQuadrilateral(Point2D* p, SSDBuffer buffer, FillMode mode)
{
	PlotLine(p[0], p[1], buffer, mode);
	PlotLine(p[1], p[2], buffer, mode);
	PlotLine(p[2], p[3], buffer, mode);
	PlotLine(p[3], p[0], buffer, mode);
}
/**
  *@ FunctionName: void PlotFillRectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制填充矩形
  *@ Time: Dec 17, 2024
  *@ Requirement：
  *@ 	1、起始点为矩形左下角
  */
void PlotFillRectangle(Point2D p0, uint8_t width, uint8_t height, SSDBuffer buffer, FillMode mode)
{
    //绘制填充矩形
    for (uint8_t y = p0.y; y <= p0.y + height; y++)
    {
    	PlotLine((Point2D){p0.x, y}, (Point2D){p0.x + width, y}, buffer, mode);
    }
}
/**
  *@ FunctionName: void PlotFillQuadrilateral(Point2D* p, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 填充四边形区域
  *@ Time: Dec 25, 2024
  *@ Requirement：
  *@ 	1、四个点坐标要按照逆时针或顺时针方向来给
  */
void PlotFillQuadrilateral(Point2D* p, SSDBuffer buffer, FillMode mode)
{
    // 获取边界框
    int8_t minX = p[0].x, maxX = p[0].x, minY = p[0].y, maxY = p[0].y;
    for (uint8_t i = 1; i < 4; i++)
    {
        if (p[i].x < minX) minX = p[i].x;
        if (p[i].x > maxX) maxX = p[i].x;
        if (p[i].y < minY) minY = p[i].y;
        if (p[i].y > maxY) maxY = p[i].y;
    }

    // 扫描线填充
    for (int8_t y = minY; y <= maxY; y++)
    {
        // 存储交点
        int8_t intersections[8], n = 0;
        // 计算与每条边的交点
        for (uint8_t i = 0; i < 4; i++)
        {
            Point2D p1 = p[i];
            Point2D p2 = p[(i + 1) % 4];

            // 检查扫描线是否穿过当前边
            if (y >= fmin(p1.y, p2.y) && y <= fmax(p1.y, p2.y))
            {
                // 如果边与扫描线重合（水平边），跳过或添加该交点
                if (p1.y == p2.y) // 水平边
                {
                    continue;
                }
                // 计算交点 x 坐标，避免浮动误差
                int16_t x = p1.x + (int16_t)(y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                // 仅在 x 坐标在 minX 和 maxX 范围内时才记录交点
                if (x >= minX && x <= maxX)
                {
                    intersections[n++] = x;
                }
            }
        }
        // 去重交点，避免重复交点的影响
        for (uint8_t i = 0; i < n; i++) {
            for (uint8_t j = i + 1; j < n; j++)
            {
                if (intersections[i] == intersections[j])
                {
                    // 将重复的交点移到数组的末尾并减少数量
                    for (uint8_t k = j; k < n - 1; k++)
                    {
                        intersections[k] = intersections[k + 1];
                    }
                    n--; // 更新交点数量
                    j--; // 检查下一个元素
                }
            }
        }
        // 排序交点，确保从左到右
        for (uint8_t i = 0; i < n - 1; i++)
        {
            for (uint8_t j = i + 1; j < n; j++)
            {
                if (intersections[i] > intersections[j])
                {
                    uint8_t temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }
        // 在交点之间填充区域
        for (uint8_t i = 0; i < n - 1; i += 2)
        {
            // 填充交点之间的区域
            for (uint8_t x = intersections[i]; x <= intersections[i + 1]; x++)
            {
                PlotPoint((Point2D){x, y}, buffer, mode);
            }
        }
    }
}
/**
  *@ FunctionName: void PlotArcRectangle(Point2D p0, uint8_t width, uint8_t height, uint8_t r, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制圆角矩形
  *@ Time: Oct 31, 2024
  *@ Requirement：
  *@	1、起始点为矩形左下角
  */
void PlotArcRectangle(Point2D p0, uint8_t width, uint8_t height, uint8_t r, SSDBuffer buffer, FillMode mode)
{
	PlotLine((Point2D){p0.x + r, p0.y}, (Point2D){p0.x + width - r, p0.y}, buffer, mode);  // 顶边
	PlotLine((Point2D){p0.x + r, p0.y + height}, (Point2D){p0.x + width - r, p0.y + height}, buffer, mode);  // 底边
	PlotLine((Point2D){p0.x, p0.y + r}, (Point2D){p0.x,p0.y + height - r}, buffer, mode);  // 左边
	PlotLine((Point2D){p0.x + width, p0.y + r}, (Point2D){p0.x + width, p0.y + height - r}, buffer, mode);  // 右边

	PlotArc((Point2D){p0.x + r, p0.y + height - r}, r, buffer, 90, 180, mode);
	PlotArc((Point2D){p0.x + r, p0.y + r}, r, buffer, 180, 270, mode);
	PlotArc((Point2D){p0.x + width - r, p0.y + r}, r, buffer, 270, 360, mode);
	PlotArc((Point2D){p0.x + width - r, p0.y + height - r}, r, buffer, 0, 90, mode);
}
/**
  *@ FunctionName: void PlotPolygon(Point2D pc, uint8_t radius, uint8_t sides, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制正多边形
  *@ Time: Oct 31, 2024
  *@ Requirement：
  */
void PlotPolygon(Point2D pc, uint8_t radius, uint8_t sides, SSDBuffer buffer, FillMode mode)
{
	//角度间隔
    double angleStep = 2 * M_PI / sides;
    int x[sides], y[sides];

    // 计算每个顶点的坐标
    for (int i = 0; i < sides; i++)
    {
        x[i] = pc.x + (int)(radius * cos(i * angleStep + 0.5 * M_PI) + 0.5);  // 四舍五入为整数
        y[i] = pc.y + (int)(radius * sin(i * angleStep + 0.5 * M_PI) + 0.5);  // 四舍五入为整数
    }

    // 绘制多边形的边
    for (int i = 0; i < sides; i++)
    {
        PlotLine((Point2D){x[i], y[i]}, (Point2D){x[(i + 1) % sides], y[(i + 1) % sides]}, buffer, mode);
    }
}
/**
  *@ FunctionName: void PlotWave(Point2D p0, uint8_t length, uint8_t amplitude, uint8_t frequency, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制波形
  *@ Time: Oct 31, 2024
  *@ Requirement：
  */
void PlotWave(Point2D p0, uint8_t length, uint8_t amplitude, uint8_t frequency, SSDBuffer buffer, FillMode mode)
{
	int8_t prevX = p0.x, prevY = p0.y;  // 起点
	uint8_t x, y;

    // 逐步绘制波浪线
    for (x = p0.x; x <= p0.x + length; x++)
    {
        // 使用正弦函数计算波形的 y 值
        y = p0.y + (int)(amplitude * sin(2 * M_PI * frequency * (x - p0.x) / length));
        // 绘制当前点和前一个点之间的线段
        PlotLine((Point2D){prevX,prevY}, (Point2D){x,y}, buffer, mode);
        // 更新前一个点的坐标
        prevX = x;
        prevY = y;
    }
}
/**
  *@ FunctionName: void PlotEllipse(Point2D p0, Point2D p1, Point2D p2, Point2D p3, SSDBuffer buffer, FillMode mode, Bool isDashed)
  *@ Author: CzrTuringB
  *@ Brief: 绘制椭圆
  *@ Time: Oct 31, 2024
  *@ Requirement：
  */
void PlotEllipse(Point2D p0, Point2D p1, Point2D p2, Point2D p3, SSDBuffer buffer, FillMode mode, Bool isDashed)
{
    // 计算椭圆的中心点
    int xc = (p0.x + p2.x) / 2;  // 长轴中心
    int yc = (p1.y + p3.y) / 2;  // 短轴中心

    // 计算长轴和短轴的半径（直接计算两点间的距离）
    float a = sqrt((p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y - p0.y)) / 2;  // 长轴半径
    float b = sqrt((p3.x - p1.x) * (p3.x - p1.x) + (p3.y - p1.y) * (p3.y - p1.y)) / 2;  // 短轴半径

    // 如果长轴小于短轴，交换它们
    if (a < b)
    {
        float temp = a;
        a = b;
        b = temp;
    }

    // 计算旋转角度，使用 atan2 来计算长轴方向的角度
    float angle = atan2(p2.y - p0.y, p2.x - p0.x);  // 使用 atan2 计算旋转角度

    // 预计算旋转矩阵的cos和sin值
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    // 椭圆绘制的核心：Bresenham算法
    int x = 0, y = b;  // 起点
    int a2 = a * a, b2 = b * b;
    int two_a2 = 2 * a2, two_b2 = 2 * b2;
    int p;
    int dx = y * two_a2, dy = x * two_b2;

    // 用来控制虚线间隔的计数器
    int dashCount = 0;
    const int dashInterval = 4;  // 每隔两个像素绘制一个点

    // 第一阶段
    p = (b2 - a2 * b + 0.25 * a2);  // 初始决策参数
    while (dx > dy)
    {
        // 根据isDashed判断是否绘制虚线
        if (!isDashed || dashCount % dashInterval == 0) // 判断是否绘制点
        {
            int xRot = (int)(xc + x * cosTheta - y * sinTheta);
            int yRot = (int)(yc + x * sinTheta + y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);

            xRot = (int)(xc - x * cosTheta - y * sinTheta);
            yRot = (int)(yc - x * sinTheta + y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);

            xRot = (int)(xc + x * cosTheta + y * sinTheta);
            yRot = (int)(yc + x * sinTheta - y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);

            xRot = (int)(xc - x * cosTheta + y * sinTheta);
            yRot = (int)(yc - x * sinTheta - y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);
        }

        dashCount++;  // 增加计数器
        x++;
        dy += two_b2;
        if (p < 0) {
            p += b2 + dy;
        } else {
            y--;
            dx -= two_a2;
            p += b2 + dy - dx;
        }
    }

    // 第二阶段
    p = (b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2);  // 更新决策参数
    while (y >= 0)
    {
        // 根据isDashed判断是否绘制虚线
        if (!isDashed || dashCount % dashInterval == 0) // 判断是否绘制点
        {
            int xRot = (int)(xc + x * cosTheta - y * sinTheta);
            int yRot = (int)(yc + x * sinTheta + y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);

            xRot = (int)(xc - x * cosTheta - y * sinTheta);
            yRot = (int)(yc - x * sinTheta + y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);

            xRot = (int)(xc + x * cosTheta + y * sinTheta);
            yRot = (int)(yc + x * sinTheta - y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);

            xRot = (int)(xc - x * cosTheta + y * sinTheta);
            yRot = (int)(yc - x * sinTheta - y * cosTheta);
            PlotPoint((Point2D){xRot, yRot}, buffer, mode);
        }

        dashCount++;  // 增加计数器
        y--;
        dx -= two_a2;
        if (p > 0)
        {
            p += a2 - dx;
        }
        else
        {
            x++;
            dy += two_b2;
            p += a2 - dx + dy;
        }
    }
}
/**
  *@ FunctionName: void PlotChar(Point2D p0, uint8_t chr, FontSize charSize, SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 绘制字符
  *@ Time: Oct 31, 2024
  *@ Requirement：
  *@ 	1、p0为左下角点坐标
  */
void PlotChar(Point2D p0, uint8_t chr, FontSize charSize, SSDBuffer buffer)
{
	uint8_t fontIndex = chr - ' ';
	uint8_t yShift = p0.y%8;
	uint8_t yIndex = p0.y/8;

	if(charSize == C8x16)
	{
		//绘制大字体
		for(uint8_t i=0; i<16; i++)
		{
			if(uiDevice->dataValid == False)
			{
				if (i < 8)
				{
					buffer[yIndex+2][p0.x] |= (Font8x16[fontIndex * 16 + i] >> (7 - yShift));
					buffer[yIndex+1][p0.x] |= (Font8x16[fontIndex * 16 + i] << yShift);
				}
				else
				{
					buffer[yIndex+1][p0.x-8] |= (Font8x16[fontIndex * 16 + i] >> (7 - yShift));
					buffer[yIndex][p0.x-8] |= (Font8x16[fontIndex * 16 + i] << yShift);
				}
			}
			else
			{
				if (i < 8)
				{
					buffer[yIndex+2][p0.x] = ~((Font8x16[fontIndex * 16 + i] >> (7 - yShift))
							| (~buffer[yIndex+2][p0.x]));
					buffer[yIndex+1][p0.x] = ~((Font8x16[fontIndex * 16 + i] << yShift)
							| (~buffer[yIndex+1][p0.x]));
				}
				else
				{
					buffer[yIndex+1][p0.x-8] = ~((Font8x16[fontIndex * 16 + i]
							>> (7 - yShift)) | (~buffer[yIndex+1][p0.x-8]));
					buffer[yIndex][p0.x-8] = ~((Font8x16[fontIndex * 16 + i] << yShift)
							| (~buffer[yIndex][p0.x-8]));
				}
			}
			p0.x++;
		}
	}
	else
	{
		for(uint8_t i=0; i<6; i++)
		{
			if(uiDevice->dataValid == False)
			{
				buffer[yIndex+1][p0.x] |= (Font6x8[fontIndex * 6 + i] >> (7 - yShift));
				buffer[yIndex][p0.x] |= (Font6x8[fontIndex * 6 + i] << yShift);
			}
			else
			{
				buffer[yIndex+1][p0.x] = ~((Font6x8[fontIndex * 6 + i] >> (7 - yShift))
						| (~buffer[yIndex+1][p0.x]));
				buffer[yIndex][p0.x] = ~((Font6x8[fontIndex * 6 + i] << yShift)
						| (~buffer[yIndex][p0.x]));
			}
			p0.x++;
		}
	}
}
/**
  *@ FunctionName: void PlotString(Point2D p0, const char* str, FontSize charSize, SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 绘制字符串
  *@ Time: Oct 31, 2024
  *@ Requirement：
  */
void PlotString(Point2D p0, const char* str, FontSize charSize, SSDBuffer buffer)
{
	size_t strSize = strlen(str);
	for(uint8_t i=0;i<strSize;i++)
	{
		PlotChar(p0,str[i],charSize, buffer);
		if(charSize == C8x16)
		{
			p0.x+=8;
		}
		else
		{
			p0.x+=6;
		}
	}
}
/**
  *@ FunctionName: void PlotBMP(Point2D p0,uint8_t width,uint8_t height,const uint8_t* picture, SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 绘制BMP图像
  *@ Time: Nov 1, 2024
  *@ Requirement：
  *@ 1、(x,y)即为图像左下角点坐标
  */
void PlotBMP(Point2D p0,uint8_t width,uint8_t height,const uint8_t* picture, SSDBuffer buffer)
{
	uint8_t pageStart = p0.y/8;
	uint8_t pageEnd   = (p0.y+height)/8;
	uint8_t shift = p0.y%8;

	for(uint8_t i=p0.x; i<p0.x+width; i++)
	{
		for(uint8_t j=pageStart; j<pageEnd;j++)
		{
			if(uiDevice->dataValid == False)
			{
				buffer[j+1][i] |= (picture[i-p0.x+(pageEnd-j-1)*width] >> (7 - shift));
				buffer[j][i] |= (picture[i-p0.x+(pageEnd-j-1)*width] << shift);
			}
			else
			{
				buffer[j+1][i] = ~((picture[i-p0.x+(pageEnd-j-1)*width] >> (7 - shift))
						| (~buffer[j+1][i]));
				buffer[j][i] = ~((picture[i-p0.x+(pageEnd-j-1)*width] << shift)
						| (~buffer[j][i]));
			}
		}
	}
}
/**
  *@ FunctionName: void Plot2Axes(Point2D pc, uint8_t xLength, uint8_t yLength, float angle, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 绘制二维坐标系
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void Plot2Axes(Point2D pc, uint8_t xLength, uint8_t yLength, float angle, SSDBuffer buffer, FillMode mode)
{
	RotateValue rot = {0,0,angle,pc.x,pc.y,0};
	uint8_t y  = pc.y + yLength;
	uint8_t x  = pc.x + xLength;
	Point2D px = (Point2D){x,pc.y};
	Point2D py = (Point2D){pc.x,y};
	RotatePoint2D(&px, rot);
	RotatePoint2D(&py, rot);
	PlotLine(pc, px, buffer, mode);
	PlotArrow(pc, px, buffer, EndArrow, mode);
	PlotLine(pc, py, buffer, mode);
	PlotArrow(pc, py, buffer, EndArrow, mode);
}
/**
  *@ FunctionName: void PlotRoseCurve(Point2D pc, float a, float k, uint16_t steps, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 玫瑰曲线的绘制
  *@ Time: Dec 18, 2024
  *@ Requirement：
  *@ 	1、xc，yc即为玫瑰曲线的中心坐标
  *@ 	2、a为曲线大小
  *@ 	3、k为玫瑰花瓣数
  *@ 	4、steps为曲线平滑程度
  */
void PlotRoseCurve(Point2D pc, float a, float k, uint16_t steps, SSDBuffer buffer, FillMode mode)
{
    float theta, r;
    Point2D p;
    float stepSize = 2 * M_PI / steps; // 每一步的角度增量

    for (uint16_t i = 0; i <= steps; i++)
    {
        theta = i * stepSize;       // 当前角度
        r = a * cos(k * theta);     // 计算极径，可以改为 sin(k * theta) 看效果
        // 将极坐标转换为笛卡尔坐标
        p.x = (uint8_t)(pc.x + r * cos(theta));
        p.y = (uint8_t)(pc.y + r * sin(theta));
        // 绘制点
        PlotPoint(p, buffer, mode);
    }
}
/**
  *@ FunctionName: void PlotHeartCurve(Point2D pc, uint16_t steps, float scale, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 心形曲线的绘制
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void PlotHeartCurve(Point2D pc, uint16_t steps, float scale, SSDBuffer buffer, FillMode mode)
{
    float t, x, y;
    Point2D p;
    float stepSize = 2 * M_PI / steps; // 每步角度增量

    for (uint16_t i = 0; i <= steps; i++)
    {
        t = i * stepSize;  // 当前角度
        // 计算心形曲线的极坐标（x, y）
        x = 16 * pow(sin(t), 3);
        y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
        // 应用缩放因子
        x *= scale;
        y *= scale;
        // 将心形曲线绘制到显示屏坐标系中
        p.x = (uint8_t)(pc.x + x);  // x坐标
        p.y = (uint8_t)(pc.y - y);  // y坐标需要反转以适应屏幕坐标系
        // 绘制点
        PlotPoint(p, buffer, mode);
    }
}
/**
  *@ FunctionName: void PlotButterflyCurve(Point2D pc, uint16_t steps, float scale, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 蝴蝶曲线的绘制
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void PlotButterflyCurve(Point2D pc, uint16_t steps, float scale, SSDBuffer buffer, FillMode mode)
{
    float t, x, y;
    Point2D p;
    float stepSize = 2 * M_PI / steps; // 每步角度增量

    for (uint16_t i = 0; i <= steps; i++)
    {
        t = i * stepSize;  // 当前角度
        // 计算蝴蝶曲线的极坐标（x, y）
        x = sin(t) * (exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t), 2));
        y = cos(t) * (exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t), 2));
        // 应用缩放因子
        x *= scale;  // x坐标放大/缩小
        y *= scale;  // y坐标放大/缩小
        // 将坐标映射到屏幕坐标范围
        p.x = (uint8_t)(pc.x + x);  // x坐标
        p.y = (uint8_t)(pc.y - y);  // y坐标需要反转，以适应屏幕坐标系
        // 绘制点
        PlotPoint(p, buffer, mode);
    }
}
/**
  *@ FunctionName: void PlotStarCurve(Point2D pc, uint16_t steps, float a, float b, float k, float scale, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 星型曲线的绘制
  *@ Time: Dec 19, 2024
  *@ Requirement：
  *@ 	1、xc，yc即为星型曲线的中心坐标
  *@ 	2、a、b决定曲线大小和形状
  *@ 	3、k为瓣数
  *@ 	4、steps为曲线平滑程度
  */
void PlotStarCurve(Point2D pc, uint16_t steps, float a, float b, float k, float scale, SSDBuffer buffer, FillMode mode)
{
    float t, x, y;         // 参数 t 和计算得到的 x, y 坐标
    Point2D p;
    float stepSize = 2 * M_PI / steps;  // 每步角度增量

    for (uint16_t i = 0; i <= steps; i++)
    {
        t = i * stepSize;  // 当前角度

        // 计算星形线的极坐标（x, y）
        x = cos(t) * (a + b * cos(k * t));
        y = sin(t) * (a + b * cos(k * t));

        // 应用缩放因子
        x *= scale;  // x坐标放大/缩小
        y *= scale;  // y坐标放大/缩小

        // 将坐标映射到屏幕坐标范围（128x64）
        p.x = (uint8_t)(pc.x + x);  // x坐标
        p.y = (uint8_t)(pc.y - y);  // y坐标需要反转，以适应屏幕坐标系
        // 绘制点
        PlotPoint(p, buffer, mode);
    }
}
/**
  *@ FunctionName: void PlotFillPolygon(Point2D pc, SSDBuffer buffer, FillMode mode, uint8_t Boundary)
  *@ Author: CzrTuringB
  *@ Brief: 种子算法填充闭合图形
  *@ Time: Dec 20, 2024
  *@ Requirement：
  *@ 	1、不要填充大面积图形(最多225个像素)
  */
void PlotFillPolygon(Point2D pc, SSDBuffer buffer, FillMode mode, uint8_t Boundary)
{
    //创建一个8*128的位图记录已填充状态，每位表示一个像素是否已填充
    uint8_t filledPoints[PageNumber][ScreenWidth] = {0};

    // 定义位操作宏
    #define IS_FILLED(p) (filledPoints[(p.y) / 8][(p.x)] & (1 << ((p.y) % 8)))
    #define MARK_FILLED(p) (filledPoints[(p.y) / 8][(p.x)] |= (1 << ((p.y) % 8)))
    #define IS_VALID(p) ((p.x) < 128 && (p.y) < 64)

    // 定义递归函数
    void Fill(Point2D p, SSDBuffer buffer)
    {
        // 边界检查，防止越界
        if (!IS_VALID(p)) return;

        // 如果点已经填充或是边界，跳过
        if (IS_FILLED(p) || GetPoint(p, buffer) == Boundary) return;

        // 填充当前点
        PlotPoint(p, buffer, mode);

        // 标记当前点为已填充
        MARK_FILLED(p);

        // 递归填充上下左右相邻点
        Fill((Point2D){p.x + 1, p.y}, buffer);
        Fill((Point2D){p.x - 1, p.y}, buffer);
        Fill((Point2D){p.x, p.y + 1}, buffer);
        Fill((Point2D){p.x, p.y - 1}, buffer);
    }
    // 从种子点开始填充
    Fill(pc, buffer);
}
/**
  *@ FunctionName: void BezierLineInit(BezierLine* line, Point2D* controlPoints, uint8_t segment)
  *@ Author: CzrTuringB
  *@ Brief: 初始化贝塞尔曲线结构体
  *@ Time: Dec 18, 2024
  *@ Requirement：
  *@ 	1、打开AutoCad软件，绘制一个从(0,0)开始到(128,64)的矩形
  *@ 	2、使用控制点策略绘制曲线形状
  *@ 	3、右键点击曲线特性，设置其控制点个数为4，并微调控制点，读取控制点坐标参数用于初始化贝塞尔结构体
  *@ 	4、段数越多，则曲线越圆滑，但计算量也就越大
  */
void BezierLineInit(BezierLine *line, Point2D *controlPoints, uint8_t segment)
{
	line->segment = segment;
	for (int i = 0; i < 4; i++)
	{
		line->controlPoints[i] = controlPoints[i];
	}
}
/**
  *@ FunctionName: void PlotBezierLine(BezierLine *line, SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 绘制贝塞尔曲线
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void PlotBezierLine(BezierLine *line, SSDBuffer buffer, FillMode mode)
{
	Point2D prevPoint = line->controlPoints[0];
	Point2D currentPoint;
	//绘制贝塞尔曲线
	for (int i = 1; i <= line->segment; i++)
	{
		float t = (float) i / line->segment;
		// 贝塞尔曲线公式
		float u = 1 - t;
		currentPoint.x = (int) (u * u * u * line->controlPoints[0].x
				+ 3 * u * u * t * line->controlPoints[1].x
				+ 3 * u * t * t * line->controlPoints[2].x
				+ t * t * t * line->controlPoints[3].x);
		currentPoint.y = (int) (u * u * u * line->controlPoints[0].y
				+ 3 * u * u * t * line->controlPoints[1].y
				+ 3 * u * t * t * line->controlPoints[2].y
				+ t * t * t * line->controlPoints[3].y);
		// 绘制曲线点
		PlotLine(prevPoint, currentPoint, buffer, mode);
		prevPoint = currentPoint;
	}
}
/**
  *@ FunctionName: void PlotBezierArrowLine(BezierLine *line, uint8_t segment, SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 绘制尾部带箭头的贝塞尔曲线
  *@ Time: Dec 18, 2024
  *@ Requirement：
  */
void PlotBezierArrowLine(BezierLine *line, SSDBuffer buffer, FillMode mode)
{
	Point2D prevPoint = line->controlPoints[0];
	Point2D currentPoint = line->controlPoints[0];
	//绘制贝塞尔曲线
	for (int i = 1; i <= line->segment; i++)
	{
		float t = (float) i / line->segment;
		// 贝塞尔曲线公式
		float u = 1 - t;
		currentPoint.x = (int) (u * u * u * line->controlPoints[0].x
				+ 3 * u * u * t * line->controlPoints[1].x
				+ 3 * u * t * t * line->controlPoints[2].x
				+ t * t * t * line->controlPoints[3].x);
		currentPoint.y = (int) (u * u * u * line->controlPoints[0].y
				+ 3 * u * u * t * line->controlPoints[1].y
				+ 3 * u * t * t * line->controlPoints[2].y
				+ t * t * t * line->controlPoints[3].y);
		// 绘制曲线点
		PlotLine(prevPoint, currentPoint, buffer, mode);
		if (i < line->segment)
		{
			prevPoint = currentPoint;
		}
	}
	//计算箭头方向（切线方向）
	float angle = atan2(currentPoint.y - prevPoint.y,
			currentPoint.x - prevPoint.x);

	//计算箭头的两个端点
	uint8_t x = currentPoint.x + (int8_t) (6 * cos(angle + M_PI - 0.5));
	uint8_t y = currentPoint.y + (int8_t) (6 * sin(angle + M_PI - 0.5));
	PlotLine(currentPoint, (Point2D){x, y}, buffer, mode);
	x = currentPoint.x + (int8_t) (6 * cos(angle + M_PI + 0.5));
	y = currentPoint.y + (int8_t) (6 * sin(angle + M_PI + 0.5));
	PlotLine(currentPoint, (Point2D){x, y}, buffer, mode);
}
/**
  *@ FunctionName: void PlotFillBlock(uint8_t bx, uint8_t by, SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 填充块
  *@ Time: Dec 25, 2024
  *@ Requirement：
  */
void PlotFillBlock(uint8_t bx, uint8_t by, SSDBuffer buffer, FillMode mode)
{
	for(uint8_t j=(8*bx);j<8;j++)
	{
		switch(mode)
		{
			case Fill0:
				buffer[by][j] = 0;
				break;
			case Fill1:
				buffer[by][j] = 0xFF;
				break;
			case Fill5:
				if(j%2 == 0)	buffer[by][j] = 0xAA;
				else			buffer[by][j] = 0x55;
				break;
			case FillA:
				if(j%2 == 0)	buffer[by][j] = 0x55;
				else			buffer[by][j] = 0xAA;
				break;
			case FillE:
				if(j%2 != 0)	buffer[by][j] = 0xFF;
				else			buffer[by][j] = 0xAA;
				break;
			case FillB:
				if(j%2 != 0)	buffer[by][j] = 0xFF;
				else			buffer[by][j] = 0x55;
				break;
			case FillD:
				if(j%2 == 0)	buffer[by][j] = 0xFF;
				else			buffer[by][j] = 0xAA;
				break;
			case Fill7:
				if(j%2 == 0)	buffer[by][j] = 0xFF;
				else			buffer[by][j] = 0xAA;
				break;
			case FillEBD7:
				if(j%4 == 0)	buffer[by][j] = 0xEE;
				if(j%4 == 1)	buffer[by][j] = 0xBB;
				if(j%4 == 2)	buffer[by][j] = 0xDD;
				if(j%4 == 3)	buffer[by][j] = 0x77;
				break;
		}
	}
}
/**
  *@ FunctionName: void PlotFillScreen(SSDBuffer buffer, FillMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 填充整个缓冲区
  *@ Time: Dec 25, 2024
  *@ Requirement：
  *@ 	1、填充完成，建议手动调用ssdwriteall函数刷屏
  */
void PlotFillScreen(SSDBuffer buffer, FillMode mode)
{
	for(uint8_t i=0;i<PageNumber;i++)
	{
		for(uint8_t j=0;j<ScreenWidth;j++)
		{
			switch(mode)
			{
				case Fill0:
					buffer[i][j] = 0x00;
					break;
				case Fill1:
					buffer[i][j] = 0xFF;
					break;
				case Fill5:
					if(j%2 == 0)	buffer[i][j] = 0xAA;
					else			buffer[i][j] = 0x55;
					break;
				case FillA:
					if(j%2 == 0)	buffer[i][j] = 0x55;
					else			buffer[i][j] = 0xAA;
					break;
				case FillE:
					if(j%2 != 0)	buffer[i][j] = 0xFF;
					else			buffer[i][j] = 0xAA;
					break;
				case FillB:
					if(j%2 != 0)	buffer[i][j] = 0xFF;
					else			buffer[i][j] = 0x55;
					break;
				case FillD:
					if(j%2 == 0)	buffer[i][j] = 0xFF;
					else			buffer[i][j] = 0xAA;
					break;
				case Fill7:
					if(j%2 == 0)	buffer[i][j] = 0xFF;
					else			buffer[i][j] = 0xAA;
					break;
				case FillEBD7:
					if(j%4 == 0)	buffer[i][j] = 0xEE;
					if(j%4 == 1)	buffer[i][j] = 0xBB;
					if(j%4 == 2)	buffer[i][j] = 0xDD;
					if(j%4 == 3)	buffer[i][j] = 0x77;
					break;
			}
		}
	}
}
/**
  *@ FunctionName: void PlotCleanBlock(uint8_t bx, uint8_t by, SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 清空整个缓冲区
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void PlotCleanBlock(uint8_t bx, uint8_t by, SSDBuffer buffer)
{
	if(uiDevice->dataValid == True)
	{
		PlotFillBlock(bx, by, buffer, Fill1);
	}
	else
	{
		PlotFillBlock(bx, by, buffer, Fill0);
	}
}
/**
  *@ FunctionName: void PlotCleanBuffer(SSDBuffer buffer)
  *@ Author: CzrTuringB
  *@ Brief: 清空整个缓冲区
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void PlotCleanBuffer(SSDBuffer buffer)
{
	if(uiDevice->dataValid == True)
	{
		PlotFillScreen(buffer, Fill1);
	}
	else
	{
		PlotFillScreen(buffer, Fill0);
	}
}
/**
  *@ FunctionName: void PlotCleanScreen()
  *@ Author: CzrTuringB
  *@ Brief: 清除屏幕
  *@ Time: Jan 2, 2025
  *@ Requirement：
  */
void PlotCleanScreen()
{
	SSDClean();
	SSDUpdateScreen();
}
#pragma once

#include "Stock.h"

#define X_START_GRAPH				10
#define Y_START_GRAPH				10

#define WIDTH_GRAPH					1000
#define HEIGHT_GRAPH					400

#define HEIGHT_VOLUME				150

#define GAP_GRAPH_VOLUME			40

#define WIDTH_LINE						1
#define WIDTH_THICK_LINE			3

struct Point {
	int X;
	int Y;
};

//-----------------------------------------------------------------------------------------------
struct PointArray {
	int quantity;
	Point point[MAX_DATA];
};

//-----------------------------------------------------------------------------------------------
struct PointData {
	PointArray startVal;			//�ð�
	PointArray highVal;				//��
	PointArray lowVal;				//����
	PointArray lastVal;				//����
	PointArray volume;				//�ŷ���

	PointArray avg5;					//5��
	PointArray avg20;				//20��
	PointArray avg60;				//60��
	PointArray avg120;				//120��
};

//-----------------------------------------------------------------------------------------------
class CGraph
{
public:
	static PointData * GetPointData(Company* data);
	static void GetDataForChart(Company* data, PointData* ptData);
};

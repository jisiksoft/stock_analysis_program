#pragma once

#include "Stock.h"

#define X_START_GRAPH				10
#define Y_START_GRAPH				10

#define WIDTH_GRAPH					1000
#define HEIGHT_GRAPH					400

#define HEIGHT_INDICATOR			150

#define GAP_GRAPH_INDICATOR		40

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

	PointArray bollingerTop;				//Bollinger Band Top Line
	PointArray bollingerMiddle;			//Bollinger Band Middle Linessss
	PointArray bollingerBottom;			//Bollinger Band Bottom Line
	PointArray bollingerOscillator;		//Bollinger Band Oscillator
};

//-----------------------------------------------------------------------------------------------
class CGraph
{
public:
	static PointData * GetPointData(Company* data);
	static void GetDataForChart(Company* data, PointData* ptData);
};

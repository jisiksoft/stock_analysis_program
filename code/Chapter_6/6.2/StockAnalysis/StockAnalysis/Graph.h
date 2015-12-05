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
	PointArray startVal;			//시가
	PointArray highVal;				//고가
	PointArray lowVal;				//저가
	PointArray lastVal;				//종가
	PointArray volume;				//거래량

	PointArray avg5;					//5평선
	PointArray avg20;				//20평선
	PointArray avg60;				//60평선
	PointArray avg120;				//120평선
};

//-----------------------------------------------------------------------------------------------
class CGraph
{
public:
	static PointData * GetPointData(Company* data);
	static void GetDataForChart(Company* data, PointData* ptData);
};

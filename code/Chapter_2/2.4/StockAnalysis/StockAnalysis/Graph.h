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
	PointArray startVal;			//start value
	PointArray highVal;				//high value
	PointArray lowVal;				//low value
	PointArray lastVal;				//last value
	PointArray volume;				//volume
};

//-----------------------------------------------------------------------------------------------
class CGraph
{
public:
	static PointData * GetPointData(Company* data);
	static void GetDataForChart(Company* data, PointData* ptData);
};

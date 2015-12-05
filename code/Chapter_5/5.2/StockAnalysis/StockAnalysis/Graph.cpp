#include "stdafx.h"
#include "Graph.h"

//-----------------------------------------------------------------------------------------------
PointData* CGraph::GetPointData(Company* company)
{
	static PointData ptData;

	GetDataForChart(company, &ptData);

	return &ptData;
}

//-----------------------------------------------------------------------------------------------
void CGraph::GetDataForChart(Company* company, PointData* ptData)
{
	int i, maxVal, minVal, maxVol, interval;

	maxVal = 0;

	int quantity = company->quantity;

	for(i=0; i<quantity; i++)
		if (company->data[i].highVal > maxVal)
			maxVal = company->data[i].highVal;

	minVal = maxVal;

	for(i=0; i<quantity; i++)
		if (company->data[i].lowVal < minVal)
			minVal = company->data[i].lowVal;

	maxVol = 0;

	for(i=0; i<quantity; i++)
		if (company->data[i].vol > maxVol)
			maxVol = company->data[i].vol;

	interval = (int)(WIDTH_GRAPH / quantity);

	int x_start = X_START_GRAPH + WIDTH_GRAPH;
	int y_start = Y_START_GRAPH + HEIGHT_GRAPH;

	ptData->startVal.quantity = company->quantity;
	for(i=0; i<ptData->startVal.quantity; i++) {
		ptData->startVal.point[i].X = x_start - i * interval;
		ptData->startVal.point[i].Y = y_start - 
			(HEIGHT_GRAPH * (company->data[i].startVal - minVal)) / (maxVal - minVal);
	}

	ptData->highVal.quantity = company->quantity;
	for(i=0; i<ptData->highVal.quantity; i++) {
		ptData->highVal.point[i].X = x_start - i * interval;
		ptData->highVal.point[i].Y = y_start - 
			(HEIGHT_GRAPH * (company->data[i].highVal - minVal)) / (maxVal - minVal);
	}

	ptData->lowVal.quantity = company->quantity;
	for(i=0; i<ptData->lowVal.quantity; i++) {
		ptData->lowVal.point[i].X = x_start - i * interval;
		ptData->lowVal.point[i].Y = y_start - 
			(HEIGHT_GRAPH * (company->data[i].lowVal - minVal)) / (maxVal - minVal);
	}

	ptData->lastVal.quantity = company->quantity;
	for(i=0; i<ptData->lastVal.quantity; i++) {
		ptData->lastVal.point[i].X = x_start - i * interval;
		ptData->lastVal.point[i].Y = y_start - 
			(HEIGHT_GRAPH * (company->data[i].lastVal - minVal)) / (maxVal - minVal);
	}

	//------------------- Bollinger Band --------------------	
	//int x_start = X_START_GRAPH + WIDTH_GRAPH;
	//int y_start = Y_START_GRAPH + HEIGHT_GRAPH;

	//int quantity = bollinger->quantity;

	ptData->bollingerTop.quantity = quantity;
	ptData->bollingerMiddle.quantity = quantity;
	ptData->bollingerBottom.quantity = quantity;
	ptData->bollingerOscillator.quantity = quantity;

	for(i=0; i<quantity; i++) {
		ptData->bollingerTop.point[i].X = x_start - i * interval;
		ptData->bollingerTop.point[i].Y = y_start - (HEIGHT_GRAPH * (company->bollinger[i].lineTop - minVal)) / (maxVal - minVal);
		ptData->bollingerMiddle.point[i].X = x_start - i * interval;
		ptData->bollingerMiddle.point[i].Y = y_start - (HEIGHT_GRAPH * (company->bollinger[i].lineMiddle - minVal)) / (maxVal - minVal);
		ptData->bollingerBottom.point[i].X = x_start - i * interval;
		ptData->bollingerBottom.point[i].Y = y_start - (HEIGHT_GRAPH * (company->bollinger[i].lineBottom - minVal)) / (maxVal - minVal);
	}

	int originY = Y_START_GRAPH + HEIGHT_GRAPH + GAP_GRAPH_INDICATOR + (int)(HEIGHT_INDICATOR/2);
	float maxY = (int)((HEIGHT_INDICATOR/2) * 0.7);

	for(i=0; i<quantity; i++) {
		ptData->bollingerOscillator.point[i].X = x_start - i * interval;
		ptData->bollingerOscillator.point[i].Y = originY - (int)(company->bollinger[i].oscillator * maxY);
	}
}

//CString temp;
//temp.Format(_T("%d \n"), algorithm);
//::AfxMessageBox(temp);

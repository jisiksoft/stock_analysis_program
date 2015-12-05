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

	int x_start, y_start;

	x_start = X_START_GRAPH + WIDTH_GRAPH;
	y_start = Y_START_GRAPH + HEIGHT_GRAPH;

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

	//------------------- MACD --------------------
	x_start = X_START_GRAPH + WIDTH_GRAPH;
	int originY = Y_START_GRAPH + HEIGHT_GRAPH + GAP_GRAPH_INDICATOR + (int)(HEIGHT_INDICATOR/2);
	int maxY = (int)((HEIGHT_INDICATOR/2) * 0.9);

	int maxOscillator = 0;
	for(i=0; i<company->quantity; i++) {
		if (abs(company->macd[i].signal) > maxOscillator)
			maxOscillator = abs(company->macd[i].signal);
	}

	if (maxOscillator != 0) {
		ptData->macdOscillator.quantity = company->quantity;
		for(i=0; i<ptData->macdOscillator.quantity; i++) {
			ptData->macdOscillator.point[i].X = x_start - i * interval;
			ptData->macdOscillator.point[i].Y = originY - (int)((maxY * company->macd[i].signal) / maxOscillator) ;
		}
	}
}

//CString temp;
//temp.Format(_T("%d \n"), algorithm);
//::AfxMessageBox(temp);

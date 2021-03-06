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

	//------------------- Movement Average --------------------
	ptData->avg5.quantity = company->quantity - 5;
	for(i=0; i<ptData->avg5.quantity; i++) {
		ptData->avg5.point[i].X = x_start - i * interval;
		ptData->avg5.point[i].Y = y_start - (HEIGHT_GRAPH * (company->moveAverage.avg5[i] - minVal)) / (maxVal - minVal);
	}

	ptData->avg20.quantity = company->quantity - 20;
	for(i=0; i<ptData->avg20.quantity; i++) {
		ptData->avg20.point[i].X = x_start - i * interval;
		ptData->avg20.point[i].Y = y_start - (HEIGHT_GRAPH * (company->moveAverage.avg20[i] - minVal)) / (maxVal - minVal);
	}

	ptData->avg60.quantity = company->quantity - 60;
	for(i=0; i<ptData->avg60.quantity; i++) {
		ptData->avg60.point[i].X = x_start - i * interval;
		ptData->avg60.point[i].Y = y_start - (HEIGHT_GRAPH * (company->moveAverage.avg60[i] - minVal)) / (maxVal - minVal);
	}

	ptData->avg120.quantity = company->quantity - 120;
	for(i=0; i<ptData->avg120.quantity; i++) {
		ptData->avg120.point[i].X = x_start - i * interval;
		ptData->avg120.point[i].Y = y_start - (HEIGHT_GRAPH * (company->moveAverage.avg120[i] - minVal)) / (maxVal - minVal);
	}

	//------------------- Volume Chart --------------------
	y_start = Y_START_GRAPH + HEIGHT_GRAPH + GAP_GRAPH_VOLUME + HEIGHT_VOLUME;

	ptData->volume.quantity = company->quantity;
	for(i=0; i<ptData->volume.quantity; i++) {
		ptData->volume.point[i].X = x_start - i * interval;
		ptData->volume.point[i].Y = y_start - (HEIGHT_VOLUME * company->data[i].vol) / maxVol;
	}
}

//CString temp;
//temp.Format(_T("%d \n"), algorithm);
//::AfxMessageBox(temp);

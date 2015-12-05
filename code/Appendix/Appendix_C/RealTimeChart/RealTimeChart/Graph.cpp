/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Graph.h"

//-----------------------------------------------------------------------------------------------
CGraph::CGraph(void)
{
	int i;

	// 시간선을 그리기 위하여 X축에 대한 데이터 값과 위치를 넣는다.
	for  (i=0; i<7; i++) {
		ptData.ptBaseX.valueX[i] = i + 9;	//9시부터 시간을 순차적으로 입력한다.
		ptData.ptBaseX.pointX[i] = (X_START_GRAPH - 3) + (60*i) * INTERVAL_X; //X축의 점의 위치를 넣어준다.
	}
}

//-----------------------------------------------------------------------------------------------
CGraph::~CGraph(void)
{
}

//-----------------------------------------------------------------------------------------------
// 그리기 위한 Call과 Put의 현재 데이터에 대한 점의 위치를 계산한다.
void CGraph::CalculateAllPointData(PairUnit *pairUnit)
{
	int i;

	float maxY = 0;		//Y축의 최대값을 갖기 위하여 0으로 초기화한다.
	float minY = 100;	//Y축의 최소값을 갖기 위하여 100으로 초기화한다.

	for (i=0; i<pairUnit->call->quantity; i++) {	//Call에 대한 최대 최소를 계산한다.
		if (pairUnit->call->data[i].highVal > maxY)
			maxY = pairUnit->call->data[i].highVal ;
		if (pairUnit->call->data[i].lowVal < minY)
			minY = pairUnit->call->data[i].lowVal ;
	}

	for (i=0; i<pairUnit->put->quantity; i++) {		//Put에 대한 최대 최소를 계산한다.
		if (pairUnit->put->data[i].highVal > maxY)
			maxY = pairUnit->put->data[i].highVal ;
		if (pairUnit->put->data[i].lowVal < minY)
			minY = pairUnit->put->data[i].lowVal ;
	}

	maxValY = (float)((int)(((maxY + 0.2) * 10) / 1) * 0.1); //Chart 영역의 Y축의 최대값을 계산한다.
	minValY = (float)((int)(((minY - 0.1) * 10) / 1) * 0.1); //Chart 영역의 Y축의 최소값을 계산한다.
	bottomY = Y_START_GRAPH + Y_MAX_SIZE_GRAPH;				 //Chart 영역의 바닥선의 Y축의 점의 위치를 저장한다.
	countLineY = (int)(((maxY - minY) * 10) / 1) + 4;		 //Y선의 갯수를 저장한다.
	originOscillatorY = Y_START_GRAPH + Y_MAX_SIZE_GRAPH + GAP_GRAPH + (int)(Y_MAX_OSCILLATOR / 2); //오실레이터 영역의 중간 0선의 점의 위치를 저장한다.

	//---------- Y축의 가로선의 값과 점의 위치를 계산하여 저장한다. ----------
	ptData.ptBaseY.quantity = countLineY;
	for (i=0; i<ptData.ptBaseY.quantity; i++) {
		ptData.ptBaseY.valueY[i] = (float)(maxValY - (0.1 * i));
		ptData.ptBaseY.pointY[i] = bottomY - (int)(((ptData.ptBaseY.valueY[i] - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	}

	ptData.lineCall.quantity = pairUnit->call->quantity; //Call과 Put의 점의 갯수
	ptData.linePut.quantity = pairUnit->put->quantity;

	if (ptData.lineCall.quantity > 360) //점의 갯수는 360개를 넘지 않는다. (360 = 6시간 * 60분)
		ptData.lineCall.quantity = 360;
	if (ptData.linePut.quantity > 360)
		ptData.linePut.quantity = 360;

	for (i=0; i<ptData.lineCall.quantity; i++) { //Call의 점의 위치를 계산한다.
		ptData.lineCall.ptLast[i].Y = bottomY - (int)(((pairUnit->call->data[i].lastVal - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptLast[i].X = X_START_GRAPH + i * INTERVAL_X;
	}

	for (i=0; i<ptData.linePut.quantity; i++) { //Put의 점의 위치를 계산한다.
		ptData.linePut.ptLast[i].Y = bottomY - (int)(((pairUnit->put->data[i].lastVal - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptLast[i].X = X_START_GRAPH + i * INTERVAL_X;
	}

	//---------------- Bollinger Band 좌표 계산 ------------------
	for (i=0; i<ptData.lineCall.quantity; i++) { //Call Bollinger Band 좌표 계산
		ptData.lineCall.ptBollingerTop[i].Y = bottomY - (int)(((pairUnit->call->bollinger[i].lineTop - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptBollingerTop[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.lineCall.ptBollingerMiddle[i].Y = bottomY - (int)(((pairUnit->call->bollinger[i].lineMiddle - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptBollingerMiddle[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.lineCall.ptBollingerBottom[i].Y = bottomY - (int)(((pairUnit->call->bollinger[i].lineBottom - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptBollingerBottom[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.lineCall.ptBollingerOscillator[i].Y = originOscillatorY - (int)(pairUnit->call->bollinger[i].oscillalor * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.lineCall.ptBollingerOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}
	
	for (i=0; i<ptData.linePut.quantity; i++) { //Put Bollinger Band 좌표 계산
		ptData.linePut.ptBollingerTop[i].Y = bottomY - (int)(((pairUnit->put->bollinger[i].lineTop - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptBollingerTop[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.linePut.ptBollingerMiddle[i].Y = bottomY - (int)(((pairUnit->put->bollinger[i].lineMiddle - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptBollingerMiddle[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.linePut.ptBollingerBottom[i].Y = bottomY - (int)(((pairUnit->put->bollinger[i].lineBottom - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptBollingerBottom[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.linePut.ptBollingerOscillator[i].Y = originOscillatorY - (int)(pairUnit->put->bollinger[i].oscillalor * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.linePut.ptBollingerOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}	

	//---------------- MACD 좌표 계산 ------------------
	maxMACD = 0;

	for (i=0; i<pairUnit->call->quantity; i++) { //Call에 대한 MACD 최대값을 구한다.
		if (pairUnit->call->macd[i].signal >= 0) {
			if (pairUnit->call->macd[i].signal > maxMACD)
				maxMACD = pairUnit->call->macd[i].signal ;
		} else {
			if ((-pairUnit->call->macd[i].signal) > maxMACD)
				maxMACD = (-pairUnit->call->macd[i].signal) ;
		}
	}

	for (i=0; i<pairUnit->put->quantity; i++) { //Put에 대한 MACD 최대값을 구한다.
		if (pairUnit->put->macd[i].signal >= 0) {
			if (pairUnit->put->macd[i].signal > maxMACD)
				maxMACD = pairUnit->put->macd[i].signal ;
		} else {
			if ((-pairUnit->put->macd[i].signal) > maxMACD)
				maxMACD = (-pairUnit->put->macd[i].signal) ;
		}
	}

	for (i=0; i<ptData.lineCall.quantity; i++) { //Call MACD 좌표 계산
		ptData.lineCall.ptMacdOscillator[i].Y = originOscillatorY - (int)((pairUnit->call->macd[i].signal / maxMACD) * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.lineCall.ptMacdOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}

	for (i=0; i<ptData.linePut.quantity; i++) { //Put MACD 좌표 계산
		ptData.linePut.ptMacdOscillator[i].Y = originOscillatorY - (int)((pairUnit->put->macd[i].signal / maxMACD) * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.linePut.ptMacdOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}	
}

//-----------------------------------------------------------------------------------------------
// 실시간 데이터에 대한 점의 위치를 계산한다.
void CGraph::CalculateRealDataPoint(PairUnit *pairUnit) {

	int index = pairUnit->call->quantity - 1;

	//Call 실시간 좌표 계산
	ptData.lineCall.ptLast[index].Y = bottomY - (int)(((pairUnit->call->data[index].lastVal - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.lineCall.ptLast[index].X = X_START_GRAPH + index * INTERVAL_X;

	ptData.lineCall.ptBollingerTop[index].Y = bottomY - (int)(((pairUnit->call->bollinger[index].lineTop - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.lineCall.ptBollingerTop[index].X = X_START_GRAPH + index * INTERVAL_X;
	ptData.lineCall.ptBollingerMiddle[index].Y = bottomY - (int)(((pairUnit->call->bollinger[index].lineMiddle - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.lineCall.ptBollingerMiddle[index].X = X_START_GRAPH + index * INTERVAL_X;
	ptData.lineCall.ptBollingerBottom[index].Y = bottomY - (int)(((pairUnit->call->bollinger[index].lineBottom - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.lineCall.ptBollingerBottom[index].X = X_START_GRAPH + index * INTERVAL_X;
	ptData.lineCall.ptBollingerOscillator[index].Y = originOscillatorY - (int)(pairUnit->call->bollinger[index].oscillalor * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
	ptData.lineCall.ptBollingerOscillator[index].X = X_START_GRAPH + index * INTERVAL_X;

	ptData.lineCall.ptMacdOscillator[index].Y = originOscillatorY - (int)((pairUnit->call->macd[index].signal / maxMACD) * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
	ptData.lineCall.ptMacdOscillator[index].X = X_START_GRAPH + index * INTERVAL_X;

	//Put 실시간 좌표 계산
	ptData.linePut.ptLast[index].Y = bottomY - (int)(((pairUnit->put->data[index].lastVal - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.linePut.ptLast[index].X = X_START_GRAPH + index * INTERVAL_X;

	ptData.linePut.ptBollingerTop[index].Y = bottomY - (int)(((pairUnit->put->bollinger[index].lineTop - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.linePut.ptBollingerTop[index].X = X_START_GRAPH + index * INTERVAL_X;
	ptData.linePut.ptBollingerMiddle[index].Y = bottomY - (int)(((pairUnit->put->bollinger[index].lineMiddle - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.linePut.ptBollingerMiddle[index].X = X_START_GRAPH + index * INTERVAL_X;
	ptData.linePut.ptBollingerBottom[index].Y = bottomY - (int)(((pairUnit->put->bollinger[index].lineBottom - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	ptData.linePut.ptBollingerBottom[index].X = X_START_GRAPH + index * INTERVAL_X;
	ptData.linePut.ptBollingerOscillator[index].Y = originOscillatorY - (int)(pairUnit->put->bollinger[index].oscillalor * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
	ptData.linePut.ptBollingerOscillator[index].X = X_START_GRAPH + index * INTERVAL_X;

	ptData.linePut.ptMacdOscillator[index].Y = originOscillatorY - (int)((pairUnit->put->macd[index].signal / maxMACD) * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
	ptData.linePut.ptMacdOscillator[index].X = X_START_GRAPH + index * INTERVAL_X;

	//점의 갯수를 데이터의 갯수와 같이 설정한다.
	ptData.lineCall.quantity = pairUnit->call->quantity;
	ptData.linePut.quantity = pairUnit->put->quantity;
}

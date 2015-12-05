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

	// �ð����� �׸��� ���Ͽ� X�࿡ ���� ������ ���� ��ġ�� �ִ´�.
	for  (i=0; i<7; i++) {
		ptData.ptBaseX.valueX[i] = i + 9;	//9�ú��� �ð��� ���������� �Է��Ѵ�.
		ptData.ptBaseX.pointX[i] = (X_START_GRAPH - 3) + (60*i) * INTERVAL_X; //X���� ���� ��ġ�� �־��ش�.
	}
}

//-----------------------------------------------------------------------------------------------
CGraph::~CGraph(void)
{
}

//-----------------------------------------------------------------------------------------------
// �׸��� ���� Call�� Put�� ���� �����Ϳ� ���� ���� ��ġ�� ����Ѵ�.
void CGraph::CalculateAllPointData(PairUnit *pairUnit)
{
	int i;

	float maxY = 0;		//Y���� �ִ밪�� ���� ���Ͽ� 0���� �ʱ�ȭ�Ѵ�.
	float minY = 100;	//Y���� �ּҰ��� ���� ���Ͽ� 100���� �ʱ�ȭ�Ѵ�.

	for (i=0; i<pairUnit->call->quantity; i++) {	//Call�� ���� �ִ� �ּҸ� ����Ѵ�.
		if (pairUnit->call->data[i].highVal > maxY)
			maxY = pairUnit->call->data[i].highVal ;
		if (pairUnit->call->data[i].lowVal < minY)
			minY = pairUnit->call->data[i].lowVal ;
	}

	for (i=0; i<pairUnit->put->quantity; i++) {		//Put�� ���� �ִ� �ּҸ� ����Ѵ�.
		if (pairUnit->put->data[i].highVal > maxY)
			maxY = pairUnit->put->data[i].highVal ;
		if (pairUnit->put->data[i].lowVal < minY)
			minY = pairUnit->put->data[i].lowVal ;
	}

	maxValY = (float)((int)(((maxY + 0.2) * 10) / 1) * 0.1); //Chart ������ Y���� �ִ밪�� ����Ѵ�.
	minValY = (float)((int)(((minY - 0.1) * 10) / 1) * 0.1); //Chart ������ Y���� �ּҰ��� ����Ѵ�.
	bottomY = Y_START_GRAPH + Y_MAX_SIZE_GRAPH;				 //Chart ������ �ٴڼ��� Y���� ���� ��ġ�� �����Ѵ�.
	countLineY = (int)(((maxY - minY) * 10) / 1) + 4;		 //Y���� ������ �����Ѵ�.
	originOscillatorY = Y_START_GRAPH + Y_MAX_SIZE_GRAPH + GAP_GRAPH + (int)(Y_MAX_OSCILLATOR / 2); //���Ƿ����� ������ �߰� 0���� ���� ��ġ�� �����Ѵ�.

	//---------- Y���� ���μ��� ���� ���� ��ġ�� ����Ͽ� �����Ѵ�. ----------
	ptData.ptBaseY.quantity = countLineY;
	for (i=0; i<ptData.ptBaseY.quantity; i++) {
		ptData.ptBaseY.valueY[i] = (float)(maxValY - (0.1 * i));
		ptData.ptBaseY.pointY[i] = bottomY - (int)(((ptData.ptBaseY.valueY[i] - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
	}

	ptData.lineCall.quantity = pairUnit->call->quantity; //Call�� Put�� ���� ����
	ptData.linePut.quantity = pairUnit->put->quantity;

	if (ptData.lineCall.quantity > 360) //���� ������ 360���� ���� �ʴ´�. (360 = 6�ð� * 60��)
		ptData.lineCall.quantity = 360;
	if (ptData.linePut.quantity > 360)
		ptData.linePut.quantity = 360;

	for (i=0; i<ptData.lineCall.quantity; i++) { //Call�� ���� ��ġ�� ����Ѵ�.
		ptData.lineCall.ptLast[i].Y = bottomY - (int)(((pairUnit->call->data[i].lastVal - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptLast[i].X = X_START_GRAPH + i * INTERVAL_X;
	}

	for (i=0; i<ptData.linePut.quantity; i++) { //Put�� ���� ��ġ�� ����Ѵ�.
		ptData.linePut.ptLast[i].Y = bottomY - (int)(((pairUnit->put->data[i].lastVal - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptLast[i].X = X_START_GRAPH + i * INTERVAL_X;
	}

	//---------------- Bollinger Band ��ǥ ��� ------------------
	for (i=0; i<ptData.lineCall.quantity; i++) { //Call Bollinger Band ��ǥ ���
		ptData.lineCall.ptBollingerTop[i].Y = bottomY - (int)(((pairUnit->call->bollinger[i].lineTop - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptBollingerTop[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.lineCall.ptBollingerMiddle[i].Y = bottomY - (int)(((pairUnit->call->bollinger[i].lineMiddle - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptBollingerMiddle[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.lineCall.ptBollingerBottom[i].Y = bottomY - (int)(((pairUnit->call->bollinger[i].lineBottom - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.lineCall.ptBollingerBottom[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.lineCall.ptBollingerOscillator[i].Y = originOscillatorY - (int)(pairUnit->call->bollinger[i].oscillalor * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.lineCall.ptBollingerOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}
	
	for (i=0; i<ptData.linePut.quantity; i++) { //Put Bollinger Band ��ǥ ���
		ptData.linePut.ptBollingerTop[i].Y = bottomY - (int)(((pairUnit->put->bollinger[i].lineTop - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptBollingerTop[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.linePut.ptBollingerMiddle[i].Y = bottomY - (int)(((pairUnit->put->bollinger[i].lineMiddle - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptBollingerMiddle[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.linePut.ptBollingerBottom[i].Y = bottomY - (int)(((pairUnit->put->bollinger[i].lineBottom - minValY) * Y_MAX_SIZE_GRAPH) / (maxValY - minValY));
		ptData.linePut.ptBollingerBottom[i].X = X_START_GRAPH + i * INTERVAL_X;
		ptData.linePut.ptBollingerOscillator[i].Y = originOscillatorY - (int)(pairUnit->put->bollinger[i].oscillalor * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.linePut.ptBollingerOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}	

	//---------------- MACD ��ǥ ��� ------------------
	maxMACD = 0;

	for (i=0; i<pairUnit->call->quantity; i++) { //Call�� ���� MACD �ִ밪�� ���Ѵ�.
		if (pairUnit->call->macd[i].signal >= 0) {
			if (pairUnit->call->macd[i].signal > maxMACD)
				maxMACD = pairUnit->call->macd[i].signal ;
		} else {
			if ((-pairUnit->call->macd[i].signal) > maxMACD)
				maxMACD = (-pairUnit->call->macd[i].signal) ;
		}
	}

	for (i=0; i<pairUnit->put->quantity; i++) { //Put�� ���� MACD �ִ밪�� ���Ѵ�.
		if (pairUnit->put->macd[i].signal >= 0) {
			if (pairUnit->put->macd[i].signal > maxMACD)
				maxMACD = pairUnit->put->macd[i].signal ;
		} else {
			if ((-pairUnit->put->macd[i].signal) > maxMACD)
				maxMACD = (-pairUnit->put->macd[i].signal) ;
		}
	}

	for (i=0; i<ptData.lineCall.quantity; i++) { //Call MACD ��ǥ ���
		ptData.lineCall.ptMacdOscillator[i].Y = originOscillatorY - (int)((pairUnit->call->macd[i].signal / maxMACD) * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.lineCall.ptMacdOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}

	for (i=0; i<ptData.linePut.quantity; i++) { //Put MACD ��ǥ ���
		ptData.linePut.ptMacdOscillator[i].Y = originOscillatorY - (int)((pairUnit->put->macd[i].signal / maxMACD) * ((Y_MAX_OSCILLATOR / 2) * RATE_COMPARE_LINE));
		ptData.linePut.ptMacdOscillator[i].X = X_START_GRAPH + i * INTERVAL_X;
	}	
}

//-----------------------------------------------------------------------------------------------
// �ǽð� �����Ϳ� ���� ���� ��ġ�� ����Ѵ�.
void CGraph::CalculateRealDataPoint(PairUnit *pairUnit) {

	int index = pairUnit->call->quantity - 1;

	//Call �ǽð� ��ǥ ���
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

	//Put �ǽð� ��ǥ ���
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

	//���� ������ �������� ������ ���� �����Ѵ�.
	ptData.lineCall.quantity = pairUnit->call->quantity;
	ptData.linePut.quantity = pairUnit->put->quantity;
}

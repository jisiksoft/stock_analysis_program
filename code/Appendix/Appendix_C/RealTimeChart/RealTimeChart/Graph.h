/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#pragma once

#include "Option.h"

#define MAX_BASE_Y				100			//Y���� ���μ� �ִ� ����

#define INTERVAL_X				3			//X���� �� ������ 3 pixel�̴�.

#define X_START_GRAPH			40		    //Chart�� X�� ���� ��ġ
#define Y_START_GRAPH			20			//Chart�� Y�� ���� ��ġ

#define X_MAX_SIZE_GRAPH		1077		//Chart ������ ���� ũ��
#define Y_MAX_SIZE_GRAPH		500			//Chart ������ ���� ũ��

#define GAP_GRAPH				20			//Chart ������ ���Ƿ����� ������ ����

#define Y_MAX_OSCILLATOR		200			//���Ƿ����� ������ ���� ũ��

#define RATE_COMPARE_LINE		0.5			//���Ƿ����Ϳ��� 1�� -1 ���� ��ġ (0.5�� 50%�μ� �߰����̴�.)

//-----------------------------------------------------------------------------------------------
// �� ����ü
struct Point {
	int X;
	int Y;
};

//-----------------------------------------------------------------------------------------------
// ���μ����μ� �ð����� �׸��� ���� ���ȴ�.
struct BasePointX {
	int pointX[7];				//9�ú��� 15�ñ����� 7�� ��
	int valueX[7];				//�ð� �����Ͱ� ����ȴ�. (ex: 9,10,11,12,13,14,15)
};

//-----------------------------------------------------------------------------------------------
// ���μ����μ� ���� ���� ���μ��� ������ �����ȴ�.
struct BasePointY {
	int quantity;				//���μ��� ����
	int pointY[MAX_BASE_Y];		//���μ��� ��ġ
	float valueY[MAX_BASE_Y];	//���μ��� ��
};

//-----------------------------------------------------------------------------------------------
// Chart�� ���� �׸��� ���� ���� ��ġ�� ������ ����ü
struct PointLine {
	int quantity;				//������ ����
	UINT time[MAX_DATA];		//�ð�

	Point ptStart[MAX_DATA];	//�ð� ���� ��ġ
	Point ptHigh[MAX_DATA];		//�� ���� ��ġ
	Point ptLow[MAX_DATA];		//���� ���� ��ġ
	Point ptLast[MAX_DATA];		//���� ���� ��ġ

	Point ptBollingerTop[MAX_DATA];			//Bollinger Band ���� ���� ��ġ
	Point ptBollingerMiddle[MAX_DATA];		//Bollinger Band �߰��� ���� ��ġ
	Point ptBollingerBottom[MAX_DATA];		//Bollinger Band �Ʒ��� ���� ��ġ
	Point ptBollingerOscillator[MAX_DATA];	//Bollinger Band ���Ƿ����� ���� ��ġ

	Point ptMacdOscillator[MAX_DATA];
};

//-----------------------------------------------------------------------------------------------
// Call�� Put�� ���� ��ġ�� �����ϴ� ����ü
struct PointData {
	PointLine lineCall;			//Call �� ������
	PointLine linePut;			//Put �� ������

	BasePointY ptBaseY;			//Y���� ���μ��� �׸��� ���� �� ������
	BasePointX ptBaseX;			//X���� ���μ��� �׸��� ���� �� ������
};

//-----------------------------------------------------------------------------------------------
// �ǽð� �����͸� �׸��� ���ѿ� ������ �׷��� ���� ��ġ�� �����Ѵ�.
// ��, �ǽð� �����ʹ� ������ ���� ����� ���� �׸��� �ȴ�.
struct PointDrawnLast {
	int x;
	int ptDataCall;
	int ptDataPut;
	int ptBollingerCall;
	int ptBollingerPut;
	int ptMacdCall;
	int ptMacdPut;
};

//-----------------------------------------------------------------------------------------------
// ȭ�鿡 �׸��� �׸��� ���� ���� ��ġ�� ������ �� �ְ�, ���� ��ġ�� ����ϴ� Ŭ����
class CGraph
{
public:
	PointData ptData;	//��� ���� ��ġ�� �����Ѵ�.

	PointDrawnLast ptDrawnLast; //�������� ��ġ�� ����Ѵ�.

	float maxValY;				//Chart ������ Y���� �ִ밪�� �����Ѵ�.
	float minValY;				//Chart ������ Y���� �ּҰ��� �����Ѵ�.
	int bottomY;				//Chart ������ �ٴڼ��� ��ġ�� �����Ѵ�.
	int countLineY;				//Chart ������ ���μ��� ������ �����Ѵ�.
	int originOscillatorY;		//Oscillator ������ �߰� 0���� ��ġ�� �����Ѵ�.
	float maxMACD;				//MACD Sinal�� �ִ밪�� �����Ѵ�.

	CGraph(void);
	~CGraph(void);

	void CalculateAllPointData(PairUnit *pairUnit);	//��ü �������� ���� ����Ѵ�.
	void CalculateRealDataPoint(PairUnit *pairUnit);//�ǽð� �������� ���� ����Ѵ�.
};

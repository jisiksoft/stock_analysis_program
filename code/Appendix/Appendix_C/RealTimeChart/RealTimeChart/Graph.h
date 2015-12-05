/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#pragma once

#include "Option.h"

#define MAX_BASE_Y				100			//Y축의 가로선 최대 갯수

#define INTERVAL_X				3			//X축의 분 간격은 3 pixel이다.

#define X_START_GRAPH			40		    //Chart의 X축 시작 위치
#define Y_START_GRAPH			20			//Chart의 Y축 시작 위치

#define X_MAX_SIZE_GRAPH		1077		//Chart 영역의 가로 크기
#define Y_MAX_SIZE_GRAPH		500			//Chart 영역의 세로 크기

#define GAP_GRAPH				20			//Chart 영역과 오실레이터 영역의 차이

#define Y_MAX_OSCILLATOR		200			//오실레이터 영역의 세로 크기

#define RATE_COMPARE_LINE		0.5			//오실레이터에서 1과 -1 선의 위치 (0.5는 50%로서 중간값이다.)

//-----------------------------------------------------------------------------------------------
// 점 구조체
struct Point {
	int X;
	int Y;
};

//-----------------------------------------------------------------------------------------------
// 세로선으로서 시간선을 그리기 위해 사용된다.
struct BasePointX {
	int pointX[7];				//9시부터 15시까지의 7개 선
	int valueX[7];				//시간 데이터가 저장된다. (ex: 9,10,11,12,13,14,15)
};

//-----------------------------------------------------------------------------------------------
// 가로선으로서 값에 따라 세로선의 갯수가 결정된다.
struct BasePointY {
	int quantity;				//가로선의 갯수
	int pointY[MAX_BASE_Y];		//가로선의 위치
	float valueY[MAX_BASE_Y];	//가로선의 값
};

//-----------------------------------------------------------------------------------------------
// Chart에 선을 그리기 위한 점의 위치를 저장한 구조체
struct PointLine {
	int quantity;				//데이터 갯수
	UINT time[MAX_DATA];		//시간

	Point ptStart[MAX_DATA];	//시가 점의 위치
	Point ptHigh[MAX_DATA];		//고가 점의 위치
	Point ptLow[MAX_DATA];		//저가 점의 위치
	Point ptLast[MAX_DATA];		//종가 점의 위치

	Point ptBollingerTop[MAX_DATA];			//Bollinger Band 윗선 점의 위치
	Point ptBollingerMiddle[MAX_DATA];		//Bollinger Band 중간선 점의 위치
	Point ptBollingerBottom[MAX_DATA];		//Bollinger Band 아랫선 점의 위치
	Point ptBollingerOscillator[MAX_DATA];	//Bollinger Band 오실레이터 점의 위치

	Point ptMacdOscillator[MAX_DATA];
};

//-----------------------------------------------------------------------------------------------
// Call과 Put의 점의 위치를 저장하는 구조체
struct PointData {
	PointLine lineCall;			//Call 선 데이터
	PointLine linePut;			//Put 선 데이터

	BasePointY ptBaseY;			//Y축의 가로선을 그리기 위한 점 데이터
	BasePointX ptBaseX;			//X축의 세로선을 그리기 위한 점 데이터
};

//-----------------------------------------------------------------------------------------------
// 실시간 데이터를 그리기 위한여 이전에 그려진 점의 위치를 저장한다.
// 즉, 실시간 데이터는 이전의 선을 지우고 새로 그리게 된다.
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
// 화면에 그림을 그리기 위한 점의 위치로 접근할 수 있고, 점의 위치를 계산하는 클래스
class CGraph
{
public:
	PointData ptData;	//모든 점의 위치를 저장한다.

	PointDrawnLast ptDrawnLast; //이전점의 위치를 계산한다.

	float maxValY;				//Chart 영역의 Y축의 최대값을 저장한다.
	float minValY;				//Chart 영역의 Y축의 최소값을 저장한다.
	int bottomY;				//Chart 영역의 바닥선의 위치를 저장한다.
	int countLineY;				//Chart 영역의 가로선의 갯수를 저장한다.
	int originOscillatorY;		//Oscillator 영역의 중간 0선의 위치를 저장한다.
	float maxMACD;				//MACD Sinal의 최대값을 저장한다.

	CGraph(void);
	~CGraph(void);

	void CalculateAllPointData(PairUnit *pairUnit);	//전체 데이터의 점을 계산한다.
	void CalculateRealDataPoint(PairUnit *pairUnit);//실시간 데이터의 점을 계산한다.
};

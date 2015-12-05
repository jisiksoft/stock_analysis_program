/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#pragma once

#define MAX_DATA					375		//분 데이터의 최대 갯수
#define MAX_UNIT					50		//Unit 구조체의 최대 갯수

#define STANDARD_VALUE				1.3		//Call과 Put은 1.3에서 가장 가까이서 시작하는 것이 선택되어진다.

#define TIMER_INTERVAL				2		//실시간 그래프를 그리는 간격 (초)
#define RESET_TIME					5		//데이터를 처음부터 다시 그리는 간격 (분)

//-----------------------------------------------------------------------------------------------
// 옵션 데이터를 저장하기 위한 구조체 : 이 프로그램에서는 시간과 종가만 사용된다.
struct Data {
	UINT time;			//시간 (ex:921:9시21분, 1304:13시4분)
	float startVal;		//시가
	float highVal;		//고가
	float lowVal;		//저가
	float lastVal;		//종가
	long vol;
};

//-----------------------------------------------------------------------------------------------
// Bollinger Band 데이터를 저장하기 위한 구조체
struct Bollinger {
	float lineTop;		//윗선
	float lineMiddle;	//중간선
	float lineBottom;	//아랫선
	float oscillalor;	//Bollinger Band 오실레이터
};

//-----------------------------------------------------------------------------------------------
// MACD 데이터를 저장하기 위한 구조체
struct MACD {
	float ema1;			//MACD EMA1
	float ema2;			//MACD EMA2
	float macd;			//MACD 계산 값
	float signal;		//MACD Signal
};

//-----------------------------------------------------------------------------------------------
// 옵션 종목들의 데이터를 저장하기 위한 구조체
struct Unit {
	CString code;					//종목코드
	float openVal;					//시작가
	int quantity;					//데이터 갯수
	Data data[MAX_DATA];			//데이터
	Bollinger bollinger[MAX_DATA];	//Bollinger Band 데이터
	MACD macd[MAX_DATA];			//MACD 데이터
};

//-----------------------------------------------------------------------------------------------
// 전체 종목을 담기 위한 구조체
struct AllUnit {
	int quantityCall;			//Call 종목 갯수
	int quantityPut;			//Put 종목 갯수
	Unit unitCall[MAX_UNIT];	//Call 종목을 저장
	Unit unitPut[MAX_UNIT];		//Put 종목을 저장
};

//-----------------------------------------------------------------------------------------------
// Chart에서 사용하는 Call과 Put의 한 쌍을 가리키는 구조체
struct PairUnit {
	Unit *put;
	Unit *call;
};

//-----------------------------------------------------------------------------------------------
// COption의 데이터에 접근하고, 계산을 위한 함수를 가지고 있는 클래스
class COption
{
public:
	AllUnit allUnit;		//전체 데이터에 접근하기 위하여 사용
	PairUnit pairUnit;		//현재 Chart에서 사용하는 Call과 Put을 가리킨다.

	float macdK1;			//MACD의 계산에서 사용되는 실수를 저장하는 변수들
	float macdK2;
	float macdK3;

public:
	COption(void);
	~COption(void);

	CString GetMonthForData();		//오늘 날자에 대한 옵션 월물 값을 반환한다. (반환값: YYY
	void CalculateBollinger();		//Bollinger Band를 계산한다.
	void CalculateMACD();			//MACD를 계산한다.
	void CalculateRealData();		//Real Data의 Bollinger Band와 MACD를 계산한다.
	void AddNewEntryCall(UINT time, float value);	//실시간 데이터에서 다음 분의 Call 데이터를 새로 추가한다.
	void AddNewEntryPut(UINT time, float value);	//실시간 데이터에서 다음 분의 Put 데이터를 새로 추가한다.
};

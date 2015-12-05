/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <windows.h>
#include <string>
#include <math.h>

#include "Option.h"

//-----------------------------------------------------------------------------------------------
// This creator initializes every variable.
COption::COption(void)
{
	allUnit.quantityCall = 0;
	allUnit.quantityPut = 0;
}

//-----------------------------------------------------------------------------------------------
COption::~COption(void)
{
}

//-----------------------------------------------------------------------------------------------
// 오늘 날자에 대한 옵션 월물 값을 반환한다. (반환값: YYYYMM)
// 두째주 목요일까지는 해당 월의 값을 반환하고, 이후 날자에는 다음달 월의 값을 반환한다.
CString COption::GetMonthForData() 
{
	CString monthForData;

	CTime time = CTime::GetTickCount();

	int year = time.GetYear();		//현재 년도
	int month = time.GetMonth();	//현재 달
	int day = time.GetDay();		//현재 날자

	int dayWeekForEightDay = CTime(year,month,8,9,0,0).GetDayOfWeek();  //8일이 주의 몇번째 요일인지를 계산한다.
	int dueDate;

	if (dayWeekForEightDay < 6) //두번째 목요일이 무슨 날인지를 계산한다.
		dueDate = 13 - dayWeekForEightDay;
	else
		dueDate = 20 - dayWeekForEightDay;

	if (day <= dueDate) { //두번째 목요일까지의 날자는 해당월물을 갖게 된다.
		if (month < 10)
			monthForData.Format(_T("%d0%d"), year, month);
		else
			monthForData.Format(_T("%d%d"), year, month);
	} else {
		if (month < 12) { //두번째 목요일 이후의 날자는 다음달 월물을 갖게 된다.
			month += 1;
			if (month < 10)
				monthForData.Format(_T("%d0%d"), year, month);
			else
				monthForData.Format(_T("%d%d"), year, month);
		} else {
			monthForData.Format(_T("%d01"), year+1);
		}
	}

	return monthForData;
}

//-----------------------------------------------------------------------------------------------
// Bollinger Band를 계산한다.
void COption::CalculateBollinger()
{
	int i, j;
	float sum;

	Unit *call = pairUnit.call;
	Unit *put = pairUnit.put;

	//Call의 Bollinger Band를 계산한다.
	for (i=BOLLINGER_AVG-1; i<call->quantity; i++) {

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //평균을 계산한다.
			sum += call->data[i-j].lastVal;
		}
		float average = sum / BOLLINGER_AVG;

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //표준편차를 계산한다.
			sum += (average - call->data[i-j].lastVal) * (average - call->data[i-j].lastVal);
		}
		float sd = sqrt((float)sum / BOLLINGER_AVG);

		//Bollinger Band의 데이터를 Call 구조체에 저장한다.
		call->bollinger[i].lineMiddle = average;
		call->bollinger[i].lineTop = average + (BOLLINGER_TIME * sd);
		call->bollinger[i].lineBottom = average - (BOLLINGER_TIME * sd);
		call->bollinger[i].oscillalor = (call->data[i].lastVal - average) / (BOLLINGER_TIME * sd);
	
	}

	//Put의 Bollinger Band를 계산한다.
	for (i=BOLLINGER_AVG-1; i<pairUnit.put->quantity; i++) {

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //평균을 계산한다.
			sum += pairUnit.put->data[i-j].lastVal;
		}
		float average = sum / BOLLINGER_AVG;

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //표준편차를 계산한다.
			sum += (average - pairUnit.put->data[i-j].lastVal) * (average - pairUnit.put->data[i-j].lastVal);
		}
		float sd = sqrt((float)sum / BOLLINGER_AVG);

		//Bollinger Band의 데이터를 Put 구조체에 저장한다.
		put->bollinger[i].lineMiddle = average;
		put->bollinger[i].lineTop = average + (BOLLINGER_TIME * sd);
		put->bollinger[i].lineBottom = average - (BOLLINGER_TIME * sd);
		put->bollinger[i].oscillalor = (pairUnit.put->data[i].lastVal - average) / (BOLLINGER_TIME * sd);
	
	}
}

//-----------------------------------------------------------------------------------------------
// MACD를 계산한다.
void COption::CalculateMACD()
{
	int i;

	Unit *call = pairUnit.call;
	Unit *put = pairUnit.put;

	macdK1 = (float)(2.0 / (MACD_EMA_1 + 1)); //상수 K1, K2, K3 값들을 계산한다.
	macdK2 = (float)(2.0 / (MACD_EMA_2 + 1));
	macdK3 = (float)(2.0 / (MACD_SIGNAL + 1));

	call->macd[0].ema1 = call->data[0].lastVal; //Call의 시작 MACD를 넣어준다.
	call->macd[0].ema2 = call->data[0].lastVal;
	call->macd[0].macd = 0;
	call->macd[0].signal = 0;

	for (i=1; i<call->quantity; i++) { //Call의 지수이동평균을 계산하여 MACD 구조체에 넣어준다.
		call->macd[i].ema1 = (float)((macdK1 * call->data[i].lastVal) + ((1.0 - macdK1) * call->macd[i-1].ema1));
		call->macd[i].ema2 = (float)((macdK2 * call->data[i].lastVal) + ((1.0 - macdK2) * call->macd[i-1].ema2));
		call->macd[i].macd = call->macd[i].ema1 - call->macd[i].ema2;
		call->macd[i].signal = (float)((macdK3 * call->macd[i].macd) + ((1.0 - macdK3) * call->macd[i-1].signal));
	}

	put->macd[0].ema1 = put->data[0].lastVal; //Put의 시작 MACD를 넣어준다.
	put->macd[0].ema2 = put->data[0].lastVal;
	put->macd[0].macd = 0;
	put->macd[0].signal = 0;

	for (i=1; i<call->quantity; i++) { //Put의 지수이동평균을 계산하여 MACD 구조체에 넣어준다.
		put->macd[i].ema1 = (float)((macdK1 * put->data[i].lastVal) + ((1.0 - macdK1) * put->macd[i-1].ema1));
		put->macd[i].ema2 = (float)((macdK2 * put->data[i].lastVal) + ((1.0 - macdK2) * put->macd[i-1].ema2));
		put->macd[i].macd = put->macd[i].ema1 - put->macd[i].ema2;
		put->macd[i].signal = (float)((macdK3 * put->macd[i].macd) + ((1.0 - macdK3) * put->macd[i-1].signal));
	}
}

//-----------------------------------------------------------------------------------------------
// 실시간 데이터에 대한 Bollinger Band와 MACD를 계산한다.
void COption::CalculateRealData()
{
	UINT i;
	float sum, average, sd;

	Unit *call = pairUnit.call;
	Unit *put = pairUnit.put;
	int index = call->quantity - 1;

	//------- Bollinger Band 계산 -------
	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //평균을 계산한다.
		sum += call->data[index-i].lastVal;
	}
	average = sum / BOLLINGER_AVG;

	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //표준편차를 계산한다.
		sum += (average - call->data[index-i].lastVal) * (average - call->data[index-i].lastVal);
	}
	sd = sqrt((float)sum / BOLLINGER_AVG);

	call->bollinger[index].lineMiddle = average; //Bollinger Band의 데이터를 Call 구조체에 저장한다.
	call->bollinger[index].lineTop = average + (BOLLINGER_TIME * sd);
	call->bollinger[index].lineBottom = average - (BOLLINGER_TIME * sd);
	call->bollinger[index].oscillalor = (call->data[index].lastVal - average) / (BOLLINGER_TIME * sd);

	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //평균을 계산한다.
		sum += put->data[index-i].lastVal;
	}
	average = sum / BOLLINGER_AVG;

	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //표준편차를 계산한다.
		sum += (average - put->data[index-i].lastVal) * (average - put->data[index-i].lastVal);
	}
	sd = sqrt((float)sum / BOLLINGER_AVG);

	put->bollinger[index].lineMiddle = average; //Bollinger Band의 데이터를 Put 구조체에 저장한다.
	put->bollinger[index].lineTop = average + (BOLLINGER_TIME * sd);
	put->bollinger[index].lineBottom = average - (BOLLINGER_TIME * sd);
	put->bollinger[index].oscillalor = (put->data[index].lastVal - average) / (BOLLINGER_TIME * sd);

	//------- MACD 계산 -------
	//Call의 지수이동평균을 계산하여 MACD 구조체에 넣어준다.
	call->macd[index].ema1 = (float)((macdK1 * call->data[index].lastVal) + ((1.0 - macdK1) * call->macd[index-1].ema1));
	call->macd[index].ema2 = (float)((macdK2 * call->data[index].lastVal) + ((1.0 - macdK2) * call->macd[index-1].ema2));
	call->macd[index].macd = call->macd[index].ema1 - call->macd[index].ema2;
	call->macd[index].signal = (float)((macdK3 * call->macd[index].macd) + ((1.0 - macdK3) * call->macd[index-1].signal));
	
	//Put의 지수이동평균을 계산하여 MACD 구조체에 넣어준다.
	put->macd[index].ema1 = (float)((macdK1 * put->data[index].lastVal) + ((1.0 - macdK1) * put->macd[index-1].ema1));
	put->macd[index].ema2 = (float)((macdK2 * put->data[index].lastVal) + ((1.0 - macdK2) * put->macd[index-1].ema2));
	put->macd[index].macd = put->macd[index].ema1 - put->macd[index].ema2;
	put->macd[index].signal = (float)((macdK3 * put->macd[index].macd) + ((1.0 - macdK3) * put->macd[index-1].signal));
}

//-----------------------------------------------------------------------------------------------
// Call의 새로운 분 데이터가 추가되었을 때 구조체에 시간과 값을 넣어준다.
void COption::AddNewEntryCall(UINT newTime, float newValue)
{
	Unit *call = pairUnit.call;
	int index = call->quantity; //추가하기 위한 index

	call->data[index].time = newTime; //시간 추가
	call->data[index].lastVal = newValue; //종가 추가
	call->quantity += 1;

	Unit *put = pairUnit.put;
	
	if (put->quantity < call->quantity) {	//Put 데이터의 갯수를 맞추어준다.

		put->data[index].time = newTime;
		put->data[index].lastVal = put->data[index-1].lastVal; //이전 값을 넣어준다.
		put->quantity = call->quantity;
	}
}

//-----------------------------------------------------------------------------------------------
// Put의 새로운 분 데이터가 추가되었을 때 구조체에 시간과 값을 넣어준다.
void COption::AddNewEntryPut(UINT newTime, float newValue)
{
	Unit *put = pairUnit.put;
	int index = put->quantity;

	put->data[index].time = newTime; //시간 추가
	put->data[index].lastVal = newValue; //종가 추가
	put->quantity += 1;

	Unit *call = pairUnit.call;

	if (call->quantity < put->quantity) {	//Call 데이터의 갯수를 맞추어준다.

		call->data[index].time = newTime;
		call->data[index].lastVal = call->data[index-1].lastVal; //이전 값을 넣어준다.
		call->quantity = put->quantity;
	}
}

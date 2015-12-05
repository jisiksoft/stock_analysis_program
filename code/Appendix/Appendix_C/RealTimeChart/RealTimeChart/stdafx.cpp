/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#include "stdafx.h"

IXingAPI iXingAPI;

UINT BOLLINGER_AVG;
float BOLLINGER_TIME;

UINT MACD_EMA_1;
UINT MACD_EMA_2;
UINT MACD_SIGNAL;

//-----------------------------------------------------------------------------------------------
// 6자리의 실수 문자열을 받아서 float 형태로 변환한다. (ex: 001234 ==> 12.34)
float changeStringToFloat(char * strFloat) 
{
	int i, j, multiply;
	float result = 0;

	for (i=0; i<6; i++) {
		multiply = 1;
		for (j=0; j<i; j++) 
			multiply *= 10;

		result += (float)((int)(strFloat[5-i] - '0') * 0.01 * multiply);
	}

	return result;
}

//-----------------------------------------------------------------------------------------------
// 4자리의 시간 문자열을 받아서 'unsigned int'로 변환한다. (ex: 0923 ==> 923)
UINT changeStringTimeToInt(char * strFloat) 
{
	int i, j, multiply;
	UINT result = 0;

	for (i=0; i<4; i++) {
		multiply = 1;
		for (j=0; j<i; j++) 
			multiply *= 10;

		result += ((UINT)(strFloat[3-i] - '0') * multiply);
	}

	return result;
}

//-----------------------------------------------------------------------------------------------
// 시간 정수 데이터인 매개변수 두개를 받아서 몇분 차이인지를 계산하여 그 값을 return한다.
// (ex: 923 & 924 ==> 1, 959 & 1001 ==> 2 (9시59분과 10시1분은 2초 차이))
int getTimeGap(UINT timeA, UINT timeB) 
{
	int gap;

	int secTimeA = ((UINT)(timeA / 100) * 60) + (timeA % 100);
	int secTimeB = ((UINT)(timeB / 100) * 60) + (timeB % 100);

	if ((secTimeA > 900) || (secTimeB > 900))	//15시 이후의 데이터는 처리하지 않기 위해서 -1을 return한다. (900 = 15*60)
		return -1;

	if (secTimeA > secTimeB) {
		gap = secTimeA - secTimeB;
	} else if (secTimeB < secTimeA) {
		gap = secTimeB - secTimeA;
	} else {
		gap = 0;
	}

	return gap;
}

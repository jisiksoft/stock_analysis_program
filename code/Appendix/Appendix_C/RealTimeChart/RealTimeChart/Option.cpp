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
// ���� ���ڿ� ���� �ɼ� ���� ���� ��ȯ�Ѵ�. (��ȯ��: YYYYMM)
// ��°�� ����ϱ����� �ش� ���� ���� ��ȯ�ϰ�, ���� ���ڿ��� ������ ���� ���� ��ȯ�Ѵ�.
CString COption::GetMonthForData() 
{
	CString monthForData;

	CTime time = CTime::GetTickCount();

	int year = time.GetYear();		//���� �⵵
	int month = time.GetMonth();	//���� ��
	int day = time.GetDay();		//���� ����

	int dayWeekForEightDay = CTime(year,month,8,9,0,0).GetDayOfWeek();  //8���� ���� ���° ���������� ����Ѵ�.
	int dueDate;

	if (dayWeekForEightDay < 6) //�ι�° ������� ���� �������� ����Ѵ�.
		dueDate = 13 - dayWeekForEightDay;
	else
		dueDate = 20 - dayWeekForEightDay;

	if (day <= dueDate) { //�ι�° ����ϱ����� ���ڴ� �ش������ ���� �ȴ�.
		if (month < 10)
			monthForData.Format(_T("%d0%d"), year, month);
		else
			monthForData.Format(_T("%d%d"), year, month);
	} else {
		if (month < 12) { //�ι�° ����� ������ ���ڴ� ������ ������ ���� �ȴ�.
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
// Bollinger Band�� ����Ѵ�.
void COption::CalculateBollinger()
{
	int i, j;
	float sum;

	Unit *call = pairUnit.call;
	Unit *put = pairUnit.put;

	//Call�� Bollinger Band�� ����Ѵ�.
	for (i=BOLLINGER_AVG-1; i<call->quantity; i++) {

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //����� ����Ѵ�.
			sum += call->data[i-j].lastVal;
		}
		float average = sum / BOLLINGER_AVG;

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //ǥ�������� ����Ѵ�.
			sum += (average - call->data[i-j].lastVal) * (average - call->data[i-j].lastVal);
		}
		float sd = sqrt((float)sum / BOLLINGER_AVG);

		//Bollinger Band�� �����͸� Call ����ü�� �����Ѵ�.
		call->bollinger[i].lineMiddle = average;
		call->bollinger[i].lineTop = average + (BOLLINGER_TIME * sd);
		call->bollinger[i].lineBottom = average - (BOLLINGER_TIME * sd);
		call->bollinger[i].oscillalor = (call->data[i].lastVal - average) / (BOLLINGER_TIME * sd);
	
	}

	//Put�� Bollinger Band�� ����Ѵ�.
	for (i=BOLLINGER_AVG-1; i<pairUnit.put->quantity; i++) {

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //����� ����Ѵ�.
			sum += pairUnit.put->data[i-j].lastVal;
		}
		float average = sum / BOLLINGER_AVG;

		sum = 0;
		for (j=0; j<(int)BOLLINGER_AVG; j++) { //ǥ�������� ����Ѵ�.
			sum += (average - pairUnit.put->data[i-j].lastVal) * (average - pairUnit.put->data[i-j].lastVal);
		}
		float sd = sqrt((float)sum / BOLLINGER_AVG);

		//Bollinger Band�� �����͸� Put ����ü�� �����Ѵ�.
		put->bollinger[i].lineMiddle = average;
		put->bollinger[i].lineTop = average + (BOLLINGER_TIME * sd);
		put->bollinger[i].lineBottom = average - (BOLLINGER_TIME * sd);
		put->bollinger[i].oscillalor = (pairUnit.put->data[i].lastVal - average) / (BOLLINGER_TIME * sd);
	
	}
}

//-----------------------------------------------------------------------------------------------
// MACD�� ����Ѵ�.
void COption::CalculateMACD()
{
	int i;

	Unit *call = pairUnit.call;
	Unit *put = pairUnit.put;

	macdK1 = (float)(2.0 / (MACD_EMA_1 + 1)); //��� K1, K2, K3 ������ ����Ѵ�.
	macdK2 = (float)(2.0 / (MACD_EMA_2 + 1));
	macdK3 = (float)(2.0 / (MACD_SIGNAL + 1));

	call->macd[0].ema1 = call->data[0].lastVal; //Call�� ���� MACD�� �־��ش�.
	call->macd[0].ema2 = call->data[0].lastVal;
	call->macd[0].macd = 0;
	call->macd[0].signal = 0;

	for (i=1; i<call->quantity; i++) { //Call�� �����̵������ ����Ͽ� MACD ����ü�� �־��ش�.
		call->macd[i].ema1 = (float)((macdK1 * call->data[i].lastVal) + ((1.0 - macdK1) * call->macd[i-1].ema1));
		call->macd[i].ema2 = (float)((macdK2 * call->data[i].lastVal) + ((1.0 - macdK2) * call->macd[i-1].ema2));
		call->macd[i].macd = call->macd[i].ema1 - call->macd[i].ema2;
		call->macd[i].signal = (float)((macdK3 * call->macd[i].macd) + ((1.0 - macdK3) * call->macd[i-1].signal));
	}

	put->macd[0].ema1 = put->data[0].lastVal; //Put�� ���� MACD�� �־��ش�.
	put->macd[0].ema2 = put->data[0].lastVal;
	put->macd[0].macd = 0;
	put->macd[0].signal = 0;

	for (i=1; i<call->quantity; i++) { //Put�� �����̵������ ����Ͽ� MACD ����ü�� �־��ش�.
		put->macd[i].ema1 = (float)((macdK1 * put->data[i].lastVal) + ((1.0 - macdK1) * put->macd[i-1].ema1));
		put->macd[i].ema2 = (float)((macdK2 * put->data[i].lastVal) + ((1.0 - macdK2) * put->macd[i-1].ema2));
		put->macd[i].macd = put->macd[i].ema1 - put->macd[i].ema2;
		put->macd[i].signal = (float)((macdK3 * put->macd[i].macd) + ((1.0 - macdK3) * put->macd[i-1].signal));
	}
}

//-----------------------------------------------------------------------------------------------
// �ǽð� �����Ϳ� ���� Bollinger Band�� MACD�� ����Ѵ�.
void COption::CalculateRealData()
{
	UINT i;
	float sum, average, sd;

	Unit *call = pairUnit.call;
	Unit *put = pairUnit.put;
	int index = call->quantity - 1;

	//------- Bollinger Band ��� -------
	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //����� ����Ѵ�.
		sum += call->data[index-i].lastVal;
	}
	average = sum / BOLLINGER_AVG;

	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //ǥ�������� ����Ѵ�.
		sum += (average - call->data[index-i].lastVal) * (average - call->data[index-i].lastVal);
	}
	sd = sqrt((float)sum / BOLLINGER_AVG);

	call->bollinger[index].lineMiddle = average; //Bollinger Band�� �����͸� Call ����ü�� �����Ѵ�.
	call->bollinger[index].lineTop = average + (BOLLINGER_TIME * sd);
	call->bollinger[index].lineBottom = average - (BOLLINGER_TIME * sd);
	call->bollinger[index].oscillalor = (call->data[index].lastVal - average) / (BOLLINGER_TIME * sd);

	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //����� ����Ѵ�.
		sum += put->data[index-i].lastVal;
	}
	average = sum / BOLLINGER_AVG;

	sum = 0;
	for (i=0; i<BOLLINGER_AVG; i++) { //ǥ�������� ����Ѵ�.
		sum += (average - put->data[index-i].lastVal) * (average - put->data[index-i].lastVal);
	}
	sd = sqrt((float)sum / BOLLINGER_AVG);

	put->bollinger[index].lineMiddle = average; //Bollinger Band�� �����͸� Put ����ü�� �����Ѵ�.
	put->bollinger[index].lineTop = average + (BOLLINGER_TIME * sd);
	put->bollinger[index].lineBottom = average - (BOLLINGER_TIME * sd);
	put->bollinger[index].oscillalor = (put->data[index].lastVal - average) / (BOLLINGER_TIME * sd);

	//------- MACD ��� -------
	//Call�� �����̵������ ����Ͽ� MACD ����ü�� �־��ش�.
	call->macd[index].ema1 = (float)((macdK1 * call->data[index].lastVal) + ((1.0 - macdK1) * call->macd[index-1].ema1));
	call->macd[index].ema2 = (float)((macdK2 * call->data[index].lastVal) + ((1.0 - macdK2) * call->macd[index-1].ema2));
	call->macd[index].macd = call->macd[index].ema1 - call->macd[index].ema2;
	call->macd[index].signal = (float)((macdK3 * call->macd[index].macd) + ((1.0 - macdK3) * call->macd[index-1].signal));
	
	//Put�� �����̵������ ����Ͽ� MACD ����ü�� �־��ش�.
	put->macd[index].ema1 = (float)((macdK1 * put->data[index].lastVal) + ((1.0 - macdK1) * put->macd[index-1].ema1));
	put->macd[index].ema2 = (float)((macdK2 * put->data[index].lastVal) + ((1.0 - macdK2) * put->macd[index-1].ema2));
	put->macd[index].macd = put->macd[index].ema1 - put->macd[index].ema2;
	put->macd[index].signal = (float)((macdK3 * put->macd[index].macd) + ((1.0 - macdK3) * put->macd[index-1].signal));
}

//-----------------------------------------------------------------------------------------------
// Call�� ���ο� �� �����Ͱ� �߰��Ǿ��� �� ����ü�� �ð��� ���� �־��ش�.
void COption::AddNewEntryCall(UINT newTime, float newValue)
{
	Unit *call = pairUnit.call;
	int index = call->quantity; //�߰��ϱ� ���� index

	call->data[index].time = newTime; //�ð� �߰�
	call->data[index].lastVal = newValue; //���� �߰�
	call->quantity += 1;

	Unit *put = pairUnit.put;
	
	if (put->quantity < call->quantity) {	//Put �������� ������ ���߾��ش�.

		put->data[index].time = newTime;
		put->data[index].lastVal = put->data[index-1].lastVal; //���� ���� �־��ش�.
		put->quantity = call->quantity;
	}
}

//-----------------------------------------------------------------------------------------------
// Put�� ���ο� �� �����Ͱ� �߰��Ǿ��� �� ����ü�� �ð��� ���� �־��ش�.
void COption::AddNewEntryPut(UINT newTime, float newValue)
{
	Unit *put = pairUnit.put;
	int index = put->quantity;

	put->data[index].time = newTime; //�ð� �߰�
	put->data[index].lastVal = newValue; //���� �߰�
	put->quantity += 1;

	Unit *call = pairUnit.call;

	if (call->quantity < put->quantity) {	//Call �������� ������ ���߾��ش�.

		call->data[index].time = newTime;
		call->data[index].lastVal = call->data[index-1].lastVal; //���� ���� �־��ش�.
		call->quantity = put->quantity;
	}
}

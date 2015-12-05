/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#pragma once

#define MAX_DATA					375		//�� �������� �ִ� ����
#define MAX_UNIT					50		//Unit ����ü�� �ִ� ����

#define STANDARD_VALUE				1.3		//Call�� Put�� 1.3���� ���� �����̼� �����ϴ� ���� ���õǾ�����.

#define TIMER_INTERVAL				2		//�ǽð� �׷����� �׸��� ���� (��)
#define RESET_TIME					5		//�����͸� ó������ �ٽ� �׸��� ���� (��)

//-----------------------------------------------------------------------------------------------
// �ɼ� �����͸� �����ϱ� ���� ����ü : �� ���α׷������� �ð��� ������ ���ȴ�.
struct Data {
	UINT time;			//�ð� (ex:921:9��21��, 1304:13��4��)
	float startVal;		//�ð�
	float highVal;		//��
	float lowVal;		//����
	float lastVal;		//����
	long vol;
};

//-----------------------------------------------------------------------------------------------
// Bollinger Band �����͸� �����ϱ� ���� ����ü
struct Bollinger {
	float lineTop;		//����
	float lineMiddle;	//�߰���
	float lineBottom;	//�Ʒ���
	float oscillalor;	//Bollinger Band ���Ƿ�����
};

//-----------------------------------------------------------------------------------------------
// MACD �����͸� �����ϱ� ���� ����ü
struct MACD {
	float ema1;			//MACD EMA1
	float ema2;			//MACD EMA2
	float macd;			//MACD ��� ��
	float signal;		//MACD Signal
};

//-----------------------------------------------------------------------------------------------
// �ɼ� ������� �����͸� �����ϱ� ���� ����ü
struct Unit {
	CString code;					//�����ڵ�
	float openVal;					//���۰�
	int quantity;					//������ ����
	Data data[MAX_DATA];			//������
	Bollinger bollinger[MAX_DATA];	//Bollinger Band ������
	MACD macd[MAX_DATA];			//MACD ������
};

//-----------------------------------------------------------------------------------------------
// ��ü ������ ��� ���� ����ü
struct AllUnit {
	int quantityCall;			//Call ���� ����
	int quantityPut;			//Put ���� ����
	Unit unitCall[MAX_UNIT];	//Call ������ ����
	Unit unitPut[MAX_UNIT];		//Put ������ ����
};

//-----------------------------------------------------------------------------------------------
// Chart���� ����ϴ� Call�� Put�� �� ���� ����Ű�� ����ü
struct PairUnit {
	Unit *put;
	Unit *call;
};

//-----------------------------------------------------------------------------------------------
// COption�� �����Ϳ� �����ϰ�, ����� ���� �Լ��� ������ �ִ� Ŭ����
class COption
{
public:
	AllUnit allUnit;		//��ü �����Ϳ� �����ϱ� ���Ͽ� ���
	PairUnit pairUnit;		//���� Chart���� ����ϴ� Call�� Put�� ����Ų��.

	float macdK1;			//MACD�� ��꿡�� ���Ǵ� �Ǽ��� �����ϴ� ������
	float macdK2;
	float macdK3;

public:
	COption(void);
	~COption(void);

	CString GetMonthForData();		//���� ���ڿ� ���� �ɼ� ���� ���� ��ȯ�Ѵ�. (��ȯ��: YYY
	void CalculateBollinger();		//Bollinger Band�� ����Ѵ�.
	void CalculateMACD();			//MACD�� ����Ѵ�.
	void CalculateRealData();		//Real Data�� Bollinger Band�� MACD�� ����Ѵ�.
	void AddNewEntryCall(UINT time, float value);	//�ǽð� �����Ϳ��� ���� ���� Call �����͸� ���� �߰��Ѵ�.
	void AddNewEntryPut(UINT time, float value);	//�ǽð� �����Ϳ��� ���� ���� Put �����͸� ���� �߰��Ѵ�.
};

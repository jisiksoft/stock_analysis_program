#pragma once

#define MAX_DATA						250
#define MAX_COMPANY					2500

#define MOVE_AVERAGE_1				5
#define MOVE_AVERAGE_2				20

#define CHARGE_BUY						0.00015
#define CHARGE_SELL					0.00315
#define CHARGE_TRADE					0.005

struct Data {
	long date;				//날자
	long startVal;			//시가
	long highVal;			//고가
	long lowVal;			//저가
	long lastVal;			//종가
	long vol;				//거래량
};

struct MovementAverage {
	long avg1;				//첫번째 이평선
	long avg2;				//두번째 이평선
};

struct Company {
	CString strJongMok, strName;			//종목번호, 종목명
	int quantity;							//데이터 갯수
	Data data[MAX_DATA];					//주가 데이터
	MovementAverage moveAverage[MAX_DATA];	//이평선 데이터
};

struct AllCompany {
	int quantity;											//전체 종목 갯수
	Company companies[MAX_COMPANY];		//2500개의 종목 구조체를 배열로 선언
};

struct SelectedCompany {
	int quantity;
	Company *companies[MAX_COMPANY];
};

class CStock
{
public:
	AllCompany allCompanies;
	SelectedCompany selectedCompanies;
	
	Company *ptrCompany;

public:
	CStock(void);
	~CStock(void);

	void Run();
	void makeSelectedCompanyFromAllCompany();
	void makeMovementAverage();
	void simulateTrade();
	void ReadDataFromFile();
	void WriteDataToFile();
};

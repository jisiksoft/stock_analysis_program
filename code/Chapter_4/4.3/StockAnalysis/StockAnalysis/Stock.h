#pragma once

#define MAX_DATA						250
#define MAX_COMPANY					2500

#define MACD_SEED_EMA_1			12
#define MACD_SEED_EMA_2			26
#define MACD_SEED_SIGNAL			9

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

//-----------------------------------------------------------------------------------------------
struct MACD {
	long ema1;
	long ema2;
	int macd;
	int signal;
};

struct Company {
	CString strJongMok, strName;	//종목번호, 종목명
	int quantity;							//데이터 갯수
	Data data[MAX_DATA];			//주가 데이터
	MACD macd[MAX_DATA];
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
	void makeMACD();							//make MACD
	void simulateTrade();
	void ReadDataFromFile();
	void WriteDataToFile();
};

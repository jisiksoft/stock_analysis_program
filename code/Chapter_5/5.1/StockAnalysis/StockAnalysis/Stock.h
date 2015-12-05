#pragma once

#define MAX_DATA						250
#define MAX_COMPANY					2500

#define BOLLINGER_MOVE_AVG		20
#define BOLLINGER_TIME_SD			2

struct Data {
	long date;				//날자
	long startVal;			//시가
	long highVal;			//고가
	long lowVal;			//저가
	long lastVal;			//종가
	long vol;				//거래량
};

//-----------------------------------------------------------------------------------------------
struct Bollinger {
	long lineTop;
	long lineMiddle;
	long lineBottom;
	float oscillator;
};

struct Company {
	CString strJongMok, strName;	//종목번호, 종목명
	int quantity;							//데이터 갯수
	Data data[MAX_DATA];			//주가 데이터
	Bollinger bollinger[MAX_DATA];
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
	void MakeBollinger();
	void ReadDataFromFile();
	void WriteDataToFile();
};

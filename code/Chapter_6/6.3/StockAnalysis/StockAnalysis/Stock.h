#pragma once

#define FLAG_ALGORITHM1			0x00000001			//algorith flag value
#define FLAG_ALGORITHM2			0x00000002
#define FLAG_ALGORITHM3			0x00000004
#define FLAG_ALGORITHM4			0x00000008
#define FLAG_ALGORITHM5			0x00000010
#define FLAG_ALGORITHM6			0x00000020
#define FLAG_ALGORITHM7			0x00000040
#define FLAG_ALGORITHM8			0x00000080

#define MAX_DATA						250
#define MAX_COMPANY					2500

struct Data {
	long date;				//날자
	long startVal;			//시가
	long highVal;			//고가
	long lowVal;			//저가
	long lastVal;			//종가
	long vol;				//거래량
};

struct MovementAverage {
	long avg5[MAX_DATA];		// 5 평선
	long avg20[MAX_DATA];		// 20 평선
	long avg60[MAX_DATA];		// 60 평선
	long avg120[MAX_DATA];	// 120 평선
};

struct Company {
	CString strJongMok, strName;	//종목번호, 종목명
	int quantity;							//데이터 갯수
	Data data[MAX_DATA];			//주가 데이터
	MovementAverage moveAverage;
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

	void Run(unsigned int algorithm);
	void makeSelectedCompanyFromAllCompany();
	void makeMovementAverage();
	bool condition1(Company *company);
	bool condition2(Company *company);
	bool condition3(Company *company);
	bool condition4(Company *company);
	bool condition5(Company *company);
	bool condition6(Company *company);
	bool condition7(Company *company);
	bool condition8(Company *company);
	void ReadDataFromFile();
	void WriteDataToFile();
};

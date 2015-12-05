#pragma once

#define MAX_DATA						250
#define MAX_COMPANY					2500

#define MACD_SEED_EMA_1		12//5//12
#define MACD_SEED_EMA_2		26//10//26
#define MACD_SEED_SIGNAL				9//3//9

struct Data {
	long date;				//����
	long startVal;			//�ð�
	long highVal;			//����
	long lowVal;			//����
	long lastVal;			//����
	long vol;				//�ŷ���
};

//-----------------------------------------------------------------------------------------------
struct MACD {
	long ema1;
	long ema2;
	int macd;
	int signal;
};

struct Company {
	CString strJongMok, strName;	//�����ȣ, �����
	int quantity;							//������ ����
	Data data[MAX_DATA];			//�ְ� ������
	MACD macd[MAX_DATA];
};

struct AllCompany {
	int quantity;											//��ü ���� ����
	Company companies[MAX_COMPANY];		//2500���� ���� ����ü�� �迭�� ����
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
	void MakeMACD();							//make MACD
	void ReadDataFromFile();
	void WriteDataToFile();
};
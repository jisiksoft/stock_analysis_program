#pragma once

#define MAX_DATA						250
#define MAX_COMPANY					2500

#define BOLLINGER_MOVE_AVG		20
#define BOLLINGER_TIME_SD			2

struct Data {
	long date;				//����
	long startVal;			//�ð�
	long highVal;			//��
	long lowVal;			//����
	long lastVal;			//����
	long vol;				//�ŷ���
};

//-----------------------------------------------------------------------------------------------
struct Bollinger {
	long lineTop;
	long lineMiddle;
	long lineBottom;
	float oscillator;
};

struct Company {
	CString strJongMok, strName;	//�����ȣ, �����
	int quantity;							//������ ����
	Data data[MAX_DATA];			//�ְ� ������
	Bollinger bollinger[MAX_DATA];
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
	void MakeBollinger();
	void ReadDataFromFile();
	void WriteDataToFile();
};

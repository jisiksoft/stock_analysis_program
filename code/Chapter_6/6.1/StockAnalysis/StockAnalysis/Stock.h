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
	long date;				//����
	long startVal;			//�ð�
	long highVal;			//��
	long lowVal;			//����
	long lastVal;			//����
	long vol;				//�ŷ���
};

struct MovementAverage {
	long avg5[MAX_DATA];		// 5 ��
	long avg20[MAX_DATA];		// 20 ��
	long avg60[MAX_DATA];		// 60 ��
	long avg120[MAX_DATA];	// 120 ��
};

struct Company {
	CString strJongMok, strName;	//�����ȣ, �����
	int quantity;							//������ ����
	Data data[MAX_DATA];			//�ְ� ������
	MovementAverage moveAverage;
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

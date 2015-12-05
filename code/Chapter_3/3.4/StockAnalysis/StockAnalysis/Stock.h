#pragma once

#define MAX_DATA						250
#define MAX_COMPANY					2500

#define MOVE_AVERAGE_1				5
#define MOVE_AVERAGE_2				20

#define CHARGE_BUY						0.00015
#define CHARGE_SELL					0.00315
#define CHARGE_TRADE					0.005

struct Data {
	long date;				//����
	long startVal;			//�ð�
	long highVal;			//��
	long lowVal;			//����
	long lastVal;			//����
	long vol;				//�ŷ���
};

struct MovementAverage {
	long avg1;				//ù��° ����
	long avg2;				//�ι�° ����
};

struct Company {
	CString strJongMok, strName;			//�����ȣ, �����
	int quantity;							//������ ����
	Data data[MAX_DATA];					//�ְ� ������
	MovementAverage moveAverage[MAX_DATA];	//���� ������
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
	void makeMovementAverage();
	void simulateTrade();
	void ReadDataFromFile();
	void WriteDataToFile();
};

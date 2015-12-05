#pragma once

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
	long avg5;				// 5 ��
	long avg20;				// 20 ��
	long avg60;				// 60 ��
	long avg120;			// 120 ��
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

struct DataGoldCross {
	long valueAtGoldCross20;			//20�򼱰� Gold Cross �Ǿ��� ���� ����
	long valueAfter1Month20;			//20�򼱰� Gold Cross �� 1�� �� ����
	long valueAfter3Month20;			//20�򼱰� Gold Cross �� 3�� �� ����
	long valueAfter6Month20;			//20�򼱰� Gold Cross �� 6�� �� ����
	long valueAtGoldCross60;			//60�򼱰� Gold Cross �Ǿ��� ���� ����
	long valueAfter1Month60;			//60�򼱰� Gold Cross �� 1�� �� ����
	long valueAfter3Month60;			//60�򼱰� Gold Cross �� 3�� �� ����
	long valueAfter6Month60;			//60�򼱰� Gold Cross �� 6�� �� ����
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
	void analyzeGoldCross();
	void analyzeCompany(Company *company, DataGoldCross *data);
	void makeMovementAverage();
	void ReadDataFromFile();
	void WriteDataToFile();
};

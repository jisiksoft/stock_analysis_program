
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

struct Company {
	CString strJongMok, strName;	//�����ȣ, �����
	int quantity;							//������ ����
	Data data[MAX_DATA];			//�ְ� ������
};

struct AllCompany {
	int quantity;											//��ü ���� ����
	Company companies[MAX_COMPANY];		//2500���� ���� ����ü�� �迭�� ����
};

class CStock
{
public:
	AllCompany allCompanies;
public:
	CStock(void);
	~CStock(void);

	void Run();
	void ReadDataFromFile();
	void WriteDataToFile();
};

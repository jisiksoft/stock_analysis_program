
#include "stdafx.h"
#include "Stock.h"


CStock::CStock(void)
{
}

CStock::~CStock(void)
{
}

void CStock::Run()
{
	ReadDataFromFile();
	//WriteDataToFile();
	makeSelectedCompanyFromAllCompany();
	makeMovementAverage();

	analyzeGoldCross();
}

void CStock::makeSelectedCompanyFromAllCompany()
{
	selectedCompanies.quantity = allCompanies.quantity;

	for ( int i=0; i<selectedCompanies.quantity; i++) {
		selectedCompanies.companies[i] = &allCompanies.companies[i];
	}
}

//-----------------------------------------------------------------------------------------------------
void CStock::analyzeGoldCross() {

	int i;
	DataGoldCross data[MAX_COMPANY];

	for (i=0; i<allCompanies.quantity; i++) {

		Company *company = &allCompanies.companies[i];
	
		analyzeCompany(company, &data[i]);
	}

	int countWin1Month20 = 0;			//20평선 Gold Cross이후 1개월 뒤 상승한 종목 개수
	int countWin3Month20 = 0;
	int countWin6Month20 = 0;
	int countWin1Month60 = 0;			//60평선 Gold Cross이후 1개월 뒤 상승한 종목 개수
	int countWin3Month60 = 0;
	int countWin6Month60 = 0;
	int countLose1Month20 = 0;		//20평선 Gold Cross이후 1개월 뒤 하락한 종목 개수
	int countLose3Month20 = 0;
	int countLose6Month20 = 0;
	int countLose1Month60 = 0;		//60평선 Gold Cross이후 1개월 뒤 하락한 종목 개수
	int countLose3Month60 = 0;
	int countLose6Month60 = 0;

	for (i=0; i<allCompanies.quantity; i++) {

		if (data[i].valueAfter1Month20 > data[i].valueAtGoldCross20) { countWin1Month20 += 1; }
		if (data[i].valueAfter3Month20 > data[i].valueAtGoldCross20) { countWin3Month20 += 1; }
		if (data[i].valueAfter6Month20 > data[i].valueAtGoldCross20) { countWin6Month20 += 1; }
		if (data[i].valueAfter1Month60 > data[i].valueAtGoldCross60) { countWin1Month60 += 1; }
		if (data[i].valueAfter3Month60 > data[i].valueAtGoldCross60) { countWin3Month60 += 1; }
		if (data[i].valueAfter6Month60 > data[i].valueAtGoldCross60) { countWin6Month60 += 1; }

		if (data[i].valueAfter1Month20 < data[i].valueAtGoldCross60) { countLose1Month20 += 1; }
		if (data[i].valueAfter3Month20 < data[i].valueAtGoldCross60) { countLose3Month20 += 1; }
		if (data[i].valueAfter6Month20 < data[i].valueAtGoldCross60) { countLose6Month20 += 1; }
		if (data[i].valueAfter1Month60 < data[i].valueAtGoldCross60) { countLose1Month60 += 1; }
		if (data[i].valueAfter3Month60 < data[i].valueAtGoldCross60) { countLose3Month60 += 1; }
		if (data[i].valueAfter6Month60 < data[i].valueAtGoldCross60) { countLose6Month60 += 1; }
	}

	CString str;
	str.AppendFormat(_T("20평선 Gold Cross이후 1개월 뒤 상승한 종목 개수 : %d \n"), countWin1Month20);
	str.AppendFormat(_T("20평선 Gold Cross이후 3개월 뒤 상승한 종목 개수 : %d \n"), countWin3Month20);
	str.AppendFormat(_T("20평선 Gold Cross이후 6개월 뒤 상승한 종목 개수 : %d \n"), countWin6Month20);
	str.AppendFormat(_T("60평선 Gold Cross이후 1개월 뒤 상승한 종목 개수 : %d \n"), countWin1Month60);
	str.AppendFormat(_T("60평선 Gold Cross이후 3개월 뒤 상승한 종목 개수 : %d \n"), countWin3Month60);
	str.AppendFormat(_T("60평선 Gold Cross이후 6개월 뒤 상승한 종목 개수 : %d \n"), countWin6Month60);
	str.AppendFormat(_T("20평선 Gold Cross이후 1개월 뒤 하락한 종목 개수 : %d \n"), countLose1Month20);
	str.AppendFormat(_T("20평선 Gold Cross이후 3개월 뒤 하락한 종목 개수 : %d \n"), countLose3Month20);
	str.AppendFormat(_T("20평선 Gold Cross이후 6개월 뒤 하락한 종목 개수 : %d \n"), countLose6Month20);
	str.AppendFormat(_T("60평선 Gold Cross이후 1개월 뒤 하락한 종목 개수 : %d \n"), countLose1Month60);
	str.AppendFormat(_T("60평선 Gold Cross이후 3개월 뒤 하락한 종목 개수 : %d \n"), countLose3Month60);
	str.AppendFormat(_T("60평선 Gold Cross이후 6개월 뒤 하락한 종목 개수 : %d \n"), countLose6Month60);
	::AfxMessageBox(str);

}

//-----------------------------------------------------------------------------------------------------
void CStock::analyzeCompany(Company *company, DataGoldCross *data) {

	int i;

	data->valueAtGoldCross20 = -1;
	data->valueAfter1Month20 = -1;
	data->valueAfter3Month20 = -1;
	data->valueAfter6Month20 = -1;
	data->valueAtGoldCross60 = -1;
	data->valueAfter1Month60 = -1;
	data->valueAfter3Month60 = -1;
	data->valueAfter6Month60 = -1;

	for (i=120; i<company->quantity-20; i++) {

		if ((company->moveAverage[i].avg5 > company->moveAverage[i].avg20) &&
			(company->moveAverage[i+1].avg5 < company->moveAverage[i+1].avg20)) {

				data->valueAtGoldCross20 = company->data[i].lastVal;
				data->valueAfter1Month20 = company->data[i-20].lastVal;
				data->valueAfter3Month20 = company->data[i-60].lastVal;
				data->valueAfter6Month20 = company->data[i-120].lastVal;

				break;
		}
	}

	for (i=120; i<company->quantity-60; i++) {

		if ((company->moveAverage[i].avg5 > company->moveAverage[i].avg60) &&
			(company->moveAverage[i+1].avg5 < company->moveAverage[i+1].avg60)) {

				data->valueAtGoldCross60 = company->data[i].lastVal;
				data->valueAfter1Month60 = company->data[i-20].lastVal;
				data->valueAfter3Month60 = company->data[i-60].lastVal;
				data->valueAfter6Month60 = company->data[i-120].lastVal;

				break;
		}
	}
}

//-----------------------------------------------------------------------------------------------------
void CStock::makeMovementAverage()
{
	int i, j, k;
	long temp;

	for (i=0; i<allCompanies.quantity; i++) {

		Company *company = &allCompanies.companies[i];
		int quantity = company->quantity;

		for (j=0; j<=quantity-5; j++)
		{
			temp = 0;
			for (k=j; k<j+5; k++)
				temp += company->data[k].lastVal;
			company->moveAverage[j].avg5 = (long)(temp / 5);
		}

		for (j=0; j<=quantity-20; j++)
		{
			temp = 0;
			for (k=j; k<j+20; k++)
				temp += company->data[k].lastVal;
			company->moveAverage[j].avg20 = (long)(temp / 20);
		}

		for (j=0; j<=quantity-60; j++)
		{
			temp = 0;
			for (k=j; k<j+60; k++)
				temp += company->data[k].lastVal;
			company->moveAverage[j].avg60 = (long)(temp / 60);
		}

		for (j=0; j<=quantity-120; j++)
		{
			temp = 0;
			for (k=j; k<j+120; k++)
				temp += company->data[k].lastVal;
			company->moveAverage[j].avg120 = (long)(temp / 120);
		}
	}
}

void CStock::ReadDataFromFile()
{
	int cn;
	FILE *fp;

	fp = fopen("data.txt", "rt");

	if (fp==NULL) {
		::AfxMessageBox(_T("data.txt file is not opened"));
		exit(1);
	}

	cn = fscanf(fp, "%d \n", &allCompanies.quantity);

	if(cn==0) {
		::AfxMessageBox(_T("Error: can't get allCompanies.quantity."));
		exit(1);
	}

	char jongmok[20];
	char name[50];

	for (int i=0; i<allCompanies.quantity; i++) {

		cn = fscanf(fp, "%s %s %d \n", jongmok, name, &allCompanies.companies[i].quantity);

		if(cn==0) {
			::AfxMessageBox(_T("Error: fscanf error inside the first for loop"));
			exit(1);
		}

		allCompanies.companies[i].strJongMok.Format("%s", jongmok);
		allCompanies.companies[i].strName.Format("%s", name);

		for( int j =  0; j < allCompanies.companies[i].quantity; j++)
		{
			cn = fscanf(fp, "%ld %ld %ld %ld %ld %ld \n", 
								&allCompanies.companies[i].data[j].date, 
								&allCompanies.companies[i].data[j].startVal, 
								&allCompanies.companies[i].data[j].highVal, 
								&allCompanies.companies[i].data[j].lowVal, 
								&allCompanies.companies[i].data[j].lastVal, 
								&allCompanies.companies[i].data[j].vol);
			
			if(cn==0) {
				::AfxMessageBox(_T("Error: fscanf error inside the second for loop"));
				exit(1);
			}
		}
	}

	fclose(fp);
}

void CStock::WriteDataToFile()
{
	FILE *fp;

	fp = fopen("data2.txt", "wt");

	if (fp==NULL) {
		::AfxMessageBox(_T("data.txt file is not opened"));
		exit(1);
	}

	fprintf(fp, "%d \n", allCompanies.quantity);

	for (int i=0; i<allCompanies.quantity; i++) {

		fprintf(fp, "%s %s %d \n", LPSTR(LPCTSTR(allCompanies.companies[i].strJongMok)), LPSTR(LPCTSTR(allCompanies.companies[i].strName)), allCompanies.companies[i].quantity);

		for( int j =  0; j < allCompanies.companies[i].quantity; j++)
		{
			fprintf(fp, "%ld %ld %ld %ld %ld %ld \n", 
				allCompanies.companies[i].data[j].date, 
				allCompanies.companies[i].data[j].startVal, 
				allCompanies.companies[i].data[j].highVal, 
				allCompanies.companies[i].data[j].lowVal, 
				allCompanies.companies[i].data[j].lastVal, 
				allCompanies.companies[i].data[j].vol);
		}
	}

	fclose(fp);
}

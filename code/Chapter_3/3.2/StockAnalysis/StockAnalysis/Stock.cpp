
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
}

void CStock::makeSelectedCompanyFromAllCompany()
{
	selectedCompanies.quantity = allCompanies.quantity;

	for ( int i=0; i<selectedCompanies.quantity; i++) {
		selectedCompanies.companies[i] = &allCompanies.companies[i];
	}
}

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

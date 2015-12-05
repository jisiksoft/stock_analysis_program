
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
}

void CStock::makeSelectedCompanyFromAllCompany()
{
	selectedCompanies.quantity = allCompanies.quantity;

	for ( int i=0; i<selectedCompanies.quantity; i++) {
		selectedCompanies.companies[i] = &allCompanies.companies[i];
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

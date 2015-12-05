
#include "stdafx.h"
#include "Stock.h"

#include <math.h>


CStock::CStock(void)
{
}

CStock::~CStock(void)
{
}

void CStock::Run()
{
	ReadDataFromFile();
	//makeSelectedCompanyFromAllCompany();
	MakeBollinger();
	simulateTrade();
}

void CStock::makeSelectedCompanyFromAllCompany()
{
	selectedCompanies.quantity = allCompanies.quantity;

	for ( int i=0; i<selectedCompanies.quantity; i++) {
		selectedCompanies.companies[i] = &allCompanies.companies[i];
	}
}

//-----------------------------------------------------------------------------------------------
// make Bollinger Band data.
void CStock::MakeBollinger()
{
	int i, j, k;
	float sum;

	for (i=0; i<allCompanies.quantity; i++) {

		Company *company = &allCompanies.companies[i];
		int quantity = company->quantity;

		for (j=company->quantity-BOLLINGER_MOVE_AVG; j>=0; j--) {
			sum = 0;
			for (k=0; k<BOLLINGER_MOVE_AVG; k++) {
				sum += company->data[j+k].lastVal;
			}
			long average = (long)(sum / BOLLINGER_MOVE_AVG);

			sum = 0;
			for (k=0; k<BOLLINGER_MOVE_AVG; k++) {
				sum += (float)(average - company->data[j+k].lastVal) * (average - company->data[j+k].lastVal);
				float sd = sqrt((float)sum / BOLLINGER_MOVE_AVG);

				company->bollinger[j].lineMiddle = average;
				company->bollinger[j].lineTop = company->bollinger[j].lineMiddle + (long)(BOLLINGER_TIME_SD * sd);
				company->bollinger[j].lineBottom = company->bollinger[j].lineMiddle - (long)(BOLLINGER_TIME_SD * sd);
				company->bollinger[j].oscillator = (company->data[j].lastVal - company->bollinger[j].lineMiddle) / (BOLLINGER_TIME_SD * sd);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void CStock::simulateTrade()
{
	int balance = 100000000;
	int prevBalance;
	int countWin = 0;
	int countLose = 0;
	int charge;	
	int chargeTradeCenter = 0;

	selectedCompanies.quantity = 0;

	int i, j;

	for (i=0; i<allCompanies.quantity; i++) {

		Company *company = &allCompanies.companies[i];
		int quantity = company->quantity;

		prevBalance = balance;
		int isBought = false;

		for (j=quantity-BOLLINGER_MOVE_AVG-1; j>0; j--) {

			if (!isBought) {
				if (company->bollinger[j+1].oscillator <= -1 && company->bollinger[j].oscillator > -1) {
					charge = (int)(company->data[j].lastVal * CHARGE_BUY);
					balance -= charge;
					chargeTradeCenter += charge;
					charge = (int)(company->data[j].lastVal * CHARGE_TRADE);
					balance -= charge;
					balance -= company->data[j].lastVal;
					isBought = true;
				}
			} else {	
				if (company->bollinger[j+1].oscillator >= 1 && company->bollinger[j].oscillator < 1) {
					charge = (int)(company->data[j].lastVal * CHARGE_SELL);
					balance -= charge;
					chargeTradeCenter += charge;
					charge = (int)(company->data[j].lastVal * CHARGE_TRADE);
					balance -= charge;
					balance += company->data[j].lastVal;	
					isBought = false;
				}
			}
		}
		if (isBought) {
			charge = (int)(company->data[0].lastVal * CHARGE_SELL);
			balance -= charge;
			chargeTradeCenter += charge;
			charge = (int)(company->data[0].lastVal * CHARGE_TRADE);
			balance -= charge;
			balance += company->data[0].lastVal;
			isBought = false;
		}

		if (balance > prevBalance) {
			countWin += 1;
			selectedCompanies.companies[selectedCompanies.quantity] = company;
			selectedCompanies.quantity += 1;
		} else if (balance < prevBalance) {
			countLose += 1;
		}
	}

	CString str;
	str.AppendFormat(_T("Win 謙跡偎熱: %d \n"), countWin);
	str.AppendFormat(_T("Lose 謙跡偎熱: %d \n"), countLose);
	str.AppendFormat(_T("Balance : %d \n"), balance);
	str.AppendFormat(_T("熱熱猿: %d \n"), chargeTradeCenter);
	::AfxMessageBox(str);	
}

//-----------------------------------------------------------------------------------------------
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

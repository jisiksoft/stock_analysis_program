
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
	//makeSelectedCompanyFromAllCompany();
	makeMACD();
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
// make MACD data.
void CStock::makeMACD()
{
	int i, j;

	float K1 = (float)(2.0 / (MACD_SEED_EMA_1 + 1));
	float K2 = (float)(2.0 / (MACD_SEED_EMA_2 + 1));
	float K3 = (float)(2.0 / (MACD_SEED_SIGNAL + 1));

	for (i=0; i<allCompanies.quantity; i++) {

		Company *company = &allCompanies.companies[i];
		int quantity = company->quantity;

		if (company->quantity > 0) {
			company->macd[company->quantity-1].ema1 = company->data[company->quantity-1].lastVal;
			company->macd[company->quantity-1].ema2 = company->data[company->quantity-1].lastVal;
			company->macd[company->quantity-1].macd = 0;
			company->macd[company->quantity-1].signal = 0;

			for (j=company->quantity-2; j>=0; j--) {
				company->macd[j].ema1 = (long)((K1 * company->data[j].lastVal) + ((1.0 - K1) * company->macd[j+1].ema1));
				company->macd[j].ema2 = (long)((K2 * company->data[j].lastVal) + ((1.0 - K2) * company->macd[j+1].ema2));
				company->macd[j].macd = company->macd[j].ema1 - company->macd[j].ema2;
				company->macd[j].signal = (int)((K3 * company->macd[j].macd) + ((1.0 - K3) * company->macd[j+1].signal));
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

		for (j=quantity-2; j>0; j--) {

			if (!isBought) {
				if (company->macd[j].signal > 0) {
						charge = (int)(company->data[j].lastVal * CHARGE_BUY);
						balance -= charge;
						chargeTradeCenter += charge;
						charge = (int)(company->data[j].lastVal * CHARGE_TRADE);
						balance -= charge;
						balance -= company->data[j].lastVal;
						isBought = true;
				}
			} else {	
				if (company->macd[j].signal < 0) {
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

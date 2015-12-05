///////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                               //
// Date       : Mar 17, 2013                             //
// eMail      : kimsehoon@hotmail.com                    //
// Description: This Program Gets Stock Data from ETRADE //
///////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Stock.h"

CStock::CStock(void)
{
}

CStock::~CStock(void)
{
}

//-----------------------------------------------------------------------------------------------
// CStock 클래스에 저장된 모든 데이터를 파일에 저장한다.
void CStock::StoreDataInOneFile(CString fileName)
{	
	FILE *fp;

	fp = fopen(LPSTR(LPCTSTR(fileName)), "wt");			//파일을 연다.
	if (fp==NULL)
	{
		printf("can not open file...\n");
		exit(1);
	}

	int i, j;
	Company *company;

	fprintf(fp, "%ld \n", allCompany.quantity);

	for (i=0; i<allCompany.quantity; i++) {
		
		company = &allCompany.company[i];				//포인터 변수인 company는 하나의 종목을 차례대로 가리키게 된다.

		//종목코드, 종목명, 데이터 갯수를 파일에 출력한다.
		fprintf(fp, "%s %s %d \n", LPSTR(LPCTSTR(company->strJongMok)), LPSTR(LPCTSTR(company->strName.SpanExcluding(" "))), company->quantity);

		//날자, 시가, 고가, 저가, 종가, 거래량을 파일에 출력한다.
		for (j=company->quantity-1; j>=0; j--)
			fprintf(fp, "%s %ld %ld %ld %ld %ld \n", 
				LPSTR(LPCTSTR(company->data[j].date)), 
				company->data[j].startVal, company->data[j].highVal, company->data[j].lowVal, company->data[j].lastVal, company->data[j].vol);
	}

	fclose(fp);			//파일을 닫는다.
}

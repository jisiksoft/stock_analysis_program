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
// CStock Ŭ������ ����� ��� �����͸� ���Ͽ� �����Ѵ�.
void CStock::StoreDataInOneFile(CString fileName)
{	
	FILE *fp;

	fp = fopen(LPSTR(LPCTSTR(fileName)), "wt");			//������ ����.
	if (fp==NULL)
	{
		printf("can not open file...\n");
		exit(1);
	}

	int i, j;
	Company *company;

	fprintf(fp, "%ld \n", allCompany.quantity);

	for (i=0; i<allCompany.quantity; i++) {
		
		company = &allCompany.company[i];				//������ ������ company�� �ϳ��� ������ ���ʴ�� ����Ű�� �ȴ�.

		//�����ڵ�, �����, ������ ������ ���Ͽ� ����Ѵ�.
		fprintf(fp, "%s %s %d \n", LPSTR(LPCTSTR(company->strJongMok)), LPSTR(LPCTSTR(company->strName.SpanExcluding(" "))), company->quantity);

		//����, �ð�, ��, ����, ����, �ŷ����� ���Ͽ� ����Ѵ�.
		for (j=company->quantity-1; j>=0; j--)
			fprintf(fp, "%s %ld %ld %ld %ld %ld \n", 
				LPSTR(LPCTSTR(company->data[j].date)), 
				company->data[j].startVal, company->data[j].highVal, company->data[j].lowVal, company->data[j].lastVal, company->data[j].vol);
	}

	fclose(fp);			//������ �ݴ´�.
}

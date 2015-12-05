///////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                               //
// Date       : Mar 17, 2013                             //
// eMail      : kimsehoon@hotmail.com                    //
// Description: This Program Gets Stock Data from ETRADE //
///////////////////////////////////////////////////////////

#include "stdafx.h"

IXingAPI iXingAPI;

//-----------------------------------------------------------------------------------------------
// length 자리의 문자열을 받아서 'long'으로 변환한다. (ex: 00012300 ==> 12300)
long changeStringToLong(char * strLong, int length) 
{
	int multiply = 1;
	long result = 0;

	for (int i=length-1; i>=0; i--) {
		result += ((UINT)(strLong[i] - '0') * multiply);
		multiply *= 10;
	}

	return result;
}

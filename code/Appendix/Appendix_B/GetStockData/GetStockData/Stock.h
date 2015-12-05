///////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                               //
// Date       : Mar 17, 2013                             //
// eMail      : kimsehoon@hotmail.com                    //
// Description: This Program Gets Stock Data from ETRADE //
///////////////////////////////////////////////////////////

#pragma once

#define MAX_DATA				600			//최대 데이터 크기
#define MAX_JONGMOK				2500		//최데 전체 종목수

//-----------------------------------------------------------------------------------------------
// 하나의 데이터를 저장하기 위한 구조체
struct Data {
	CString date;		//날짜
	LONG startVal;		//시가
	LONG highVal;		//고가
	LONG lowVal;		//저가
	LONG lastVal;		//종가
	LONG vol;			//거래량
};

//-----------------------------------------------------------------------------------------------
// 하나의 종목 데이터를 저장하기 위한 구조체
struct Company {
	CString strJongMok, strName;				//종목코드와 종목명을 저장한다.
	int quantity;								//해당 종목에 저장되는 데이터의 갯수를 저장한다.
	Data data[MAX_DATA];						//데이터를 저장하기 위한 구조체 배열
	
	CString keyToRecv;							//xing으로부터 데이터를 연속으로 받을 때 키를 저장한다.
};

//-----------------------------------------------------------------------------------------------
// 모든 데이터를 저장하기 위한 구조체로서 모든 종목의 배열을 포함한다.
struct AllCompany {
	int quantity;								//전체 종목 수를 저장한다.
	Company company[MAX_JONGMOK];				//종목들을 저장하기 위한 구조체 배열
};

//-----------------------------------------------------------------------------------------------
// 데이터를 저장하고 화일에 출력하기 위한 클래스
class CStock
{
public:
	AllCompany allCompany;						//모든 데이터를 저장하기 위한 구조체

	CStock(void);
	~CStock(void);

	void StoreDataInOneFile(CString fileName);	//화일에 데이터를 저장하기 위한 함수
};

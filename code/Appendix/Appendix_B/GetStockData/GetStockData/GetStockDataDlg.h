///////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                               //
// Date       : Mar 17, 2013                             //
// eMail      : kimsehoon@hotmail.com                    //
// Description: This Program Gets Stock Data from ETRADE //
///////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"

#include "Stock.h"

class CGetStockDataDlg : public CDialog
{

public:

	CStock *stock;							//주식 data를 저장하고 파일에 저장하기 위한 클래스

	int indexCompany;						//현재 종목을 알기위한 index
	int flagNext;							//t8413은 연속적으로 데이터를 받기 때문에 받는 순서를 알기위해 사용되는 flag
	char dataType[2];						//dataType (2:일 3:주 4:월)
	char quantityData[5];					//한 종목당 받기위한 데이터 갯수 --> 최대 500개까지만 허용된다.
	CString date;							//현재 날자를 알기위한 date 변수 (YYYYMMDD)

	CComboBox m_datatype;					//화면의 데이터 타입을 위한 멤버변수(일,주,월)
	CComboBox m_server;						//화면의 접속서버를 구분하기 위한 멤버변수 (hts, demo)
	CString m_id;							//화면의 사용자 id를 가져오기 위한 멤버변수
	CString m_password;						//화면의 비밀번호를 가져오기 위한 멤버변수
	CString m_password2;					//화면의 공인인증 비밀번호를 가져오기 위한 멤버변수
	CString m_message;						//화면에 메세지를 출력하기 위한 멤버변수
	CString m_output;						//화면의 출력 파일명을 가져오기 위한 멤버변수
	CString m_jongmokcode;					//화면의 종목코드를 가져오기 위한 멤버변수
	BOOL m_radio;							//화면의 종목 구분을 위한 멤버변수 (전체종목/종목선택)
	int m_quantity;							//화면의 데이터 갯수를 가져오기 위한 멤버변수 --> 최대 500개까지만 허용된다.

	void StoreDataInFile();					//가져온 데이터를 파일에 저장하기 위한 함수
	VOID EntryOrderError(INT nRqID);		//에러메시지 처리하기 위한 함수 "서경자씨 프로그램에서 가져왔음"

	afx_msg void OnBnClickedBtnConnect();	//서버 접속을 위한 '접속' 버튼의 이벤트 함수
	afx_msg void OnBnClickedBtnDisconnect();//서버 연결을 끊고 프로그램을 종료하는 '종료' 버튼의 이벤트 함수
	afx_msg void OnBnClickedBtnGetdata();	//ETRADE로부터 데이터를 가져오는 '가져오기' 버튼의 이벤트 함수

	CGetStockDataDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_GETSTOCKDATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg	LRESULT	OnXMReceiveData( WPARAM wParam, LPARAM lParam );	//Xing으로부터 메시지를 받기위한 callback 함수 ===> 가장 중요함.
};

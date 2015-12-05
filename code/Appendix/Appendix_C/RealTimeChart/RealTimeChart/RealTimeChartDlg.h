/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"

#include "Option.h"
#include "Graph.h"

class CRealTimeChartDlg : public CDialog
{

public:

	COption *option;			//데이터를 저장하고 계산하기 위한 클래스
	CGraph *graph;				//데이터를 화면에 그리기 위한 클래스

	CString date;				//오늘 날자
	bool flagPaint;				//화면에 그래프를 그릴 때 사용하는 flag
	CDC* pDC;					//화면에 실시간 그래프를 그리기 위한 CDC

	CString monthForData;		//데이터를 받기 위한 월물 (YYYYMM)

	int countTimer;				//OnTimer가 실행될 때마다 1 증가한다.
	int flagT2301;				//t2301을 받을 때마다 1 증가한다.
	int flagT8415;				//t8415를 받을 때마다 1 증가한다.

	CStatic m_title;			//제목을 쓰기 위한 멤버변수
	CString m_id;				//id를 저장하기 위한 멤버변수
	CString m_password;			//비밀번호를 저장하기 위한 멤버변수
	CString m_password2;		//공인인증 번호를 저장하기 위한 멤버변수
	CComboBox m_server;			//서버 List를 위한 Control 멤버변수
	CComboBox m_callList;		//Call List를 위한 Control 멤버변수
	CComboBox m_putList;		//Put List를 위한 Control 멤버변수
	float m_startCall;			//Call의 시작가를 출력하기 위한 멤버변수
	float m_curCall;			//Call의 현재가를 출력하기 위한 멤버변수
	float m_startPut;			//Put의 시작가를 출력하기 위한 멤버변수
	float m_curPut;				//Put의 현재가를 출력하기 위한 멤버변수
	BOOL m_flagBollinger;		//Bollinger Band를 화면에 그리기 위한 멤버변수
	UINT m_bollingerAvg;		//Bollinger Band 평균 데이터를 저장하기 위한 멤버변수
	float m_bollingerTime;		//Bollinger Band 배수 데이터를 저장하기 위한 멤버변수
	BOOL m_flagMACD;			//MACD를 화면에 그리기 위한 멤버변수
	UINT m_macd1;				//MACD 첫번째 데이터를 저장하기 위한 멤버변수
	UINT m_macd2;				//MACD 두번째 데이터를 저장하기 위한 멤버변수
	UINT m_macd3;				//MACD 세번째 데이터를 저장하기 위한 멤버변수
	BOOL m_radioMACD;			//MACD에서 Call or Put을 선택하기 위한 Radio 멤버변수
	CString m_message;			//화면에 Message를 출력하기 위한 멤버변수
	BOOL m_drawBollinger;		//Chart 영역에 Bollinger Band를 그리기 위한 멤버변수
	
	//--------- 아래의 함수들은 .cpp 파일에 설명되었다. ----------
	void DrawEmptyScreen();
	void DrawGraph();
	void DrawRealData();
	void ExecuteRealTime();
	void AdviseRealData();
	void UnadviseRealData();
	void InitalizeComboBox();

	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedRadioMacdcall();
	afx_msg void OnBnClickedRadioMacdput();
	afx_msg void OnEnChangeEditBollingeravg();
	afx_msg void OnEnChangeEditBollingertime();
	afx_msg void OnEnChangeEditMacd1();
	afx_msg void OnEnChangeEditMacd2();
	afx_msg void OnEnChangeEditMacd3();
	afx_msg void OnBnClickedCheckBollinger();
	afx_msg void OnBnClickedCheckDraw();
	afx_msg void OnBnClickedCheckMacd();
	afx_msg void OnTimer(UINT nIDEvent);

	VOID EntryOrderError(INT nRqID);		//에러메시지 처리하기 위한 함수 "서경자씨 프로그램에서 가져왔음"

	CRealTimeChartDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_REALTIMECHART_DIALOG };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg	LRESULT	OnXMReceiveData( WPARAM wParam, LPARAM lParam );	//Xing으로부터 TR 데이터를 받기위한 callback 함수 ===> 가장 중요함.
	afx_msg	LRESULT OnXMReceiveRealData( WPARAM wParam, LPARAM lParam );//Xing으로부터 Real 데이터를 받기위한 callback 함수 ===> 가장 중요함.

	afx_msg	LRESULT	OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnDisconnect( WPARAM wParam, LPARAM lParam );
};

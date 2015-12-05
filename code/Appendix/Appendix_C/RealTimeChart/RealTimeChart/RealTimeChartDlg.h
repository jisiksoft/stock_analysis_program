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

	COption *option;			//�����͸� �����ϰ� ����ϱ� ���� Ŭ����
	CGraph *graph;				//�����͸� ȭ�鿡 �׸��� ���� Ŭ����

	CString date;				//���� ����
	bool flagPaint;				//ȭ�鿡 �׷����� �׸� �� ����ϴ� flag
	CDC* pDC;					//ȭ�鿡 �ǽð� �׷����� �׸��� ���� CDC

	CString monthForData;		//�����͸� �ޱ� ���� ���� (YYYYMM)

	int countTimer;				//OnTimer�� ����� ������ 1 �����Ѵ�.
	int flagT2301;				//t2301�� ���� ������ 1 �����Ѵ�.
	int flagT8415;				//t8415�� ���� ������ 1 �����Ѵ�.

	CStatic m_title;			//������ ���� ���� �������
	CString m_id;				//id�� �����ϱ� ���� �������
	CString m_password;			//��й�ȣ�� �����ϱ� ���� �������
	CString m_password2;		//�������� ��ȣ�� �����ϱ� ���� �������
	CComboBox m_server;			//���� List�� ���� Control �������
	CComboBox m_callList;		//Call List�� ���� Control �������
	CComboBox m_putList;		//Put List�� ���� Control �������
	float m_startCall;			//Call�� ���۰��� ����ϱ� ���� �������
	float m_curCall;			//Call�� ���簡�� ����ϱ� ���� �������
	float m_startPut;			//Put�� ���۰��� ����ϱ� ���� �������
	float m_curPut;				//Put�� ���簡�� ����ϱ� ���� �������
	BOOL m_flagBollinger;		//Bollinger Band�� ȭ�鿡 �׸��� ���� �������
	UINT m_bollingerAvg;		//Bollinger Band ��� �����͸� �����ϱ� ���� �������
	float m_bollingerTime;		//Bollinger Band ��� �����͸� �����ϱ� ���� �������
	BOOL m_flagMACD;			//MACD�� ȭ�鿡 �׸��� ���� �������
	UINT m_macd1;				//MACD ù��° �����͸� �����ϱ� ���� �������
	UINT m_macd2;				//MACD �ι�° �����͸� �����ϱ� ���� �������
	UINT m_macd3;				//MACD ����° �����͸� �����ϱ� ���� �������
	BOOL m_radioMACD;			//MACD���� Call or Put�� �����ϱ� ���� Radio �������
	CString m_message;			//ȭ�鿡 Message�� ����ϱ� ���� �������
	BOOL m_drawBollinger;		//Chart ������ Bollinger Band�� �׸��� ���� �������
	
	//--------- �Ʒ��� �Լ����� .cpp ���Ͽ� ����Ǿ���. ----------
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

	VOID EntryOrderError(INT nRqID);		//�����޽��� ó���ϱ� ���� �Լ� "�����ھ� ���α׷����� ��������"

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

	afx_msg	LRESULT	OnXMReceiveData( WPARAM wParam, LPARAM lParam );	//Xing���κ��� TR �����͸� �ޱ����� callback �Լ� ===> ���� �߿���.
	afx_msg	LRESULT OnXMReceiveRealData( WPARAM wParam, LPARAM lParam );//Xing���κ��� Real �����͸� �ޱ����� callback �Լ� ===> ���� �߿���.

	afx_msg	LRESULT	OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnDisconnect( WPARAM wParam, LPARAM lParam );
};

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

	CStock *stock;							//�ֽ� data�� �����ϰ� ���Ͽ� �����ϱ� ���� Ŭ����

	int indexCompany;						//���� ������ �˱����� index
	int flagNext;							//t8413�� ���������� �����͸� �ޱ� ������ �޴� ������ �˱����� ���Ǵ� flag
	char dataType[2];						//dataType (2:�� 3:�� 4:��)
	char quantityData[5];					//�� ����� �ޱ����� ������ ���� --> �ִ� 500�������� ���ȴ�.
	CString date;							//���� ���ڸ� �˱����� date ���� (YYYYMMDD)

	CComboBox m_datatype;					//ȭ���� ������ Ÿ���� ���� �������(��,��,��)
	CComboBox m_server;						//ȭ���� ���Ӽ����� �����ϱ� ���� ������� (hts, demo)
	CString m_id;							//ȭ���� ����� id�� �������� ���� �������
	CString m_password;						//ȭ���� ��й�ȣ�� �������� ���� �������
	CString m_password2;					//ȭ���� �������� ��й�ȣ�� �������� ���� �������
	CString m_message;						//ȭ�鿡 �޼����� ����ϱ� ���� �������
	CString m_output;						//ȭ���� ��� ���ϸ��� �������� ���� �������
	CString m_jongmokcode;					//ȭ���� �����ڵ带 �������� ���� �������
	BOOL m_radio;							//ȭ���� ���� ������ ���� ������� (��ü����/������)
	int m_quantity;							//ȭ���� ������ ������ �������� ���� ������� --> �ִ� 500�������� ���ȴ�.

	void StoreDataInFile();					//������ �����͸� ���Ͽ� �����ϱ� ���� �Լ�
	VOID EntryOrderError(INT nRqID);		//�����޽��� ó���ϱ� ���� �Լ� "�����ھ� ���α׷����� ��������"

	afx_msg void OnBnClickedBtnConnect();	//���� ������ ���� '����' ��ư�� �̺�Ʈ �Լ�
	afx_msg void OnBnClickedBtnDisconnect();//���� ������ ���� ���α׷��� �����ϴ� '����' ��ư�� �̺�Ʈ �Լ�
	afx_msg void OnBnClickedBtnGetdata();	//ETRADE�κ��� �����͸� �������� '��������' ��ư�� �̺�Ʈ �Լ�

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

	afx_msg	LRESULT	OnXMReceiveData( WPARAM wParam, LPARAM lParam );	//Xing���κ��� �޽����� �ޱ����� callback �Լ� ===> ���� �߿���.
};

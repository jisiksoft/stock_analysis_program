#include "Stock.h"

#pragma once

class CStockAnalysisDlg : public CDialog
{
private:
	CStock *stock;

public:
	CStockAnalysisDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_STOCKANALYSIS_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRun();
};

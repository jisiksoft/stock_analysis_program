

#pragma once

#include "Stock.h"
#include "Graph.h"

class CStockAnalysisDlg : public CDialog
{
private:
	CStock *stock;
	bool flagPaint;

public:
	CComboBox m_jongmok;

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
	void DrawGraph();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnCbnSelchangeComboJongmok();
};

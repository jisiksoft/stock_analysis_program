
// StockAnalysisDlg.stockp : 구현 파일
//

#include "stdafx.h"
#include "StockAnalysis.h"
#include "StockAnalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CStockAnalysisDlg::CStockAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockAnalysisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStockAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_JONGMOK, m_jongmok);
}

BEGIN_MESSAGE_MAP(CStockAnalysisDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RUN, &CStockAnalysisDlg::OnBnClickedBtnRun)
	ON_CBN_SELCHANGE(IDC_COMBO_JONGMOK, &CStockAnalysisDlg::OnCbnSelchangeComboJongmok)
END_MESSAGE_MAP()

BOOL CStockAnalysisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	stock = new CStock();
	flagPaint = false;

	return TRUE;
}

void CStockAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//------------------------------------------------------------------------------------------
void CStockAnalysisDlg::OnPaint()
{
	if (flagPaint) {
		DrawGraph();
	}

	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//------------------------------------------------------------------------------------------
HCURSOR CStockAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------------------------------------------------------------------
void CStockAnalysisDlg::DrawGraph()
{		
	int i;

	PointData *ptData= CGraph::GetPointData(stock->ptrCompany);

	CPaintDC dc(this);

	CBrush whiteBrush(RGB(255,255,255));
	RECT rect = {X_START_GRAPH, Y_START_GRAPH, 
						X_START_GRAPH+WIDTH_GRAPH, Y_START_GRAPH+HEIGHT_GRAPH};
	dc.FillRect(&rect, &whiteBrush);

	CPen bluePen, redPen, bluePenThick, redPenThick;
	bluePen.CreatePen(PS_SOLID, WIDTH_LINE, RGB(0,0,255));
	redPen.CreatePen(PS_SOLID, WIDTH_LINE, RGB(255,0,0));
	bluePenThick.CreatePen(PS_SOLID, WIDTH_THICK_LINE, RGB(0,0,255));
	redPenThick.CreatePen(PS_SOLID, WIDTH_THICK_LINE, RGB(255,0,0));

	for(i=0; i<stock->ptrCompany->quantity; i++) {
		if (ptData->startVal.point[i].Y > ptData->lastVal.point[i].Y) {
			dc.SelectObject(redPen);
		}	
		else {
			dc.SelectObject(bluePen);
		}
		dc.MoveTo(ptData->highVal.point[i].X, ptData->highVal.point[i].Y);
		dc.LineTo(ptData->lowVal.point[i].X, ptData->lowVal.point[i].Y);

		if (ptData->startVal.point[i].Y > ptData->lastVal.point[i].Y) {
			dc.SelectObject(redPenThick);
		}	
		else {
			dc.SelectObject(bluePenThick);
		}
		dc.MoveTo(ptData->startVal.point[i].X, ptData->startVal.point[i].Y);
		dc.LineTo(ptData->lastVal.point[i].X, ptData->lastVal.point[i].Y);
	}

	//------------------------ MACD ------------------------
	RECT rect2 = {X_START_GRAPH, Y_START_GRAPH+HEIGHT_GRAPH+GAP_GRAPH_INDICATOR, 
						X_START_GRAPH+WIDTH_GRAPH, Y_START_GRAPH+HEIGHT_GRAPH+GAP_GRAPH_INDICATOR+HEIGHT_INDICATOR};
	dc.FillRect(&rect2, &whiteBrush);

	int originY = Y_START_GRAPH + HEIGHT_GRAPH + GAP_GRAPH_INDICATOR + (int)(HEIGHT_INDICATOR/2);

	for(i=0; i<stock->ptrCompany->quantity; i++) {
		if (ptData->macdOscillator.point[i].Y < originY) {
			dc.SelectObject(redPenThick);
		}	
		else {
			dc.SelectObject(bluePenThick);
		}
		dc.MoveTo(ptData->macdOscillator.point[i].X, originY);
		dc.LineTo(ptData->macdOscillator.point[i].X, ptData->macdOscillator.point[i].Y);
	}

	//------------------------ Write Text on DC ------------------------
	int maxVal, minVal;

	maxVal = 0;

	int quantity = stock->ptrCompany->quantity;

	for(i=0; i<quantity; i++)
		if (stock->ptrCompany->data[i].highVal > maxVal)
			maxVal = stock->ptrCompany->data[i].highVal;

	minVal = maxVal;

	for(i=0; i<quantity; i++)
		if (stock->ptrCompany->data[i].lowVal < minVal)
			minVal = stock->ptrCompany->data[i].lowVal;

	CString str;
	str.Format("- %d원", maxVal);
	dc.TextOut(WIDTH_GRAPH+13, 5, str, str.GetLength());

	str.Format("- %d원", minVal);
	dc.TextOut(WIDTH_GRAPH+13, HEIGHT_GRAPH, str, str.GetLength());
}

//------------------------------------------------------------------------------------------
void CStockAnalysisDlg::OnBnClickedBtnRun()
{
	int i;

	stock->Run();

	m_jongmok.ResetContent();

	for (i=0; i<stock->selectedCompanies.quantity; i++) {
		m_jongmok.AddString(stock->selectedCompanies.companies[i]->strName);
	}

	m_jongmok.SetCurSel(0);

	UpdateData(FALSE);

	if (i == 0)
		return;

	stock->ptrCompany = stock->selectedCompanies.companies[0];

	flagPaint = true;

	RedrawWindow();
}

//------------------------------------------------------------------------------------------
void CStockAnalysisDlg::OnCbnSelchangeComboJongmok()
{
	int index = m_jongmok.GetCurSel();

	stock->ptrCompany = stock->selectedCompanies.companies[index];

	RedrawWindow();
}

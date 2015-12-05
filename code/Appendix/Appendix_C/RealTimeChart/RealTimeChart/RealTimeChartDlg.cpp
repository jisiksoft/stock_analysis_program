/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealTimeChart.h"
#include "RealTimeChartDlg.h"

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

//--------------------------------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

//--------------------------------------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------
CRealTimeChartDlg::CRealTimeChartDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRealTimeChartDlg::IDD, pParent)
, m_id(_T(""))
, m_password(_T(""))
, m_password2(_T(""))
, m_startCall(0)
, m_curCall(0)
, m_startPut(0)
, m_curPut(0)
, m_bollingerAvg(20)
, m_bollingerTime(2.0)
, m_message(_T(""))
, m_flagBollinger(TRUE)
, m_drawBollinger(TRUE)
, m_flagMACD(TRUE)
, m_macd1(12)
, m_macd2(26)
, m_macd3(9)
, m_radioMACD(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//--------------------------------------------------------------------------------------------------------------
void CRealTimeChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_PW, m_password);
	DDX_Text(pDX, IDC_EDIT_PW2, m_password2);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_server);
	DDX_Control(pDX, IDC_COMBO_CALL, m_callList);
	DDX_Control(pDX, IDC_COMBO_PUT, m_putList);
	DDX_Text(pDX, IDC_STATIC_STARTCALL, m_startCall);
	DDX_Text(pDX, IDC_STATIC_CURCALL, m_curCall);
	DDX_Text(pDX, IDC_STATIC_STARTPUT, m_startPut);
	DDX_Text(pDX, IDC_STATIC_CURPUT, m_curPut);
	DDX_Text(pDX, IDC_EDIT_BOLLINGERAVG, m_bollingerAvg);
	DDX_Text(pDX, IDC_EDIT_BOLLINGERTIME, m_bollingerTime);
	DDX_Check(pDX, IDC_CHECK_BOLLINGER, m_flagBollinger);
	DDX_Check(pDX, IDC_CHECK_DRAW, m_drawBollinger);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_title);
	DDX_Check(pDX, IDC_CHECK_MACD, m_flagMACD);
	DDX_Text(pDX, IDC_EDIT_MACD1, m_macd1);
	DDX_Text(pDX, IDC_EDIT_MACD2, m_macd2);
	DDX_Text(pDX, IDC_EDIT_MACD3, m_macd3);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_message);
	DDX_Radio(pDX, IDC_RADIO_MACDCALL, m_radioMACD);
}

//--------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CRealTimeChartDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData )
	ON_MESSAGE( WM_USER + XM_RECEIVE_REAL_DATA, OnXMReceiveRealData )
	ON_MESSAGE( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
	ON_MESSAGE( WM_USER + XM_DISCONNECT, OnDisconnect )
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CRealTimeChartDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CRealTimeChartDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_RUN, &CRealTimeChartDlg::OnBnClickedBtnRun)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CRealTimeChartDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_RADIO_MACDCALL, &CRealTimeChartDlg::OnBnClickedRadioMacdcall)
	ON_BN_CLICKED(IDC_RADIO_MACDPUT, &CRealTimeChartDlg::OnBnClickedRadioMacdput)
	ON_EN_CHANGE(IDC_EDIT_BOLLINGERAVG, &CRealTimeChartDlg::OnEnChangeEditBollingeravg)
	ON_EN_CHANGE(IDC_EDIT_BOLLINGERTIME, &CRealTimeChartDlg::OnEnChangeEditBollingertime)
	ON_EN_CHANGE(IDC_EDIT_MACD1, &CRealTimeChartDlg::OnEnChangeEditMacd1)
	ON_EN_CHANGE(IDC_EDIT_MACD2, &CRealTimeChartDlg::OnEnChangeEditMacd2)
	ON_EN_CHANGE(IDC_EDIT_MACD3, &CRealTimeChartDlg::OnEnChangeEditMacd3)
	ON_BN_CLICKED(IDC_CHECK_BOLLINGER, &CRealTimeChartDlg::OnBnClickedCheckBollinger)
	ON_BN_CLICKED(IDC_CHECK_DRAW, &CRealTimeChartDlg::OnBnClickedCheckDraw)
	ON_BN_CLICKED(IDC_CHECK_MACD, &CRealTimeChartDlg::OnBnClickedCheckMacd)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------
BOOL CRealTimeChartDlg::OnInitDialog()
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

	CFont fontTitle; //���� "Real-Time Chart"�� ���� ũ�� ����
	fontTitle.CreateFont(100,100,0,0,1,1,0,1,1,OUT_DEFAULT_PRECIS,0,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Arial" );
	m_title.SetFont(&fontTitle, TRUE);

	m_server.AddString(_T("hts.etrade.co.kr"));		//���Ӽ����� �޺��ڽ��� ����Ѵ�.
	m_server.AddString(_T("demo.etrade.co.kr"));
	m_server.SetCurSel(1);

	flagPaint = false;						//�׷����� �׸��� ���� flagPaine�� false�� �����Ͽ� ���α׷� ���۽� Chart�� �׸��� �ʴ´�.

	option = new COption();					//�����͸� �����ϱ� ���Ͽ� ���Ǵ� COption Ŭ������ �����Ѵ�.
	graph = new CGraph();					//�׷����� �׸��� ���Ͽ� ���Ǵ� CGraph Ŭ������ �����Ѵ�.

	CTime t = CTime::GetCurrentTime();		//���� �ð��� �����´�.
	date = t.Format("%Y%m%d");				//���� ���ڸ� �����´�. (YYYYMMDD)
	//date.Format(_T("20140410"));

	//////////////// 1. xingAPI �ʱ�ȭ ////////////////
	if( iXingAPI.Init() == FALSE ) {
		CString strPath;
		int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		strPath.ReleaseBuffer( nLen );
		int nFind = strPath.ReverseFind( '\\' );
		if( nFind > 0 ) strPath = strPath.Left( nFind );
		if( iXingAPI.Init( strPath ) == FALSE ) {
			MessageBox( (LPCTSTR)"xingAPI DLL�� Load �� �� �����ϴ�.");
		}
	}

	monthForData = option->GetMonthForData(); //���� ������ �ش� �ɼǹ��� �������� ���� Month �����͸� �����´�.

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------
void CRealTimeChartDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//--------------------------------------------------------------------------------------------------------------
void CRealTimeChartDlg::OnPaint()
{
	if (flagPaint) {
		DrawGraph(); //Chart�� �׸���.
	} else {
		DrawEmptyScreen(); //ó���� �� ȭ���� �׸���.
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

//--------------------------------------------------------------------------------------------------------------
HCURSOR CRealTimeChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//--------------------------------------------------------------------------------------------------------------
// ���α׷� ���۽� ó�� ��ȭ���� �׷��ش�.
void CRealTimeChartDlg::DrawEmptyScreen()
{
	CPaintDC dc(this);

	//�׷��� �׸��� ������ �Ͼ� �簢������ �׸���.
	CBrush whiteBrush(RGB(255,255,255));
	RECT rect = {X_START_GRAPH, Y_START_GRAPH, X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH};
	dc.FillRect(&rect, &whiteBrush);

	//���Ƿ����� ������ �Ͼ�� �簢������ �׷��ش�.
	RECT rect2 = {X_START_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH, 
		X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+Y_MAX_OSCILLATOR};
	dc.FillRect(&rect2, &whiteBrush);
}

//--------------------------------------------------------------------------------------------------------------
// ���α׷��� �����͸� ������ �� Chart�� �׷��ش�.
// �� �������� Chart�� ���� �׸���.
void CRealTimeChartDlg::DrawGraph()
{		

	int i;
	
	int quantity = graph->ptData.lineCall.quantity;	//�������� ������ quantity�� �����Ѵ�.
	if (quantity > 360)	//15�� ������ �����ʹ� �׸��� �ʴ´�. (360 = 6�ð� * 60��)
		quantity = 360;

	CPaintDC dc(this);

	//�׷��� �׸��� ������ �Ͼ� �簢������ �׸���.
	CBrush whiteBrush(RGB(255,255,255));
	RECT rect = {X_START_GRAPH, Y_START_GRAPH, X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH};
	dc.FillRect(&rect, &whiteBrush);

	RECT rect2 = {X_START_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH, 
		X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+Y_MAX_OSCILLATOR};
	dc.FillRect(&rect2, &whiteBrush);

	//�׸��� ������ ���� �����Ѵ�.
	CPen redPen, bluePen, blackPen, grayPen, bollingerCallPen, bollingerPutPen, macdRedPen, macdBluePen;
	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));				//���� ��
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));				//�Ķ� ��
	blackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));				//���� ��
	grayPen.CreatePen(PS_SOLID, 1, RGB(200,200,200));			//Chart�� ȸ����
	bollingerCallPen.CreatePen(PS_SOLID, 1, RGB(255,180,0));	//Call�� Bollinger Band ����� ��
	bollingerPutPen.CreatePen(PS_SOLID, 1, RGB(0,255,255));		//Put�� Bollinger Band �ϴû� ��
	macdRedPen.CreatePen(PS_SOLID, 2, RGB(255,200,200));		//MACD Signal�� ������ ��
	macdBluePen.CreatePen(PS_SOLID, 2, RGB(200,200,255));		//MACD Signal�� �Ķ��� ��

	/////////////////////////////////// Y-axis //////////////////////////////////
	CString str;

	dc.SelectObject(grayPen);
	for(i=0; i<graph->ptData.ptBaseY.quantity; i++) { //Y���� �������� ���� ȸ������ �׸���.
		dc.MoveTo(X_START_GRAPH, graph->ptData.ptBaseY.pointY[i]);
		dc.LineTo(X_START_GRAPH+X_MAX_SIZE_GRAPH, graph->ptData.ptBaseY.pointY[i]);

		str.Format("%0.1f", graph->ptData.ptBaseY.valueY[i]); //Y���� ������ ���� ������ ǥ���Ѵ�.
		dc.TextOut(15, graph->ptData.ptBaseY.pointY[i]-8, str, str.GetLength());
	}

	/////////////////////////////////// X-axis //////////////////////////////////
	dc.SelectObject(grayPen);
	for(i=1; i<6; i++) {	//X���� �������� ���� ȸ������ �׸���.
		dc.MoveTo(graph->ptData.ptBaseX.pointX[i], Y_START_GRAPH);
		dc.LineTo(graph->ptData.ptBaseX.pointX[i], Y_START_GRAPH + Y_MAX_SIZE_GRAPH);

		str.Format("%d", graph->ptData.ptBaseX.valueX[i]); //�ð� ���� ǥ���Ѵ�.
		dc.TextOut(graph->ptData.ptBaseX.pointX[i]-6, Y_START_GRAPH + Y_MAX_SIZE_GRAPH + 1, str, str.GetLength());
	}

	/////////////////////////////////// Data Lines //////////////////////////////////

	if (m_drawBollinger) { //Bollinger Band�� checkbox�� ���õǾ� ������ Bollinger Band ���� Chart ������ �׸���.
		//----- Chart ������ Call Bollinger Band �� �׸��� -----
		dc.SelectObject(bollingerCallPen);
		dc.MoveTo(graph->ptData.lineCall.ptBollingerTop[BOLLINGER_AVG-1].X, graph->ptData.lineCall.ptBollingerTop[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG; i<quantity; i++)
			dc.LineTo(graph->ptData.lineCall.ptBollingerTop[i].X, graph->ptData.lineCall.ptBollingerTop[i].Y);
		dc.MoveTo(graph->ptData.lineCall.ptBollingerMiddle[BOLLINGER_AVG-1].X, graph->ptData.lineCall.ptBollingerMiddle[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG; i<quantity; i++)
			dc.LineTo(graph->ptData.lineCall.ptBollingerMiddle[i].X, graph->ptData.lineCall.ptBollingerMiddle[i].Y);
		dc.MoveTo(graph->ptData.lineCall.ptBollingerBottom[BOLLINGER_AVG-1].X, graph->ptData.lineCall.ptBollingerBottom[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG; i<quantity; i++)
			dc.LineTo(graph->ptData.lineCall.ptBollingerBottom[i].X, graph->ptData.lineCall.ptBollingerBottom[i].Y);
		//----- Chart ������ Put Bollinger Band �� �׸��� -----
		dc.SelectObject(bollingerPutPen);
		dc.MoveTo(graph->ptData.linePut.ptBollingerTop[BOLLINGER_AVG-1].X, graph->ptData.linePut.ptBollingerTop[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG; i<quantity; i++)
			dc.LineTo(graph->ptData.linePut.ptBollingerTop[i].X, graph->ptData.linePut.ptBollingerTop[i].Y);
		dc.MoveTo(graph->ptData.linePut.ptBollingerMiddle[BOLLINGER_AVG-1].X, graph->ptData.linePut.ptBollingerMiddle[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG; i<quantity; i++)
			dc.LineTo(graph->ptData.linePut.ptBollingerMiddle[i].X, graph->ptData.linePut.ptBollingerMiddle[i].Y);
		dc.MoveTo(graph->ptData.linePut.ptBollingerBottom[BOLLINGER_AVG-1].X, graph->ptData.linePut.ptBollingerBottom[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG; i<quantity; i++)
			dc.LineTo(graph->ptData.linePut.ptBollingerBottom[i].X, graph->ptData.linePut.ptBollingerBottom[i].Y);
	}

	//----- Call ������ íƮ �׸��� -----
	dc.SelectObject(redPen);
	dc.MoveTo(X_START_GRAPH, graph->ptData.lineCall.ptLast[0].Y);
	for(i=1; i<quantity; i++) {
		dc.LineTo(graph->ptData.lineCall.ptLast[i].X, graph->ptData.lineCall.ptLast[i].Y);
	}
	//----- Put ������ íƮ �׸��� -----
	dc.SelectObject(bluePen);
	dc.MoveTo(X_START_GRAPH, graph->ptData.linePut.ptLast[0].Y);
	for(i=1; i<quantity; i++) {
		dc.LineTo(graph->ptData.linePut.ptLast[i].X, graph->ptData.linePut.ptLast[i].Y);
	}

	/////////////////////////////////// MACD �׸��� (���Ƿ����� ����) //////////////////////////////////

	if (m_flagMACD) { //MACD checkbox�� ���õǾ� ������ MACD�� �׸���.
		if (!m_radioMACD) {	//Call�� MACD Signal�� �׸���.

			for(i=0; i<quantity; i++) {

				if (graph->ptData.lineCall.ptMacdOscillator[i].Y < graph->originOscillatorY) //���Ƿ������� ��� ������ ���� ���� �� ���� �׸���.
					dc.SelectObject(macdRedPen);
				else //���Ƿ������� ��� ������ �Ʒ��� �Ķ� ���� �׸���.
					dc.SelectObject(macdBluePen);

				dc.MoveTo(graph->ptData.lineCall.ptMacdOscillator[i].X, graph->originOscillatorY);
				dc.LineTo(graph->ptData.lineCall.ptMacdOscillator[i].X, graph->ptData.lineCall.ptMacdOscillator[i].Y);
			}
		} else { //Put�� MACD Signal�� �׸���.

			for(i=0; i<quantity; i++) {

				if (graph->ptData.linePut.ptMacdOscillator[i].Y < graph->originOscillatorY) //���Ƿ������� ��� ������ ���� ���� �� ���� �׸���.
					dc.SelectObject(macdRedPen);
				else //���Ƿ������� ��� ������ �Ʒ��� �Ķ� ���� �׸���.
					dc.SelectObject(macdBluePen);

				dc.MoveTo(graph->ptData.linePut.ptMacdOscillator[i].X, graph->originOscillatorY);
				dc.LineTo(graph->ptData.linePut.ptMacdOscillator[i].X, graph->ptData.linePut.ptMacdOscillator[i].Y);
			}
		}
	}

	/////////////////////////////////// Bollinger Band (���Ƿ����� ����) //////////////////////////////////	

	dc.SelectObject(grayPen);
	//----- middle line -----
	int lineMiddle = Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+(int)(Y_MAX_OSCILLATOR/2);
	dc.MoveTo(X_START_GRAPH, lineMiddle);
	dc.LineTo(X_START_GRAPH+X_MAX_SIZE_GRAPH, lineMiddle);
	str.Format(" 0");
	dc.TextOut(20, lineMiddle-8, str, str.GetLength());
	//----- top line -----
	int lineTop = Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+(int)(Y_MAX_OSCILLATOR/2)-(int)((Y_MAX_OSCILLATOR/2) *RATE_COMPARE_LINE);
	dc.MoveTo(X_START_GRAPH, lineTop);
	dc.LineTo(X_START_GRAPH+X_MAX_SIZE_GRAPH, lineTop);
	str.Format(" 1");
	dc.TextOut(20, lineTop-8, str, str.GetLength());
	//----- bottom line -----
	int lineBottom = Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+(int)(Y_MAX_OSCILLATOR/2)+(int)((Y_MAX_OSCILLATOR/2) *RATE_COMPARE_LINE);
	dc.MoveTo(X_START_GRAPH, lineBottom);
	dc.LineTo(X_START_GRAPH+X_MAX_SIZE_GRAPH, lineBottom);
	str.Format("-1");
	dc.TextOut(20, lineBottom-8, str, str.GetLength());

	if (m_flagBollinger) { //Bollinger Band�� checkbox�� ���õǾ� ������ Bollinger Band�� �׸���.

		//----- Call Bollinger Band Oscillaot �׸��� -----
		dc.SelectObject(redPen);
		dc.MoveTo(graph->ptData.lineCall.ptBollingerOscillator[BOLLINGER_AVG-1].X, graph->ptData.lineCall.ptBollingerOscillator[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG-1; i<quantity; i++) {
			dc.LineTo(graph->ptData.lineCall.ptBollingerOscillator[i].X, graph->ptData.lineCall.ptBollingerOscillator[i].Y);
		}

		//----- Put Bollinger Band Oscillaot �׸��� -----
		dc.SelectObject(bluePen);
		dc.MoveTo(graph->ptData.linePut.ptBollingerOscillator[BOLLINGER_AVG-1].X, graph->ptData.linePut.ptBollingerOscillator[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG-1; i<quantity; i++) {
			dc.LineTo(graph->ptData.linePut.ptBollingerOscillator[i].X, graph->ptData.linePut.ptBollingerOscillator[i].Y);
		}
	}

	//----- ���Ŀ� �ǽð� �׷����� �׸��� ���Ͽ� �������� ���� ��ġ�� �����Ѵ�. -----
	int index = graph->ptData.lineCall.quantity - 1;
	graph->ptDrawnLast.x = graph->ptData.lineCall.ptLast[index].X;
	graph->ptDrawnLast.ptDataCall = graph->ptData.lineCall.ptLast[index].Y;
	graph->ptDrawnLast.ptDataPut = graph->ptData.linePut.ptLast[index].Y;
	graph->ptDrawnLast.ptBollingerCall = graph->ptData.lineCall.ptBollingerOscillator[index].Y;
	graph->ptDrawnLast.ptBollingerPut = graph->ptData.linePut.ptBollingerOscillator[index].Y;
	graph->ptDrawnLast.ptMacdCall = graph->ptData.lineCall.ptMacdOscillator[index].Y;
	graph->ptDrawnLast.ptMacdPut = graph->ptData.linePut.ptMacdOscillator[index].Y;

	flagPaint = false;
}

//--------------------------------------------------------------------------------------------------------------
// �ǽð� �����͸� �׸��� ������ ���� �������� ���� ����� �ٽ� �׸���.
// ���� ���ο� ���� ù �������̸� ȭ�� ��ü�� �ٽ� �׷��ش�.
void CRealTimeChartDlg::DrawRealData()
{
	if (graph->ptData.lineCall.quantity > 360)
		return;

	int index = graph->ptData.lineCall.quantity - 1;

	option->CalculateRealData(); //�ǽð� �����͸� ����Ѵ�. ���⼭�� Bollinger Band�� MACD�� ���� ����Ѵ�.
	graph->CalculateRealDataPoint(&option->pairUnit); //�����͵��� ȭ�鿡���� ���� ��ġ�� ����Ѵ�.

	//�׸��� ������ ���� �����Ѵ�.
	CPen whitePen, whiteThickPen, redPen, bluePen, macdRedPen, macdBluePen;
	whitePen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	whiteThickPen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	macdRedPen.CreatePen(PS_SOLID, 2, RGB(255,200,200));
	macdBluePen.CreatePen(PS_SOLID, 2, RGB(200,200,255));

	if (graph->ptData.lineCall.ptLast[index].X > graph->ptDrawnLast.x) { //���ο� �ð��� ���̸� ȭ���� ���� �׸���.

		flagPaint = true;
		RedrawWindow();

	} else {

		//------------------- ���� �� ����� ------------------	
		pDC->SelectObject(whiteThickPen); //�Ͼ������ ���� ������ �ٽ� �׸��� ����� ���� �ȴ�.

		if (m_flagMACD) { //----- MACD ���� line ����� -----

			if (!m_radioMACD) {

				pDC->MoveTo(graph->ptDrawnLast.x, graph->originOscillatorY);
				pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptMacdCall);

			} else {

				pDC->MoveTo(graph->ptDrawnLast.x, graph->originOscillatorY);
				pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptMacdPut);
			}
		}

		if (m_flagBollinger) { //----- Bollinger ���� line ����� -----
			
			pDC->MoveTo(graph->ptData.lineCall.ptLast[index-1].X, graph->ptData.lineCall.ptBollingerOscillator[index-1].Y);	//���� �Ͼ������ �ٽ� �׸��鼭 �����.
			pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptBollingerCall);

			pDC->MoveTo(graph->ptData.linePut.ptLast[index-1].X, graph->ptData.linePut.ptBollingerOscillator[index-1].Y);
			pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptBollingerPut);
		}

		//----- Call ���� line ����� -----
		pDC->MoveTo(graph->ptData.lineCall.ptLast[index-1].X, graph->ptData.lineCall.ptLast[index-1].Y);	//���� �Ͼ������ �ٽñ׸��鼭 �����.
		pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptDataCall);

		//----- Put ���� line ����� -----
		pDC->MoveTo(graph->ptData.linePut.ptLast[index-1].X, graph->ptData.linePut.ptLast[index-1].Y);
		pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptDataPut);


		//----------------- MACD �� �׸��� (���Ƿ����� ����) -------------------
		if (m_flagMACD) {
			
			//----- MACD �� �׸��� -----
			if (!m_radioMACD) {

				if (graph->ptData.lineCall.ptMacdOscillator[index].Y < graph->originOscillatorY)
					pDC->SelectObject(macdRedPen);
				else
					pDC->SelectObject(macdBluePen);

				pDC->MoveTo(graph->ptData.lineCall.ptMacdOscillator[index].X, graph->originOscillatorY);
				pDC->LineTo(graph->ptData.lineCall.ptMacdOscillator[index].X, graph->ptData.lineCall.ptMacdOscillator[index].Y);
			
			} else {

				if (graph->ptData.linePut.ptMacdOscillator[index].Y < graph->originOscillatorY)
					pDC->SelectObject(macdRedPen);
				else
					pDC->SelectObject(macdBluePen);

				pDC->MoveTo(graph->ptData.linePut.ptMacdOscillator[index].X, graph->originOscillatorY);
				pDC->LineTo(graph->ptData.linePut.ptMacdOscillator[index].X, graph->ptData.linePut.ptMacdOscillator[index].Y);
			
			}
		}
		
		//----------------- Bollinger Band �� �׸��� (���Ƿ����� ����) -------------------
		if (m_flagBollinger) {
			
			pDC->SelectObject(redPen);
			pDC->MoveTo(graph->ptData.lineCall.ptBollingerOscillator[index-1].X, graph->ptData.lineCall.ptBollingerOscillator[index-1].Y); //���� �ٽ� �׷��ش�.
			pDC->LineTo(graph->ptData.lineCall.ptBollingerOscillator[index].X, graph->ptData.lineCall.ptBollingerOscillator[index].Y);
			
			pDC->SelectObject(bluePen);
			pDC->MoveTo(graph->ptData.linePut.ptBollingerOscillator[index-1].X, graph->ptData.linePut.ptBollingerOscillator[index-1].Y);
			pDC->LineTo(graph->ptData.linePut.ptBollingerOscillator[index].X, graph->ptData.linePut.ptBollingerOscillator[index].Y);
		}

		//----- Call ���ο� �� �׸��� -----
		pDC->SelectObject(redPen);
		pDC->MoveTo(graph->ptData.lineCall.ptLast[index-1].X, graph->ptData.lineCall.ptLast[index-1].Y);	//���� �ٽ� �׷��ش�.
		pDC->LineTo(graph->ptData.lineCall.ptLast[index].X, graph->ptData.lineCall.ptLast[index].Y);


		//----- Put ���ο� �� �׸��� -----
		pDC->SelectObject(bluePen);
		pDC->MoveTo(graph->ptData.linePut.ptLast[index-1].X, graph->ptData.linePut.ptLast[index-1].Y);
		pDC->LineTo(graph->ptData.linePut.ptLast[index].X, graph->ptData.linePut.ptLast[index].Y);

	}

	//----- ���Ŀ� �ǽð� �׷����� �׸��� ���Ͽ� �������� ���� ��ġ�� �����Ѵ�. -----
	graph->ptDrawnLast.x = graph->ptData.lineCall.ptLast[index].X;
	graph->ptDrawnLast.ptDataCall = graph->ptData.lineCall.ptLast[index].Y;
	graph->ptDrawnLast.ptDataPut = graph->ptData.linePut.ptLast[index].Y;
	graph->ptDrawnLast.ptBollingerCall = graph->ptData.lineCall.ptBollingerOscillator[index].Y;
	graph->ptDrawnLast.ptBollingerPut = graph->ptData.linePut.ptBollingerOscillator[index].Y;
	graph->ptDrawnLast.ptMacdCall = graph->ptData.lineCall.ptMacdOscillator[index].Y;
	graph->ptDrawnLast.ptMacdPut = graph->ptData.linePut.ptMacdOscillator[index].Y;

	// ȭ�鿡 ���� ���� ���� ������Ʈ�Ѵ�.
	m_curCall = option->pairUnit.call->data[index].lastVal;
	m_curPut = option->pairUnit.put->data[index].lastVal;
	UpdateData(FALSE);

}

//--------------------------------------------------------------------------------------------------------------
// �ɼ��� Call/Put�� ComboBox List�� �־��ְ� STANDARD_VALUE(1.3)�� ������ Call�� Put�� �����Ͽ� ���´�.
// ���õ� �� ���� Call�� Put�� COption Ŭ������ pairUnit�� ������ �ȴ�.
void CRealTimeChartDlg::InitalizeComboBox() 
{
	int i;
	float min, valToCompare;

	//Call �޺��ڽ��� �����ڵ带 �ִ´�.
	for (i=0; i<option->allUnit.quantityCall; i++)
		m_callList.AddString(option->allUnit.unitCall[i].code);

	//Put �޺��ڽ��� �����ڵ带 �ִ´�.
	for (i=0; i<option->allUnit.quantityCall; i++)
		m_putList.AddString(option->allUnit.unitPut[i].code);

	min = 100; //�ּҰ��� �ʱ�ȭ�Ѵ�.

	for (i=0; i<option->allUnit.quantityCall; i++) { //1.3�� ������ �ּҰ��� �����Ѵ�.
		if (option->allUnit.unitCall[i].openVal > STANDARD_VALUE) //1.3���� ���̸� ����Ͽ� valToCompare�� �ִ´�.
			valToCompare = option->allUnit.unitCall[i].openVal - (float)STANDARD_VALUE;
		else
			valToCompare = (float)STANDARD_VALUE - option->allUnit.unitCall[i].openVal;

		if (valToCompare < min) { //1.3�� ������ Call�� �޺��ڽ����� �������� pairUnit�� ��´�.
			option->pairUnit.call = &option->allUnit.unitCall[i];
			min = valToCompare;
			m_callList.SetCurSel(i);
		}
	}

	min = 100; //�ּҰ��� �ʱ�ȭ�Ѵ�.

	for (i=0; i<option->allUnit.quantityPut; i++) { //1.3�� ������ �ּҰ��� �����Ѵ�.
		if (option->allUnit.unitPut[i].openVal > STANDARD_VALUE) //1.3���� ���̸� ����Ͽ� valToCompare�� �ִ´�.
			valToCompare = option->allUnit.unitPut[i].openVal - (float)STANDARD_VALUE;
		else
			valToCompare = (float)STANDARD_VALUE - option->allUnit.unitPut[i].openVal;

		if (valToCompare < min) { //1.3�� ������ Put�� �޺��ڽ����� �������� pairUnit�� ��´�.
			option->pairUnit.put = &option->allUnit.unitPut[i];
			min = valToCompare;
			m_putList.SetCurSel(i);
		}
	}

	UpdateData(FALSE);	//����� ������ ȭ�鿡 ������Ʈ�Ѵ�.

	ExecuteRealTime();
}

//--------------------------------------------------------------------------------------------------------------
// Call�� Put�� �޺��ڽ��� ���õ� ������� �� �����͸� ��û�Ѵ�.
void CRealTimeChartDlg::ExecuteRealTime() 
{
	//������ �ǽð� �����͸� �޴� ���� ������ ��û�� ����Ѵ�.(UnadviseRealData)
	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.call->code, 0 ) ); //Call �ǽð� ������ ����
	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.put->code, 0 ) ); //Put �ǽð� ������ ����

	UpdateData(TRUE);

	//�޺��ڽ����� �������� ���� Call�� Put�� ���ο� ������ �����Ѵ�.
	option->pairUnit.call = &option->allUnit.unitCall[m_callList.GetCurSel()];
	option->pairUnit.put = &option->allUnit.unitPut[m_putList.GetCurSel()];

	//���õ� Call�� Put�� ���۰��� ȭ�鿡 �����ش�.
	m_startCall = option->pairUnit.call->openVal;
	m_startPut = option->pairUnit.put->openVal;
	UpdateData(FALSE);

	flagT8415 = 0;
	t8415InBlock pckInBlock;
	//Call�� �к� �����͸� ��û�Ѵ�.
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	memcpy( pckInBlock.shcode, option->pairUnit.call->code, sizeof(pckInBlock.shcode));			//�ɼ� �����ڵ�
	memcpy( pckInBlock.ncnt, "0001", sizeof(pckInBlock.ncnt));									//1�� ������
	memcpy( pckInBlock.sdate, date, sizeof(pckInBlock.sdate));									//�������� ����
	memcpy( pckInBlock.edate, date, sizeof(pckInBlock.edate));									//�������� ����
	memcpy( pckInBlock.comp_yn, "N", sizeof(pckInBlock.comp_yn));								//�������� �ʴ´�.
	EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8415", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// ������ ��û

	Sleep(1100); //t8415�� ��û���� �ʴ� 1ȸ�̱� ������ 1.1���� Delay�� �ش�.

	//Put�� �к� �����͸� ��û�Ѵ�.
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	memcpy( pckInBlock.shcode, option->pairUnit.put->code, sizeof(pckInBlock.shcode));			//�ɼ� �����ڵ�
	memcpy( pckInBlock.ncnt, "0001", sizeof(pckInBlock.ncnt));									//1�� ������
	memcpy( pckInBlock.sdate, date, sizeof(pckInBlock.sdate));									//�������� ����
	memcpy( pckInBlock.edate, date, sizeof(pckInBlock.edate));									//�������� ����
	memcpy( pckInBlock.comp_yn, "N", sizeof(pckInBlock.comp_yn));								//�������� �ʴ´�.
	EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8415", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// ������ ��û
}

//--------------------------------------------------------------------------------------------------------------
// ������ ���õ� Call�� Put�� �ǽð� �����͸� ��û�ϰ�(AdviseRealData), �ǽð� �׷����� �׸��� ���� Ÿ�̸Ӹ� �����Ѵ�.
void CRealTimeChartDlg::AdviseRealData() 
{
	int index = option->pairUnit.put->quantity - 1;
	m_curCall = option->pairUnit.call->data[index].lastVal;
	m_curPut = option->pairUnit.put->data[index].lastVal;

	CTime t = CTime::GetCurrentTime();
	if (t.GetHour() >= 15 && t.GetMinute() > 3) { //15��3�� ������ �ð����� �ǽð� �����͸� ��û���� �ʴ´�.
		m_message.Format(_T("�ǽð� ������ �ð��� �ƴմϴ�."));
		UpdateData(FALSE);
		return;
	}

	EntryOrderError( iXingAPI.AdviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.call->code, 8) ); //Call �ǽð� ������ ��û
	EntryOrderError( iXingAPI.AdviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.put->code, 8 ) ); //Put �ǽð� ������ ��û

	pDC = GetDC(); //�ǽð� �׷����� �׸��� ���Ͽ� DC�� �����Ѵ�.

	countTimer = 0; //5�и��� �����͸� �ʱ�ȭ�ϱ� ���Ͽ� ���Ǵ� count�� 0���� �ʱ�ȭ�Ѵ�.
	SetTimer(1004, TIMER_INTERVAL * 1000, NULL); //Ÿ�̸Ӹ� �����Ų��. �� ���α׷����� TIMER_INTERVAL�� 2�μ� 2�ʸ��� OnTimer�� ����ȴ�.
	
	m_message.Format(_T("�ǽð� ������ ��û�߽��ϴ�."));
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------
// �ǽð� ������ ��û�� �����Ѵ�. (UnadviseRealData)
void CRealTimeChartDlg::UnadviseRealData() 
{
	//ReleaseDC(pDC);

	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.call->code, 0 ) ); //Call �ǽð� ������ ����
	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.put->code, 0 ) ); //Put �ǽð� ������ ����

	KillTimer(1004); //Timer�� �����Ų��.

	m_message.Format(_T("�ǽð� ������ �����߽��ϴ�."));
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------
// �ǽð� �����͸� �޴� Callback �Լ�
LRESULT CRealTimeChartDlg::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if(!strcmp( pRealPacket->szTrCode, "OC0")) { //KOSPI200 �ɼ�ü�� �ǽð� �����͸� ������ �����Ѵ�.

		LPOC0_OutBlock pOutBlock = (LPOC0_OutBlock)pRealPacket->pszData;

		UINT chetime = changeStringTimeToInt(pOutBlock->chetime);				//���� ������ �ð��� �����´�.
		CString optcode((char*)pOutBlock->optcode, sizeof(pOutBlock->optcode));	//�ɼ� �ڵ带 �����´�.

		if (chetime > 1500) { //15�� ������ �����ʹ� ���� �ʰ�, �ǽð� �����͸� �����Ѵ�.
			UnadviseRealData();
			return 0L;
		}

		m_message.Format(_T("%d��%02d�� �����͸� �޴� ��..."), chetime/100, chetime%100);
		UpdateData(FALSE);

		if (!option->pairUnit.call->code.Compare(optcode)) { //���� �����Ͱ� Call ���������� �����ڵ带 ���Ѵ�.

			//����ü�� ����� ������ �к� ������ �ð��� ���� ������ �ð��� ���Ѵ�.
			int timeGap = getTimeGap(chetime, option->pairUnit.call->data[option->pairUnit.call->quantity-1].time);
			float price = changeStringToFloat(pOutBlock->price); //������ �Ǽ��� �ٲپ��ش�.

			if (timeGap == 0) { //���� �����Ͱ� �к� �����ͷ� ����� ���� �ð��� ���� ��� => �ǽð� ��ȭ�� �׷��ش�.

				option->pairUnit.call->data[option->pairUnit.call->quantity - 1].lastVal = price;

			} else if (timeGap == 1) { //���� �����Ͱ� �к� �����ͷ� ����� ������ 1�� ���� �������� ��� => ���� �߰��Ѵ�.

				option->AddNewEntryCall(chetime, price);

			} else { //���� �����Ͱ� �к� �����ͷ� ����� ������ 2�� �̻� ���̰� ���� ��� => �к������͸� ���� �޴´�.

				ExecuteRealTime();
			}

		} else if (!option->pairUnit.put->code.Compare(optcode)) { //���� �����Ͱ� Put �������� ���...

			//����ü�� ����� ������ �к� ������ �ð��� ���� ������ �ð��� ���Ѵ�.
			UINT timeGap = getTimeGap(chetime, option->pairUnit.put->data[option->pairUnit.put->quantity-1].time); 
			float price = changeStringToFloat(pOutBlock->price);

			if (timeGap == 0) { //���� �����Ͱ� �к� �����ͷ� ����� ���� �ð��� ���� ��� => �ǽð� ��ȭ�� �׷��ش�.

				option->pairUnit.put->data[option->pairUnit.put->quantity - 1].lastVal = price;

			} else if (timeGap == 1) { //���� �����Ͱ� �к� �����ͷ� ����� ������ 1�� ���� �������� ��� => ���� �߰��Ѵ�.

				option->AddNewEntryPut(chetime, price);

			} else { //���� �����Ͱ� �к� �����ͷ� ����� ������ 2�� �̻� ���̰� ���� ��� => �к������͸� ���� �޴´�.

				ExecuteRealTime();
			}
		}
	}

	return 0L;
}

//--------------------------------------------------------------------------------------------------------------
// Xing���κ��� ȣ��Ǵ� Callback �Լ��μ� �����͸� �޾��� �� ����Ǹ�, �޸𸮿� ����Ǿ� ���޹��� �����͵��� ó���Ѵ�.
LRESULT CRealTimeChartDlg::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{

	int i;

	if( wParam == REQUEST_DATA ) {

		LPRECV_PACKET lpRecvPacket = (LPRECV_PACKET)lParam;

		//------------ t2301 �����ʹ� '�ɼ� ������' �����͸� �������� ���� ���̸� ���α׷� ���۽� ȣ��ȴ�. -------------
		// Call�� Put�� List�� �����ͼ� COption Ŭ������ ����ü�� �̸��� �־��ش�.
		if(strcmp( lpRecvPacket->szTrCode, "t2301") == 0) {

			int count;

			if (flagT2301 == 0) {

			} if (flagT2301 == 1) { //�ι�° �޴� Block���� Call �����Ͱ� �ִ�.

				count = (int)( lpRecvPacket->nDataLength / sizeof(t2301OutBlock1));	

				LPt2301OutBlock1 pOutBlock1;

				for (i=0; i<count; i++) {
					pOutBlock1 = (LPt2301OutBlock1)(lpRecvPacket->lpData + (sizeof(t2301OutBlock1) * i));

					//Call ����ü�� �����͵��� �־ �ʱ�ȭ�Ѵ�.
					CString optcode((char*)pOutBlock1->optcode, sizeof(pOutBlock1->optcode) );
					option->allUnit.unitCall[option->allUnit.quantityCall].code = optcode;
					option->allUnit.unitCall[option->allUnit.quantityCall].openVal = changeStringToFloat(pOutBlock1->open);
					option->allUnit.quantityCall += 1;
				}

			} else if (flagT2301 == 2) { //����° �޴� Block���� Put �����Ͱ� �ִ�.

				count = (int)(lpRecvPacket->nDataLength / sizeof(t2301OutBlock2));	
				LPt2301OutBlock2 pOutBlock2;

				for (i=0; i<count; i++) {
					pOutBlock2 = (LPt2301OutBlock2)(lpRecvPacket->lpData + (sizeof(t2301OutBlock2) * i));

					//Put ����ü�� �����͵��� �־ �ʱ�ȭ�Ѵ�.
					CString optcode( (char*)pOutBlock2->optcode, sizeof(pOutBlock2->optcode) );
					option->allUnit.unitPut[option->allUnit.quantityPut].code = optcode;
					option->allUnit.unitPut[option->allUnit.quantityPut].openVal = changeStringToFloat(pOutBlock2->open);
					option->allUnit.quantityPut += 1;
				}

				InitalizeComboBox(); 	
			}

			flagT2301 += 1;

		//------------ t8415 �����ʹ� ���α׷� ���۽� �� �����͸� �����ͼ� ó�� �����͸� �׷��ֱ� ���ؼ� ���ȴ�. ------------
		} else if(strcmp( lpRecvPacket->szTrCode, "t8415") == 0) {

			int count;
			Unit *unit;

			//t8415�� �ι� ��û�Ǿ���, �ռ� ���� Call �������̰�, �ڿ����� Put �������̴�.
			if (flagT8415 == 0) { //ù��° �޴� ���� �����Ͱ� �������� Ȯ���ϰ� �ȴ�.

				LPt8415OutBlock pOutBlock = (LPt8415OutBlock)(lpRecvPacket->lpData);

				CString diclose( (char*)pOutBlock->diclose, sizeof(pOutBlock->diclose) );
				if(diclose.Compare(_T("000000")) == 0) {
					m_message.Format(_T("������ �����Ͱ� �����ϴ�."));	//�ɼ� �����Ͱ� ������ ����Ѵ�.
					UpdateData(FALSE);
				}

			} else if (flagT8415 == 1) { //Call �� �����͸� �޾Ƽ� ����ü�� �����Ѵ�.

				unit = option->pairUnit.call;

				count = (int)(lpRecvPacket->nDataLength / sizeof(t8415OutBlock1)); //Block ������ ����Ѵ�.
				LPt8415OutBlock1 pOutBlock1;

				for (i=0; i<count; i++) { //Call �� �����͸� ����ü�� ���������� �ִ´�.
					pOutBlock1 = (LPt8415OutBlock1)(lpRecvPacket->lpData + (sizeof(t8415OutBlock1) * i));

					unit->data[i].time = changeStringTimeToInt(pOutBlock1->time);
					unit->data[i].startVal = changeStringToFloat(pOutBlock1->open);
					unit->data[i].highVal = changeStringToFloat(pOutBlock1->high);
					unit->data[i].lowVal = changeStringToFloat(pOutBlock1->low);
					unit->data[i].lastVal = changeStringToFloat(pOutBlock1->close);
				}

				unit->quantity = count;

			} else if (flagT8415 == 2) {


			} else if (flagT8415 == 3) { //Put �� �����͸� �޾Ƽ� ����ü�� �����Ѵ�.

				unit = option->pairUnit.put;

				count = (int)(lpRecvPacket->nDataLength / sizeof(t8415OutBlock1)); //Block ������ ����Ѵ�.
				LPt8415OutBlock1 pOutBlock1;

				for (i=0; i<count; i++) { //Put �� �����͸� ����ü�� ���������� �ִ´�.
					pOutBlock1 = (LPt8415OutBlock1)(lpRecvPacket->lpData + (sizeof(t8415OutBlock1) * i));

					unit->data[i].time = changeStringTimeToInt(pOutBlock1->time);
					unit->data[i].startVal = changeStringToFloat(pOutBlock1->open);
					unit->data[i].highVal = changeStringToFloat(pOutBlock1->high);
					unit->data[i].lowVal = changeStringToFloat(pOutBlock1->low);
					unit->data[i].lastVal = changeStringToFloat(pOutBlock1->close);
				}
				unit->quantity = count;

				//Call�� Put�� �� �����͸� �޾ұ� ������ ������ǥ�� ������ ����ϰ� �׷����� �׷��� ��, �ǽð� �����͸� ��û�Ѵ�.
				option->CalculateBollinger(); //Bollinger Band�� ����Ѵ�.
				option->CalculateMACD(); //MACD�� ����Ѵ�.
				graph->CalculateAllPointData(&option->pairUnit); //ȭ�鿡 �׸��� ���� ������ ����Ѵ�.

				flagPaint = true;
				RedrawWindow(); //ȭ���� �׸���.

				AdviseRealData(); //�ǽð� �����͸� ��û�Ѵ�.
			}

			flagT8415 += 1;
		}
	} else if( wParam == MESSAGE_DATA ) {// �޽����� ����
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		iXingAPI.ReleaseMessageData( lParam );
	} else if( wParam == SYSTEM_ERROR_DATA ) {// System Error�� ����
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		::AfxMessageBox(strMsg);
		iXingAPI.ReleaseMessageData( lParam );
	} else if( wParam == RELEASE_DATA )	{ // Release Data�� ����
		iXingAPI.ReleaseRequestData( (int)lParam );
	}

	return 0L;
}

//--------------------------------------------------------------------------------------------------------------
LRESULT CRealTimeChartDlg::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	iXingAPI.ReleaseRequestData( (int)lParam );
	return 0L;
}

//--------------------------------------------------------------------------------------------------------------
LRESULT CRealTimeChartDlg::OnDisconnect( WPARAM wParam, LPARAM lParam )
{
	::AfxMessageBox(_T("�������� ������ ����Ǿ����ϴ�."));
	return 0L;
}

//--------------------------------------------------------------------------------------------------------------
// ���� �޽����� ����Ѵ�.
VOID CRealTimeChartDlg::EntryOrderError(INT nRqID)
{
	if( nRqID < 0 )	{
		CString str;
		str.Format(_T("[%d] %s"), nRqID, iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(str);
	}
}

//--------------------------------------------------------------------------------------------------------------
// ������ �����ϰ� �α����� �Ѵ�.
void CRealTimeChartDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE); //ȭ�鿡�� �Էµ� ���� ��������� ����

	if (iXingAPI.IsConnected()) {// �̹� ���ӵǾ� ������ ������ �����Ѵ�.
		iXingAPI.Disconnect();
	}

	m_message.Format(_T("���� ���� ��..."));
	UpdateData(FALSE);

	/////////////// 2. ���� ���� ////////////////
	BOOL bResult;
	if (m_server.GetCurSel() == 0) {//�Ǽ������..
		bResult = iXingAPI.Connect( GetSafeHwnd(),"hts.etrade.co.kr",20001,	WM_USER, 3000, 512 );
	} else {//���Ǽ������
		bResult = iXingAPI.Connect( GetSafeHwnd(),"demo.etrade.co.kr",20001, WM_USER, 3000, 512 );
	}
	if( bResult == FALSE )	{// ���ӽ��� ó��
		CString strMsg = iXingAPI.GetErrorMessage( iXingAPI.GetLastError());
		::AfxMessageBox(_T("�������� ���� : ") + strMsg);
	}

	m_message.Format(_T("������ ���ӵǾ����ϴ�."));
	UpdateData(FALSE);

	//////////////// 3. �α��� ////////////////
	if( FALSE == iXingAPI.Login( GetSafeHwnd(), m_id, m_password, m_password2, m_server.GetCurSel(), TRUE ) )	{// �α���
		CString strMsg = iXingAPI.GetErrorMessage( iXingAPI.GetLastError() );
		::AfxMessageBox(_T("�α��� ���� : ") + strMsg);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �α� �ƿ��� �ϰ� ���� ������ ������ ��, ���α׷��� �ݴ´�.
void CRealTimeChartDlg::OnBnClickedBtnClose()
{
	KillTimer(1004);

	//////////////// 4. �α׾ƿ� ////////////////
	if( FALSE == iXingAPI.Logout( GetSafeHwnd() ) )	{// �α���
		CString strMsg = iXingAPI.GetErrorMessage( iXingAPI.GetLastError() );
		::AfxMessageBox(_T("�α׾ƿ� ���� : ") + strMsg);
	}

	m_message.Format(_T("�α׾ƿ� �Ǿ����ϴ�."));	//�α׾ƿ� �޽��� ���
	UpdateData(FALSE);

	//////////////// 5. ���� ���� ���� ////////////////
	iXingAPI.Disconnect();

	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

//--------------------------------------------------------------------------------------------------------------
// '�����ϱ�' ��ư�� �������� �� ����Ǵ� �Լ��μ� ���α׷��� �����Ѵ�.
void CRealTimeChartDlg::OnBnClickedBtnRun()
{	
	KillTimer(1004);

	UpdateData(TRUE);

	BOLLINGER_AVG = m_bollingerAvg; //Bollinger Band�� ���� �����Ѵ�.
	BOLLINGER_TIME = m_bollingerTime;

	MACD_EMA_1 = m_macd1; //MACD�� ���� �����Ѵ�.
	MACD_EMA_2 = m_macd2;
	MACD_SIGNAL = m_macd3;

	if (m_callList.GetCount() == 0) { //�޺��ڽ��� �����Ͱ� ������ �ɼ������� �����͸� ��û�Ѵ�.

		flagT2301 = 0; //���������� ������ t2301 �������� ������ �ֱ����Ͽ� 0���� �ʱ�ȭ �Ѵ�.
		t2301InBlock	pckInBlock;
		FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
		memcpy( pckInBlock.yyyymm, monthForData, sizeof(pckInBlock.yyyymm));
		EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t2301", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// ������ ����

	} else { //�޺��ڽ��� �����Ͱ� ������ �ɼ������� �����͸� ��û���� �ʰ�, �ɼ� �� �����͸� ������ ���α׷��� �����Ѵ�.

		ExecuteRealTime();
	}
}

//--------------------------------------------------------------------------------------------------------------
// ������ ��ư�� �������� ȭ�鿡 ���ڼ��� �׷��ش�.
void CRealTimeChartDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CPen pen(PS_SOLID,0, RGB(250,150,250));
	dc.SelectObject( &pen );

	if(point.x > X_START_GRAPH && point.x < X_START_GRAPH+X_MAX_SIZE_GRAPH) {
		if(point.y > Y_START_GRAPH && point.y < Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+Y_MAX_OSCILLATOR) {
			dc.MoveTo(point.x, Y_START_GRAPH);
			dc.LineTo(point.x, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+Y_MAX_OSCILLATOR);
			dc.MoveTo(X_START_GRAPH, point.y);
			dc.LineTo(X_START_GRAPH+X_MAX_SIZE_GRAPH, point.y);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

//--------------------------------------------------------------------------------------------------------------
// 'Clear' ��ư�� ������ ��, ����Ǵ� �Լ��� ���콺 ������ ��ư���� �׸� ������ �����ش�.
// ��, Chart�� ���� �׸��� ���� ���� �����ִ� ���̴�.
void CRealTimeChartDlg::OnBnClickedBtnClear()
{
	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// 2�ʸ��� ����Ǵ� OnTimer Callback �Լ���.
void CRealTimeChartDlg::OnTimer(UINT nIDEvent)
{

	countTimer += 1;	//RESET_TIME ���� ȭ���� �ʱ�ȭ ��Ű�� ���� countTimer�� 1 ������Ų��.

	if (countTimer > (int)((RESET_TIME * 60) / TIMER_INTERVAL)) {	//RESET_TIME(5��) ���� �ʱ�ȭ�� ��Ų��.

		countTimer = 0;		//countTimer�� 0���� �ʱ�ȭ ��Ų��.
		ExecuteRealTime();	//�к� �����͸� ���� �޾� Reset �ϱ� ���Ͽ� ExecuteRealTime()�� �����Ѵ�.

	} else {	//�ǽð� �����͸� 2�ʸ��� �׸���.

		DrawRealData();
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���Ƿ����� ������ Bollinger Band ���� �׷��ִ� Checkbox�� ���õǾ��� �� ����ȴ�.
void CRealTimeChartDlg::OnBnClickedCheckBollinger()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Chart ������ Bollinger Band ���� �׷��ִ� Checkbox�� ���õǾ��� �� ����ȴ�.
void CRealTimeChartDlg::OnBnClickedCheckDraw()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// ���Ƿ����� ������ MACD ���� �׷��ִ� Checkbox�� ���õǾ��� �� ����ȴ�.
void CRealTimeChartDlg::OnBnClickedCheckMacd()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Radio ��ư�� Call�� ���õǾ��� �� ����ȴ�.
void CRealTimeChartDlg::OnBnClickedRadioMacdcall()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Radio ��ư�� Put�� ���õǾ��� �� ����ȴ�.
void CRealTimeChartDlg::OnBnClickedRadioMacdput()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Bollinger Band�� ��հ��� ����Ǿ��� �� ����ȴ�.
void CRealTimeChartDlg::OnEnChangeEditBollingeravg()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// Bollinger Band�� ������� ����Ǿ��� �� ����ȴ�.
void CRealTimeChartDlg::OnEnChangeEditBollingertime()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// MACD�� ù��° ���� ����Ǿ��� �� ����ȴ�.
void CRealTimeChartDlg::OnEnChangeEditMacd1()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// MACD�� �ι�° ���� ����Ǿ��� �� ����ȴ�.
void CRealTimeChartDlg::OnEnChangeEditMacd2()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// MACD�� ����° ���� ����Ǿ��� �� ����ȴ�.
void CRealTimeChartDlg::OnEnChangeEditMacd3()
{
	UpdateData(TRUE);
}

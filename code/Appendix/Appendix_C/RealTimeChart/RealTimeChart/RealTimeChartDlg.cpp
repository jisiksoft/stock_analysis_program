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

	CFont fontTitle; //제목 "Real-Time Chart"의 글자 크기 변경
	fontTitle.CreateFont(100,100,0,0,1,1,0,1,1,OUT_DEFAULT_PRECIS,0,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Arial" );
	m_title.SetFont(&fontTitle, TRUE);

	m_server.AddString(_T("hts.etrade.co.kr"));		//접속서버를 콤보박스에 등록한다.
	m_server.AddString(_T("demo.etrade.co.kr"));
	m_server.SetCurSel(1);

	flagPaint = false;						//그래프를 그리기 위한 flagPaine를 false로 설정하여 프로그램 시작시 Chart를 그리지 않는다.

	option = new COption();					//데이터를 저장하기 위하여 사용되는 COption 클래스를 생성한다.
	graph = new CGraph();					//그래프를 그리기 위하여 사용되는 CGraph 클래스를 생성한다.

	CTime t = CTime::GetCurrentTime();		//현재 시간을 가져온다.
	date = t.Format("%Y%m%d");				//현재 날자를 가져온다. (YYYYMMDD)
	//date.Format(_T("20140410"));

	//////////////// 1. xingAPI 초기화 ////////////////
	if( iXingAPI.Init() == FALSE ) {
		CString strPath;
		int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		strPath.ReleaseBuffer( nLen );
		int nFind = strPath.ReverseFind( '\\' );
		if( nFind > 0 ) strPath = strPath.Left( nFind );
		if( iXingAPI.Init( strPath ) == FALSE ) {
			MessageBox( (LPCTSTR)"xingAPI DLL을 Load 할 수 없습니다.");
		}
	}

	monthForData = option->GetMonthForData(); //현재 날자의 해당 옵션물을 가져오기 위한 Month 데이터를 가져온다.

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
		DrawGraph(); //Chart를 그린다.
	} else {
		DrawEmptyScreen(); //처음에 빈 화면을 그린다.
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
// 프로그램 시작시 처음 빈화면을 그려준다.
void CRealTimeChartDlg::DrawEmptyScreen()
{
	CPaintDC dc(this);

	//그래프 그리는 영역을 하얀 사각형으로 그린다.
	CBrush whiteBrush(RGB(255,255,255));
	RECT rect = {X_START_GRAPH, Y_START_GRAPH, X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH};
	dc.FillRect(&rect, &whiteBrush);

	//오실레이터 영역을 하얀색 사각형으로 그려준다.
	RECT rect2 = {X_START_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH, 
		X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+Y_MAX_OSCILLATOR};
	dc.FillRect(&rect2, &whiteBrush);
}

//--------------------------------------------------------------------------------------------------------------
// 프로그램이 데이터를 가져온 후 Chart를 그려준다.
// 분 데이터의 Chart를 새로 그린다.
void CRealTimeChartDlg::DrawGraph()
{		

	int i;
	
	int quantity = graph->ptData.lineCall.quantity;	//데이터의 갯수를 quantity에 저장한다.
	if (quantity > 360)	//15시 이후의 데이터는 그리지 않는다. (360 = 6시간 * 60분)
		quantity = 360;

	CPaintDC dc(this);

	//그래프 그리는 영역을 하얀 사각형으로 그린다.
	CBrush whiteBrush(RGB(255,255,255));
	RECT rect = {X_START_GRAPH, Y_START_GRAPH, X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH};
	dc.FillRect(&rect, &whiteBrush);

	RECT rect2 = {X_START_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH, 
		X_START_GRAPH+X_MAX_SIZE_GRAPH, Y_START_GRAPH+Y_MAX_SIZE_GRAPH+GAP_GRAPH+Y_MAX_OSCILLATOR};
	dc.FillRect(&rect2, &whiteBrush);

	//그리는 선들의 색을 정의한다.
	CPen redPen, bluePen, blackPen, grayPen, bollingerCallPen, bollingerPutPen, macdRedPen, macdBluePen;
	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));				//붉은 선
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));				//파란 선
	blackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));				//검은 선
	grayPen.CreatePen(PS_SOLID, 1, RGB(200,200,200));			//Chart의 회색선
	bollingerCallPen.CreatePen(PS_SOLID, 1, RGB(255,180,0));	//Call의 Bollinger Band 노란색 선
	bollingerPutPen.CreatePen(PS_SOLID, 1, RGB(0,255,255));		//Put의 Bollinger Band 하늘색 선
	macdRedPen.CreatePen(PS_SOLID, 2, RGB(255,200,200));		//MACD Signal의 붉은색 선
	macdBluePen.CreatePen(PS_SOLID, 2, RGB(200,200,255));		//MACD Signal의 파란색 선

	/////////////////////////////////// Y-axis //////////////////////////////////
	CString str;

	dc.SelectObject(grayPen);
	for(i=0; i<graph->ptData.ptBaseY.quantity; i++) { //Y축을 기준으로 수평 회색선을 그린다.
		dc.MoveTo(X_START_GRAPH, graph->ptData.ptBaseY.pointY[i]);
		dc.LineTo(X_START_GRAPH+X_MAX_SIZE_GRAPH, graph->ptData.ptBaseY.pointY[i]);

		str.Format("%0.1f", graph->ptData.ptBaseY.valueY[i]); //Y축의 데이터 기준 값들을 표시한다.
		dc.TextOut(15, graph->ptData.ptBaseY.pointY[i]-8, str, str.GetLength());
	}

	/////////////////////////////////// X-axis //////////////////////////////////
	dc.SelectObject(grayPen);
	for(i=1; i<6; i++) {	//X축을 기준으로 수직 회색선을 그린다.
		dc.MoveTo(graph->ptData.ptBaseX.pointX[i], Y_START_GRAPH);
		dc.LineTo(graph->ptData.ptBaseX.pointX[i], Y_START_GRAPH + Y_MAX_SIZE_GRAPH);

		str.Format("%d", graph->ptData.ptBaseX.valueX[i]); //시간 값을 표시한다.
		dc.TextOut(graph->ptData.ptBaseX.pointX[i]-6, Y_START_GRAPH + Y_MAX_SIZE_GRAPH + 1, str, str.GetLength());
	}

	/////////////////////////////////// Data Lines //////////////////////////////////

	if (m_drawBollinger) { //Bollinger Band의 checkbox가 선택되어 있으면 Bollinger Band 선을 Chart 영역에 그린다.
		//----- Chart 영역의 Call Bollinger Band 선 그리기 -----
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
		//----- Chart 영역의 Put Bollinger Band 선 그리기 -----
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

	//----- Call 데이터 챠트 그리기 -----
	dc.SelectObject(redPen);
	dc.MoveTo(X_START_GRAPH, graph->ptData.lineCall.ptLast[0].Y);
	for(i=1; i<quantity; i++) {
		dc.LineTo(graph->ptData.lineCall.ptLast[i].X, graph->ptData.lineCall.ptLast[i].Y);
	}
	//----- Put 데이터 챠트 그리기 -----
	dc.SelectObject(bluePen);
	dc.MoveTo(X_START_GRAPH, graph->ptData.linePut.ptLast[0].Y);
	for(i=1; i<quantity; i++) {
		dc.LineTo(graph->ptData.linePut.ptLast[i].X, graph->ptData.linePut.ptLast[i].Y);
	}

	/////////////////////////////////// MACD 그리기 (오실레이터 영역) //////////////////////////////////

	if (m_flagMACD) { //MACD checkbox가 선택되어 있으면 MACD를 그린다.
		if (!m_radioMACD) {	//Call의 MACD Signal을 그린다.

			for(i=0; i<quantity; i++) {

				if (graph->ptData.lineCall.ptMacdOscillator[i].Y < graph->originOscillatorY) //오실레이터의 가운데 선보다 위면 붉은 색 선을 그린다.
					dc.SelectObject(macdRedPen);
				else //오실레이터의 가운데 선보다 아래면 파란 선을 그린다.
					dc.SelectObject(macdBluePen);

				dc.MoveTo(graph->ptData.lineCall.ptMacdOscillator[i].X, graph->originOscillatorY);
				dc.LineTo(graph->ptData.lineCall.ptMacdOscillator[i].X, graph->ptData.lineCall.ptMacdOscillator[i].Y);
			}
		} else { //Put의 MACD Signal을 그린다.

			for(i=0; i<quantity; i++) {

				if (graph->ptData.linePut.ptMacdOscillator[i].Y < graph->originOscillatorY) //오실레이터의 가운데 선보다 위면 붉은 색 선을 그린다.
					dc.SelectObject(macdRedPen);
				else //오실레이터의 가운데 선보다 아래면 파란 선을 그린다.
					dc.SelectObject(macdBluePen);

				dc.MoveTo(graph->ptData.linePut.ptMacdOscillator[i].X, graph->originOscillatorY);
				dc.LineTo(graph->ptData.linePut.ptMacdOscillator[i].X, graph->ptData.linePut.ptMacdOscillator[i].Y);
			}
		}
	}

	/////////////////////////////////// Bollinger Band (오실레이터 영역) //////////////////////////////////	

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

	if (m_flagBollinger) { //Bollinger Band의 checkbox가 선택되어 있으면 Bollinger Band를 그린다.

		//----- Call Bollinger Band Oscillaot 그리기 -----
		dc.SelectObject(redPen);
		dc.MoveTo(graph->ptData.lineCall.ptBollingerOscillator[BOLLINGER_AVG-1].X, graph->ptData.lineCall.ptBollingerOscillator[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG-1; i<quantity; i++) {
			dc.LineTo(graph->ptData.lineCall.ptBollingerOscillator[i].X, graph->ptData.lineCall.ptBollingerOscillator[i].Y);
		}

		//----- Put Bollinger Band Oscillaot 그리기 -----
		dc.SelectObject(bluePen);
		dc.MoveTo(graph->ptData.linePut.ptBollingerOscillator[BOLLINGER_AVG-1].X, graph->ptData.linePut.ptBollingerOscillator[BOLLINGER_AVG-1].Y);
		for(i=BOLLINGER_AVG-1; i<quantity; i++) {
			dc.LineTo(graph->ptData.linePut.ptBollingerOscillator[i].X, graph->ptData.linePut.ptBollingerOscillator[i].Y);
		}
	}

	//----- 이후에 실시간 그래프를 그리기 위하여 마지막에 점의 위치를 저장한다. -----
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
// 실시간 데이터를 그리는 것으로 이전 데이터의 선을 지우고 다시 그린다.
// 만약 새로운 분의 첫 데이터이면 화면 전체를 다시 그려준다.
void CRealTimeChartDlg::DrawRealData()
{
	if (graph->ptData.lineCall.quantity > 360)
		return;

	int index = graph->ptData.lineCall.quantity - 1;

	option->CalculateRealData(); //실시간 데이터를 계산한다. 여기서는 Bollinger Band와 MACD의 선을 계산한다.
	graph->CalculateRealDataPoint(&option->pairUnit); //데이터들의 화면에서의 점의 위치를 계산한다.

	//그리는 선들의 색을 정의한다.
	CPen whitePen, whiteThickPen, redPen, bluePen, macdRedPen, macdBluePen;
	whitePen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	whiteThickPen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	macdRedPen.CreatePen(PS_SOLID, 2, RGB(255,200,200));
	macdBluePen.CreatePen(PS_SOLID, 2, RGB(200,200,255));

	if (graph->ptData.lineCall.ptLast[index].X > graph->ptDrawnLast.x) { //새로운 시간의 점이면 화면을 새로 그린다.

		flagPaint = true;
		RedrawWindow();

	} else {

		//------------------- 이전 선 지우기 ------------------	
		pDC->SelectObject(whiteThickPen); //하얀색으로 이전 선들을 다시 그리면 지우는 것이 된다.

		if (m_flagMACD) { //----- MACD 이전 line 지우기 -----

			if (!m_radioMACD) {

				pDC->MoveTo(graph->ptDrawnLast.x, graph->originOscillatorY);
				pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptMacdCall);

			} else {

				pDC->MoveTo(graph->ptDrawnLast.x, graph->originOscillatorY);
				pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptMacdPut);
			}
		}

		if (m_flagBollinger) { //----- Bollinger 이전 line 지우기 -----
			
			pDC->MoveTo(graph->ptData.lineCall.ptLast[index-1].X, graph->ptData.lineCall.ptBollingerOscillator[index-1].Y);	//선을 하얀색으로 다시 그리면서 지운다.
			pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptBollingerCall);

			pDC->MoveTo(graph->ptData.linePut.ptLast[index-1].X, graph->ptData.linePut.ptBollingerOscillator[index-1].Y);
			pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptBollingerPut);
		}

		//----- Call 이전 line 지우기 -----
		pDC->MoveTo(graph->ptData.lineCall.ptLast[index-1].X, graph->ptData.lineCall.ptLast[index-1].Y);	//선을 하얀색으로 다시그리면서 지운다.
		pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptDataCall);

		//----- Put 이전 line 지우기 -----
		pDC->MoveTo(graph->ptData.linePut.ptLast[index-1].X, graph->ptData.linePut.ptLast[index-1].Y);
		pDC->LineTo(graph->ptDrawnLast.x, graph->ptDrawnLast.ptDataPut);


		//----------------- MACD 선 그리기 (오실레이터 영역) -------------------
		if (m_flagMACD) {
			
			//----- MACD 선 그리기 -----
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
		
		//----------------- Bollinger Band 선 그리기 (오실레이터 영역) -------------------
		if (m_flagBollinger) {
			
			pDC->SelectObject(redPen);
			pDC->MoveTo(graph->ptData.lineCall.ptBollingerOscillator[index-1].X, graph->ptData.lineCall.ptBollingerOscillator[index-1].Y); //선을 다시 그려준다.
			pDC->LineTo(graph->ptData.lineCall.ptBollingerOscillator[index].X, graph->ptData.lineCall.ptBollingerOscillator[index].Y);
			
			pDC->SelectObject(bluePen);
			pDC->MoveTo(graph->ptData.linePut.ptBollingerOscillator[index-1].X, graph->ptData.linePut.ptBollingerOscillator[index-1].Y);
			pDC->LineTo(graph->ptData.linePut.ptBollingerOscillator[index].X, graph->ptData.linePut.ptBollingerOscillator[index].Y);
		}

		//----- Call 새로운 선 그리기 -----
		pDC->SelectObject(redPen);
		pDC->MoveTo(graph->ptData.lineCall.ptLast[index-1].X, graph->ptData.lineCall.ptLast[index-1].Y);	//선을 다시 그려준다.
		pDC->LineTo(graph->ptData.lineCall.ptLast[index].X, graph->ptData.lineCall.ptLast[index].Y);


		//----- Put 새로운 선 그리기 -----
		pDC->SelectObject(bluePen);
		pDC->MoveTo(graph->ptData.linePut.ptLast[index-1].X, graph->ptData.linePut.ptLast[index-1].Y);
		pDC->LineTo(graph->ptData.linePut.ptLast[index].X, graph->ptData.linePut.ptLast[index].Y);

	}

	//----- 이후에 실시간 그래프를 그리기 위하여 마지막에 점의 위치를 저장한다. -----
	graph->ptDrawnLast.x = graph->ptData.lineCall.ptLast[index].X;
	graph->ptDrawnLast.ptDataCall = graph->ptData.lineCall.ptLast[index].Y;
	graph->ptDrawnLast.ptDataPut = graph->ptData.linePut.ptLast[index].Y;
	graph->ptDrawnLast.ptBollingerCall = graph->ptData.lineCall.ptBollingerOscillator[index].Y;
	graph->ptDrawnLast.ptBollingerPut = graph->ptData.linePut.ptBollingerOscillator[index].Y;
	graph->ptDrawnLast.ptMacdCall = graph->ptData.lineCall.ptMacdOscillator[index].Y;
	graph->ptDrawnLast.ptMacdPut = graph->ptData.linePut.ptMacdOscillator[index].Y;

	// 화면에 현재 값을 새로 업데이트한다.
	m_curCall = option->pairUnit.call->data[index].lastVal;
	m_curPut = option->pairUnit.put->data[index].lastVal;
	UpdateData(FALSE);

}

//--------------------------------------------------------------------------------------------------------------
// 옵션의 Call/Put을 ComboBox List에 넣어주고 STANDARD_VALUE(1.3)에 근접한 Call과 Put을 선택하여 갖는다.
// 선택된 한 쌍의 Call과 Put은 COption 클래스의 pairUnit이 가지게 된다.
void CRealTimeChartDlg::InitalizeComboBox() 
{
	int i;
	float min, valToCompare;

	//Call 콤보박스에 종목코드를 넣는다.
	for (i=0; i<option->allUnit.quantityCall; i++)
		m_callList.AddString(option->allUnit.unitCall[i].code);

	//Put 콤보박스에 종목코드를 넣는다.
	for (i=0; i<option->allUnit.quantityCall; i++)
		m_putList.AddString(option->allUnit.unitPut[i].code);

	min = 100; //최소값을 초기화한다.

	for (i=0; i<option->allUnit.quantityCall; i++) { //1.3에 근접한 최소값을 선택한다.
		if (option->allUnit.unitCall[i].openVal > STANDARD_VALUE) //1.3와의 차이를 계산하여 valToCompare에 넣는다.
			valToCompare = option->allUnit.unitCall[i].openVal - (float)STANDARD_VALUE;
		else
			valToCompare = (float)STANDARD_VALUE - option->allUnit.unitCall[i].openVal;

		if (valToCompare < min) { //1.3에 근접한 Call이 콤보박스에서 보여지고 pairUnit에 담는다.
			option->pairUnit.call = &option->allUnit.unitCall[i];
			min = valToCompare;
			m_callList.SetCurSel(i);
		}
	}

	min = 100; //최소값을 초기화한다.

	for (i=0; i<option->allUnit.quantityPut; i++) { //1.3에 근접한 최소값을 선택한다.
		if (option->allUnit.unitPut[i].openVal > STANDARD_VALUE) //1.3와의 차이를 계산하여 valToCompare에 넣는다.
			valToCompare = option->allUnit.unitPut[i].openVal - (float)STANDARD_VALUE;
		else
			valToCompare = (float)STANDARD_VALUE - option->allUnit.unitPut[i].openVal;

		if (valToCompare < min) { //1.3에 근접한 Put이 콤보박스에서 보여지고 pairUnit에 담는다.
			option->pairUnit.put = &option->allUnit.unitPut[i];
			min = valToCompare;
			m_putList.SetCurSel(i);
		}
	}

	UpdateData(FALSE);	//변경된 사항을 화면에 업데이트한다.

	ExecuteRealTime();
}

//--------------------------------------------------------------------------------------------------------------
// Call과 Put의 콤보박스에 선택된 종목들의 분 데이터를 요청한다.
void CRealTimeChartDlg::ExecuteRealTime() 
{
	//이전에 실시간 데이터를 받는 것이 있으면 요청을 취소한다.(UnadviseRealData)
	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.call->code, 0 ) ); //Call 실시간 데이터 종료
	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.put->code, 0 ) ); //Put 실시간 데이터 종료

	UpdateData(TRUE);

	//콤보박스에서 보여지는 현재 Call과 Put을 새로운 쌍으로 선택한다.
	option->pairUnit.call = &option->allUnit.unitCall[m_callList.GetCurSel()];
	option->pairUnit.put = &option->allUnit.unitPut[m_putList.GetCurSel()];

	//선택된 Call과 Put의 시작가를 화면에 보여준다.
	m_startCall = option->pairUnit.call->openVal;
	m_startPut = option->pairUnit.put->openVal;
	UpdateData(FALSE);

	flagT8415 = 0;
	t8415InBlock pckInBlock;
	//Call의 분봉 데이터를 요청한다.
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	memcpy( pckInBlock.shcode, option->pairUnit.call->code, sizeof(pckInBlock.shcode));			//옵션 종목코드
	memcpy( pckInBlock.ncnt, "0001", sizeof(pckInBlock.ncnt));									//1분 데이터
	memcpy( pckInBlock.sdate, date, sizeof(pckInBlock.sdate));									//시작일은 오늘
	memcpy( pckInBlock.edate, date, sizeof(pckInBlock.edate));									//종료일은 오늘
	memcpy( pckInBlock.comp_yn, "N", sizeof(pckInBlock.comp_yn));								//압축하지 않는다.
	EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8415", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// 데이터 요청

	Sleep(1100); //t8415의 요청수는 초당 1회이기 때문에 1.1초의 Delay를 준다.

	//Put의 분봉 데이터를 요청한다.
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	memcpy( pckInBlock.shcode, option->pairUnit.put->code, sizeof(pckInBlock.shcode));			//옵션 종목코드
	memcpy( pckInBlock.ncnt, "0001", sizeof(pckInBlock.ncnt));									//1분 데이터
	memcpy( pckInBlock.sdate, date, sizeof(pckInBlock.sdate));									//시작일은 오늘
	memcpy( pckInBlock.edate, date, sizeof(pckInBlock.edate));									//종료일은 오늘
	memcpy( pckInBlock.comp_yn, "N", sizeof(pckInBlock.comp_yn));								//압축하지 않는다.
	EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8415", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// 데이터 요청
}

//--------------------------------------------------------------------------------------------------------------
// 쌍으로 선택된 Call과 Put의 실시간 데이터를 요청하고(AdviseRealData), 실시간 그래프를 그리기 위한 타이머를 실행한다.
void CRealTimeChartDlg::AdviseRealData() 
{
	int index = option->pairUnit.put->quantity - 1;
	m_curCall = option->pairUnit.call->data[index].lastVal;
	m_curPut = option->pairUnit.put->data[index].lastVal;

	CTime t = CTime::GetCurrentTime();
	if (t.GetHour() >= 15 && t.GetMinute() > 3) { //15시3분 이후의 시간에는 실시간 데이터를 요청하지 않는다.
		m_message.Format(_T("실시간 데이터 시간이 아닙니다."));
		UpdateData(FALSE);
		return;
	}

	EntryOrderError( iXingAPI.AdviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.call->code, 8) ); //Call 실시간 데이터 요청
	EntryOrderError( iXingAPI.AdviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.put->code, 8 ) ); //Put 실시간 데이터 요청

	pDC = GetDC(); //실시간 그래프를 그리기 위하여 DC를 선언한다.

	countTimer = 0; //5분마다 데이터를 초기화하기 위하여 사용되는 count를 0으로 초기화한다.
	SetTimer(1004, TIMER_INTERVAL * 1000, NULL); //타이머를 실행시킨다. 이 프로그램에서 TIMER_INTERVAL는 2로서 2초마다 OnTimer가 실행된다.
	
	m_message.Format(_T("실시간 데이터 요청했습니다."));
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------
// 실시간 데이터 요청을 종료한다. (UnadviseRealData)
void CRealTimeChartDlg::UnadviseRealData() 
{
	//ReleaseDC(pDC);

	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.call->code, 0 ) ); //Call 실시간 데이터 종료
	EntryOrderError( iXingAPI.UnadviseRealData(GetSafeHwnd(), "OC0", option->pairUnit.put->code, 0 ) ); //Put 실시간 데이터 종료

	KillTimer(1004); //Timer를 종료시킨다.

	m_message.Format(_T("실시간 데이터 종료했습니다."));
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------
// 실시간 데이터를 받는 Callback 함수
LRESULT CRealTimeChartDlg::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if(!strcmp( pRealPacket->szTrCode, "OC0")) { //KOSPI200 옵션체결 실시간 데이터를 받으면 실행한다.

		LPOC0_OutBlock pOutBlock = (LPOC0_OutBlock)pRealPacket->pszData;

		UINT chetime = changeStringTimeToInt(pOutBlock->chetime);				//받은 데이터 시간을 가져온다.
		CString optcode((char*)pOutBlock->optcode, sizeof(pOutBlock->optcode));	//옵션 코드를 가져온다.

		if (chetime > 1500) { //15시 이후의 데이터는 받지 않고, 실시간 데이터를 해제한다.
			UnadviseRealData();
			return 0L;
		}

		m_message.Format(_T("%d시%02d분 데이터를 받는 중..."), chetime/100, chetime%100);
		UpdateData(FALSE);

		if (!option->pairUnit.call->code.Compare(optcode)) { //받은 데이터가 Call 데이터인지 종목코드를 비교한다.

			//구조체에 저장된 마지막 분봉 데이터 시간과 받은 데이터 시간을 비교한다.
			int timeGap = getTimeGap(chetime, option->pairUnit.call->data[option->pairUnit.call->quantity-1].time);
			float price = changeStringToFloat(pOutBlock->price); //가격을 실수로 바꾸어준다.

			if (timeGap == 0) { //받은 데이터가 분봉 데이터로 저장된 값과 시간이 같은 경우 => 실시간 변화를 그려준다.

				option->pairUnit.call->data[option->pairUnit.call->quantity - 1].lastVal = price;

			} else if (timeGap == 1) { //받은 데이터가 분봉 데이터로 저장된 값보다 1초 많은 데이터일 경우 => 값을 추가한다.

				option->AddNewEntryCall(chetime, price);

			} else { //받은 데이터가 분봉 데이터로 저장된 값보다 2초 이상 차이가 나는 경우 => 분봉데이터를 새로 받는다.

				ExecuteRealTime();
			}

		} else if (!option->pairUnit.put->code.Compare(optcode)) { //받은 데이터가 Put 데이터일 경우...

			//구조체에 저장된 마지막 분봉 데이터 시간과 받은 데이터 시간을 비교한다.
			UINT timeGap = getTimeGap(chetime, option->pairUnit.put->data[option->pairUnit.put->quantity-1].time); 
			float price = changeStringToFloat(pOutBlock->price);

			if (timeGap == 0) { //받은 데이터가 분봉 데이터로 저장된 값과 시간이 같은 경우 => 실시간 변화를 그려준다.

				option->pairUnit.put->data[option->pairUnit.put->quantity - 1].lastVal = price;

			} else if (timeGap == 1) { //받은 데이터가 분봉 데이터로 저장된 값보다 1초 많은 데이터일 경우 => 값을 추가한다.

				option->AddNewEntryPut(chetime, price);

			} else { //받은 데이터가 분봉 데이터로 저장된 값보다 2초 이상 차이가 나는 경우 => 분봉데이터를 새로 받는다.

				ExecuteRealTime();
			}
		}
	}

	return 0L;
}

//--------------------------------------------------------------------------------------------------------------
// Xing으로부터 호출되는 Callback 함수로서 데이터를 받았을 때 실행되며, 메모리에 저장되어 전달받은 데이터들을 처리한다.
LRESULT CRealTimeChartDlg::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{

	int i;

	if( wParam == REQUEST_DATA ) {

		LPRECV_PACKET lpRecvPacket = (LPRECV_PACKET)lParam;

		//------------ t2301 데이터는 '옵션 전광판' 데이터를 가져오기 위한 것이며 프로그램 시작시 호출된다. -------------
		// Call과 Put의 List를 가져와서 COption 클래스의 구조체에 이름을 넣어준다.
		if(strcmp( lpRecvPacket->szTrCode, "t2301") == 0) {

			int count;

			if (flagT2301 == 0) {

			} if (flagT2301 == 1) { //두번째 받는 Block에는 Call 데이터가 있다.

				count = (int)( lpRecvPacket->nDataLength / sizeof(t2301OutBlock1));	

				LPt2301OutBlock1 pOutBlock1;

				for (i=0; i<count; i++) {
					pOutBlock1 = (LPt2301OutBlock1)(lpRecvPacket->lpData + (sizeof(t2301OutBlock1) * i));

					//Call 구조체에 데이터들을 넣어서 초기화한다.
					CString optcode((char*)pOutBlock1->optcode, sizeof(pOutBlock1->optcode) );
					option->allUnit.unitCall[option->allUnit.quantityCall].code = optcode;
					option->allUnit.unitCall[option->allUnit.quantityCall].openVal = changeStringToFloat(pOutBlock1->open);
					option->allUnit.quantityCall += 1;
				}

			} else if (flagT2301 == 2) { //세번째 받는 Block에는 Put 데이터가 있다.

				count = (int)(lpRecvPacket->nDataLength / sizeof(t2301OutBlock2));	
				LPt2301OutBlock2 pOutBlock2;

				for (i=0; i<count; i++) {
					pOutBlock2 = (LPt2301OutBlock2)(lpRecvPacket->lpData + (sizeof(t2301OutBlock2) * i));

					//Put 구조체에 데이터들을 넣어서 초기화한다.
					CString optcode( (char*)pOutBlock2->optcode, sizeof(pOutBlock2->optcode) );
					option->allUnit.unitPut[option->allUnit.quantityPut].code = optcode;
					option->allUnit.unitPut[option->allUnit.quantityPut].openVal = changeStringToFloat(pOutBlock2->open);
					option->allUnit.quantityPut += 1;
				}

				InitalizeComboBox(); 	
			}

			flagT2301 += 1;

		//------------ t8415 데이터는 프로그램 시작시 분 데이터를 가져와서 처음 데이터를 그려주기 위해서 사용된다. ------------
		} else if(strcmp( lpRecvPacket->szTrCode, "t8415") == 0) {

			int count;
			Unit *unit;

			//t8415은 두번 요청되었고, 앞선 것은 Call 데이터이고, 뒤에것이 Put 데이터이다.
			if (flagT8415 == 0) { //첫번째 받는 것은 데이터가 없는지를 확인하게 된다.

				LPt8415OutBlock pOutBlock = (LPt8415OutBlock)(lpRecvPacket->lpData);

				CString diclose( (char*)pOutBlock->diclose, sizeof(pOutBlock->diclose) );
				if(diclose.Compare(_T("000000")) == 0) {
					m_message.Format(_T("오늘은 데이터가 없습니다."));	//옵션 데이터가 없음을 출력한다.
					UpdateData(FALSE);
				}

			} else if (flagT8415 == 1) { //Call 분 데이터를 받아서 구조체에 저장한다.

				unit = option->pairUnit.call;

				count = (int)(lpRecvPacket->nDataLength / sizeof(t8415OutBlock1)); //Block 갯수를 계산한다.
				LPt8415OutBlock1 pOutBlock1;

				for (i=0; i<count; i++) { //Call 분 데이터를 구조체에 순차적으로 넣는다.
					pOutBlock1 = (LPt8415OutBlock1)(lpRecvPacket->lpData + (sizeof(t8415OutBlock1) * i));

					unit->data[i].time = changeStringTimeToInt(pOutBlock1->time);
					unit->data[i].startVal = changeStringToFloat(pOutBlock1->open);
					unit->data[i].highVal = changeStringToFloat(pOutBlock1->high);
					unit->data[i].lowVal = changeStringToFloat(pOutBlock1->low);
					unit->data[i].lastVal = changeStringToFloat(pOutBlock1->close);
				}

				unit->quantity = count;

			} else if (flagT8415 == 2) {


			} else if (flagT8415 == 3) { //Put 분 데이터를 받아서 구조체에 저장한다.

				unit = option->pairUnit.put;

				count = (int)(lpRecvPacket->nDataLength / sizeof(t8415OutBlock1)); //Block 갯수를 계산한다.
				LPt8415OutBlock1 pOutBlock1;

				for (i=0; i<count; i++) { //Put 분 데이터를 구조체에 순차적으로 넣는다.
					pOutBlock1 = (LPt8415OutBlock1)(lpRecvPacket->lpData + (sizeof(t8415OutBlock1) * i));

					unit->data[i].time = changeStringTimeToInt(pOutBlock1->time);
					unit->data[i].startVal = changeStringToFloat(pOutBlock1->open);
					unit->data[i].highVal = changeStringToFloat(pOutBlock1->high);
					unit->data[i].lowVal = changeStringToFloat(pOutBlock1->low);
					unit->data[i].lastVal = changeStringToFloat(pOutBlock1->close);
				}
				unit->quantity = count;

				//Call과 Put의 분 데이터를 받았기 때문에 보조지표의 값들을 계산하고 그래프를 그려준 후, 실시간 데이터를 요청한다.
				option->CalculateBollinger(); //Bollinger Band를 계산한다.
				option->CalculateMACD(); //MACD를 계산한다.
				graph->CalculateAllPointData(&option->pairUnit); //화면에 그리기 위한 점들을 계산한다.

				flagPaint = true;
				RedrawWindow(); //화면을 그린다.

				AdviseRealData(); //실시간 데이터를 요청한다.
			}

			flagT8415 += 1;
		}
	} else if( wParam == MESSAGE_DATA ) {// 메시지를 받음
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		iXingAPI.ReleaseMessageData( lParam );
	} else if( wParam == SYSTEM_ERROR_DATA ) {// System Error를 받음
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		::AfxMessageBox(strMsg);
		iXingAPI.ReleaseMessageData( lParam );
	} else if( wParam == RELEASE_DATA )	{ // Release Data를 받음
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
	::AfxMessageBox(_T("서버와의 연결이 종료되었습니다."));
	return 0L;
}

//--------------------------------------------------------------------------------------------------------------
// 에러 메시지를 출력한다.
VOID CRealTimeChartDlg::EntryOrderError(INT nRqID)
{
	if( nRqID < 0 )	{
		CString str;
		str.Format(_T("[%d] %s"), nRqID, iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(str);
	}
}

//--------------------------------------------------------------------------------------------------------------
// 서버에 연결하고 로그인을 한다.
void CRealTimeChartDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE); //화면에서 입력된 값을 멤버변수에 삽입

	if (iXingAPI.IsConnected()) {// 이미 접속되어 있으면 접속을 종료한다.
		iXingAPI.Disconnect();
	}

	m_message.Format(_T("서버 접속 중..."));
	UpdateData(FALSE);

	/////////////// 2. 서버 접속 ////////////////
	BOOL bResult;
	if (m_server.GetCurSel() == 0) {//실서버라면..
		bResult = iXingAPI.Connect( GetSafeHwnd(),"hts.etrade.co.kr",20001,	WM_USER, 3000, 512 );
	} else {//모의서버라면
		bResult = iXingAPI.Connect( GetSafeHwnd(),"demo.etrade.co.kr",20001, WM_USER, 3000, 512 );
	}
	if( bResult == FALSE )	{// 접속실패 처리
		CString strMsg = iXingAPI.GetErrorMessage( iXingAPI.GetLastError());
		::AfxMessageBox(_T("서버접속 실패 : ") + strMsg);
	}

	m_message.Format(_T("서버에 접속되었습니다."));
	UpdateData(FALSE);

	//////////////// 3. 로그인 ////////////////
	if( FALSE == iXingAPI.Login( GetSafeHwnd(), m_id, m_password, m_password2, m_server.GetCurSel(), TRUE ) )	{// 로그인
		CString strMsg = iXingAPI.GetErrorMessage( iXingAPI.GetLastError() );
		::AfxMessageBox(_T("로그인 실패 : ") + strMsg);
	}
}

//--------------------------------------------------------------------------------------------------------------
// 로그 아웃을 하고 서버 연결을 종료한 후, 프로그램을 닫는다.
void CRealTimeChartDlg::OnBnClickedBtnClose()
{
	KillTimer(1004);

	//////////////// 4. 로그아웃 ////////////////
	if( FALSE == iXingAPI.Logout( GetSafeHwnd() ) )	{// 로그인
		CString strMsg = iXingAPI.GetErrorMessage( iXingAPI.GetLastError() );
		::AfxMessageBox(_T("로그아웃 실패 : ") + strMsg);
	}

	m_message.Format(_T("로그아웃 되었습니다."));	//로그아웃 메시지 출력
	UpdateData(FALSE);

	//////////////// 5. 서버 접속 종료 ////////////////
	iXingAPI.Disconnect();

	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

//--------------------------------------------------------------------------------------------------------------
// '실행하기' 버튼이 눌러졌을 때 실행되는 함수로서 프로그램을 시작한다.
void CRealTimeChartDlg::OnBnClickedBtnRun()
{	
	KillTimer(1004);

	UpdateData(TRUE);

	BOLLINGER_AVG = m_bollingerAvg; //Bollinger Band의 값을 설정한다.
	BOLLINGER_TIME = m_bollingerTime;

	MACD_EMA_1 = m_macd1; //MACD의 값을 설정한다.
	MACD_EMA_2 = m_macd2;
	MACD_SIGNAL = m_macd3;

	if (m_callList.GetCount() == 0) { //콤보박스에 데이터가 없으면 옵션전광판 데이터를 요청한다.

		flagT2301 = 0; //순차적으로 들어오는 t2301 데이터의 순서를 주기위하여 0으로 초기화 한다.
		t2301InBlock	pckInBlock;
		FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
		memcpy( pckInBlock.yyyymm, monthForData, sizeof(pckInBlock.yyyymm));
		EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t2301", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// 데이터 전송

	} else { //콤보박스에 데이터가 있으면 옵션전광판 데이터를 요청하지 않고, 옵션 분 데이터를 가져와 프로그램을 실행한다.

		ExecuteRealTime();
	}
}

//--------------------------------------------------------------------------------------------------------------
// 오른쪽 버튼이 눌러지면 화면에 십자선을 그려준다.
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
// 'Clear' 버튼이 눌렸을 때, 실행되는 함수로 마우스 오른쪽 버튼으로 그린 점들을 지워준다.
// 즉, Chart를 새로 그리는 것이 선을 지워주는 것이다.
void CRealTimeChartDlg::OnBnClickedBtnClear()
{
	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// 2초마다 실행되는 OnTimer Callback 함수다.
void CRealTimeChartDlg::OnTimer(UINT nIDEvent)
{

	countTimer += 1;	//RESET_TIME 마다 화면을 초기화 시키기 위한 countTimer를 1 증가시킨다.

	if (countTimer > (int)((RESET_TIME * 60) / TIMER_INTERVAL)) {	//RESET_TIME(5분) 마다 초기화를 시킨다.

		countTimer = 0;		//countTimer를 0으로 초기화 시킨다.
		ExecuteRealTime();	//분봉 데이터를 새로 받아 Reset 하기 위하여 ExecuteRealTime()을 실행한다.

	} else {	//실시간 데이터를 2초마다 그린다.

		DrawRealData();
	}
}

//--------------------------------------------------------------------------------------------------------------
// 오실레이터 영역에 Bollinger Band 선을 그려주는 Checkbox가 선택되었을 때 실행된다.
void CRealTimeChartDlg::OnBnClickedCheckBollinger()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Chart 영역에 Bollinger Band 선을 그려주는 Checkbox가 선택되었을 때 실행된다.
void CRealTimeChartDlg::OnBnClickedCheckDraw()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// 오실레이터 영역에 MACD 선을 그려주는 Checkbox가 선택되었을 때 실행된다.
void CRealTimeChartDlg::OnBnClickedCheckMacd()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Radio 버튼의 Call이 선택되었을 때 실행된다.
void CRealTimeChartDlg::OnBnClickedRadioMacdcall()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Radio 버튼의 Put이 선택되었을 때 실행된다.
void CRealTimeChartDlg::OnBnClickedRadioMacdput()
{
	UpdateData(TRUE);

	flagPaint = true;
	RedrawWindow();
}

//--------------------------------------------------------------------------------------------------------------
// Bollinger Band의 평균값이 변경되었을 때 실행된다.
void CRealTimeChartDlg::OnEnChangeEditBollingeravg()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// Bollinger Band의 배수값이 변경되었을 때 실행된다.
void CRealTimeChartDlg::OnEnChangeEditBollingertime()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// MACD의 첫번째 값이 변경되었을 때 실행된다.
void CRealTimeChartDlg::OnEnChangeEditMacd1()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// MACD의 두번째 값이 변경되었을 때 실행된다.
void CRealTimeChartDlg::OnEnChangeEditMacd2()
{
	UpdateData(TRUE);
}

//--------------------------------------------------------------------------------------------------------------
// MACD의 세번째 값이 변경되었을 때 실행된다.
void CRealTimeChartDlg::OnEnChangeEditMacd3()
{
	UpdateData(TRUE);
}

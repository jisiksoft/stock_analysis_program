///////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                               //
// Date       : Mar 17, 2013                             //
// eMail      : kimsehoon@hotmail.com                    //
// Description: This Program Gets Stock Data from ETRADE //
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetStockData.h"
#include "GetStockDataDlg.h"

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
CGetStockDataDlg::CGetStockDataDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGetStockDataDlg::IDD, pParent)
, m_id(_T(""))
, m_password(_T(""))
, m_password2(_T(""))
, m_message(_T("접속후 가져오기 버튼을 누르세요."))
, m_output(_T("data.txt"))
, m_jongmokcode(_T("000020"))
, m_radio(TRUE)
, m_quantity(250)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//--------------------------------------------------------------------------------------------------------------
void CGetStockDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATATYPE, m_datatype);
	DDX_Control(pDX, IDC_COMBO1, m_server);
	DDX_Text(pDX, IDC_EDIT1, m_id);
	DDX_Text(pDX, IDC_EDIT2, m_password);
	DDX_Text(pDX, IDC_EDIT3, m_password2);
	DDX_Text(pDX, IDC_STATIC_MSG, m_message);
	DDX_Text(pDX, IDC_EDIT4, m_output);
	DDX_Text(pDX, IDC_EDIT5, m_jongmokcode);
	DDX_Radio(pDX, IDC_RADIO_TOTAL, m_radio);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_quantity);
	DDV_MinMaxInt(pDX, m_quantity, 1, 9999);
}

BEGIN_MESSAGE_MAP(CGetStockDataDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_GETDATA, &CGetStockDataDlg::OnBnClickedBtnGetdata)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CGetStockDataDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CGetStockDataDlg::OnBnClickedBtnDisconnect)
	ON_MESSAGE( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData )
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------
BOOL CGetStockDataDlg::OnInitDialog()
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

	m_server.AddString(_T("hts.etrade.co.kr"));		//접속서버를 콤보박스에 등록한다.
	m_server.AddString(_T("demo.etrade.co.kr"));
	m_server.SetCurSel(1);

	m_datatype.AddString(_T("Day"));		//데이터 타입을 콤보박스에 등록한다.
	m_datatype.AddString(_T("Week"));
	m_datatype.AddString(_T("Month"));
	m_datatype.SetCurSel(0);

	stock = new CStock();					//데이터를 저장하기 위하여 사용되는 CStock 클래스를 생성한다.

	CTime t = CTime::GetCurrentTime();		//현재 시간을 가져온다.
	date = t.Format("%Y%m%d");				//현재 날자를 가져온다. (YYYYMMDD)

	//////////////// 1. xingAPI 초기화 ////////////////
	if( iXingAPI.Init() == FALSE ) {
		CString strPath;
		int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		strPath.ReleaseBuffer( nLen );
		int nFind = strPath.ReverseFind( '\\' );
		if( nFind > 0 ) strPath = strPath.Left( nFind );
		if( iXingAPI.Init( strPath ) == FALSE ) {
			::AfxMessageBox(_T("xingAPI DLL을 Load 할 수 없습니다."));
		}
	}

	return TRUE;
}

void CGetStockDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CGetStockDataDlg::OnPaint()
{
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
HCURSOR CGetStockDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//--------------------------------------------------------------------------------------------------------------
// '접속' 버튼이 클릭되면 실행되는 이벤트 함수이다.
// 서버에 접속후, 로그인을 수행한다.
void CGetStockDataDlg::OnBnClickedBtnConnect()
{	
	UpdateData(TRUE);	//화면에서 입력된 값을 멤버변수에 삽입

	if( iXingAPI.IsConnected() )	{// 이미 접속되어 있으면 접속을 종료한다.
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
// '종료' 버튼이 클릭되면 실행되는 이벤트 함수이다.
// 로그아웃 후 서버 접속을 종료하고 프로그램을 종료한다.
void CGetStockDataDlg::OnBnClickedBtnDisconnect()
{
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
// '가져오기' 버튼이 클릭되면 실행되는 이벤트 함수이다.
// 화면의 설정 값에 해당하는 데이터를 가져와서 화일에 저장한다.
void CGetStockDataDlg::OnBnClickedBtnGetdata()
{	
	UpdateData(TRUE);

	//데이터 타입을 문자열로 만든다. ("2":일 "3":주 "4":월)
	dataType[1] = '\0';
	int index = m_datatype.GetCurSel();
	if (index == 0) dataType[0] = '2';
	if (index == 1) dataType[0] = '3';
	if (index == 2) dataType[0] = '4';

	//데이터 갯수는 500 이상을 설정할 수 없다. t8413에서 받는 OutBlock1의 배열은 최대 500개까지이다.
	if (m_quantity > 500) {
		m_quantity = 500;
		m_message.Format(_T("데이터는 최대 500개입니다."));
		UpdateData(FALSE);
		return;
	}

	//데이터 갯수인 정수 m_quantity를 4자리 문자열로 만든다. (ex: 250 => "0250")
	quantityData[4] = '\0';
	quantityData[3] = (char)(((int)'0')+(int)(m_quantity%10));
	quantityData[2] = (char)(((int)'0')+(int)((m_quantity%100)/10));
	quantityData[1] = (char)(((int)'0')+(int)((m_quantity%1000)/100));
	quantityData[0] = (char)(((int)'0')+(int)((m_quantity%10000)/1000));

	indexCompany = 0;	//종목의 index를 0으로 설정한다.

	if (!m_radio) { // 전체 종목 데이터 받기 위해서는 t8413을 요청한다.

		t8430InBlock	pckInBlock;
		FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
		memcpy( pckInBlock.gubun, "0", sizeof( pckInBlock.gubun));
		EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8430", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// 데이터 전송

	} else { // 한 종목 데이터 받기 위해서는 t8407을 요청하는데, 이프로그램에서는 종목명을 얻기 위해서다.

		stock->allCompany.quantity = 1;	//한 종목을 받기 때문에 전체 종목수는 1이다.

		t8407InBlock	pckInBlock;
		FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
		memcpy( pckInBlock.nrec, "001", sizeof( pckInBlock.nrec));
		memcpy( pckInBlock.shcode, m_jongmokcode, sizeof( pckInBlock.shcode));			//종목 코드를 입력한다.
		EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8407", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// 데이터 전송

	}
}

//--------------------------------------------------------------------------------------------------------------
// 모든 데이터를 가져온 후 프로그램의 마지막에 수행되는 함수로서, 데이터를 화일에 저장한다.
void CGetStockDataDlg::StoreDataInFile() 
{
	m_message.Format(_T("파일에 저장 중..."));	//파일에 저장중인 메시지를 출력한다.
	UpdateData(FALSE);

	stock->StoreDataInOneFile(m_output);	//저장된 모든 데이터를 파일에 저장한다.

	m_message.Format(_T("데이터 가져오기 완료!!!"));//데이터 가져오기 완료를 출력한다.
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------
// Xing으로부터 호출되는 Callback 함수로서 데이터를 받았을 때 실행되며, 메모리에 저장되어 전달받은 데이터들을 처리한다.
LRESULT CGetStockDataDlg::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{

	int i;
	Company *company;	//종목 데이터를 저장하기 위한 구조체를 가리키는 포인터 변수

	if( wParam == REQUEST_DATA ) {

		LPRECV_PACKET lpRecvPacket = (LPRECV_PACKET)lParam;

		//------------ t8407은 이 프로그램에서 하나의 종목만을 받기 위하여 사용되며 종목명을 가져오기 위해서이다. -------------
		if(strcmp( lpRecvPacket->szTrCode, "t8407") == 0) {

			LPt8407OutBlock1 pOutBlock1 = (LPt8407OutBlock1)lpRecvPacket->lpData;

			company = &stock->allCompany.company[0];	//이 프로그램에서는 하나의 종목 요청시 처리되는 부분이므로 index 0의 위치에 데이터를 저장한다.

			CString hname( (char*)pOutBlock1->hname, sizeof(pOutBlock1->hname) );	//종목명을 저장한다.
			company->strName = hname;

			CString shcode( (char*)pOutBlock1->shcode, sizeof(pOutBlock1->shcode) );//종목코드를 저장한다.
			company->strJongMok = shcode;

			flagNext = false;

			//해당 종목의 데이터를 받기 위하여 t8413을 요청한다.
			t8413InBlock	pckInBlock;
			FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
			memcpy( pckInBlock.shcode, m_jongmokcode, sizeof(pckInBlock.shcode));	//종목코드 --> 하나의 종목만 받는다.
			memcpy( pckInBlock.gubun, dataType, sizeof( pckInBlock.gubun));			//주기구분(2:일 3:주 4:월)
			memcpy( pckInBlock.qrycnt, quantityData, sizeof( pckInBlock.qrycnt));	//데이터 요청 갯수
			memcpy( pckInBlock.edate, date, sizeof( pckInBlock.edate));				//종료일자 (현재날자)
			memcpy( pckInBlock.comp_yn, "N", sizeof( pckInBlock.comp_yn));			//압축유무 (Y/N)
			EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8413", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );//Request 전송

		//------------ t8430 데이터를 수신시 상장된 모든 주식의 종목코드와 종목명을 구조체에 저장한다. ------------
		} else if(strcmp( lpRecvPacket->szTrCode, "t8430") == 0) {

			LPt8430OutBlock pOutBlock;

			//Non-Block으로 받은 Block의 갯수을 계산한다. 이것은 전체 종목의 갯수와 같다. t8430은 모든 Block들은 한번에 받는다.
			stock->allCompany.quantity = (int)( lpRecvPacket->nDataLength / sizeof(t8430OutBlock));	

			for (i=0; i<stock->allCompany.quantity; i++) {
				pOutBlock = (LPt8430OutBlock)(lpRecvPacket->lpData + (sizeof(t8430OutBlock) * i)); //데이터를 가져오기 위한 Block을 설장한다.

				company = &stock->allCompany.company[i];				//데이터를 저장하기 위한 구조체를 포인터 변수인 company가 가리킨다.

				CString hname( (char*)pOutBlock->hname, sizeof(pOutBlock->hname) );	//종명명을 저장한다.
				company->strName = hname;

				CString shcode( (char*)pOutBlock->shcode, sizeof(pOutBlock->shcode) ); //종목코드를 저장한다.
				company->strJongMok = shcode;

			}

			flagNext = false;	//t8413에서 받는 데이터의 순서를 정하기 위하여 flagNext를 false로 설정한다.
			company = &stock->allCompany.company[indexCompany];		//데이터를 가져오기 위해서는 종목코드가 필요하며, 현재 indexCompany는 0의 값으로서 첫 데이터를 요청하게 된다.

			t8413InBlock	pckInBlock;
			FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
			memcpy( pckInBlock.shcode, company->strJongMok, sizeof( pckInBlock.shcode));	//종목코드
			memcpy( pckInBlock.gubun, dataType, sizeof( pckInBlock.gubun));					//주기구분(2:일 3:주 4:월)
			memcpy( pckInBlock.qrycnt, quantityData, sizeof( pckInBlock.qrycnt));			//요청 데이터 갯수
			memcpy( pckInBlock.edate, date, sizeof( pckInBlock.edate));						//종료일자(현재 날자)
			memcpy( pckInBlock.comp_yn, "N", sizeof( pckInBlock.comp_yn));					//압축하지 않는다. (Y/N)
			EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8413", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// 데이터 전송


		//------------ t8413 데이터 수신시 한 종목의 데이터를 저장하고 다음데이터를 요청한다. -------------
		} else if(!strcmp( lpRecvPacket->szTrCode, "t8413" ) ) { 

			LPt8413OutBlock1 pOutBlock1;

			if (!flagNext) { //연속조회시 첫번째 데이터는 Pass하고 두번째 받는 것이 OutBlock1이다.

				//처음 받은 메시지의 종목코드를 가져온다. 종목코드와 일치하는 종목에 데이터를 저장하기 위해서이다.
				LPt8413OutBlock pOutBlock = (LPt8413OutBlock)lpRecvPacket->lpData;
				CString shcode( (char*)pOutBlock->shcode, sizeof(pOutBlock->shcode) );

				//for loop의 i를 indexCompany부터 시작하는 이유는 불필요한 반복횟수를 줄이기 위해서이다.
				for (i=indexCompany; i>=0; i--) {	//t8413는 연속 데이터를 받기 때문에 처음 받은 데이터의 키 값을 종목코드가 같은 종목에 저장한다.
					if (!shcode.Compare(stock->allCompany.company[i].strJongMok)) {
						stock->allCompany.company[i].keyToRecv = CString((char*)lpRecvPacket->szContKey, sizeof(lpRecvPacket->szContKey));
						break;
					}
				}

				flagNext = true;	//다음에 받는 데이터는 두번째 데이터이기 때문에 flagNext를 true로 설정한다.

			} else { //OutBlock1을 받아서 처리한다.

				//t8314는 연속 데이터를 받기 때문에 연속데이터의 키를 가진 종목에 데이터를 저장하기 위하여 company을 설정한다.
				for (i=indexCompany; i>=0; i--) {
					CString szContKey((char*)lpRecvPacket->szContKey, sizeof(lpRecvPacket->szContKey));
					if (!szContKey.Compare(stock->allCompany.company[i].keyToRecv)) {
						company = &stock->allCompany.company[i];
						break;
					}
				}

				//t8413은 Non-Block 메시지이며, 받은 Block의 갯수를 계산하여 quantity에 저장한다.
				int quantity = (int)(lpRecvPacket->nDataLength / sizeof(t8413OutBlock1));

				for (i=0; i<quantity; i++) {

					pOutBlock1 = (LPt8413OutBlock1)(lpRecvPacket->lpData + (sizeof(t8413OutBlock1) * i));	//데이터를 가져오기 위한 Block을 설장한다.

					company->quantity = quantity;											//데이터의 갯수를 저장한다.

					CString dateData( (char*)pOutBlock1->date, sizeof(pOutBlock1->date) );	//날자을 저장한다.
					company->data[i].date = dateData;

					company->data[i].startVal = changeStringToLong((char*)pOutBlock1->open, 8);			//시가를 저장한다.
					company->data[i].highVal = changeStringToLong((char*)pOutBlock1->high, 8);			//고가를 저장한다.
					company->data[i].lowVal = changeStringToLong((char*)pOutBlock1->low, 8);			//저가를 저장한다.
					company->data[i].lastVal = changeStringToLong((char*)pOutBlock1->close, 8);			//종가를 저장한다.
					company->data[i].vol = changeStringToLong((char*)pOutBlock1->jdiff_vol, 12);		//거래량을 저장한다.
				}

				m_message.Format(_T("%d / %d 의 데이터를 수신중..."), indexCompany, stock->allCompany.quantity); //수신되는 데이터의 상태 메시지 출력
				UpdateData(FALSE);

				Sleep(1000);	// 1초의 Delay를 준다. t8413은 1초에 한번만 요청이 허용된다.

				indexCompany += 1;	//데이터를 받기위한 종목 index를 1 증가시킨다. 다음 종목의 데이터를 요청하기 위해서다.

				//모든 종목의 데이터를 가져왔으면 데이터를 화면에 출력한다.
				if (indexCompany >= stock->allCompany.quantity) {
					StoreDataInFile();
					return 0L;
				}

				flagNext = false;
				company = &stock->allCompany.company[indexCompany];	//요청하는 종목의 종목코드를 가져오기 위하여 새로운 종목으로 포인터 변수 company를 재 설정한다.

				//다음 종목 데이터 요청
				t8413InBlock	pckInBlock;
				FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
				memcpy( pckInBlock.shcode, company->strJongMok, sizeof( pckInBlock.shcode));	//종목코드
				memcpy( pckInBlock.gubun, dataType, sizeof( pckInBlock.gubun));					//주기구분(2:일 3:주 4:월)
				memcpy( pckInBlock.qrycnt, quantityData, sizeof( pckInBlock.qrycnt));			//요청 데이터 갯수
				memcpy( pckInBlock.edate, date, sizeof( pckInBlock.edate));						//종료일자 (현재 날자)
				memcpy( pckInBlock.comp_yn, "N", sizeof( pckInBlock.comp_yn));					//압축하지 않는다.(Y/N)
				EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8413", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) ); // Request 전송
			}
		}
	} else if( wParam == MESSAGE_DATA ) {// 메시지를 받음
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		if (!strMsg.Compare(_T("해당자료가 없습니다."))) {
			m_message.Format(_T("종목코드가 잘못 되었습니다."));	//종목코드가 잘못되었음을 출력한다.
			UpdateData(FALSE);
		}
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
// 에러 메시지를 출력한다.
VOID CGetStockDataDlg::EntryOrderError(INT nRqID)
{
	if( nRqID < 0 )	{
		CString str;
		str.Format(_T("[%d] %s"), nRqID, iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(str);
	}
}

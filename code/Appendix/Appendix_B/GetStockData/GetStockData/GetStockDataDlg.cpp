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
, m_message(_T("������ �������� ��ư�� ��������."))
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

	m_server.AddString(_T("hts.etrade.co.kr"));		//���Ӽ����� �޺��ڽ��� ����Ѵ�.
	m_server.AddString(_T("demo.etrade.co.kr"));
	m_server.SetCurSel(1);

	m_datatype.AddString(_T("Day"));		//������ Ÿ���� �޺��ڽ��� ����Ѵ�.
	m_datatype.AddString(_T("Week"));
	m_datatype.AddString(_T("Month"));
	m_datatype.SetCurSel(0);

	stock = new CStock();					//�����͸� �����ϱ� ���Ͽ� ���Ǵ� CStock Ŭ������ �����Ѵ�.

	CTime t = CTime::GetCurrentTime();		//���� �ð��� �����´�.
	date = t.Format("%Y%m%d");				//���� ���ڸ� �����´�. (YYYYMMDD)

	//////////////// 1. xingAPI �ʱ�ȭ ////////////////
	if( iXingAPI.Init() == FALSE ) {
		CString strPath;
		int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		strPath.ReleaseBuffer( nLen );
		int nFind = strPath.ReverseFind( '\\' );
		if( nFind > 0 ) strPath = strPath.Left( nFind );
		if( iXingAPI.Init( strPath ) == FALSE ) {
			::AfxMessageBox(_T("xingAPI DLL�� Load �� �� �����ϴ�."));
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
// '����' ��ư�� Ŭ���Ǹ� ����Ǵ� �̺�Ʈ �Լ��̴�.
// ������ ������, �α����� �����Ѵ�.
void CGetStockDataDlg::OnBnClickedBtnConnect()
{	
	UpdateData(TRUE);	//ȭ�鿡�� �Էµ� ���� ��������� ����

	if( iXingAPI.IsConnected() )	{// �̹� ���ӵǾ� ������ ������ �����Ѵ�.
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
// '����' ��ư�� Ŭ���Ǹ� ����Ǵ� �̺�Ʈ �Լ��̴�.
// �α׾ƿ� �� ���� ������ �����ϰ� ���α׷��� �����Ѵ�.
void CGetStockDataDlg::OnBnClickedBtnDisconnect()
{
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
// '��������' ��ư�� Ŭ���Ǹ� ����Ǵ� �̺�Ʈ �Լ��̴�.
// ȭ���� ���� ���� �ش��ϴ� �����͸� �����ͼ� ȭ�Ͽ� �����Ѵ�.
void CGetStockDataDlg::OnBnClickedBtnGetdata()
{	
	UpdateData(TRUE);

	//������ Ÿ���� ���ڿ��� �����. ("2":�� "3":�� "4":��)
	dataType[1] = '\0';
	int index = m_datatype.GetCurSel();
	if (index == 0) dataType[0] = '2';
	if (index == 1) dataType[0] = '3';
	if (index == 2) dataType[0] = '4';

	//������ ������ 500 �̻��� ������ �� ����. t8413���� �޴� OutBlock1�� �迭�� �ִ� 500�������̴�.
	if (m_quantity > 500) {
		m_quantity = 500;
		m_message.Format(_T("�����ʹ� �ִ� 500���Դϴ�."));
		UpdateData(FALSE);
		return;
	}

	//������ ������ ���� m_quantity�� 4�ڸ� ���ڿ��� �����. (ex: 250 => "0250")
	quantityData[4] = '\0';
	quantityData[3] = (char)(((int)'0')+(int)(m_quantity%10));
	quantityData[2] = (char)(((int)'0')+(int)((m_quantity%100)/10));
	quantityData[1] = (char)(((int)'0')+(int)((m_quantity%1000)/100));
	quantityData[0] = (char)(((int)'0')+(int)((m_quantity%10000)/1000));

	indexCompany = 0;	//������ index�� 0���� �����Ѵ�.

	if (!m_radio) { // ��ü ���� ������ �ޱ� ���ؼ��� t8413�� ��û�Ѵ�.

		t8430InBlock	pckInBlock;
		FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
		memcpy( pckInBlock.gubun, "0", sizeof( pckInBlock.gubun));
		EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8430", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// ������ ����

	} else { // �� ���� ������ �ޱ� ���ؼ��� t8407�� ��û�ϴµ�, �����α׷������� ������� ��� ���ؼ���.

		stock->allCompany.quantity = 1;	//�� ������ �ޱ� ������ ��ü ������� 1�̴�.

		t8407InBlock	pckInBlock;
		FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
		memcpy( pckInBlock.nrec, "001", sizeof( pckInBlock.nrec));
		memcpy( pckInBlock.shcode, m_jongmokcode, sizeof( pckInBlock.shcode));			//���� �ڵ带 �Է��Ѵ�.
		EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8407", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// ������ ����

	}
}

//--------------------------------------------------------------------------------------------------------------
// ��� �����͸� ������ �� ���α׷��� �������� ����Ǵ� �Լ��μ�, �����͸� ȭ�Ͽ� �����Ѵ�.
void CGetStockDataDlg::StoreDataInFile() 
{
	m_message.Format(_T("���Ͽ� ���� ��..."));	//���Ͽ� �������� �޽����� ����Ѵ�.
	UpdateData(FALSE);

	stock->StoreDataInOneFile(m_output);	//����� ��� �����͸� ���Ͽ� �����Ѵ�.

	m_message.Format(_T("������ �������� �Ϸ�!!!"));//������ �������� �ϷḦ ����Ѵ�.
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------
// Xing���κ��� ȣ��Ǵ� Callback �Լ��μ� �����͸� �޾��� �� ����Ǹ�, �޸𸮿� ����Ǿ� ���޹��� �����͵��� ó���Ѵ�.
LRESULT CGetStockDataDlg::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{

	int i;
	Company *company;	//���� �����͸� �����ϱ� ���� ����ü�� ����Ű�� ������ ����

	if( wParam == REQUEST_DATA ) {

		LPRECV_PACKET lpRecvPacket = (LPRECV_PACKET)lParam;

		//------------ t8407�� �� ���α׷����� �ϳ��� ������ �ޱ� ���Ͽ� ���Ǹ� ������� �������� ���ؼ��̴�. -------------
		if(strcmp( lpRecvPacket->szTrCode, "t8407") == 0) {

			LPt8407OutBlock1 pOutBlock1 = (LPt8407OutBlock1)lpRecvPacket->lpData;

			company = &stock->allCompany.company[0];	//�� ���α׷������� �ϳ��� ���� ��û�� ó���Ǵ� �κ��̹Ƿ� index 0�� ��ġ�� �����͸� �����Ѵ�.

			CString hname( (char*)pOutBlock1->hname, sizeof(pOutBlock1->hname) );	//������� �����Ѵ�.
			company->strName = hname;

			CString shcode( (char*)pOutBlock1->shcode, sizeof(pOutBlock1->shcode) );//�����ڵ带 �����Ѵ�.
			company->strJongMok = shcode;

			flagNext = false;

			//�ش� ������ �����͸� �ޱ� ���Ͽ� t8413�� ��û�Ѵ�.
			t8413InBlock	pckInBlock;
			FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
			memcpy( pckInBlock.shcode, m_jongmokcode, sizeof(pckInBlock.shcode));	//�����ڵ� --> �ϳ��� ���� �޴´�.
			memcpy( pckInBlock.gubun, dataType, sizeof( pckInBlock.gubun));			//�ֱⱸ��(2:�� 3:�� 4:��)
			memcpy( pckInBlock.qrycnt, quantityData, sizeof( pckInBlock.qrycnt));	//������ ��û ����
			memcpy( pckInBlock.edate, date, sizeof( pckInBlock.edate));				//�������� (���糯��)
			memcpy( pckInBlock.comp_yn, "N", sizeof( pckInBlock.comp_yn));			//�������� (Y/N)
			EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8413", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );//Request ����

		//------------ t8430 �����͸� ���Ž� ����� ��� �ֽ��� �����ڵ�� ������� ����ü�� �����Ѵ�. ------------
		} else if(strcmp( lpRecvPacket->szTrCode, "t8430") == 0) {

			LPt8430OutBlock pOutBlock;

			//Non-Block���� ���� Block�� ������ ����Ѵ�. �̰��� ��ü ������ ������ ����. t8430�� ��� Block���� �ѹ��� �޴´�.
			stock->allCompany.quantity = (int)( lpRecvPacket->nDataLength / sizeof(t8430OutBlock));	

			for (i=0; i<stock->allCompany.quantity; i++) {
				pOutBlock = (LPt8430OutBlock)(lpRecvPacket->lpData + (sizeof(t8430OutBlock) * i)); //�����͸� �������� ���� Block�� �����Ѵ�.

				company = &stock->allCompany.company[i];				//�����͸� �����ϱ� ���� ����ü�� ������ ������ company�� ����Ų��.

				CString hname( (char*)pOutBlock->hname, sizeof(pOutBlock->hname) );	//������� �����Ѵ�.
				company->strName = hname;

				CString shcode( (char*)pOutBlock->shcode, sizeof(pOutBlock->shcode) ); //�����ڵ带 �����Ѵ�.
				company->strJongMok = shcode;

			}

			flagNext = false;	//t8413���� �޴� �������� ������ ���ϱ� ���Ͽ� flagNext�� false�� �����Ѵ�.
			company = &stock->allCompany.company[indexCompany];		//�����͸� �������� ���ؼ��� �����ڵ尡 �ʿ��ϸ�, ���� indexCompany�� 0�� �����μ� ù �����͸� ��û�ϰ� �ȴ�.

			t8413InBlock	pckInBlock;
			FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
			memcpy( pckInBlock.shcode, company->strJongMok, sizeof( pckInBlock.shcode));	//�����ڵ�
			memcpy( pckInBlock.gubun, dataType, sizeof( pckInBlock.gubun));					//�ֱⱸ��(2:�� 3:�� 4:��)
			memcpy( pckInBlock.qrycnt, quantityData, sizeof( pckInBlock.qrycnt));			//��û ������ ����
			memcpy( pckInBlock.edate, date, sizeof( pckInBlock.edate));						//��������(���� ����)
			memcpy( pckInBlock.comp_yn, "N", sizeof( pckInBlock.comp_yn));					//�������� �ʴ´�. (Y/N)
			EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8413", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) );// ������ ����


		//------------ t8413 ������ ���Ž� �� ������ �����͸� �����ϰ� ���������͸� ��û�Ѵ�. -------------
		} else if(!strcmp( lpRecvPacket->szTrCode, "t8413" ) ) { 

			LPt8413OutBlock1 pOutBlock1;

			if (!flagNext) { //������ȸ�� ù��° �����ʹ� Pass�ϰ� �ι�° �޴� ���� OutBlock1�̴�.

				//ó�� ���� �޽����� �����ڵ带 �����´�. �����ڵ�� ��ġ�ϴ� ���� �����͸� �����ϱ� ���ؼ��̴�.
				LPt8413OutBlock pOutBlock = (LPt8413OutBlock)lpRecvPacket->lpData;
				CString shcode( (char*)pOutBlock->shcode, sizeof(pOutBlock->shcode) );

				//for loop�� i�� indexCompany���� �����ϴ� ������ ���ʿ��� �ݺ�Ƚ���� ���̱� ���ؼ��̴�.
				for (i=indexCompany; i>=0; i--) {	//t8413�� ���� �����͸� �ޱ� ������ ó�� ���� �������� Ű ���� �����ڵ尡 ���� ���� �����Ѵ�.
					if (!shcode.Compare(stock->allCompany.company[i].strJongMok)) {
						stock->allCompany.company[i].keyToRecv = CString((char*)lpRecvPacket->szContKey, sizeof(lpRecvPacket->szContKey));
						break;
					}
				}

				flagNext = true;	//������ �޴� �����ʹ� �ι�° �������̱� ������ flagNext�� true�� �����Ѵ�.

			} else { //OutBlock1�� �޾Ƽ� ó���Ѵ�.

				//t8314�� ���� �����͸� �ޱ� ������ ���ӵ������� Ű�� ���� ���� �����͸� �����ϱ� ���Ͽ� company�� �����Ѵ�.
				for (i=indexCompany; i>=0; i--) {
					CString szContKey((char*)lpRecvPacket->szContKey, sizeof(lpRecvPacket->szContKey));
					if (!szContKey.Compare(stock->allCompany.company[i].keyToRecv)) {
						company = &stock->allCompany.company[i];
						break;
					}
				}

				//t8413�� Non-Block �޽����̸�, ���� Block�� ������ ����Ͽ� quantity�� �����Ѵ�.
				int quantity = (int)(lpRecvPacket->nDataLength / sizeof(t8413OutBlock1));

				for (i=0; i<quantity; i++) {

					pOutBlock1 = (LPt8413OutBlock1)(lpRecvPacket->lpData + (sizeof(t8413OutBlock1) * i));	//�����͸� �������� ���� Block�� �����Ѵ�.

					company->quantity = quantity;											//�������� ������ �����Ѵ�.

					CString dateData( (char*)pOutBlock1->date, sizeof(pOutBlock1->date) );	//������ �����Ѵ�.
					company->data[i].date = dateData;

					company->data[i].startVal = changeStringToLong((char*)pOutBlock1->open, 8);			//�ð��� �����Ѵ�.
					company->data[i].highVal = changeStringToLong((char*)pOutBlock1->high, 8);			//���� �����Ѵ�.
					company->data[i].lowVal = changeStringToLong((char*)pOutBlock1->low, 8);			//������ �����Ѵ�.
					company->data[i].lastVal = changeStringToLong((char*)pOutBlock1->close, 8);			//������ �����Ѵ�.
					company->data[i].vol = changeStringToLong((char*)pOutBlock1->jdiff_vol, 12);		//�ŷ����� �����Ѵ�.
				}

				m_message.Format(_T("%d / %d �� �����͸� ������..."), indexCompany, stock->allCompany.quantity); //���ŵǴ� �������� ���� �޽��� ���
				UpdateData(FALSE);

				Sleep(1000);	// 1���� Delay�� �ش�. t8413�� 1�ʿ� �ѹ��� ��û�� ���ȴ�.

				indexCompany += 1;	//�����͸� �ޱ����� ���� index�� 1 ������Ų��. ���� ������ �����͸� ��û�ϱ� ���ؼ���.

				//��� ������ �����͸� ���������� �����͸� ȭ�鿡 ����Ѵ�.
				if (indexCompany >= stock->allCompany.quantity) {
					StoreDataInFile();
					return 0L;
				}

				flagNext = false;
				company = &stock->allCompany.company[indexCompany];	//��û�ϴ� ������ �����ڵ带 �������� ���Ͽ� ���ο� �������� ������ ���� company�� �� �����Ѵ�.

				//���� ���� ������ ��û
				t8413InBlock	pckInBlock;
				FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
				memcpy( pckInBlock.shcode, company->strJongMok, sizeof( pckInBlock.shcode));	//�����ڵ�
				memcpy( pckInBlock.gubun, dataType, sizeof( pckInBlock.gubun));					//�ֱⱸ��(2:�� 3:�� 4:��)
				memcpy( pckInBlock.qrycnt, quantityData, sizeof( pckInBlock.qrycnt));			//��û ������ ����
				memcpy( pckInBlock.edate, date, sizeof( pckInBlock.edate));						//�������� (���� ����)
				memcpy( pckInBlock.comp_yn, "N", sizeof( pckInBlock.comp_yn));					//�������� �ʴ´�.(Y/N)
				EntryOrderError( iXingAPI.Request(GetSafeHwnd(), "t8413", &pckInBlock, sizeof(pckInBlock), 0, " ", -1) ); // Request ����
			}
		}
	} else if( wParam == MESSAGE_DATA ) {// �޽����� ����
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		if (!strMsg.Compare(_T("�ش��ڷᰡ �����ϴ�."))) {
			m_message.Format(_T("�����ڵ尡 �߸� �Ǿ����ϴ�."));	//�����ڵ尡 �߸��Ǿ����� ����Ѵ�.
			UpdateData(FALSE);
		}
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
// ���� �޽����� ����Ѵ�.
VOID CGetStockDataDlg::EntryOrderError(INT nRqID)
{
	if( nRqID < 0 )	{
		CString str;
		str.Format(_T("[%d] %s"), nRqID, iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(str);
	}
}

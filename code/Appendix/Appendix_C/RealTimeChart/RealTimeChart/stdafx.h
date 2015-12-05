/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Mar 28, 2014                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Real-Time Bollinger Band & MACD for Option //
/////////////////////////////////////////////////////////////

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS 

#define _AFX_ALL_WARNINGS

#include <afxwin.h>
#include <afxext.h>

#include <afxdisp.h> 

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h> 
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> 
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
#include "IXingAPI.h"

extern IXingAPI iXingAPI;		//Xing�� ����ϱ� ���� dll Ŭ���� IXingAPI�� ���⿡ ���������� ���ǵǾ���.

extern UINT BOLLINGER_AVG;		//Bollinger Band ����� ����ϱ� ���� ��
extern float BOLLINGER_TIME;	//Bollinger Band ����� ����ϱ� ���� ��

extern UINT MACD_EMA_1;			//MACD �����͸� ����ϱ� ���� ù��° ��
extern UINT MACD_EMA_2;			//MACD �����͸� ����ϱ� ���� �ι�° ��
extern UINT MACD_SIGNAL;		//MACD Signal�� ����ϱ� ���� ��

#include "./Packet/t2301.h"		//Xing�� ����ϱ� ���Ͽ� ���Ǵ� header file�� ���⼭ ���ǵǾ���.
#include "./Packet/t8415.h"
#include "./Packet/OC0.h"

float changeStringToFloat(char * strFloat);		//�Ǽ� ���ڿ��� Float�� ��ȯ�Ѵ�.
UINT changeStringTimeToInt(char * strFloat);	//�ð� ���ڿ��� Unsigned Integer�� ��ȯ�Ѵ�.
int getTimeGap(UINT timeA, UINT timeB);			//�ΰ��� �ð��� ���� ���̸� ����Ͽ� ��ȯ�Ѵ�.

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

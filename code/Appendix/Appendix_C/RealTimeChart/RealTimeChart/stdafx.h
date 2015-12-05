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

extern IXingAPI iXingAPI;		//Xing과 통신하기 위한 dll 클래스 IXingAPI는 여기에 전역변수로 정의되었다.

extern UINT BOLLINGER_AVG;		//Bollinger Band 평균을 계산하기 위한 값
extern float BOLLINGER_TIME;	//Bollinger Band 배수를 계산하기 위한 값

extern UINT MACD_EMA_1;			//MACD 데이터를 계산하기 위한 첫번째 값
extern UINT MACD_EMA_2;			//MACD 데이터를 계산하기 위한 두번째 값
extern UINT MACD_SIGNAL;		//MACD Signal을 계산하기 위한 값

#include "./Packet/t2301.h"		//Xing과 통신하기 위하여 사용되는 header file은 여기서 정의되었다.
#include "./Packet/t8415.h"
#include "./Packet/OC0.h"

float changeStringToFloat(char * strFloat);		//실수 문자열을 Float로 변환한다.
UINT changeStringTimeToInt(char * strFloat);	//시간 문자열을 Unsigned Integer로 변환한다.
int getTimeGap(UINT timeA, UINT timeB);			//두개의 시간의 분의 차이를 계산하여 반환한다.

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

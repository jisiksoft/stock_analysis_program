///////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                               //
// Date       : Mar 17, 2013                             //
// eMail      : kimsehoon@hotmail.com                    //
// Description: This Program Gets Stock Data from ETRADE //
///////////////////////////////////////////////////////////

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

#include "./Packet/t8407.h"		//Xing과 통신하기 위하여 사용되는 header file은 여기서 정의되었다.
#include "./Packet/t8413.h"
#include "./Packet/t8430.h"

long changeStringToLong(char * strLong, int length);

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



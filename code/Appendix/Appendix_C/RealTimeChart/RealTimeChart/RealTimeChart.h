
// RealTimeChart.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRealTimeChartApp:
// �� Ŭ������ ������ ���ؼ��� RealTimeChart.cpp�� �����Ͻʽÿ�.
//

class CRealTimeChartApp : public CWinAppEx
{
public:
	CRealTimeChartApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRealTimeChartApp theApp;
// QQFaceDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQQFaceDlgApp:
// �йش����ʵ�֣������ QQFaceDlg.cpp
//

class CQQFaceDlgApp : public CWinApp
{
public:
	CQQFaceDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQQFaceDlgApp theApp;
// ImageBat.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <gdiplus.h>
using namespace Gdiplus;

// CImageBatApp:
// �йش����ʵ�֣������ ImageBat.cpp
//

class CImageBatApp : public CWinApp
{
public:
	CImageBatApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImageBatApp theApp;
// CppClassToCSharp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCppClassToCSharpApp:
// �йش����ʵ�֣������ CppClassToCSharp.cpp
//

class CCppClassToCSharpApp : public CWinApp
{
public:
	CCppClassToCSharpApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCppClassToCSharpApp theApp;
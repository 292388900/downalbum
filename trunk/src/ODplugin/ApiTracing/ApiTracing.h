// ApiTracing.h : ApiTracing DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// extern "C" __declspec(dllexport) BOOL StaticScanDir(char* szPath);
// extern "C" __declspec(dllexport) BOOL StaticScanFile(CString strFileName);

// CApiTracingApp
// �йش���ʵ�ֵ���Ϣ������� ApiTracing.cpp
//

class CApiTracingApp : public CWinApp
{
public:
	CApiTracingApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

CString GetStartPath(HMODULE h);

// ColorCopy.h : ColorCopy DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// extern "C" __declspec(dllexport) BOOL StaticScanDir(char* szPath);
// extern "C" __declspec(dllexport) BOOL StaticScanFile(CString strFileName);

// CColorCopyApp
// �йش���ʵ�ֵ���Ϣ������� ColorCopy.cpp
//

class CColorCopyApp : public CWinApp
{
public:
	CColorCopyApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

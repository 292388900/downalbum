// ColorCopy.h : ColorCopy DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// extern "C" __declspec(dllexport) BOOL StaticScanDir(char* szPath);
// extern "C" __declspec(dllexport) BOOL StaticScanFile(CString strFileName);

// CColorCopyApp
// 有关此类实现的信息，请参阅 ColorCopy.cpp
//

class CColorCopyApp : public CWinApp
{
public:
	CColorCopyApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

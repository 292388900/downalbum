// StarLib.h : StarLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Star.h"
using namespace Star;

// CStarLibApp
// �йش���ʵ�ֵ���Ϣ������� StarLib.cpp
//

class CStarLibApp : public CWinApp
{
public:
	CStarLibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

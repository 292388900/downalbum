// WaveEffect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWaveEffectApp:
// �йش����ʵ�֣������ WaveEffect.cpp
//

class CWaveEffectApp : public CWinApp
{
public:
	CWaveEffectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWaveEffectApp theApp;
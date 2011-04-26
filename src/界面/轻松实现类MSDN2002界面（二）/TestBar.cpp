// TestBar.cpp : implementation file
//

#include "stdafx.h"
#include "dotNET½çÃæ.h"
#include "TestBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestBar

CTestBar::CTestBar()
{
}

CTestBar::~CTestBar()
{
}


BEGIN_MESSAGE_MAP(CTestBar, CDockPageBar)
	//{{AFX_MSG_MAP(CTestBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTestBar message handlers

int CTestBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockPageBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CTestBar::AddPage(CRuntimeClass *pClass, UINT nIDTemplate, LPCTSTR sText, UINT IconID)
{
	CDialog *pDlg = (CDialog*)pClass->CreateObject();
	if(pDlg != NULL)
	{
		if(pDlg->Create(nIDTemplate,this))
		{
			return AddPage(pDlg, sText, IconID);
		}
	}

	return FALSE;
}

// Text_CTreeBT_Demo.h : main header file for the TEXT_CTREEBT_DEMO application
//

#if !defined(AFX_TEXT_CTREEBT_DEMO_H__20482424_06B2_4C03_B658_5A4E81E226B6__INCLUDED_)
#define AFX_TEXT_CTREEBT_DEMO_H__20482424_06B2_4C03_B658_5A4E81E226B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CText_CTreeBT_DemoApp:
// See Text_CTreeBT_Demo.cpp for the implementation of this class
//

class CText_CTreeBT_DemoApp : public CWinApp
{
public:
	CText_CTreeBT_DemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CText_CTreeBT_DemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CText_CTreeBT_DemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT_CTREEBT_DEMO_H__20482424_06B2_4C03_B658_5A4E81E226B6__INCLUDED_)

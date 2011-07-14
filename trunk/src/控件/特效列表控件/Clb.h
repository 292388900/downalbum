// Clb.h : main header file for the CLB application
//

#if !defined(AFX_CLB_H__639ED6D5_739E_4ED3_8529_59B0B47CEFB3__INCLUDED_)
#define AFX_CLB_H__639ED6D5_739E_4ED3_8529_59B0B47CEFB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClbApp:
// See Clb.cpp for the implementation of this class
//

class CClbApp : public CWinApp
{
public:
	CClbApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClbApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClbApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLB_H__639ED6D5_739E_4ED3_8529_59B0B47CEFB3__INCLUDED_)

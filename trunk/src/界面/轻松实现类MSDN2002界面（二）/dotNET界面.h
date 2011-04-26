// dotNET界面.h : main header file for the DOTNET界面 application
//

#if !defined(AFX_DOTNET_H__40C28AC4_FD48_4C62_B26C_7A5C11D9BA03__INCLUDED_)
#define AFX_DOTNET_H__40C28AC4_FD48_4C62_B26C_7A5C11D9BA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDotNETApp:
// See dotNET界面.cpp for the implementation of this class
//

class CDotNETApp : public CWinApp
{
public:
	CDotNETApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDotNETApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDotNETApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOTNET_H__40C28AC4_FD48_4C62_B26C_7A5C11D9BA03__INCLUDED_)

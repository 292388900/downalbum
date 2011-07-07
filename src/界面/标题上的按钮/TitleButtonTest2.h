// TitleButtonTest2.h : main header file for the TITLEBUTTONTEST2 application
//

#if !defined(AFX_TITLEBUTTONTEST2_H__5B7737AB_C5D8_4B0D_84F2_CF64E0E9A3DD__INCLUDED_)
#define AFX_TITLEBUTTONTEST2_H__5B7737AB_C5D8_4B0D_84F2_CF64E0E9A3DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2App:
// See TitleButtonTest2.cpp for the implementation of this class
//

class CTitleButtonTest2App : public CWinApp
{
public:
	CTitleButtonTest2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleButtonTest2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTitleButtonTest2App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEBUTTONTEST2_H__5B7737AB_C5D8_4B0D_84F2_CF64E0E9A3DD__INCLUDED_)

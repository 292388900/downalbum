// QQFaceMenu.h : main header file for the QQFACEMENU application
//

#if !defined(AFX_QQFACEMENU_H__A477CBC6_A163_4DFD_AE70_6B4460E7F6C2__INCLUDED_)
#define AFX_QQFACEMENU_H__A477CBC6_A163_4DFD_AE70_6B4460E7F6C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQQFaceMenuApp:
// See QQFaceMenu.cpp for the implementation of this class

class CQQFaceMenuApp : public CWinApp
{
public:
	CQQFaceMenuApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQFaceMenuApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQQFaceMenuApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQFACEMENU_H__A477CBC6_A163_4DFD_AE70_6B4460E7F6C2__INCLUDED_)

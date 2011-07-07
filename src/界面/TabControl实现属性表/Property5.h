// Property5.h : main header file for the PROPERTY5 application
//

#if !defined(AFX_PROPERTY5_H__0EB8F198_5180_4224_9776_1A8EAA389383__INCLUDED_)
#define AFX_PROPERTY5_H__0EB8F198_5180_4224_9776_1A8EAA389383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProperty5App:
// See Property5.cpp for the implementation of this class
//

class CProperty5App : public CWinApp
{
public:
	CProperty5App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperty5App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProperty5App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTY5_H__0EB8F198_5180_4224_9776_1A8EAA389383__INCLUDED_)

// Gradient.h : main header file for the GRADIENT application
//

#if !defined(AFX_GRADIENT_H__76EA2A51_C951_11D6_B74E_004033A0FB96__INCLUDED_)
#define AFX_GRADIENT_H__76EA2A51_C951_11D6_B74E_004033A0FB96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGradientApp:
// See Gradient.cpp for the implementation of this class
//

class CGradientApp : public CWinApp
{
public:
	CGradientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGradientApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENT_H__76EA2A51_C951_11D6_B74E_004033A0FB96__INCLUDED_)

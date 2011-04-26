// multi_hyperedit.h : main header file for the MULTI_HYPEREDIT application
//

#if !defined(AFX_MULTI_HYPEREDIT_H__4489FA13_DD77_4951_BE34_F284C73CCFCE__INCLUDED_)
#define AFX_MULTI_HYPEREDIT_H__4489FA13_DD77_4951_BE34_F284C73CCFCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMulti_hypereditApp:
// See multi_hyperedit.cpp for the implementation of this class
//

class CMulti_hypereditApp : public CWinApp
{
public:
	CMulti_hypereditApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulti_hypereditApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMulti_hypereditApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTI_HYPEREDIT_H__4489FA13_DD77_4951_BE34_F284C73CCFCE__INCLUDED_)

// XColorPickerXPTest.h : main header file for the XCOLORPICKERXPTEST application
//

#ifndef XCOLORPICKERXPTEST_H
#define XCOLORPICKERXPTEST_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestApp:
// See XColorPickerXPTest.cpp for the implementation of this class
//

class CXColorPickerXPTestApp : public CWinApp
{
public:
	CXColorPickerXPTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorPickerXPTestApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXColorPickerXPTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XCOLORPICKERXPTEST_H

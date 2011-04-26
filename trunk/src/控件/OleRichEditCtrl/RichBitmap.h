// RichBitmap.h : main header file for the RICHBITMAP application
//

#if !defined(AFX_RICHBITMAP_H__4E405409_4436_47FB_B987_69E08422F3D0__INCLUDED_)
#define AFX_RICHBITMAP_H__4E405409_4436_47FB_B987_69E08422F3D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapApp:
// See RichBitmap.cpp for the implementation of this class
//

class CRichBitmapApp : public CWinApp
{
public:
	CRichBitmapApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichBitmapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRichBitmapApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHBITMAP_H__4E405409_4436_47FB_B987_69E08422F3D0__INCLUDED_)

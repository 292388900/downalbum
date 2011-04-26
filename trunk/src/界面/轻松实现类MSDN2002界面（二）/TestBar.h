#if !defined(AFX_TESTBAR_H__022CA4C6_99C2_498D_A641_7F6B2F67426C__INCLUDED_)
#define AFX_TESTBAR_H__022CA4C6_99C2_498D_A641_7F6B2F67426C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestBar.h : header file
//

#include ".\src\DockPageBar.h"

/////////////////////////////////////////////////////////////////////////////
// CTestBar window

class CTestBar : public CDockPageBar
{
// Construction
public:
	CTestBar();
	using CDockPageBar::AddPage;
// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL AddPage(CRuntimeClass* pClass,UINT nIDTemplate, LPCTSTR sText, UINT IconID);
	virtual ~CTestBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBAR_H__022CA4C6_99C2_498D_A641_7F6B2F67426C__INCLUDED_)

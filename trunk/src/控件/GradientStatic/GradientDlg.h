// GradientDlg.h : header file
//

#if !defined(AFX_GRADIENTDLG_H__76EA2A53_C951_11D6_B74E_004033A0FB96__INCLUDED_)
#define AFX_GRADIENTDLG_H__76EA2A53_C951_11D6_B74E_004033A0FB96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GradientStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CGradientDlg dialog

class CGradientDlg : public CDialog
{
// Construction
public:
	~CGradientDlg();
	CGradientDlg(CWnd* pParent = NULL);	// standard constructor
	CFont* m_pBoldFont;

// Dialog Data
	//{{AFX_DATA(CGradientDlg)
	enum { IDD = IDD_GRADIENT_DIALOG };
	CGradientStatic	m_cBar2;
	CGradientStatic	m_cBar1;
	CGradientStatic	m_cExample2;
	CButton	m_cTH;
	CGradientStatic	m_cExample;
	CGradientStatic	m_cTitle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGradientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENTDLG_H__76EA2A53_C951_11D6_B74E_004033A0FB96__INCLUDED_)

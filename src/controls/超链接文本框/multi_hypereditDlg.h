// multi_hypereditDlg.h : header file
//

#if !defined(AFX_MULTI_HYPEREDITDLG_H__C1E38A09_3F2D_451D_AB42_A920D8ACE847__INCLUDED_)
#define AFX_MULTI_HYPEREDITDLG_H__C1E38A09_3F2D_451D_AB42_A920D8ACE847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hyperedit/hyperedit.h"

/////////////////////////////////////////////////////////////////////////////
// CMulti_hypereditDlg dialog

class CMulti_hypereditDlg : public CDialog
{
// Construction
public:
	CMulti_hypereditDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMulti_hypereditDlg)
	enum { IDD = IDD_MULTI_HYPEREDIT_DIALOG };
	CHyperEdit	m_edit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulti_hypereditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMulti_hypereditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTI_HYPEREDITDLG_H__C1E38A09_3F2D_451D_AB42_A920D8ACE847__INCLUDED_)

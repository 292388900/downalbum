// Property5Dlg.h : header file
//

#if !defined(AFX_PROPERTY5DLG_H__D21CF3E0_5910_4D67_B327_3C53444A94B2__INCLUDED_)
#define AFX_PROPERTY5DLG_H__D21CF3E0_5910_4D67_B327_3C53444A94B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProperty5Dlg dialog
#include "TabSheet.h"
#include "Page1.h"	// Added by ClassView
#include "Page2.h"	// Added by ClassView

class CProperty5Dlg : public CDialog
{
// Construction
public:
	CPage2 m_page2;
	CPage1 m_page1;
	CProperty5Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CProperty5Dlg)
	enum { IDD = IDD_PROPERTY5_DIALOG };
	CTabSheet	m_sheet;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperty5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProperty5Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTY5DLG_H__D21CF3E0_5910_4D67_B327_3C53444A94B2__INCLUDED_)

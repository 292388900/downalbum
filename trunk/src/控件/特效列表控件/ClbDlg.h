// ClbDlg.h : header file
//

#if !defined(AFX_CLBDLG_H__B9204E97_9BAB_4915_B1F9_4B0A8D754BF8__INCLUDED_)
#define AFX_CLBDLG_H__B9204E97_9BAB_4915_B1F9_4B0A8D754BF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_CLB_DIALOG };
	CTaskListBox	m_lbTxt;
	CTaskListBox	m_lbTask;
	//}}AFX_DATA

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	LRESULT OnTaskClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLBDLG_H__B9204E97_9BAB_4915_B1F9_4B0A8D754BF8__INCLUDED_)

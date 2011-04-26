// Text_CTreeBT_DemoDlg.h : header file
//

#if !defined(AFX_TEXT_CTREEBT_DEMODLG_H__139BB72E_D7CD_4F34_BE2B_976572337A1C__INCLUDED_)
#define AFX_TEXT_CTREEBT_DEMODLG_H__139BB72E_D7CD_4F34_BE2B_976572337A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CText_CTreeBT_DemoDlg dialog

#include "TreeCtrlBT.h"

class CText_CTreeBT_DemoDlg : public CDialog
{
// Construction
public:
	CText_CTreeBT_DemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CText_CTreeBT_DemoDlg();

	void InitTree();
	void ReSizeTree();
// Dialog Data
	//{{AFX_DATA(CText_CTreeBT_DemoDlg)
	enum { IDD = IDD_TEXT_CTREEBT_DEMO_DIALOG };
	CTreeCtrlBT	m_tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CText_CTreeBT_DemoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CText_CTreeBT_DemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRowline();
	afx_msg void OnRootbk();
	afx_msg void OnUpdatebitmap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT_CTREEBT_DEMODLG_H__139BB72E_D7CD_4F34_BE2B_976572337A1C__INCLUDED_)

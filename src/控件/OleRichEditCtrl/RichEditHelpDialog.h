#if !defined(AFX_RICHEDITHELPDIALOG_H__33B4B342_0E57_4846_B8BA_4BC30C5D8A0F__INCLUDED_)
#define AFX_RICHEDITHELPDIALOG_H__33B4B342_0E57_4846_B8BA_4BC30C5D8A0F__INCLUDED_

#include "OleRichEditCtrl.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichEditHelpDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRichEditHelpDialog dialog

class CRichEditHelpDialog : public CDialog
{
// Construction
public:
	CRichEditHelpDialog(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bAutoDelete;		// manually set this to TRUE for auto deletion of heap-created modal dialogs
	int m_iOffsetOkButton;		// offset used in OnSize handler
	CRect m_rcOffsetRichEdit;	// offset used in OnSize handler
	COleRichEditCtrl m_RichEditCtrl;

// Dialog Data
	//{{AFX_DATA(CRichEditHelpDialog)
	enum { IDD = IDD_OLE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditHelpDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRichEditHelpDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEDITHELPDIALOG_H__33B4B342_0E57_4846_B8BA_4BC30C5D8A0F__INCLUDED_)

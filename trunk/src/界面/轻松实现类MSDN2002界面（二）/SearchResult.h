#if !defined(AFX_SEARCHRESULT_H__2CBA6C28_4EAF_4F9D_A9E2_928646C30420__INCLUDED_)
#define AFX_SEARCHRESULT_H__2CBA6C28_4EAF_4F9D_A9E2_928646C30420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchResult dialog

class CSearchResult : public CDialog
{
// Construction
public:
	CSearchResult(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CSearchResult)
// Dialog Data
	//{{AFX_DATA(CSearchResult)
	enum { IDD = IDD_OLE_PROPPAGE_LARGE };
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHRESULT_H__2CBA6C28_4EAF_4F9D_A9E2_928646C30420__INCLUDED_)

// TimeDlg.h : header file
//

#if !defined(AFX_TIMEDLG_H__58E49D0D_181A_46E1_9553_A601C1FC8A4B__INCLUDED_)
#define AFX_TIMEDLG_H__58E49D0D_181A_46E1_9553_A601C1FC8A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

class CTimeDlg : public CDialog
{
// Construction
public:
	void FormatUpdatedTime (SYSTEMTIME * pstOld, SYSTEMTIME * pstNew);
	void ChangeSystemTime ();
	void EditPrintf (TCHAR * szFormat,...);
	CTimeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeDlg)
	enum { IDD = IDD_TIME_DIALOG };
	CEdit	m_edit;
	CListCtrl	m_list;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;



	// Generated message map functions
	//{{AFX_MSG(CTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSocketNotify(WPARAM wp,LPARAM lp);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void OnDblclkList1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEDLG_H__58E49D0D_181A_46E1_9553_A601C1FC8A4B__INCLUDED_)

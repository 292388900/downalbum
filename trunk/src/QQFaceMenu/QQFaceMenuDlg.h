// QQFaceMenuDlg.h : header file
//

#if !defined(AFX_QQFACEMENUDLG_H__A459121B_8451_422F_8961_56B08A8B3A80__INCLUDED_)
#define AFX_QQFACEMENUDLG_H__A459121B_8451_422F_8961_56B08A8B3A80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQQFaceMenuDlg dialog

class CQQFaceMenuDlg : public CDialog
{
private:
	CImageList	ImgQQFace;
// Construction
public:
	CQQFaceMenuDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQQFaceMenuDlg)
	enum { IDD = IDD_QQFACEMENU_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQFaceMenuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQQFaceMenuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnShowFaceMenu();
	afx_msg void OnMeasureItem(int nIDCtl,LPMEASUREITEMSTRUCT lpmis);
	afx_msg void OnDrawItem(int nIDCtl,LPDRAWITEMSTRUCT lpdis);
	afx_msg void OnFaceMenuClicked(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQFACEMENUDLG_H__A459121B_8451_422F_8961_56B08A8B3A80__INCLUDED_)

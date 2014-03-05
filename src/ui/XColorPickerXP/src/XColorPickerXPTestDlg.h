// XColorPickerXPTestDlg.h : header file
//

#ifndef XCOLORPICKERXPTESTDLG_H
#define XCOLORPICKERXPTESTDLG_H

#include "XColorStatic.h"
#include "XColorPickerXP.h"

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestDlg dialog

class CXColorPickerXPTestDlg : public CDialog
{
// Construction
public:
	CXColorPickerXPTestDlg(CWnd* pParent = NULL);	// standard constructor
	~CXColorPickerXPTestDlg();

// Dialog Data
	//{{AFX_DATA(CXColorPickerXPTestDlg)
	enum { IDD = IDD_XCOLORPICKERXPTEST_DIALOG };
	CXColorStatic	m_stcColorSample;
	CXColorPickerXP	m_ctlBackgroundColor;
	CXColorPickerXP	m_ctlTextColor;
	//}}AFX_DATA

	COLORREF m_rgbText;
	COLORREF m_rgbBackground;

	CString m_strTextCustomColors;
	CString m_strBackgroundCustomColors;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorPickerXPTestDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CFont m_font;

	void LoadColors();
	void SaveColors();

	// Generated message map functions
	//{{AFX_MSG(CXColorPickerXPTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnColorChange(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XCOLORPICKERXPTESTDLG_H

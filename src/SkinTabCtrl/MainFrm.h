#if !defined(AFX_MAINFRM_H__821F164C_F183_4981_880F_1B5E4A02A66E__INCLUDED_)
#define AFX_MAINFRM_H__821F164C_F183_4981_880F_1B5E4A02A66E__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DemoDlg.h"


class CMainFrame : public CFrameWnd
{public:
	CMainFrame();
	virtual ~CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
		// 
	TabCtrlEx<TabCtrlStyle_VS2003_client> m_TabCtrl;
	CListCtrl m_List1,m_List2,m_List3,m_List4,m_List5;
		// 
	DemoDlg m_DemoDlg;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnUpdateTestdlg(CCmdUI *pCmdUI);
	afx_msg void OnTestdlg();
		// 
	afx_msg void OnUpdateLayoutTop(CCmdUI *pCmdUI);
	afx_msg void OnLayoutTop();
	afx_msg void OnUpdateLayoutBottom(CCmdUI *pCmdUI);
	afx_msg void OnLayoutBottom();
	afx_msg void OnUpdateBehaviorScaling(CCmdUI *pCmdUI);
	afx_msg void OnBehaviorScaling();
	afx_msg void OnUpdateBehaviorScrolling(CCmdUI *pCmdUI);
	afx_msg void OnBehaviorScrolling();
	afx_msg void OnUpdateEqualsize(CCmdUI *pCmdUI);
	afx_msg void OnEqualsize();
	afx_msg void OnUpdateDragtabs(CCmdUI *pCmdUI);
	afx_msg void OnDragtabs();
	afx_msg void OnUpdateShowclosebutton(CCmdUI *pCmdUI);
	afx_msg void OnShowclosebutton();
	afx_msg void OnUpdateShowmenubutton(CCmdUI *pCmdUI);
	afx_msg void OnShowmenubutton();
	afx_msg void OnUpdateShowscrollbuttons(CCmdUI *pCmdUI);
	afx_msg void OnShowscrollbuttons();
	afx_msg void OnUpdateShowclientedgeborder(CCmdUI *pCmdUI);
	afx_msg void OnShowclientedgeborder();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__821F164C_F183_4981_880F_1B5E4A02A66E__INCLUDED_)

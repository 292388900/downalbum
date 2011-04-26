/////////////////////////////////////////////////////////////////////////
//
// CAutoHideBar            Version 1.2
//
// Created: Mar 30, 2004
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 2004 by Cuick. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// Cuick@163.net
/////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOHIDEBAR_H__FD8E1698_F985_4904_8249_4D92F37ECF80__INCLUDED_)
#define AFX_AUTOHIDEBAR_H__FD8E1698_F985_4904_8249_4D92F37ECF80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoHideBar.h : header file
//

#include "DockPageBar.h"

/////////////////////////////////////////////////////////////////////////////
// CAHFloatWnd window
class CHideItem;

class CAHFloatWnd : public CWnd
{
// Construction
public:
	CAHFloatWnd();

// Attributes
public:
	DWORD			m_dwStyle;
	CCloseButton	m_biHide;
	CStudButton		m_stud;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAHFloatWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CHideItem* m_HideItem;
	void OnStudClick();
	CFont m_font;
	CString m_Title;
	BOOL m_isActive;
	virtual ~CAHFloatWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAHFloatWnd)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////////////
// CHidePageItem
class CHidePageItem : public CPageItem
{
public:
	BOOL	m_isActive;
public:
	CHidePageItem();
	virtual ~CHidePageItem();
	void Draw(CDC *pDC, BOOL bHorz);
};

/////////////////////////////////////////////////////////////////////////////////////
// CHideItem
class CHideItem  
{
public:
	CRect			m_rect;
	CDockPageBar*	m_pDockPageBar;
	CAHFloatWnd		m_wndFloat;
	CPtrArray		m_arrPags;
	CRect			m_oldWndRect;
	DWORD			m_dwStyle;
	int				m_nActivePage;
	CWnd*			m_pAutoHideBar;
public:
	void RemoveAll();
	void Dock();
	void Show(int nShow);
	BOOL OnMouseMove(CPoint pt);
	void UpDateSize(CDC* pDC, CRect* rect);
	void AddPageItem(CDockPageBar* pPageBar, CWnd* pWnd, DWORD dwStyle);
	CHideItem();
	virtual ~CHideItem();
	void Draw(CDC *pDC);

};

/////////////////////////////////////////////////////////////////////////////
// CAutoHideBar window

class CAutoHideBar : public CControlBar
{
// Construction
public:
	CAutoHideBar();

// Attributes
public:
	CPtrList		m_listBars;
	CSize			m_size;
	CFont			m_Font;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoHideBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateBar();
	void GetClientRect(CRect* rect);
	void UpDateSize();
	void HidePageBar(CDockPageBar* pDockPageBar);
	void DrawItem(CDC* pDC);
	DWORD m_dwStyle;
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);
	virtual ~CAutoHideBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoHideBar)
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAutoHideFrame frame

class CAutoHideFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CAutoHideFrame)
protected:
	CAutoHideFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void EnableDocking(DWORD dwDockStyle);

protected:
	static const DWORD dwHideBarMap[4][2];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoHideFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAutoHideFrame();

	// Generated message map functions
	//{{AFX_MSG(CAutoHideFrame)
	afx_msg LRESULT OnAutoHideOn(WPARAM, LPARAM);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOHIDEBAR_H__FD8E1698_F985_4904_8249_4D92F37ECF80__INCLUDED_)

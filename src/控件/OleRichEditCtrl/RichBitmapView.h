// RichBitmapView.h : interface of the CRichBitmapView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHBITMAPVIEW_H__42287AB4_A1C6_4C14_835C_748D1B099386__INCLUDED_)
#define AFX_RICHBITMAPVIEW_H__42287AB4_A1C6_4C14_835C_748D1B099386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRichBitmapView : public CView
{
protected: // create from serialization only
	CRichBitmapView();
	DECLARE_DYNCREATE(CRichBitmapView)

// Attributes
public:
	CRichBitmapDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichBitmapView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichBitmapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRichBitmapView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RichBitmapView.cpp
inline CRichBitmapDoc* CRichBitmapView::GetDocument()
   { return (CRichBitmapDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHBITMAPVIEW_H__42287AB4_A1C6_4C14_835C_748D1B099386__INCLUDED_)

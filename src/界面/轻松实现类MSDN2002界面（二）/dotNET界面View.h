// dotNET界面View.h : interface of the CDotNETView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTNETVIEW_H__A65A2A49_F64B_4267_A69E_5849A765579F__INCLUDED_)
#define AFX_DOTNETVIEW_H__A65A2A49_F64B_4267_A69E_5849A765579F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDotNETView : public CView
{
protected: // create from serialization only
	CDotNETView();
	DECLARE_DYNCREATE(CDotNETView)

// Attributes
public:
	CDotNETDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDotNETView)
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
	virtual ~CDotNETView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDotNETView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dotNET界面View.cpp
inline CDotNETDoc* CDotNETView::GetDocument()
   { return (CDotNETDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOTNETVIEW_H__A65A2A49_F64B_4267_A69E_5849A765579F__INCLUDED_)

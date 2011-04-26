// TitleButtonTest2Doc.h : interface of the CTitleButtonTest2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TITLEBUTTONTEST2DOC_H__517CDE96_8BEA_430E_9272_13F690751C74__INCLUDED_)
#define AFX_TITLEBUTTONTEST2DOC_H__517CDE96_8BEA_430E_9272_13F690751C74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTitleButtonTest2Doc : public CDocument
{
protected: // create from serialization only
	CTitleButtonTest2Doc();
	DECLARE_DYNCREATE(CTitleButtonTest2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleButtonTest2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTitleButtonTest2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTitleButtonTest2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEBUTTONTEST2DOC_H__517CDE96_8BEA_430E_9272_13F690751C74__INCLUDED_)

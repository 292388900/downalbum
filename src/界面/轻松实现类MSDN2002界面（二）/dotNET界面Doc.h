// dotNET½çÃæDoc.h : interface of the CDotNETDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTNETDOC_H__9159DA36_9555_4D38_ACAF_199BD06C5AD6__INCLUDED_)
#define AFX_DOTNETDOC_H__9159DA36_9555_4D38_ACAF_199BD06C5AD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDotNETDoc : public CDocument
{
protected: // create from serialization only
	CDotNETDoc();
	DECLARE_DYNCREATE(CDotNETDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDotNETDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDotNETDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDotNETDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOTNETDOC_H__9159DA36_9555_4D38_ACAF_199BD06C5AD6__INCLUDED_)

// TitleButtonTest2Doc.cpp : implementation of the CTitleButtonTest2Doc class
//

#include "stdafx.h"
#include "TitleButtonTest2.h"

#include "TitleButtonTest2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2Doc

IMPLEMENT_DYNCREATE(CTitleButtonTest2Doc, CDocument)

BEGIN_MESSAGE_MAP(CTitleButtonTest2Doc, CDocument)
	//{{AFX_MSG_MAP(CTitleButtonTest2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2Doc construction/destruction

CTitleButtonTest2Doc::CTitleButtonTest2Doc()
{
	// TODO: add one-time construction code here

}

CTitleButtonTest2Doc::~CTitleButtonTest2Doc()
{
}

BOOL CTitleButtonTest2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2Doc serialization

void CTitleButtonTest2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2Doc diagnostics

#ifdef _DEBUG
void CTitleButtonTest2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTitleButtonTest2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2Doc commands

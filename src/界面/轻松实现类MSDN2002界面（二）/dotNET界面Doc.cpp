// dotNET界面Doc.cpp : implementation of the CDotNETDoc class
//

#include "stdafx.h"
#include "dotNET界面.h"

#include "dotNET界面Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDotNETDoc

IMPLEMENT_DYNCREATE(CDotNETDoc, CDocument)

BEGIN_MESSAGE_MAP(CDotNETDoc, CDocument)
	//{{AFX_MSG_MAP(CDotNETDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDotNETDoc construction/destruction

CDotNETDoc::CDotNETDoc()
{
	// TODO: add one-time construction code here

}

CDotNETDoc::~CDotNETDoc()
{
}

BOOL CDotNETDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDotNETDoc serialization

void CDotNETDoc::Serialize(CArchive& ar)
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
// CDotNETDoc diagnostics

#ifdef _DEBUG
void CDotNETDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDotNETDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDotNETDoc commands

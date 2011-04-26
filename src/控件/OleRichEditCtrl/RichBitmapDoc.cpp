// RichBitmapDoc.cpp : implementation of the CRichBitmapDoc class
//

#include "stdafx.h"
#include "RichBitmap.h"

#include "RichBitmapDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapDoc

IMPLEMENT_DYNCREATE(CRichBitmapDoc, CDocument)

BEGIN_MESSAGE_MAP(CRichBitmapDoc, CDocument)
	//{{AFX_MSG_MAP(CRichBitmapDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapDoc construction/destruction

CRichBitmapDoc::CRichBitmapDoc()
{
	// TODO: add one-time construction code here

}

CRichBitmapDoc::~CRichBitmapDoc()
{
}

BOOL CRichBitmapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRichBitmapDoc serialization

void CRichBitmapDoc::Serialize(CArchive& ar)
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
// CRichBitmapDoc diagnostics

#ifdef _DEBUG
void CRichBitmapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRichBitmapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapDoc commands

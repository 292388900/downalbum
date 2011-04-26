// dotNET界面View.cpp : implementation of the CDotNETView class
//

#include "stdafx.h"
#include "dotNET界面.h"

#include "dotNET界面Doc.h"
#include "dotNET界面View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDotNETView

IMPLEMENT_DYNCREATE(CDotNETView, CView)

BEGIN_MESSAGE_MAP(CDotNETView, CView)
	//{{AFX_MSG_MAP(CDotNETView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDotNETView construction/destruction

CDotNETView::CDotNETView()
{
	// TODO: add construction code here

}

CDotNETView::~CDotNETView()
{
}

BOOL CDotNETView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDotNETView drawing

void CDotNETView::OnDraw(CDC* pDC)
{
	CDotNETDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDotNETView printing

BOOL CDotNETView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDotNETView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDotNETView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDotNETView diagnostics

#ifdef _DEBUG
void CDotNETView::AssertValid() const
{
	CView::AssertValid();
}

void CDotNETView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDotNETDoc* CDotNETView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDotNETDoc)));
	return (CDotNETDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDotNETView message handlers

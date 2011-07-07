// TitleButtonTest2View.cpp : implementation of the CTitleButtonTest2View class
//

#include "stdafx.h"
#include "TitleButtonTest2.h"

#include "TitleButtonTest2Doc.h"
#include "TitleButtonTest2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2View

IMPLEMENT_DYNCREATE(CTitleButtonTest2View, CView)

BEGIN_MESSAGE_MAP(CTitleButtonTest2View, CView)
	//{{AFX_MSG_MAP(CTitleButtonTest2View)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2View construction/destruction

CTitleButtonTest2View::CTitleButtonTest2View()
{
	// TODO: add construction code here

}

CTitleButtonTest2View::~CTitleButtonTest2View()
{
}

BOOL CTitleButtonTest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2View drawing

void CTitleButtonTest2View::OnDraw(CDC* pDC)
{
	CTitleButtonTest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2View printing

BOOL CTitleButtonTest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTitleButtonTest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTitleButtonTest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2View diagnostics

#ifdef _DEBUG
void CTitleButtonTest2View::AssertValid() const
{
	CView::AssertValid();
}

void CTitleButtonTest2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTitleButtonTest2Doc* CTitleButtonTest2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTitleButtonTest2Doc)));
	return (CTitleButtonTest2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTitleButtonTest2View message handlers

// RichBitmapView.cpp : implementation of the CRichBitmapView class
//

#include "stdafx.h"
#include "RichBitmap.h"

#include "RichBitmapDoc.h"
#include "RichBitmapView.h"

#include "RichEditHelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapView

IMPLEMENT_DYNCREATE(CRichBitmapView, CView)

BEGIN_MESSAGE_MAP(CRichBitmapView, CView)
	//{{AFX_MSG_MAP(CRichBitmapView)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapView construction/destruction

CRichBitmapView::CRichBitmapView()
{
	// TODO: add construction code here

}

CRichBitmapView::~CRichBitmapView()
{
}

BOOL CRichBitmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapView drawing

void CRichBitmapView::OnDraw(CDC* pDC)
{
	CRichBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect rc;

	rc.SetRectEmpty();

	int iHeight = pDC->DrawText( _T("Left-click to display \"Help\" dialog modelessly"),
		&rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_CALCRECT );

	GetClientRect( &rc );

	rc.OffsetRect( 0, -iHeight );
	pDC->DrawText( _T("Left-click to display \"Help\" dialog modelessly"),
		&rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

	rc.OffsetRect( 0, 2*iHeight );
	pDC->DrawText( _T("Right-click to display it modally"),
		&rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
}

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapView printing

BOOL CRichBitmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRichBitmapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRichBitmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapView diagnostics

#ifdef _DEBUG
void CRichBitmapView::AssertValid() const
{
	CView::AssertValid();
}

void CRichBitmapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRichBitmapDoc* CRichBitmapView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRichBitmapDoc)));
	return (CRichBitmapDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRichBitmapView message handlers

void CRichBitmapView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Display dialog modelessly

	CRichEditHelpDialog* pDlg = new CRichEditHelpDialog;

	pDlg->m_bAutoDelete = TRUE;	// forces the dialog to delete itself when it closes
	pDlg->Create( IDD_OLE_DIALOG, this );
	pDlg->ShowWindow( SW_SHOW );

	CView::OnLButtonUp(nFlags, point);
}

void CRichBitmapView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Display dialog modally

	CRichEditHelpDialog dlg;
	dlg.DoModal();
	
	CView::OnRButtonUp(nFlags, point);
}

// RichEditHelpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RichBitmap.h"
#include "RichEditHelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditHelpDialog dialog


CRichEditHelpDialog::CRichEditHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRichEditHelpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRichEditHelpDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iOffsetOkButton = 0;
	m_rcOffsetRichEdit.SetRectEmpty();
	m_bAutoDelete = FALSE;
}


void CRichEditHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRichEditHelpDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRichEditHelpDialog, CDialog)
	//{{AFX_MSG_MAP(CRichEditHelpDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditHelpDialog message handlers

BOOL CRichEditHelpDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CWnd* pWnd = GetDlgItem( IDC_STATIC_PLACEHOLDER );
	CRect rcCtrl;
	pWnd->GetClientRect( &rcCtrl );
	pWnd->ClientToScreen( &rcCtrl );
	pWnd->DestroyWindow();

	ScreenToClient( &rcCtrl );
	DWORD dwStyle = WS_CHILD|WS_BORDER|WS_VSCROLL|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN;
	m_RichEditCtrl.Create( dwStyle, rcCtrl, this, 0x1245 );

	m_RichEditCtrl.StreamInFromResource( IDR_RTF_TEXT1, "RTF_TEXT" );

	// store positions of RichEditCtrl and OK button relative to parent
	// (for proportional re-sizing and re-positioning in OnSize)

	CRect rcOkBtn, rcThis;

	GetClientRect( &rcThis );
	m_rcOffsetRichEdit.SetRect( rcCtrl.left, rcCtrl.top,
		rcThis.right-rcCtrl.right, rcThis.bottom-rcCtrl.bottom );

	GetDlgItem( IDOK )->GetWindowRect( &rcOkBtn );
	ScreenToClient( &rcOkBtn );
	m_iOffsetOkButton = rcThis.bottom-rcOkBtn.top;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CRichEditHelpDialog::OnSize(UINT nType, int cx, int cy) 
{
	// don't call base class 
	// CDialog::OnSize(nType, cx, cy);

	// OnSize is called for the dialog before its child windows are created
	// so check before trying to resize or reposition the controls

	if ( m_RichEditCtrl.m_hWnd == NULL ) return;


	// re-position OK button and re-size the rich edit control

	CRect rcCtrl, rcOkBtn;

	rcCtrl.SetRect( 0, 0, cx, cy );
	rcCtrl.DeflateRect( m_rcOffsetRichEdit );
	m_RichEditCtrl.MoveWindow( &rcCtrl, TRUE );

	CWnd* pOkBtn = GetDlgItem( IDOK );
	pOkBtn->GetClientRect( &rcOkBtn );
	CPoint lefttop( (int)((cx-rcOkBtn.Width())/2), cy-m_iOffsetOkButton );
	CPoint rightbottom( rcOkBtn.Width(), rcOkBtn.Height() );
	rightbottom += lefttop;
	pOkBtn->MoveWindow( CRect( lefttop, rightbottom), TRUE );

}

void CRichEditHelpDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	// call base calss first, or the CWnd object will not yet be detached
	// from the Windows window

	CDialog::PostNcDestroy();

	if ( m_bAutoDelete ) delete this;
}

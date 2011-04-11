// QQFaceMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QQFaceMenu.h"
#include "QQFaceMenuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQQFaceMenuDlg dialog

CQQFaceMenuDlg::CQQFaceMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQFaceMenuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQQFaceMenuDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQFaceMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQQFaceMenuDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQQFaceMenuDlg, CDialog)
	//{{AFX_MSG_MAP(CQQFaceMenuDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnBtnShowFaceMenu)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_COMMAND_RANGE(0x785,0x7B6,OnFaceMenuClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQFaceMenuDlg message handlers

BOOL CQQFaceMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CGdiObject	GdiObj;
	ImgQQFace.Create(20,20,25,50,1);
	GdiObj.Attach((HIMAGELIST)LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_QQFace)));
	ImageList_AddMasked(ImgQQFace.m_hImageList,(HBITMAP)GdiObj.m_hObject,0X808000);//CYAN
	GdiObj.DeleteObject();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQQFaceMenuDlg::OnPaint() 
{
	CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQQFaceMenuDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void Func()
{

}
DWORD _CallBack = (DWORD)Func;

void CQQFaceMenuDlg::OnBtnShowFaceMenu() 
{
	// TODO: Add your control notification handler code here
	CMenu		popFaceMenu;
	POINT		point;
	
	popFaceMenu.Attach(CreatePopupMenu());
	
	int flags = MF_BYCOMMAND|MF_ENABLED|MF_STRING|MF_OWNERDRAW;
	for (int col=0;col<10;col++)
	{
		int index;
		
		for (int row=0;row<5;row++)
		{
			index=row*10+col;			
			AppendMenu(popFaceMenu.m_hMenu,flags,index+0x785,NULL);
			flags = MF_BYCOMMAND|MF_ENABLED|MF_STRING|MF_OWNERDRAW;
		}
		flags|=MF_MENUBREAK;
	}
	
	GetCursorPos(&point);
	
	popFaceMenu.TrackPopupMenu(0x62,point.x,point.y,AfxGetApp()->GetMainWnd());
}

void CQQFaceMenuDlg::OnMeasureItem(int nIDCtl,LPMEASUREITEMSTRUCT lpmis)
{
	if (lpmis->CtlType==ODT_MENU)
	{
		lpmis->itemHeight=20+2;
		lpmis->itemWidth=10;
	}
}

void CQQFaceMenuDlg::OnDrawItem(int nIDCtl,LPDRAWITEMSTRUCT lpdis)
{
	int index;
	CDC dc;
	POINT pt;
	
	
	CBrush *pBrush = new CBrush(::GetSysColor((lpdis->itemState & ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_MENU));
	dc.Attach(lpdis->hDC);
	dc.FrameRect(&(lpdis->rcItem),pBrush);
	delete pBrush;
	
	if (lpdis->CtlType==ODT_MENU)
	{
		
		index=lpdis->itemID-0x785;
		pt.x=lpdis->rcItem.left+1;
		pt.y=lpdis->rcItem.top+1;
		ImgQQFace.Draw(&dc,index,pt,ILD_NORMAL);
	}
	dc.Detach();
}

void CQQFaceMenuDlg::OnFaceMenuClicked(UINT nID)
{
	CClientDC dc(this);
	POINT pt;

	pt.x=0;
	pt.y=0;
	ImgQQFace.Draw(&dc,nID-0x785,pt,ILD_NORMAL);
}
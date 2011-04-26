// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TitleButtonTest2.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		ON_BN_CLICKED(ID_BUTTON1, OnNcButton1Clicked)
		ON_BN_CLICKED(ID_BUTTON2, OnNcButton2Clicked)
		ON_BN_CLICKED(ID_BUTTON3, OnNcButton3Clicked)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	POINT ptBtn1Offset = {120, 10};
	POINT ptBtn2Offset = {20, 32};
	POINT ptBtn3Offset = {190, 10};
	SIZE sizeBtn1 = {60, 14};
	SIZE sizeBtn2 = {16, 14};
	SIZE sizeBtn3 = {16, 14};
	
	m_NcButton1.Create("", this->m_hWnd, ptBtn1Offset, sizeBtn1, ID_BUTTON1);
	m_NcButton2.Create("", this->m_hWnd, ptBtn2Offset, sizeBtn2, ID_BUTTON2);
	m_NcButton3.Create("", this->m_hWnd, ptBtn3Offset, sizeBtn3, ID_BUTTON3);
	
	m_NcButton1.SetText("VC知识库", "宋体", 12);
	m_NcButton1.SetTooltip("VC知识库欢迎您!");
	m_NcButton2.SetButtonBitmap(IDB_PEN1, BTNBMP_NORMAL);
	m_NcButton2.SetButtonBitmap(IDB_PEN2, BTNBMP_MOUSEOVER);
	m_NcButton2.SetTooltip("pen");

	m_NcButton3.SetButtonBitmap(IDB_PEN1, BTNBMP_NORMAL);
	m_NcButton3.SetButtonBitmap(IDB_PEN2, BTNBMP_MOUSEOVER);
	m_NcButton3.SetTooltip("pen");

	m_NcButtonManager.AddButton(&m_NcButton1);
	m_NcButtonManager.AddButton(&m_NcButton2);
	m_NcButtonManager.AddButton(&m_NcButton3);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if(m_NcButtonManager.PreTranslateMessage(pMsg))
		return TRUE;
	else
		return CFrameWnd::PreTranslateMessage(pMsg);	
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	TRACE("消息:%x\r\n", message);
	
	LRESULT lResult = CFrameWnd::DefWindowProc(message, wParam, lParam);
	if(!IsWindow(this->m_hWnd))
		return lResult;

	LRESULT lMyResult = m_NcButtonManager.DefWindowProc(this->m_hWnd, message, wParam, lParam);
	if(lMyResult != 0)
		return lMyResult;
	else
		return lResult;	
}

void CMainFrame::OnNcButton1Clicked()
{
	AfxMessageBox("VC知识库欢迎您! www.vckbase.com");
}

void CMainFrame::OnNcButton2Clicked()
{
	AfxMessageBox("pen!(vckbase.com)");
}

void CMainFrame::OnNcButton3Clicked()
{
	AfxMessageBox("hello!");
}

/*

BOOL CMainFrame::DestroyWindow() 
{
	m_pTitleButtonManager->RemoveButton(&m_NcButton1);
	m_pTitleButtonManager = NULL;
	return CFrameWnd::DestroyWindow();
}
*/


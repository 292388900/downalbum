// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "dotNET界面.h"

#include "MainFrm.h"
#include "SearchDlg.h"
#include "SearchResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CAutoHideFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CAutoHideFrame)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
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
	// TODO: add member initialization code here
	
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


	if (!m_wndDockPageBar.Create(_T("DockPageBar"), this, CSize(230,260),TRUE,123))
	{
		TRACE0("Failed to create DockPageBar.\n");
			return -1;
	}
	m_wndDockPageBar.SetBarStyle(m_wndDockPageBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndDockPageBar.EnableDocking (CBRS_ALIGN_ANY);

	if (!m_wndDockPageBar1.Create(_T("DockPageBar"), this, CSize(230,260),TRUE,1234))
	{
		TRACE0("Failed to create DockPageBar.\n");
			return -1;
	}
	m_wndDockPageBar1.SetBarStyle(m_wndDockPageBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndDockPageBar1.EnableDocking (CBRS_ALIGN_ANY);


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndDockPageBar, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndDockPageBar1, AFX_IDW_DOCKBAR_BOTTOM);

	if (!m_wndTree.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), this, 100))
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;
	}

	HTREEITEM hti = m_wndTree.InsertItem(_T("Welcome to the VCKbase"));
	m_wndTree.InsertItem(_T("Visual c++"), hti);
	m_wndTree.InsertItem(_T("C++论坛"), hti);
	m_wndTree.InsertItem(_T(".NET论坛"), hti);
	m_wndTree.InsertItem(_T("C++ Builder"), hti);
	m_wndTree.InsertItem(_T("Java"), hti);
	m_wndTree.InsertItem(_T("软件工程"), hti);
	m_wndTree.InsertItem(_T("灌水乐园"), hti);
	
	m_wndDockPageBar.AddPage(&m_wndTree,"Contents",IDI_HELP);
	m_wndDockPageBar.AddPage (RUNTIME_CLASS(CSearchDlg), IDD_SEARCHDLG, "Search", IDI_SEARCH);


	if (!m_wndEdit.Create(WS_CHILD|WS_VISIBLE|
			ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL,
		CRect(0,0,0,0), this, 123))
		return -1;
	m_wndDockPageBar.AddPage(&m_wndEdit,"Edit",IDI_INDEX);

	m_wndDockPageBar1.AddPage (RUNTIME_CLASS(CSearchResult), IDD_SEARCHRESULT, "Search Result", IDI_SEARCHRESULT);
	
	//m_wndDockPageBar.AddPage (&m_wndDockPageBar1);
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


// MainFrm.cpp : implementation of the CMainFrame class
//
// Copyright 2001 WangJun									  //
// All Rights Reserved.										  //
//															  //
// Email: wangjun98@sohu.com								  //
// URL:   www.vckbase.com									  //
//															  //
// 1.0     2001/9/30   First release version.				  //
//															  //
// 2004/2/8 ��Ӷ�TreeCtrl��Ϣ�Ĵ���						  //
//                                                            //
////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarDemo.h"
#include "MyDlg.h"
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
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_SHOWLEFTBAR, OnUpdateShowLeftbar)
	ON_UPDATE_COMMAND_UI(ID_SHOWBOTTOMBAR, OnUpdateShowBottombar)
	ON_COMMAND(ID_SHOWBOTTOMBAR, OnShowBottombar)
	ON_COMMAND(ID_SHOWLEFTBAR, OnShowLeftbar)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MYTREECTRL, OnSelchangedTree)
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
	m_strInfo = "";
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
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

	/////////////����������///////////////
	if (!m_wndMyBar1.Create(_T("�ҵĿ�����"), this, CSize(230,260),TRUE,123))
	{
		TRACE0("Failed to create mybar\n");
			return -1;
	}
	if (!m_wndMyBar2.Create(_T("�ҵĿ�����2"), this,CSize(200,100),TRUE,124))
	{
		TRACE0("Failed to create mybar\n");
			return -1;
	}
	m_wndToolBar.SetButtonText(0," ���� ");
	m_wndToolBar.SetButtonText(1," �༭ ");
	/////////////����������/////////////////
	CRect rc(0, 0, 0, 0);
	CSize sizeMax(0, 0);
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();
	for (int nIndex = bar.GetButtonCount() - 1; nIndex >= 0; nIndex--)
	{
		bar.GetItemRect(nIndex, rc);

		rc.NormalizeRect();
		sizeMax.cx = __max(rc.Size().cx, sizeMax.cx);
		sizeMax.cy = __max(rc.Size().cy, sizeMax.cy);
	}
	//sizeMax.cx += 10;
	m_wndToolBar.SetSizes(sizeMax, CSize(16,15));
	////////////����TAB Control�����������뵽ControlBar��
	/// ��Ҫע�����:ͬһ��ControlBarֻ�ܼ���һ��"һ���Ӵ���"
	m_TabCtrl.Create(TCS_DOWN|WS_CHILD|WS_VISIBLE,CRect(0,0,100,100),&m_wndMyBar1,125);
	/////////////��һ�δ��봴�����Ϳؼ�////////////
	if (!m_wndTree.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_TabCtrl, IDC_MYTREECTRL))
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;
	}
	m_wndTree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	///�����Ϳؼ��������
	HTREEITEM hti = m_wndTree.InsertItem(_T("VC֪ʶ��������־"));
	m_wndTree.InsertItem(_T("�����ĵ�"));
	m_wndTree.InsertItem(_T("������־��һ��"), hti);
	m_wndTree.InsertItem(_T("������־�ڶ���"), hti);
	///�����Ϳؼ����뵽TabCtrl��
	m_TabCtrl.AddPage(&m_wndTree,"VC֪ʶ��",IDI_ICON1);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CVCKBASEDlg),IDD_DIALOGBAR,"�ڶ�ҳ",IDI_ICON2);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CMyDlg),IDD_DIALOGBAR2,"����ҳ",IDI_ICON3);

	m_TabCtrl.UpdateWindow();
	////////�Ի���///////////
	//if(!m_wndVCKBASE.Create(IDD_DIALOGBAR,&m_wndMyBar1))
	//	return -1;
	//m_wndVCKBASE.ShowWindow(SW_SHOW);

	////////�༭�ؼ�///////////
	if(!m_wndEdit.Create(WS_VSCROLL|WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN,CRect(0,0,0,0),&m_wndMyBar2,101))
		return -1;
	m_wndEdit.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	///////////////ͣ������///////////////////
	m_wndMyBar1.SetBarStyle(m_wndMyBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndMyBar2.SetBarStyle(m_wndMyBar2.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMyBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMyBar2.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndMyBar1, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndMyBar2, AFX_IDW_DOCKBAR_BOTTOM);
	/////////ʹ������ͣ����ʾ��ͬ��ͷ��////
	#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
	#endif //_SCB_REPLACE_MINIFRAME
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
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
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnUpdateShowLeftbar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndMyBar1.IsVisible());		
}

void CMainFrame::OnUpdateShowBottombar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndMyBar2.IsVisible());		
}

void CMainFrame::OnShowLeftbar() 
{
	ShowControlBar(&m_wndMyBar1, !m_wndMyBar1.IsVisible(), FALSE);
}

void CMainFrame::OnShowBottombar() 
{
	ShowControlBar(&m_wndMyBar2, !m_wndMyBar2.IsVisible(), FALSE);
}

CString CMainFrame::GetInfo()
{
	return m_strInfo;
}

void CMainFrame::SetInfo(CString &strInfo)
{
	m_strInfo = strInfo;
}

void CMainFrame::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	SetInfo(m_wndTree.GetItemText(pNMTreeView->itemNew.hItem));

	Invalidate();
	*pResult = 0;
}

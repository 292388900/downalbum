// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Project.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_TESTDLG, OnUpdateTestdlg)
	ON_COMMAND(ID_TESTDLG, OnTestdlg)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_TOP, CMainFrame::OnUpdateLayoutTop)
	ON_COMMAND(ID_LAYOUT_TOP, CMainFrame::OnLayoutTop)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_BOTTOM, CMainFrame::OnUpdateLayoutBottom)
	ON_COMMAND(ID_LAYOUT_BOTTOM, CMainFrame::OnLayoutBottom)
	ON_UPDATE_COMMAND_UI(ID_BEHAVIOR_SCALING, CMainFrame::OnUpdateBehaviorScaling)
	ON_COMMAND(ID_BEHAVIOR_SCALING, CMainFrame::OnBehaviorScaling)
	ON_UPDATE_COMMAND_UI(ID_BEHAVIOR_SCROLLING, CMainFrame::OnUpdateBehaviorScrolling)
	ON_COMMAND(ID_BEHAVIOR_SCROLLING, CMainFrame::OnBehaviorScrolling)
	ON_UPDATE_COMMAND_UI(ID_EQUALSIZE, CMainFrame::OnUpdateEqualsize)
	ON_COMMAND(ID_EQUALSIZE, CMainFrame::OnEqualsize)
	ON_UPDATE_COMMAND_UI(ID_DRAGTABS, CMainFrame::OnUpdateDragtabs)
	ON_COMMAND(ID_DRAGTABS, CMainFrame::OnDragtabs)
	ON_UPDATE_COMMAND_UI(ID_SHOWCLOSEBUTTON, CMainFrame::OnUpdateShowclosebutton)
	ON_COMMAND(ID_SHOWCLOSEBUTTON, CMainFrame::OnShowclosebutton)
	ON_UPDATE_COMMAND_UI(ID_SHOWMENUBUTTON, CMainFrame::OnUpdateShowmenubutton)
	ON_COMMAND(ID_SHOWMENUBUTTON, CMainFrame::OnShowmenubutton)
	ON_UPDATE_COMMAND_UI(ID_SHOWSCROLLBUTTONS, CMainFrame::OnUpdateShowscrollbuttons)
	ON_COMMAND(ID_SHOWSCROLLBUTTONS, CMainFrame::OnShowscrollbuttons)
	ON_UPDATE_COMMAND_UI(ID_SHOWCLIENTEDGEBORDER, CMainFrame::OnUpdateShowclientedgeborder)
	ON_COMMAND(ID_SHOWCLIENTEDGEBORDER, CMainFrame::OnShowclientedgeborder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// 
CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		// 
		// 
		// 
		// Creation and initialization of TabCtrl.
	if(m_TabCtrl.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),AFX_IDW_PANE_FIRST)==false) return -1;
	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCROLL);
		// 
	CImageList imagelist,imagelistDis, imagelistSys;
	CBitmap bmp,bmpDis, bmpSys;
		// 
	imagelist.Create(16,16,ILC_COLOR24 | ILC_MASK,7,0);
	bmp.LoadBitmap(IDB_BITMAP1);
	imagelist.Add(&bmp,RGB(255,0,255));
		// 
	imagelistDis.Create(16,16,ILC_COLOR24 | ILC_MASK,7,0);
	bmpDis.LoadBitmap(IDB_BITMAP2);
	imagelistDis.Add(&bmpDis,RGB(255,0,255));
		// 
	m_TabCtrl.SetImageLists(&imagelist,&imagelistDis);
		// 
	imagelistSys.Create(14,14,ILC_COLOR24 | ILC_MASK,7,0);
	bmpSys.LoadBitmap(IDB_BITMAP3);
	imagelistSys.Add(&bmpSys,RGB(255,0,255));
		// 
	m_TabCtrl.SetSystemImageList(&imagelistSys);
		// 
		// Creation of child windows.
	if(m_List1.Create(WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,2001)==0 ||
		m_List2.Create(WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,2002)==0 ||
		m_List3.Create(WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,2003)==0 ||
		m_List4.Create(WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,2004)==0 ||
		m_List5.Create(WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,2005)==0)
		return -1;
		// 
		// Initialization of child windows.
	m_List1.InsertColumn(0,_T("Mail"),LVCFMT_LEFT,100);
	m_List2.InsertColumn(0,_T("Calendar"),LVCFMT_LEFT,100);
	m_List3.InsertColumn(0,_T("Contacts"),LVCFMT_LEFT,100);
	m_List4.InsertColumn(0,_T("Tasks"),LVCFMT_LEFT,100);
	m_List5.InsertColumn(0,_T("Business Affairs"),LVCFMT_LEFT,100);
		// 
	m_List1.InsertItem(0,_T("Mail 1"));
	m_List1.InsertItem(1,_T("Mail 2"));
	m_List2.InsertItem(0,_T("Calendar 1"));
	m_List2.InsertItem(1,_T("Calendar 2"));
	m_List3.InsertItem(0,_T("Contact 1"));
	m_List3.InsertItem(1,_T("Contact 2"));
	m_List4.InsertItem(0,_T("Task 1"));
	m_List4.InsertItem(1,_T("Task 2"));
	m_List5.InsertItem(0,_T("Business Affair 1"));
	m_List5.InsertItem(1,_T("Business Affair 2"));
		// 
		// Attaching of child windows to the TabCtrl.
	if(m_TabCtrl.Add(m_List1,_T("1.Mail"),0)==NULL ||
		m_TabCtrl.Add(m_List2,_T("2.Calendar"),1)==NULL ||
		m_TabCtrl.Add(m_List3,_T("3.Contacts"),2)==NULL ||
		m_TabCtrl.Add(m_List4,_T("4.Tasks"),-1)==NULL ||
		m_TabCtrl.Add(m_List5,_T("5.Business Affairs"),3)==NULL)
		return -1;
	m_TabCtrl.SetTabTooltipText(m_TabCtrl.GetTab(1),_T("Tooltip for Calendar"));
	m_TabCtrl.SetTabTooltipText(m_TabCtrl.GetTab(2),_T("Tooltip for Contacts"));
	m_TabCtrl.SetTabTooltipText(m_TabCtrl.GetTab(3),_T("Tooltip for Tasks"));
		// 
	m_TabCtrl.Disable(m_TabCtrl.GetTab(2),true);	// simply for example.
		// 
		// Load state from registry and update.
	m_TabCtrl.LoadState(AfxGetApp(),_T("TabCtrl"),_T("State"));
	m_TabCtrl.Update();
		// 
		// 
		// 
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
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
		// 
		// 
	OnTestdlg();
	m_DemoDlg.RedrawWindow(0,0,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
		// 
		// 
	return 0;
}

void CMainFrame::OnDestroy()
{	m_TabCtrl.SaveState(AfxGetApp(),_T("TabCtrl"),_T("State"));
		// 
	CFrameWnd::OnDestroy();
}




void CMainFrame::OnUpdateTestdlg(CCmdUI *pCmdUI)
{	pCmdUI->Enable((::IsWindow(m_DemoDlg.m_hWnd)!=0 && ::IsWindowVisible(m_DemoDlg.m_hWnd)!=0) ? 0 : 1);
}
// 
void CMainFrame::OnTestdlg()
{	if(m_DemoDlg.m_hWnd==NULL)
	{	if(m_DemoDlg.Create(IDD_TESTDLG,this)==0) return;
		m_DemoDlg.CenterWindow(this);
	}
	m_DemoDlg.ShowWindow(SW_SHOWNORMAL);
}





// Layout - Top.
void CMainFrame::OnUpdateLayoutTop(CCmdUI *pCmdUI)
{	pCmdUI->SetRadio(m_TabCtrl.GetLayout()==TAB_LAYOUT_TOP ? 1 : 0);
}
void CMainFrame::OnLayoutTop()
{	m_TabCtrl.SetLayout(TAB_LAYOUT_TOP);
	m_TabCtrl.Update();
}
// 
// Layout - Bottom.
void CMainFrame::OnUpdateLayoutBottom(CCmdUI *pCmdUI)
{	pCmdUI->SetRadio(m_TabCtrl.GetLayout()==TAB_LAYOUT_BOTTOM ? 1 : 0);
}
void CMainFrame::OnLayoutBottom()
{	m_TabCtrl.SetLayout(TAB_LAYOUT_BOTTOM);
	m_TabCtrl.Update();
}
// 
// Behavior - Scaling.
void CMainFrame::OnUpdateBehaviorScaling(CCmdUI *pCmdUI)
{	pCmdUI->SetRadio(m_TabCtrl.GetBehavior()==TAB_BEHAVIOR_SCALE ? 1 : 0);
}
void CMainFrame::OnBehaviorScaling()
{	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCALE);
	m_TabCtrl.Update();
}
// 
// Behavior - Scrolling.
void CMainFrame::OnUpdateBehaviorScrolling(CCmdUI *pCmdUI)
{	pCmdUI->SetRadio(m_TabCtrl.GetBehavior()==TAB_BEHAVIOR_SCROLL ? 1 : 0);
}
void CMainFrame::OnBehaviorScrolling()
{	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCROLL);
	m_TabCtrl.Update();
}
// 
// Equal size.
void CMainFrame::OnUpdateEqualsize(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_TabCtrl.IsEqualTabsSize()==true ? 1 : 0);
}
void CMainFrame::OnEqualsize()
{	m_TabCtrl.EqualTabsSize( !m_TabCtrl.IsEqualTabsSize() );
	m_TabCtrl.Update();
}
// 
// Remove tabs.
void CMainFrame::OnUpdateDragtabs(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_TabCtrl.IsRemoveTabEnable()==true ? 1 : 0);
}
void CMainFrame::OnDragtabs()
{	m_TabCtrl.RemoveTabEnable( !m_TabCtrl.IsRemoveTabEnable() );
	m_TabCtrl.Update();
}
// 
// Show close button.
void CMainFrame::OnUpdateShowclosebutton(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_TabCtrl.IsShowCloseButton()==true ? 1 : 0);
}
void CMainFrame::OnShowclosebutton()
{	m_TabCtrl.ShowCloseButton( !m_TabCtrl.IsShowCloseButton() );
	m_TabCtrl.Update();
}
// 
// Show menu button.
void CMainFrame::OnUpdateShowmenubutton(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_TabCtrl.IsShowMenuButton()==true ? 1 : 0);
}
void CMainFrame::OnShowmenubutton()
{	m_TabCtrl.ShowMenuButton( !m_TabCtrl.IsShowMenuButton() );
	m_TabCtrl.Update();
}
// 
// Show scroll buttons.
void CMainFrame::OnUpdateShowscrollbuttons(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_TabCtrl.IsShowScrollButtons()==true ? 1 : 0);
}
void CMainFrame::OnShowscrollbuttons()
{	m_TabCtrl.ShowScrollButtons( !m_TabCtrl.IsShowScrollButtons() );
	m_TabCtrl.Update();
}
// 
// Show ClientEdge border.
void CMainFrame::OnUpdateShowclientedgeborder(CCmdUI *pCmdUI)
{	LONG styleEx = ::GetWindowLong(m_TabCtrl.m_hWnd,GWL_EXSTYLE);
	pCmdUI->SetCheck((styleEx & WS_EX_CLIENTEDGE)!=0 ? 1 : 0);
}
void CMainFrame::OnShowclientedgeborder()
{	LONG styleEx = ::GetWindowLong(m_TabCtrl.m_hWnd,GWL_EXSTYLE);
		// 
	if((styleEx & WS_EX_CLIENTEDGE)!=0)
	{	m_TabCtrl.ModifyStyleEx(WS_EX_CLIENTEDGE,0);
		m_TabCtrl.ShowBorder(true);
	}
	else
	{	m_TabCtrl.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		m_TabCtrl.ShowBorder(false);
	}
	m_TabCtrl.Update();
	m_TabCtrl.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);	// border update.
}




















BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_TabCtrl.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_TabCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
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
















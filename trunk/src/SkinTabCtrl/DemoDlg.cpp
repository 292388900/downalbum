/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
#include "DemoDlg.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// TabDialog.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(TabDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
void TabDialog::OnBnClickedButton1()
{	::MessageBox(m_hWnd,_T("TabDialog::OnBnClickedButton1"),_T("TabDialog"),MB_OK);
}
// 
void TabDialog::OnCancel()
{	//CDialog::OnCancel();
}
// 
void TabDialog::OnOK()
{	//CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// DemoDlg.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
#define PTR_BUT_ID(id)		((CButton *)GetDlgItem(IDC_BUT##id))
#define SET_CHECK(id,state)	(PTR_BUT_ID(id)->SetCheck((state)==true ? BST_CHECKED : BST_UNCHECKED))
#define GET_CHECK(id)			(PTR_BUT_ID(id)->GetCheck()==BST_CHECKED)
#define SET_ENABLE(id,state)		\
	((state)==true ? PTR_BUT_ID(id)->ModifyStyle(WS_DISABLED,0) : PTR_BUT_ID(id)->ModifyStyle(0,WS_DISABLED));	\
	PTR_BUT_ID(id)->Invalidate(FALSE);
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(DemoDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, DemoDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUT1, DemoDlg::OnBnClickedBut1)
	ON_BN_CLICKED(IDC_BUT2, DemoDlg::OnBnClickedBut2)
	ON_BN_CLICKED(IDC_BUT3, DemoDlg::OnBnClickedBut3)
	ON_BN_CLICKED(IDC_BUT4, DemoDlg::OnBnClickedBut4)
	ON_BN_CLICKED(IDC_BUT5, DemoDlg::OnBnClickedBut5)
	ON_BN_CLICKED(IDC_BUT6, DemoDlg::OnBnClickedBut6)
	ON_BN_CLICKED(IDC_BUT7, DemoDlg::OnBnClickedBut7)
	ON_BN_CLICKED(IDC_BUT8, DemoDlg::OnBnClickedBut8)
	ON_BN_CLICKED(IDC_BUT9, DemoDlg::OnBnClickedBut9)
	ON_BN_CLICKED(IDC_BUT10, DemoDlg::OnBnClickedBut10)
	ON_BN_CLICKED(IDC_BUT11, DemoDlg::OnBnClickedBut11)
	ON_BN_CLICKED(IDC_BUT12, DemoDlg::OnBnClickedBut12)
	ON_BN_CLICKED(IDC_BUT13, DemoDlg::OnBnClickedBut13)
	ON_BN_CLICKED(IDC_BUT14, DemoDlg::OnBnClickedBut14)
	ON_BN_CLICKED(IDC_BUT15, DemoDlg::OnBnClickedBut15)
	ON_BN_CLICKED(IDC_BUT16, DemoDlg::OnBnClickedBut16)
	ON_BN_CLICKED(IDC_BUT17, DemoDlg::OnBnClickedBut17)
	ON_BN_CLICKED(IDC_BUT20, DemoDlg::OnBnClickedBut20)
	ON_BN_CLICKED(IDC_BUT21, DemoDlg::OnBnClickedBut21)
	ON_BN_CLICKED(IDC_BUT22, DemoDlg::OnBnClickedBut22)
	ON_BN_CLICKED(IDC_BUT23, DemoDlg::OnBnClickedBut23)
	ON_BN_CLICKED(IDC_BUT24, DemoDlg::OnBnClickedBut24)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
DemoDlg::DemoDlg()
{	m_bDelDragOutside = false;
}
// 
int DemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	if(CDialog::OnCreate(lpCreateStruct)==-1) return -1;
	ModifyStyle(0,WS_CLIPCHILDREN);	// to avoid flicks of dialog child controls.
		// 
	if(m_TabCtrl.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),3001)==false) return -1;
		// 
		// 
	if(m_List1.Create(WS_CLIPCHILDREN | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,3002)==0 ||
		m_List2.Create(WS_CLIPCHILDREN | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,3003)==0 ||
		m_List3.Create(WS_CLIPCHILDREN | LVS_REPORT,CRect(0,0,0,0),&m_TabCtrl,3004)==0)
		return -1;
		// 
	if(m_Tree1.Create(WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,CRect(0,0,0,0),&m_TabCtrl,3005)==0 ||
		m_Tree2.Create(WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,CRect(0,0,0,0),&m_TabCtrl,3006)==0 ||
		m_Tree3.Create(WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,CRect(0,0,0,0),&m_TabCtrl,3007)==0)
		return -1;
		// 
	if(m_Edit1.Create(ES_MULTILINE | WS_CHILD,CRect(0,0,0,0),&m_TabCtrl,3008)==0 ||
		m_Edit2.Create(ES_MULTILINE | WS_CHILD,CRect(0,0,0,0),&m_TabCtrl,3009)==0)
		return -1;
		// 
	if(m_Dlg1.Create(IDD_DIALOG,&m_TabCtrl)==0) return -1;	// create as modeless dialog box.
	m_Dlg1.SetDlgCtrlID(3010);		// set unique id - important for dialog box.
		// 
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 
BOOL DemoDlg::OnInitDialog()
{	CDialog::OnInitDialog();
		// 
	GetDlgItem(IDC_TAB)->ShowWindow(SW_HIDE);
	SetTabsPosition();
		// 
		// 
	for(int i=0; i<6; ++i)
		OnBnClickedBut22();	// add tabs.
		// 
	m_List1.InsertColumn(0,_T("Calendar"),LVCFMT_LEFT,100);
	m_List2.InsertColumn(0,_T("Business Affairs"),LVCFMT_LEFT,100);
	m_List3.InsertColumn(0,_T("Folder List"),LVCFMT_LEFT,100);
		// 
	m_Tree1.InsertItem(_T("Mail"));
	m_Tree2.InsertItem(_T("Tasks"));
	m_Tree3.InsertItem(_T("Shortcuts"));
		// 
	m_Edit1.SetWindowText(_T("Contacts"));
	m_Edit2.SetWindowText(_T("Notes"));
		// 
		// 
	SetImages();
		// 
	CImageList imagelistSys;
	CBitmap bmpSys;
	imagelistSys.Create(14,14,ILC_COLOR24 | ILC_MASK,4,0);
	bmpSys.LoadBitmap(IDB_BITMAP3);
	imagelistSys.Add(&bmpSys,RGB(255,0,255));
		// 
	m_TabCtrl.SetSystemImageList(&imagelistSys);
		// 
	CFont font;
	font.CreatePointFont(85,_T("Tahoma"));
	m_TabCtrl.SetFont(&font);
	m_TabCtrl.SetFontSelect(&font);
		// 
	m_TabCtrl.SetCursor(IDC_CURSOR1);
	m_TabCtrl.RemoveTabEnable(true);
		// 
	m_TabCtrl.SetNotifyManager(this);
		// 
	m_TabCtrl.InstallStyle(&m_TabCtrl.styleBase);	// install style.
	m_TabCtrl.Update();
		// 
		// 
		// 
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1);
	pCombo->AddString(_T("1. Base style"));
	pCombo->AddString(_T("2. Like client area VS2003"));
	pCombo->AddString(_T("3. Kind of 2"));
	pCombo->AddString(_T("4. Like bars VS2003"));
	pCombo->AddString(_T("5. Kind of 4"));
	pCombo->AddString(_T("6. Like client area VS2008 classic"));
	pCombo->AddString(_T("7. Like client area VS2008 blue"));
	pCombo->AddString(_T("8. Like client area VS2008 silver"));
	pCombo->AddString(_T("9. Like client area VS2008 olive"));
	pCombo->AddString(_T("10. Like bars VS2008 classic"));
	pCombo->AddString(_T("11. Kind of 10"));
	pCombo->AddString(_T("12. Like bars VS2008 blue"));
	pCombo->AddString(_T("13. Kind of 12"));
	pCombo->AddString(_T("14. Like bars VS2008 silver"));
	pCombo->AddString(_T("15. Kind of 14"));
	pCombo->AddString(_T("16. Like bars VS2008 olive"));
	pCombo->AddString(_T("17. Kind of 16"));
	pCombo->AddString(_T("18. Like client area VS2010"));
	pCombo->AddString(_T("19. Kind of 18"));
	pCombo->AddString(_T("20. Kind of 18"));
	pCombo->AddString(_T("21. Like bars VS2010"));
	pCombo->SetCurSel(0);
		// 
	SET_CHECK(1,m_TabCtrl.GetLayout()==TAB_LAYOUT_TOP);
	SET_CHECK(2,m_TabCtrl.GetLayout()==TAB_LAYOUT_BOTTOM);
	SET_CHECK(3,m_TabCtrl.GetBehavior()==TAB_BEHAVIOR_SCALE);
	SET_CHECK(4,m_TabCtrl.GetBehavior()==TAB_BEHAVIOR_SCROLL);
	SET_CHECK(5,m_TabCtrl.GetCursor()!=NULL);
	SET_CHECK(6,m_TabCtrl.IsEqualTabsSize());
	SET_CHECK(7,m_TabCtrl.IsHideSingleTab());
	SET_CHECK(8,m_TabCtrl.IsRemoveTabEnable());
	SET_ENABLE(9,m_TabCtrl.IsRemoveTabEnable());
	SET_CHECK(9,m_bDelDragOutside);
	SET_CHECK(10,m_TabCtrl.IsShowBorder());
		// 
	CImageList *pImageList, *pImageListDisabled;
	m_TabCtrl.GetImageLists(&pImageList,&pImageListDisabled);
	SET_CHECK(12,pImageList->m_hImageList!=NULL || pImageListDisabled->m_hImageList!=NULL);
		// 
	SET_CHECK(13,m_TabCtrl.IsWatchActivityCtrl());
		// 
	LOGFONT logfont;
	m_TabCtrl.GetFontSelect()->GetLogFont(&logfont);
	SET_CHECK(14,(logfont.lfWeight==FW_BOLD));
		// 
	SET_CHECK(15,m_TabCtrl.IsShowCloseButton());
	SET_CHECK(16,m_TabCtrl.IsShowMenuButton());
	SET_CHECK(17,m_TabCtrl.IsShowScrollButtons());
		// 
		// 
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::OnSize(UINT nType, int cx, int cy)
{	CDialog::OnSize(nType, cx, cy);
		// 
	SetTabsPosition();
}
/////////////////////////////////////////////////////////////////////////////
// Style.
void DemoDlg::OnCbnSelchangeCombo1()
{	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1);
	int sel = pCombo->GetCurSel();
		// 
	switch(sel)
	{	case 0: m_TabCtrl.InstallStyle(&m_TabCtrl.styleBase); break;
		case 1: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2003_client); break;
		case 2: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2003_client_custom1); break;
		case 3: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2003_bars); break;
		case 4: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2003_bars_custom1); break;
		case 5: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_client_classic); break;
		case 6: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_client_blue); break;
		case 7: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_client_silver); break;
		case 8: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_client_olive); break;
		case 9: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_classic); break;
		case 10: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_classic_custom1); break;
		case 11: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_blue); break;
		case 12: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_blue_custom1); break;
		case 13: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_silver); break;
		case 14: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_silver_custom1); break;
		case 15: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_olive); break;
		case 16: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2008_bars_olive_custom1); break;
		case 17: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2010_client); break;
		case 18: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2010_client_custom1); break;
		case 19: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2010_client_custom2); break;
		case 20: m_TabCtrl.InstallStyle(&m_TabCtrl.styleVS2010_bars); break;
	}
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Layout - top.
void DemoDlg::OnBnClickedBut1()
{	m_TabCtrl.SetLayout(TAB_LAYOUT_TOP);
	m_TabCtrl.Update();
}
// 
// Layout - bottom.
void DemoDlg::OnBnClickedBut2()
{	m_TabCtrl.SetLayout(TAB_LAYOUT_BOTTOM);
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Behavior - scaling.
void DemoDlg::OnBnClickedBut3()
{	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCALE);
	m_TabCtrl.Update();
}
// 
// Behavior - scrolling.
void DemoDlg::OnBnClickedBut4()
{	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCROLL);
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Custom cursor.
void DemoDlg::OnBnClickedBut5()
{	(GET_CHECK(5)==true ? m_TabCtrl.SetCursor(IDC_CURSOR1) : m_TabCtrl.SetCursor((UINT)0));
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Equal size of tabs.
void DemoDlg::OnBnClickedBut6()
{	m_TabCtrl.EqualTabsSize( GET_CHECK(6) );
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Hide single tab.
void DemoDlg::OnBnClickedBut7()
{	m_TabCtrl.HideSingleTab( GET_CHECK(7) );
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Remove tabs.
void DemoDlg::OnBnClickedBut8()
{	m_TabCtrl.RemoveTabEnable( GET_CHECK(8) );
	SET_ENABLE(9,GET_CHECK(8));
}
// 
// Delete tab for drag outside.
void DemoDlg::OnBnClickedBut9()
{	m_bDelDragOutside = GET_CHECK(9);
}
/////////////////////////////////////////////////////////////////////////////
// Show border.
void DemoDlg::OnBnClickedBut10()
{	m_TabCtrl.ShowBorder(GET_CHECK(10));
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Show controls borders.
void DemoDlg::OnBnClickedBut11()
{	if(GET_CHECK(11))
	{	m_List1.ModifyStyle(0,WS_BORDER);
		m_List2.ModifyStyle(0,WS_BORDER);
		m_List3.ModifyStyle(0,WS_BORDER);
		m_Tree1.ModifyStyle(0,WS_BORDER);
		m_Tree2.ModifyStyle(0,WS_BORDER);
		m_Tree3.ModifyStyle(0,WS_BORDER);
		m_Edit1.ModifyStyle(0,WS_BORDER);
		m_Edit2.ModifyStyle(0,WS_BORDER);
		m_Dlg1.ModifyStyle(0,WS_BORDER);
	}
	else
	{	m_List1.ModifyStyle(WS_BORDER,0);
		m_List2.ModifyStyle(WS_BORDER,0);
		m_List3.ModifyStyle(WS_BORDER,0);
		m_Tree1.ModifyStyle(WS_BORDER,0);
		m_Tree2.ModifyStyle(WS_BORDER,0);
		m_Tree3.ModifyStyle(WS_BORDER,0);
		m_Edit1.ModifyStyle(WS_BORDER,0);
		m_Edit2.ModifyStyle(WS_BORDER,0);
		m_Dlg1.ModifyStyle(WS_BORDER,0);
	}
		// 
	m_List1.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_List2.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_List3.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_Tree1.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_Tree2.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_Tree3.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_Edit1.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_Edit2.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	m_Dlg1.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}
/////////////////////////////////////////////////////////////////////////////
// Show images.
void DemoDlg::OnBnClickedBut12()
{	(GET_CHECK(12)==true ? SetImages() : m_TabCtrl.SetImageLists(NULL,NULL));
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Show selected font.
void DemoDlg::OnBnClickedBut13()
{	m_TabCtrl.WatchActivityCtrl( GET_CHECK(13) );
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Watch activity control.
void DemoDlg::OnBnClickedBut14()
{	if(GET_CHECK(14)==true)
	{	LOGFONT logfont;
		m_TabCtrl.GetFont()->GetLogFont(&logfont);
		logfont.lfWeight = FW_BOLD;
		m_TabCtrl.SetFontSelect(&logfont);
	}
	else
	{	CFont *font = m_TabCtrl.GetFont();
		m_TabCtrl.SetFontSelect(font);
	}
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Show close button.
void DemoDlg::OnBnClickedBut15()
{	m_TabCtrl.ShowCloseButton( GET_CHECK(15) );
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Show menu button.
void DemoDlg::OnBnClickedBut16()
{	m_TabCtrl.ShowMenuButton( GET_CHECK(16) );
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Show scroll buttons.
void DemoDlg::OnBnClickedBut17()
{	m_TabCtrl.ShowScrollButtons( GET_CHECK(17) );
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Enable / Disable.
void DemoDlg::OnBnClickedBut20()
{	HTAB hTab = m_TabCtrl.GetSel();
	if(hTab==NULL && m_TabCtrl.GetCount()>0)
		hTab = m_TabCtrl.GetTab(0);
	if(hTab!=NULL)
	{	m_TabCtrl.Disable(hTab,!m_TabCtrl.IsDisable(hTab));
		m_TabCtrl.Update();
	}
}
// 
// Enable All.
void DemoDlg::OnBnClickedBut21()
{	for(int i=0, c=m_TabCtrl.GetCount(); i<c; ++i)
		m_TabCtrl.Disable( m_TabCtrl.GetTab(i), false);
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Add.
void DemoDlg::OnBnClickedBut22()
{	try
	{	if(IsExist(&m_Tree1)==false) m_TabCtrl.Add(m_Tree1,_T("Mail"),0);
		else if(IsExist(&m_List1)==false) m_TabCtrl.Add(m_List1,_T("Calendar"),1);
		else if(IsExist(&m_Dlg1)==false) m_TabCtrl.Add(m_Dlg1,_T("Modeless Dialog Box"),-1);
		else if(IsExist(&m_Edit1)==false) m_TabCtrl.Add(m_Edit1,_T("Contacts"),2);
		else if(IsExist(&m_Tree2)==false) m_TabCtrl.Add(m_Tree2,_T("Tasks"),3);
		else if(IsExist(&m_List2)==false) m_TabCtrl.Add(m_List2,_T("Business Affairs"),4);
		else if(IsExist(&m_Edit2)==false) m_TabCtrl.Add(m_Edit2,_T("Notes"),5);
		else if(IsExist(&m_List3)==false) m_TabCtrl.Add(m_List3,_T("Folder List"),-1);
		else if(IsExist(&m_Tree3)==false) m_TabCtrl.Add(m_Tree3,_T("Shortcuts"),6);
	}
	catch(std::bad_alloc &)
	{	return;
	}
	m_TabCtrl.Update();
}
// 
// Delete.
void DemoDlg::OnBnClickedBut23()
{	HTAB hTab = m_TabCtrl.GetSel();
	if(hTab!=NULL)
	{	m_TabCtrl.Delete(hTab);
		m_TabCtrl.Update();
	}
}
// 
// Delete All.
void DemoDlg::OnBnClickedBut24()
{	m_TabCtrl.DeleteAll();
	m_TabCtrl.Update();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::SetTabsPosition()
{	CWnd *pBaseWnd = GetDlgItem(IDC_TAB);
		// 
	if(pBaseWnd!=NULL)
	{	CRect rcTab, rcDlg;
		pBaseWnd->GetWindowRect(&rcTab);
		ScreenToClient(&rcTab);
		GetClientRect(&rcDlg);
			// 
		rcTab.right = max(rcTab.left,rcDlg.right-7);
		rcTab.bottom = max(rcTab.top,rcDlg.bottom-6);
			// 
		m_TabCtrl.MoveWindow(&rcTab);
	}
}
// 
void DemoDlg::SetImages()
{	CImageList imagelist,imagelistDis;
	CBitmap bmp,bmpDis;
		// 
	imagelist.Create(16,16,ILC_COLOR24 | ILC_MASK,4,0);
	bmp.LoadBitmap(IDB_BITMAP1);
	imagelist.Add(&bmp,RGB(255,0,255));
		// 
	imagelistDis.Create(16,16,ILC_COLOR24 | ILC_MASK,4,0);
	bmpDis.LoadBitmap(IDB_BITMAP2);
	imagelistDis.Add(&bmpDis,RGB(255,0,255));
		// 
	m_TabCtrl.SetImageLists(&imagelist,&imagelistDis);
}
// 
bool DemoDlg::IsExist(CWnd *pWnd) const
{	for(int i=0, c=m_TabCtrl.GetCount(); i<c; ++i)
		if(m_TabCtrl.GetTabWnd( m_TabCtrl.GetTab(i) )==pWnd->m_hWnd)
			return true;
	return false;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// TabCtrlNotify.
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::OnCloseButtonClicked(TabCtrl * /*pCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/)
{	::MessageBox(m_hWnd,_T("DemoDlg::OnCloseButtonClicked"),_T("DemoDlg"),MB_OK);
}
void DemoDlg::OnMenuButtonClicked(TabCtrl * /*pCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/)
{	::MessageBox(m_hWnd,_T("DemoDlg::OnMenuButtonClicked"),_T("DemoDlg"),MB_OK);
}
void DemoDlg::OnDrag(TabCtrl *pCtrl, HTAB hTab, CPoint /*ptScr*/, bool outside)
{	if(outside==true && m_bDelDragOutside==true)
	{	pCtrl->Delete(hTab);
		pCtrl->Update();
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////













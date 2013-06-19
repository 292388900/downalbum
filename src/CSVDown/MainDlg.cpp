// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <afxmt.h>
#include "CSVDown.h"
#include "MainDlg.h"
#include "global.h"
#include <StarLib/File/File.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//初始化全局配置
//////////////////////////////////////////////////////////////////////////
extern CString g_sAPPNAME=APPNAME;
extern CString g_sAPPNAME_CH=APPNAME_CH;
extern CString g_sSTRFILEVER=STRFILEVER;
extern CString g_sHARDCODEPREFIX=HARDCODEPREFIX;

extern CString g_sUPDATE_CONFIG_URL=UPDATE_CONFIG_URL;

extern CString g_sREG_URL=REG_URL;
extern CString g_sHELP_URL=HELP_URL;
extern CString g_sHOME_URL=HOME_URL;
extern CString g_sBBS_URL=BBS_URL;


extern int g_nMAIN_WIDTH=MAIN_WIDTH;
extern int g_nMAIN_HEIGHT=MAIN_HEIGHT;
extern int g_nIMAGE_WIDTH=IMAGE_WIDTH;
extern int g_nIMAGE_HEIGHT=IMAGE_HEIGHT;
//////////////////////////////////////////////////////////////////////////

extern CMainDlg *g_pMainDlg = NULL;

bool CMainDlg::m_bSortAsc=false;



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMainDlg 对话框




CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_lstCtrl);
	DDX_Control(pDX, IDC_BUTTON_VIEW_MODE, m_ctrlViewMode);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POPMSG,&CMainDlg::OnTrayPopMsg)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CMainDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_MODE, &CMainDlg::OnBnClickedButtonViewMode)
	ON_COMMAND(ID_COMMAND_OPTIONS, &CMainDlg::OnCommandOptions)
	ON_COMMAND(ID_BUYNOW, &CMainDlg::OnBuynow)
	ON_MESSAGE(WM_SHOWREGDLG,&CMainDlg::OnShowRegDlg)
	ON_COMMAND(ID_CHECK_UPDATE, &CMainDlg::OnCheckUpdate)
	ON_COMMAND(ID_VIEW_ALLSOFTS, &CMainDlg::OnViewAllsofts)
	ON_COMMAND(ID_VIEW_HELP, &CMainDlg::OnViewHelp)
	ON_COMMAND_RANGE(ID_SHARETO_START,ID_SHARETO_END,&CMainDlg::OnShareToOthers)
	ON_COMMAND(ID_BBS, &CMainDlg::OnBbs)
	ON_COMMAND(ID_EMAIL, &CMainDlg::OnEmailMe)
	ON_COMMAND(ID_EXIT, &CMainDlg::OnExit)
	ON_MESSAGE(WM_SENDMESSAGEBOX,&CMainDlg::OnSendMessageBox)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//////////////////////////////////////////////////////////////////////////
	InitAllControls();
	AppendText("正在检查是否有最新版本……",0);
	AfxBeginThread(ThreadCheckVersion,NULL);
	UpdateData(FALSE);

	SetupTrayIcon();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

UINT CMainDlg::ThreadCheckVersion(LPVOID lpParam)
{
	static CEvent evt(true);    //初始为设置状态的自动重置事件
	if  ( ::WaitForSingleObject(evt.m_hObject,0)==WAIT_OBJECT_0 ){
		//事件之前为设置状态，先在自动重置为重置状态

		CString strText;
		CMainDlg*pMainDlg = g_pMainDlg;
		//检查更新信息
		if ( CheckUpdateInfoIni(g_sUPDATE_CONFIG_URL,g_stUpdateInfo )==TRUE ){

			//获取更新信息成功
			AfxBeginThread(ThreadGetAdNews,pMainDlg);
			AppendText(g_stUpdateInfo.strNews);

			if ( g_stUpdateInfo.strVersion.Compare(g_sSTRFILEVER)!=0 ){
				g_bNeedUpdate=TRUE;
				//版本不一致则下载更新的程序
				strText.Format("%s发现可用更新(v%s)，是否立即更新?",g_sAPPNAME,g_stUpdateInfo.strVersion);
				int nRet = ::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),strText,
					"更新提示",MB_YESNO|MB_ICONINFORMATION);
				if ( nRet==IDYES ){
					strText=g_stUpdateInfo.strDownUrl;
					strText.Replace(".exe",".rar");
					Star::Common::OpenUrl(strText);
				}
			}else{
				strText = _T("当前版本是最新版!");
				AppendText(strText,0);
				if ( lpParam!=NULL ){
					//手动点击检查是否有新版
					pMainDlg->SendMessage(CMainDlg::WM_SENDMESSAGEBOX,MSGBOXTIP::LatestVersion);
				}
			}
		}else{
			pMainDlg->SendMessage(CMainDlg::WM_SENDMESSAGEBOX,MSGBOXTIP::CheckVersionError);
		}

		/*********************************************/

		//函数执行完设置为设置状态
		evt.SetEvent();
	}

	return 0;
}

UINT CMainDlg::ThreadGetAdNews(LPVOID lpParam)		//获取公告、新闻、推广信息
{
	if ( lpParam==NULL || g_config.bShowScrollAds==FALSE || g_stUpdateInfo.strAdsUrl.IsEmpty()==TRUE ){
		return 0;
	}

	//vector<CString>vtNews;
	//CString strLine;
	//CString strText;
	//CString strUrl;
	//CString strHtml;
	//int nPos1 = 0;
	//int nPos2 = 0;

	//CMainDlg*pMainDlg=(CMainDlg*)lpParam;

	//GetHttpFileContent(g_stUpdateInfo.strAdsUrl,strHtml);
	//Star::Common::SplitString(strHtml,"\n",vtNews);

	//for ( int i=0; i<(int)vtNews.size(); ++i ){
	//	strLine = vtNews[i];
	//	nPos1 = strLine.Find('|');
	//	if ( nPos1==-1 ){
	//		continue;;
	//	}

	//	strText = strLine.Left(nPos1);
	//	strUrl = strLine.Mid(nPos1+1).Trim();

	//	if ( strText.IsEmpty()==FALSE ){
	//		pMainDlg->m_stcAdNews.SetWindowText(strText);
	//		pMainDlg->m_stcAdNews.SetURL(strUrl);
	//		break;
	//	}
	//}

	return 0;
}

void CMainDlg::InitAllControls()
{
	//////////////////////////////////////////////////////////////////////////
	CString strText;
	strText.Format( _T("%s - v%s"), g_sAPPNAME_CH, g_sSTRFILEVER );
	this->SetWindowText(strText);
	//////////////////////////////////////////////////////////////////////////

	CRect rcWnd;
	this->GetClientRect(&rcWnd);


	//创建状态栏
	CRect rcStatusbar(rcWnd);
	rcStatusbar.top = rcWnd.bottom-20;
	m_wndStatusbar.Create(WS_VISIBLE|CBRS_BOTTOM,rcStatusbar,this,3);  
	int nParts[]= {150, -1};								//分割尺寸
	m_wndStatusbar.SetParts(_countof(nParts), nParts);		//分割状态栏
	g_pStatusbar = &m_wndStatusbar;


	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_VIEWMODE);
	m_ctrlViewMode.SetIcon(hIcon);


	m_lstCtrl.SetExtendedStyle( m_lstCtrl.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT & (~(LVS_EX_GRIDLINES)) );
	CRect rcList;
	m_lstCtrl.GetClientRect(&rcList);
	m_lstCtrl.InsertColumn( COLUMN_INDEX_NAME, _T("名称"), LVCFMT_LEFT, rcList.Width()-80-80-80-150-120 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_DIMENSION, _T("尺寸"), LVCFMT_LEFT, 80 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_FILESIZE, _T("文件大小"), LVCFMT_LEFT, 80 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_IMAGETYPE, _T("类型"), LVCFMT_LEFT, 80 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_PHOTOTIME, _T("拍摄日期"), LVCFMT_LEFT, 150 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_UPDATETIME, _T("修改日期"), LVCFMT_LEFT, 150 );

	ResetImageList();

	//窗口布局
	m_arranger.Init(m_hWnd);
	//////////////////////////////////////////////////////////////////////////
	m_arranger.FixAll(CDlgResizeHelper::kWidthLeft,CDlgResizeHelper::kHeightTop);

	//靠左下固定，高度固定
	m_arranger.Fix(m_wndStatusbar.GetSafeHwnd(), CDlgResizeHelper::kLeft, CDlgResizeHelper::kHeightBottom);

	//铺满其余底部
	m_arranger.Fix(m_lstCtrl.GetSafeHwnd(), CDlgResizeHelper::kLeftRight, CDlgResizeHelper::kTopBottom);
	//////////////////////////////////////////////////////////////////////////

}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_arranger.OnSize();
}

//图片预览模式发生变化，重新创建imagelist
void CMainDlg::ResetImageList()
{
	if ( m_imageList.GetSafeHandle() ){
		// Remove every other image from the image list.
		for ( int i=0; i < m_imageList.GetImageCount(); ++i){
			m_imageList.Remove(i);
		}
		m_imageList.DeleteImageList();
	}

	if ( g_config.nViewMode==VIEWMODE_BIGICON ){			//大图标
		m_lstCtrl.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
		g_config.nImageViewWidth = WIDTH_VIEWMODE_BIGICON;
		g_config.nImageViewHeight = HEIGHT_VIEWMODE_BIGICON;
		m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	}else if ( g_config.nViewMode==VIEWMODE_LARGEICON ){	//超大图标
		g_config.nImageViewWidth = WIDTH_VIEWMODE_LARGEICON;
		g_config.nImageViewHeight = HEIGHT_VIEWMODE_LARGEICON;
		m_lstCtrl.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
		m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	}else{							//详细信息
		m_lstCtrl.ModifyStyle( LVS_LIST | LVS_SMALLICON | LVS_ICON | LVS_NOCOLUMNHEADER, LVS_REPORT);
		m_imageList.Create(16,16,ILC_COLOR24,10,1);
	}
	m_lstCtrl.SetImageList(&m_imageList,LVSIL_NORMAL);
}

void CMainDlg::SetupTrayIcon()
{
	m_popMsg.cbSize=sizeof(m_popMsg);
	m_popMsg.hWnd=m_hWnd;
	m_popMsg.uID=ID_POPMSG;
	m_popMsg.uFlags=NIF_ICON | NIF_TIP | NIF_MESSAGE;
	m_popMsg.uCallbackMessage=WM_POPMSG;
	m_popMsg.hIcon=m_hIcon;
	lstrcpy(m_popMsg.szTip,g_sAPPNAME_CH);
	Shell_NotifyIconA(NIM_ADD,&m_popMsg);
}

void CMainDlg::MinimizeToTrayArea()
{
	ShowWindow(SW_HIDE);
	ShowToolTip("已最小化到托盘区域，双击后打开界面!");
}

LRESULT CMainDlg::OnTrayPopMsg(WPARAM wParam,LPARAM lParam)
{
	if ( lParam==WM_LBUTTONDBLCLK ){
		this->ShowWindow(SW_SHOW);
		this->SendMessage(WM_SYSCOMMAND,SC_RESTORE);
		this->BringWindowToTop();

	}else if ( lParam==WM_RBUTTONUP ){

		CMenu m_popmenu;
		m_popmenu.LoadMenu(IDR_MENU_TRAY);	
		CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
		CPoint pt;
		GetCursorPos(&pt);
		SetForegroundWindow();
		DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
			pt.x,pt.y, this);

		this->SendMessage(WM_COMMAND,dwID);
		m_popmenu.DestroyMenu();
	}

	return 0;
}

void CMainDlg::Finalization()
{
	Shell_NotifyIconA(NIM_DELETE,(PNOTIFYICONDATAA)&m_popMsg);
}

void CMainDlg::OnOK()
{
	Finalization();
	__super::OnOK();
}

void CMainDlg::OnCancel()
{
	Finalization();
	__super::OnCancel();
}

void CMainDlg::PostNcDestroy()
{
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnBnClickedCheckAll()
{
	int nCheck = ((CButton *)GetDlgItem(IDC_CHECK_ALL))->GetCheck();
	BOOL bChecked = (nCheck==BST_CHECKED);

	for ( int i=0; i<m_lstCtrl.GetItemCount(); ++i ){
		m_lstCtrl.SetCheck(i,bChecked);
	}
}

void CMainDlg::OnBnClickedButtonViewMode()
{
	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_POPMENU_VIEWMODE);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	psub->CheckMenuRadioItem(ID_VIEWMODE_START,ID_VIEWMODE_END,g_config.nViewMode+ID_VIEWMODE_START,MF_BYCOMMAND);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),pt.x,pt.y, this);
	//SendMessage(WM_COMMAND,dwID);
	if ( dwID ){
		//只有选中的时候才更新
		int nLastViewMode = g_config.nViewMode;
		g_config.nViewMode = dwID - ID_VIEWMODE_START;
		if ( g_config.nViewMode!=nLastViewMode ){
			//只有切换了不同的模式才更新
			Star::File::SetIniInt(g_config.m_strCfgFile,"main","viewmode",g_config.nViewMode);
			ResetImageList();
			/*if ( m_vtPhotos.empty()==false ){
				OnBnClickedButtonViewPhotos();
			}*/
		}
	}
}

//让线程中弹出的对话框也有皮肤并置前，wParam区分消息号，lParam如果不为NULL就是字符串指针
LRESULT CMainDlg::OnSendMessageBox(WPARAM wParam,LPARAM lParam)
{
	CString strText;
	char*szText=NULL;

	switch ( (int)wParam )
	{
	case MSGBOXTIP::LatestVersion:
		{
			//当前版本已经是最新版!
#ifdef _UNICODE
			TCHAR szLatestVersion[] = { 0x5F53, 0x524D, 0x7248, 0x672C, 0x5DF2, 0x7ECF, 0x662F, 0x6700, 0x65B0, 0x7248, '!', '\0' };
#else
			CHAR szLatestVersion[] = { 0xB5, 0xB1, 0xC7, 0xB0, 0xB0, 0xE6, 0xB1, 0xBE, 0xD2, 0xD1, 0xBE, 0xAD, 0xCA, 0xC7, 0xD7, 0xEE, 0xD0, 0xC2, 0xB0, 0xE6, '!', '\0' };
#endif
			strText=szLatestVersion;
			AppendText(strText);
			ShowToolTip(strText);
			AfxMessageBox(strText,MB_OK|MB_ICONINFORMATION);
		}
		break;
	case MSGBOXTIP::CheckVersionError:
		{

			strText="无法获取最新版本信息，可能是以下原因导致：\n"
				"1.网络连接断开；\n"
				"2.杀软误报，请把本软件添加为信任；\n"
				"3.防火墙阻止了本软件，请允许本软件访问网络；\n";
			AppendText(strText);
			ShowToolTip(strText);
			//AfxMessageBox(strText,MB_OK|MB_ICONINFORMATION);
		}
		break;
	case MSGBOXTIP::TrialVersion:
		{

			//您当前使用的是试用版(只能处理部分图片)，\n\n请购买正式版!
#ifdef _UNICODE
			TCHAR szshiyongban1[] = { 0x60A8, 0x5F53, 0x524D, 0x4F7F, 0x7528, 0x7684, 0x662F, 0x8BD5, 0x7528, 0x7248, '(', 0x53EA, 0x80FD, 0x5904, 0x7406, 0x90E8, 0x5206, 0x56FE, 0x7247, ')', 0xFF0C, '\n', '\n', 0x8BF7, 0x8D2D, 0x4E70, 0x6B63, 0x5F0F, 0x7248, '!', '\0' };
#else
			CHAR szshiyongban1[] = { 0xC4, 0xFA, 0xB5, 0xB1, 0xC7, 0xB0, 0xCA, 0xB9, 0xD3, 0xC3, 0xB5, 0xC4, 0xCA, 0xC7, 0xCA, 0xD4, 0xD3, 0xC3, 0xB0, 0xE6, '(', 0xD6, 0xBB, 0xC4, 0xDC, 0xB4, 0xA6, 0xC0, 0xED, 0xB2, 0xBF, 0xB7, 0xD6, 0xCD, 0xBC, 0xC6, 0xAC, ')', 0xA3, 0xAC, '\n', '\n', 0xC7, 0xEB, 0xB9, 0xBA, 0xC2, 0xF2, 0xD5, 0xFD, 0xCA, 0xBD, 0xB0, 0xE6, '!', '\0' };
#endif
			//处理全部图片请购买正式版，谢谢支持!
#ifdef _UNICODE
			TCHAR szqinggoumai[] = { 0x5904, 0x7406, 0x5168, 0x90E8, 0x56FE, 0x7247, 0x8BF7, 0x8D2D, 0x4E70, 0x6B63, 0x5F0F, 0x7248, 0xFF0C, 0x8C22, 0x8C22, 0x652F, 0x6301, '!', '\0' };
#else
			CHAR szqinggoumai[] = { 0xB4, 0xA6, 0xC0, 0xED, 0xC8, 0xAB, 0xB2, 0xBF, 0xCD, 0xBC, 0xC6, 0xAC, 0xC7, 0xEB, 0xB9, 0xBA, 0xC2, 0xF2, 0xD5, 0xFD, 0xCA, 0xBD, 0xB0, 0xE6, 0xA3, 0xAC, 0xD0, 0xBB, 0xD0, 0xBB, 0xD6, 0xA7, 0xB3, 0xD6, '!', '\0' };
#endif
			//试用版-只能处理部分图片
#ifdef _UNICODE
			TCHAR szshiyongban2[] = { 0x8BD5, 0x7528, 0x7248, '-', 0x53EA, 0x80FD, 0x5904, 0x7406, 0x90E8, 0x5206, 0x56FE, 0x7247, '\0' };
#else
			CHAR sszshiyongban2[] = { 0xCA, 0xD4, 0xD3, 0xC3, 0xB0, 0xE6, '-', 0xD6, 0xBB, 0xC4, 0xDC, 0xB4, 0xA6, 0xC0, 0xED, 0xB2, 0xBF, 0xB7, 0xD6, 0xCD, 0xBC, 0xC6, 0xAC, '\0' };
#endif

			strText=szshiyongban1;
			AppendText(strText);
			ShowToolTip(strText);
			::MessageBox(m_hWnd,szqinggoumai,sszshiyongban2,MB_OK|MB_ICONINFORMATION);
			//OnBuynow();
		}
		break;
	case MSGBOXTIP::InvalidSavePath:
		{

			//图片保存目录不存在，请在“选项设置”中重新设置
#ifdef _UNICODE
			TCHAR szInvalidSavePath[] = { 0x56FE, 0x7247, 0x4FDD, 0x5B58, 0x76EE, 0x5F55, 0x4E0D, 0x5B58, 0x5728, 0xFF0C, 0x8BF7, 0x5728, 0x201C, 0x9009, 0x9879, 0x8BBE, 0x7F6E, 0x201D, 0x4E2D, 0x91CD, 0x65B0, 0x8BBE, 0x7F6E, '\0' };
#else
			CHAR szInvalidSavePath[] = { 0xCD, 0xBC, 0xC6, 0xAC, 0xB1, 0xA3, 0xB4, 0xE6, 0xC4, 0xBF, 0xC2, 0xBC, 0xB2, 0xBB, 0xB4, 0xE6, 0xD4, 0xDA, 0xA3, 0xAC, 0xC7, 0xEB, 0xD4, 0xDA, 0xA1, 0xB0, 0xD1, 0xA1, 0xCF, 0xEE, 0xC9, 0xE8, 0xD6, 0xC3, 0xA1, 0xB1, 0xD6, 0xD0, 0xD6, 0xD8, 0xD0, 0xC2, 0xC9, 0xE8, 0xD6, 0xC3, '\0' };
#endif

			strText=szInvalidSavePath;
			AppendText(strText);
			ShowToolTip(strText);
			AfxMessageBox(strText,MB_OK|MB_ICONERROR);
		}
		break;
	case MSGBOXTIP::SelectNoAlbumToDown:
		{
			//请先勾选要处理的图片，然后再进行批量操作!
#ifdef _UNICODE
			TCHAR szSelectNoAlbumToDown[] = { 0x8BF7, 0x5148, 0x52FE, 0x9009, 0x8981, 0x5904, 0x7406, 0x7684, 0x56FE, 0x7247, 0xFF0C, 0x7136, 0x540E, 0x518D, 0x8FDB, 0x884C, 0x6279, 0x91CF, 0x64CD, 0x4F5C, '!', '\0' };
#else
			CHAR szSelectNoAlbumToDown[] = { 0xC7, 0xEB, 0xCF, 0xC8, 0xB9, 0xB4, 0xD1, 0xA1, 0xD2, 0xAA, 0xB4, 0xA6, 0xC0, 0xED, 0xB5, 0xC4, 0xCD, 0xBC, 0xC6, 0xAC, 0xA3, 0xAC, 0xC8, 0xBB, 0xBA, 0xF3, 0xD4, 0xD9, 0xBD, 0xF8, 0xD0, 0xD0, 0xC5, 0xFA, 0xC1, 0xBF, 0xB2, 0xD9, 0xD7, 0xF7, '!', '\0' };
#endif

			strText=szSelectNoAlbumToDown;
			AppendText(strText);
			ShowToolTip(strText);
			AfxMessageBox(strText,MB_OK|MB_ICONERROR);
		}
		break;
	case MSGBOXTIP::PhotoDownOver:
		{

			szText=(char*)lParam;
			AppendText(szText);
			ShowToolTip(szText);
			//AfxMessageBox(szText,MB_OK|MB_ICONINFORMATION);
		}
		break;
	}

	return 0;
}

//选项设置
void CMainDlg::OnCommandOptions()
{
	//COptionsdlg dlg;
	//dlg.DoModal();
}

LRESULT CMainDlg::OnShowRegDlg(WPARAM wParam,LPARAM lParam)
{
	this->ShowWindow(SW_SHOW);
	this->SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	this->BringWindowToTop();

	return 0;
}

//购买注册
void CMainDlg::OnBuynow()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strRegUrl);
}

//更新升级
void CMainDlg::OnCheckUpdate()
{
	AfxBeginThread(ThreadCheckVersion,this);
}

//查看其他作品
void CMainDlg::OnViewAllsofts()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strAllsoft);
}

//查看使用帮助
void CMainDlg::OnViewHelp()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strHelpUrl);
}

//分享推荐
void CMainDlg::OnShareToOthers(UINT uCtrlID)
{
	CString strUrl;
	CString strText;
	strText.Format("分享一款小软件：%s 批量处理图片很方便",g_sAPPNAME_CH);

	CString strTextURI = Star::Common::GBKEncodeURI(strText);
	CString strSoftUrl = g_stUpdateInfo.strHelpUrl;
	strSoftUrl.Replace("=","%3D");

	switch( uCtrlID )
	{
	case ID_SHARETO_SINAWEIBO:
		strUrl.Format("http://v.t.sina.com.cn/share/share.php?url=%s&title=%s&pic=%s",
			strSoftUrl,strTextURI,"");
		break;
	case ID_SHARETO_TENGXUNWEIBO:
		strUrl.Format("http://v.t.qq.com/share/share.php?title=%s&url=%s&site=%s&pic=%s",
			Star::Common::UTF8EncodeURI(strText),strSoftUrl,strSoftUrl,"");
		break;
	case ID_SHARETO_SOUHUWEIBO:
		strUrl.Format("http://t.sohu.com/third/post.jsp?url=%s&title=%s&content=%s&pic=%s",
			strSoftUrl,strTextURI,strTextURI,"");
		break;
	case ID_SHARETO_WANGYIWEIBO:
		strUrl.Format("http://t.163.com/article/user/checkLogin.do?info=%s+%s&images=%s",
			strTextURI,strSoftUrl,"");
		break;
	case ID_SHARETO_RENRENWANG:
		strUrl.Format("http://share.renren.com/share/buttonshare.do?link=%s&title=%s",
			strSoftUrl,strTextURI);
		break;
	case ID_SHARETO_KAIXINWANG:
		strUrl.Format("http://www.kaixin001.com/repaste/share.php?rtitle=%s&rurl=%s&rcontent=%s",
			strText,strSoftUrl,strSoftUrl);
		break;
	case ID_SHARETO_QQZONE:
		strUrl.Format("http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=%s&title=%s",
			strSoftUrl,strText);
		break;
	}

	if ( strUrl.IsEmpty()==FALSE ){
		Star::Common::OpenUrl(strUrl);
	}
}

//访问官方论坛
void CMainDlg::OnBbs()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strHome);
}

void CMainDlg::OnEmailMe()
{
	Star::Common::OpenUrl("http://mail.qq.com/cgi-bin/qm_share?t=qm_mailme&email=1390226247@qq.com");
}

//退出
void CMainDlg::OnExit()
{
	OnOK();
}
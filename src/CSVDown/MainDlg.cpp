// MainDlg.cpp : ʵ���ļ�
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


//��ʼ��ȫ������
//////////////////////////////////////////////////////////////////////////
extern CString g_sAPPNAME=APPNAME;
extern CString g_sAPPNAME_CH=APPNAME_CH;
extern CString g_sSTRFILEVER=STRFILEVER;
extern CString g_sHARDCODEPREFIX=HARDCODEPREFIX;

extern CString g_sUPDATE_CONFIG_URL=UPDATE_CONFIG_URL;
extern CString g_sUPDATE_CONFIG_URL2=UPDATE_CONFIG_URL2;

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
vector<CBaseAlbumInfo*>CMainDlg::m_vtAlbumList;



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMainDlg �Ի���




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
	DDX_Control(pDX, IDC_COMBO_HISTORY, m_cmbHistory);
	DDX_Text(pDX, IDC_EDIT_URL, m_strAccount);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_ITEMS, &CMainDlg::OnNMClickListAlbum)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ITEMS, &CMainDlg::OnNMRClickListAlbum)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ITEMS, &CMainDlg::OnColumnClickListAlbum)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ITEMS, &CMainDlg::OnGetDispInfo)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_ITEMS, &CMainDlg::OnDeleteItemList) 
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
	ON_BN_CLICKED(IDC_BUTTON_GETITEMS, &CMainDlg::OnBnClickedButtonGetitems)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_CSV, &CMainDlg::OnBnClickedButtonDownCsv)
	ON_MESSAGE(WM_GETITEMSBEGIN,&CMainDlg::OnGetItemsBegin)
	ON_MESSAGE(WM_GETITEMSEND,&CMainDlg::OnGetItemsEnd)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//////////////////////////////////////////////////////////////////////////
	InitAllControls();
	AppendText("���ڼ���Ƿ������°汾����",0);
	AfxBeginThread(ThreadCheckVersion,NULL);
	UpdateData(FALSE);

	SetupTrayIcon();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

UINT CMainDlg::ThreadCheckVersion(LPVOID lpParam)
{
	static CEvent evt(true);    //��ʼΪ����״̬���Զ������¼�
	if  ( ::WaitForSingleObject(evt.m_hObject,0)==WAIT_OBJECT_0 ){
		//�¼�֮ǰΪ����״̬�������Զ�����Ϊ����״̬

		CString strText;
		CMainDlg*pMainDlg = g_pMainDlg;

		//��������Ϣ
		BOOL bOK = CheckUpdateInfoIni(g_sUPDATE_CONFIG_URL,g_stUpdateInfo);
		if ( bOK==FALSE ){
			bOK = CheckUpdateInfoIni(g_sUPDATE_CONFIG_URL2,g_stUpdateInfo);
		}

		if ( bOK==TRUE ){

			//��ȡ������Ϣ�ɹ�
			AfxBeginThread(ThreadGetAdNews,pMainDlg);
			AppendText(g_stUpdateInfo.strNews);

			if ( g_stUpdateInfo.strVersion.Compare(g_sSTRFILEVER)!=0 ){
				g_bNeedUpdate=TRUE;
				//�汾��һ�������ظ��µĳ���
				strText.Format("%s���ֿ��ø���(v%s)���Ƿ���������?",g_sAPPNAME,g_stUpdateInfo.strVersion);
				int nRet = ::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),strText,
					"������ʾ",MB_YESNO|MB_ICONINFORMATION);
				if ( nRet==IDYES ){
#ifdef _TRIAL 
					strText=g_stUpdateInfo.strDownUrl;
#else
					strText=g_stUpdateInfo.strDownUrl2;
#endif
					strText.Replace(".exe",".rar");
					Star::Common::OpenUrl(strText);
				}
			}else{
				strText = _T("��ǰ�汾�����°�!");
				AppendText(strText,0);
				if ( lpParam!=NULL ){
					//�ֶ��������Ƿ����°�
					pMainDlg->SendMessage(CMainDlg::WM_SENDMESSAGEBOX,MSGBOXTIP::LatestVersion);
				}
			}
		}else{
			pMainDlg->SendMessage(CMainDlg::WM_SENDMESSAGEBOX,MSGBOXTIP::CheckVersionError);
		}

		/*********************************************/

		//����ִ��������Ϊ����״̬
		evt.SetEvent();
	}

	return 0;
}

UINT CMainDlg::ThreadGetAdNews(LPVOID lpParam)		//��ȡ���桢���š��ƹ���Ϣ
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


	//����״̬��
	CRect rcStatusbar(rcWnd);
	rcStatusbar.top = rcWnd.bottom-20;
	m_wndStatusbar.Create(WS_VISIBLE|CBRS_BOTTOM,rcStatusbar,this,3);  
	int nParts[]= {150, -1};								//�ָ�ߴ�
	m_wndStatusbar.SetParts(_countof(nParts), nParts);		//�ָ�״̬��
	g_pStatusbar = &m_wndStatusbar;


	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_VIEWMODE);
	m_ctrlViewMode.SetIcon(hIcon);


	m_lstCtrl.SetExtendedStyle( m_lstCtrl.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT & (~(LVS_EX_GRIDLINES)) );
	CRect rcList;
	m_lstCtrl.GetClientRect(&rcList);
	m_lstCtrl.InsertColumn( COLUMN_INDEX_NAME, _T("����"), LVCFMT_LEFT, rcList.Width()-80-80-80-150-120 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_DIMENSION, _T("�ߴ�"), LVCFMT_LEFT, 80 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_FILESIZE, _T("�ļ���С"), LVCFMT_LEFT, 80 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_IMAGETYPE, _T("����"), LVCFMT_LEFT, 80 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_PHOTOTIME, _T("��������"), LVCFMT_LEFT, 150 );
	m_lstCtrl.InsertColumn( COLUMN_INDEX_UPDATETIME, _T("�޸�����"), LVCFMT_LEFT, 150 );

	ResetImageList();

	//////////////////////////////////////////////////////////////////////////
	//�ʵ��������֣�������m_cmbHistory�ı༭��
	CWnd *pEditUrl = GetDlgItem(IDC_EDIT_URL);
	CWnd *pEdit = m_cmbHistory.FindWindowEx(m_cmbHistory.GetSafeHwnd(),0,NULL,NULL);
	if ( pEdit!=NULL && pEditUrl!=NULL ){
		CRect rcEdit;
		CRect rcCmb;
		CRect rcCmbEdit;
		int nMargin = 0;
		m_cmbHistory.GetWindowRect(rcCmb);
		pEditUrl->GetWindowRect(rcEdit);
		pEdit->GetWindowRect(rcCmbEdit);
		nMargin = rcCmb.Width() - rcCmbEdit.Width();
		if ( nMargin>4 ){
			nMargin -= 4;
		}else{
			nMargin = 13;
		}

		//ʹ��m_cmbHistory�պñ�m_edtUrl��һ��nMargin
		//m_cmbHistory�ĸ��ǹ̶��ģ�����m_edtUrl�ĸ߶�Ǩ��m_cmbAccountһ��
		rcEdit.bottom = rcEdit.top + rcCmb.Height();
		rcCmb.top = rcEdit.top;
		rcCmb.bottom = rcEdit.bottom;
		rcCmb.left = rcEdit.left;
		rcCmb.right = rcEdit.right + nMargin;

		this->ScreenToClient(rcEdit);
		this->ScreenToClient(rcCmb);
		pEditUrl->MoveWindow(rcEdit);
		m_cmbHistory.MoveWindow(rcCmb);

		pEdit->ShowWindow(SW_HIDE);
	}

	//������ʷ��¼
	LoadHistoryUrlToFile();

	//���ڲ���
	m_arranger.Init(m_hWnd);
	//////////////////////////////////////////////////////////////////////////
	m_arranger.FixAll(CDlgResizeHelper::kWidthLeft,CDlgResizeHelper::kHeightTop);

	//�����¹̶����߶ȹ̶�
	m_arranger.Fix(m_wndStatusbar.GetSafeHwnd(), CDlgResizeHelper::kLeft, CDlgResizeHelper::kHeightBottom);

	//��������ײ�
	m_arranger.Fix(m_lstCtrl.GetSafeHwnd(), CDlgResizeHelper::kLeftRight, CDlgResizeHelper::kTopBottom);
	//////////////////////////////////////////////////////////////////////////

}

void CMainDlg::AddHistoryUrl(const CString&str)
{
	BOOL bFound = FALSE;

	CString strText;
	int nCount = m_cmbHistory.GetCount();
	for ( int i=0; i<nCount; ++i ){
		m_cmbHistory.GetLBText(i,strText);
		if ( strText.Compare(str)==0 ){
			bFound = TRUE;
			m_cmbHistory.DeleteString(i);
			m_cmbHistory.InsertString(0,str);
			break;
		}
	}

	if ( bFound==FALSE ){
		if ( nCount>=10 ){
			m_cmbHistory.DeleteString(nCount-1);
		}

		m_cmbHistory.InsertString(0,str);
	}
}

void CMainDlg::SaveHistoryUrlToFile()
{
	int nIndex = 0;
	CString strText;
	CString strTemp;

	for ( int i=0; i<m_cmbHistory.GetCount(); ++i ){
		m_cmbHistory.GetLBText(i,strTemp);
		strText = strText + strTemp + _T("|");
	}

	Star::File::SetIniString(g_config.m_strCfgFile,"history","urls",strText);
}

void CMainDlg::LoadHistoryUrlToFile()
{
	CString strText = Star::File::GetIniString(g_config.m_strCfgFile,"history","urls");
	if ( strText.IsEmpty()==FALSE ){
		vector<CString>vtHistoryUrl;
		Star::Common::SplitString(strText,_T("|"),vtHistoryUrl);
		for ( vector<CString>::iterator i=vtHistoryUrl.begin(); i!=vtHistoryUrl.end(); ++i ){
			strText = *i;
			strText.Trim();
			if ( strText.IsEmpty()==FALSE ){
				m_cmbHistory.InsertString(-1,strText);
			}
		}//end for
	}
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_arranger.OnSize();
}

//ͼƬԤ��ģʽ�����仯�����´���imagelist
void CMainDlg::ResetImageList()
{
	if ( m_imageList.GetSafeHandle() ){
		// Remove every other image from the image list.
		for ( int i=0; i < m_imageList.GetImageCount(); ++i){
			m_imageList.Remove(i);
		}
		m_imageList.DeleteImageList();
	}

	if ( g_config.nViewMode==VIEWMODE_BIGICON ){			//��ͼ��
		m_lstCtrl.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
		g_config.nImageViewWidth = WIDTH_VIEWMODE_BIGICON;
		g_config.nImageViewHeight = HEIGHT_VIEWMODE_BIGICON;
		m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	}else if ( g_config.nViewMode==VIEWMODE_LARGEICON ){	//����ͼ��
		g_config.nImageViewWidth = WIDTH_VIEWMODE_LARGEICON;
		g_config.nImageViewHeight = HEIGHT_VIEWMODE_LARGEICON;
		m_lstCtrl.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
		m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	}else{							//��ϸ��Ϣ
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
	ShowToolTip("����С������������˫����򿪽���!");
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
	ClearAlbumList(m_vtAlbumList);
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

void ClearAlbumList(vector<CBaseAlbumInfo*>&vtAlbum)
{
	for ( int i=0; i<(int)vtAlbum.size(); ++i ){
		CBaseAlbumInfo*pAlbum=vtAlbum[i];
		delete pAlbum;
	}

	vtAlbum.clear();
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		//ֻ��ѡ�е�ʱ��Ÿ���
		int nLastViewMode = g_config.nViewMode;
		g_config.nViewMode = dwID - ID_VIEWMODE_START;
		if ( g_config.nViewMode!=nLastViewMode ){
			//ֻ���л��˲�ͬ��ģʽ�Ÿ���
			Star::File::SetIniInt(g_config.m_strCfgFile,"main","viewmode",g_config.nViewMode);
			ResetImageList();
			/*if ( m_vtPhotos.empty()==false ){
				OnBnClickedButtonViewPhotos();
			}*/
		}
	}
}

#pragma warning(disable:4309)

//���߳��е����ĶԻ���Ҳ��Ƥ������ǰ��wParam������Ϣ�ţ�lParam�����ΪNULL�����ַ���ָ��
LRESULT CMainDlg::OnSendMessageBox(WPARAM wParam,LPARAM lParam)
{
	CString strText;
	char*szText=NULL;

	switch ( (int)wParam )
	{
	case MSGBOXTIP::LatestVersion:
		{
			//��ǰ�汾�Ѿ������°�!
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

			strText="�޷���ȡ���°汾��Ϣ������������ԭ���£�\n"
				"1.�������ӶϿ���\n"
				"2.ɱ���󱨣���ѱ�������Ϊ���Σ�\n"
				"3.����ǽ��ֹ�˱����������������������磻\n";
			AppendText(strText);
			ShowToolTip(strText);
			//AfxMessageBox(strText,MB_OK|MB_ICONINFORMATION);
		}
		break;
	case MSGBOXTIP::TrialVersion:
		{

			//����ǰʹ�õ������ð�(ֻ�ܴ�����ͼƬ)��\n\n�빺����ʽ��!
#ifdef _UNICODE
			TCHAR szshiyongban1[] = { 0x60A8, 0x5F53, 0x524D, 0x4F7F, 0x7528, 0x7684, 0x662F, 0x8BD5, 0x7528, 0x7248, '(', 0x53EA, 0x80FD, 0x5904, 0x7406, 0x90E8, 0x5206, 0x56FE, 0x7247, ')', 0xFF0C, '\n', '\n', 0x8BF7, 0x8D2D, 0x4E70, 0x6B63, 0x5F0F, 0x7248, '!', '\0' };
#else
			CHAR szshiyongban1[] = { 0xC4, 0xFA, 0xB5, 0xB1, 0xC7, 0xB0, 0xCA, 0xB9, 0xD3, 0xC3, 0xB5, 0xC4, 0xCA, 0xC7, 0xCA, 0xD4, 0xD3, 0xC3, 0xB0, 0xE6, '(', 0xD6, 0xBB, 0xC4, 0xDC, 0xB4, 0xA6, 0xC0, 0xED, 0xB2, 0xBF, 0xB7, 0xD6, 0xCD, 0xBC, 0xC6, 0xAC, ')', 0xA3, 0xAC, '\n', '\n', 0xC7, 0xEB, 0xB9, 0xBA, 0xC2, 0xF2, 0xD5, 0xFD, 0xCA, 0xBD, 0xB0, 0xE6, '!', '\0' };
#endif
			//����ȫ��ͼƬ�빺����ʽ�棬лл֧��!
#ifdef _UNICODE
			TCHAR szqinggoumai[] = { 0x5904, 0x7406, 0x5168, 0x90E8, 0x56FE, 0x7247, 0x8BF7, 0x8D2D, 0x4E70, 0x6B63, 0x5F0F, 0x7248, 0xFF0C, 0x8C22, 0x8C22, 0x652F, 0x6301, '!', '\0' };
#else
			CHAR szqinggoumai[] = { 0xB4, 0xA6, 0xC0, 0xED, 0xC8, 0xAB, 0xB2, 0xBF, 0xCD, 0xBC, 0xC6, 0xAC, 0xC7, 0xEB, 0xB9, 0xBA, 0xC2, 0xF2, 0xD5, 0xFD, 0xCA, 0xBD, 0xB0, 0xE6, 0xA3, 0xAC, 0xD0, 0xBB, 0xD0, 0xBB, 0xD6, 0xA7, 0xB3, 0xD6, '!', '\0' };
#endif
			//���ð�-ֻ�ܴ�����ͼƬ
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

			//ͼƬ����Ŀ¼�����ڣ����ڡ�ѡ�����á�����������
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
			//���ȹ�ѡҪ�����ͼƬ��Ȼ���ٽ�����������!
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

#pragma warning(default:4309)

//ѡ������
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

//����ע��
void CMainDlg::OnBuynow()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strRegUrl);
}

//��������
void CMainDlg::OnCheckUpdate()
{
	AfxBeginThread(ThreadCheckVersion,this);
}

//�鿴������Ʒ
void CMainDlg::OnViewAllsofts()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strAllsoft);
}

//�鿴ʹ�ð���
void CMainDlg::OnViewHelp()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strHelpUrl);
}

//�����Ƽ�
void CMainDlg::OnShareToOthers(UINT uCtrlID)
{
	CString strUrl;
	CString strText;
	strText.Format("����һ��С�����%s ��������ͼƬ�ܷ���",g_sAPPNAME_CH);

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

//���ʹٷ���̳
void CMainDlg::OnBbs()
{
	Star::Common::OpenUrl(g_stUpdateInfo.strHome);
}

void CMainDlg::OnEmailMe()
{
	Star::Common::OpenUrl("http://mail.qq.com/cgi-bin/qm_share?t=qm_mailme&email=1390226247@qq.com");
}

//�˳�
void CMainDlg::OnExit()
{
	OnOK();
}

void CMainDlg::OnBnClickedButtonGetitems()
{
	GetDlgItem(IDC_BUTTON_GETITEMS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN_CSV)->EnableWindow(FALSE);
	StartGetItems();
}

void CMainDlg::OnBnClickedButtonDownCsv()
{
	GetDlgItem(IDC_BUTTON_GETITEMS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN_CSV)->EnableWindow(FALSE);
	StartDownCSV();
}

void CMainDlg::StartGetItems()
{
	UpdateData();
	m_strAccount.Trim();
	m_strAccount.MakeLower();
	UpdateData(FALSE);
	AfxBeginThread(ThreadGetItems,this);
}

UINT CMainDlg::ThreadGetItems(LPVOID lpParam)
{
	CoInitialize(NULL);	//���һ��Ҫ��������URLDownloadToFile����0x80040155L
	AppendText("���ڻ�ȡ�����Ϣ�����Եȡ���");
	ShowToolTip("���ڻ�ȡ�����Ϣ�����Եȡ���");
	CMainDlg *pMainDlg = (CMainDlg*)lpParam;
	pMainDlg->SendMessage(WM_GETITEMSBEGIN);

//	//ͨ���ű���ȡ
//#ifdef _DEBUG
//	RunScript(NULL);
//#endif
//
//	if ( L!=NULL ){
//		lua_getglobal(L, "GetAlbums");  
//		//����в�����ѹ�������
//		//lua_pushstring(L, szInParam);  
//		lua_pushnumber(L, pAlbumDlg->m_nDownType);  
//		lua_pushstring(L, pAlbumDlg->m_strAccount);  
//		int err = lua_pcall( L, 2, 0, 0 );  
//		if ( err ){
//			const char *szMsg = lua_tostring(L,-1);
//			pAlbumDlg->MessageBox(szMsg,"�ű���д������ο��ĵ���д",MB_ICONERROR);
//		}
//	}
//
//	//����������ľ��巽����ȡ����б�
//	pAlbumDlg->GetAlbumList();
//
//	AppendText("������ʾ��ᣬ���Եȡ���");
//	ShowToolTip("������ʾ��ᣬ���Եȡ���");
//	pAlbumDlg->ShowAlbumList();
//	AppendText("�����ʾ��ϣ���ѡ����Ҫ�����ص�����������");
//	ShowToolTip("�����ʾ��ϣ���ѡ����Ҫ�����ص�����������");

	CoUninitialize();
	pMainDlg->SendMessage(WM_GETITEMSEND);
	return 0;
}

LRESULT CMainDlg::OnGetItemsBegin(WPARAM wParam,LPARAM lParam)
{
	m_lstCtrl.DeleteAllItems();
	for ( int i=m_imageList.GetImageCount()-1; i>=0; i-- ){
		m_imageList.Remove(i);
	}

	//��¼�������ʷ��ַ
	AddHistoryUrl(m_strAccount);
	SaveHistoryUrlToFile();

	//����ϴε�����б���Ϣ
	ClearAlbumList(m_vtAlbumList);

	return 0;
}

LRESULT CMainDlg::OnGetItemsEnd(WPARAM wParam,LPARAM lParam)
{
	//CString strText;

	//if ( m_vtAlbumList.empty()==true ){
	//	AppendText(
	//		"���δ�ܻ�ȡ�������Ϣ������������˺��Ƿ�����\r\n"
	//		"����ͨ���鿴������ҳ��ȡ��ȷ��ʹ�÷���");
	//}else{
	//	strText.Format("����⵽ %d ����ᣬ��ѡ��Ҫ����Ὺʼ����",m_vtAlbumList.size());
	//	AppendText(strText);
	//}

	////GetAlbumEnd();

	////Ĭ��ȫ����ѡ
	//if ( m_vtAlbumList.empty()==false ){
	//	CButton*pButton=(CButton*)GetDlgItem(IDC_CHECK_SELECT_ALL);
	//	pButton->SetCheck(BST_CHECKED);
	//	OnBnClickedCheckSelectAll();
	//}
	//this->GetDlgItem(IDC_BUTTON_VIEW_ALBUM)->EnableWindow(TRUE);
	//this->GetDlgItem(IDC_BUTTON_DOWN_PHOTO)->EnableWindow(m_lstCtrl.GetItemCount()>0);
	//m_bIsGettingAlbumList=FALSE;


	////�Ӹ��жϣ��������lua�ű��͵���lua���򲻵���
	////////////////////////////////////////////////////////////////////////////
	////����lua�ű�OnGetAlbumEnd����֪ͨ��ȡ������
	//if ( L!=NULL ){
	//	lua_getglobal(L, "OnGetAlbumEnd");
	//	if ( lua_isfunction(L,-1) ){
	//		int err = lua_pcall( L, 0, 0, 0 );  
	//		if ( err ){
	//			const char *szMsg = lua_tostring(L,-1);
	//			AppendTextV("�ű�����%s",szMsg);
	//		}
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////

	return 0;
}

void CMainDlg::StartDownCSV()
{
}



void CMainDlg::OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO hitinfo;

	//Copy click point
	hitinfo.pt = pNMListView->ptAction;

	//Make the hit test...
	int nItem = m_lstCtrl.HitTest(&hitinfo);
	if ( nItem!=-1 ){
		// Didn't click on an icon
		if(hitinfo.flags != LVHT_ONITEMSTATEICON){
			BOOL bChecked=!m_lstCtrl.GetCheck(nItem);
			m_lstCtrl.SetCheck(nItem,bChecked);
		}
	}

	*pResult = 0;
}

void CMainDlg::OnNMRClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult)
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//*pResult = 0;

	//int nItemCnt=m_lstCtrl.GetItemCount();
	//if ( nItemCnt<=0 || m_bIsGettingAlbumList==TRUE ){
	//	return;
	//}
	//POSITION pos=m_lstCtrl.GetFirstSelectedItemPosition();
	//int nItem=0;
	//BOOL bChecked=FALSE;

	//CMenu m_popmenu;
	//m_popmenu.LoadMenu(IDR_MENU_LIST);	
	//CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	//CPoint pt;
	//GetCursorPos(&pt);
	//psub->EnableMenuItem(ID_COPY_ALBUM_URL,pos==NULL);
	//psub->EnableMenuItem(ID_NAVIGATE_ALBUM_URL,pos==NULL);
	//psub->EnableMenuItem(ID_CHECK_SELECTED,pos==NULL);
	//psub->EnableMenuItem(ID_UNCHECK_SELECTED,pos==NULL);
	//DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
	//	pt.x,pt.y, this);

	//if ( dwID==ID_COPY_ALBUM_URL ){
	//	CString strText;
	//	while ( pos!=NULL ){
	//		nItem=m_lstCtrl.GetNextSelectedItem(pos);
	//		if ( nItem<(int)m_vtAlbumList.size() ){
	//			CBaseAlbumInfo *pAlbum = m_vtAlbumList[nItem];
	//			if ( pAlbum!=NULL && pAlbum->strAlbumUrl.IsEmpty()==FALSE ){
	//				strText += pAlbum->strAlbumUrl + "\n";
	//			}
	//		}
	//	}
	//	Star::Common::CopyToClipboard(strText);
	//}else if ( dwID==ID_NAVIGATE_ALBUM_URL ){
	//	while ( pos!=NULL ){
	//		nItem=m_lstCtrl.GetNextSelectedItem(pos);
	//		if ( nItem<(int)m_vtAlbumList.size() ){
	//			CBaseAlbumInfo *pAlbum = m_vtAlbumList[nItem];
	//			if ( pAlbum!=NULL && pAlbum->strAlbumUrl.IsEmpty()==FALSE ){
	//				Star::Common::OpenUrl(pAlbum->strAlbumUrl);
	//			}
	//		}
	//	}
	//}else if ( dwID==ID_CHECK_ALL ){
	//	for ( int nItem=0; nItem<m_lstCtrl.GetItemCount(); ++nItem ){
	//		m_lstCtrl.SetCheck(nItem,TRUE);
	//	}
	//}else if ( dwID==ID_UNCHECK_ALL ){
	//	for ( int nItem=0; nItem<nItemCnt; ++nItem ){
	//		bChecked=!m_lstCtrl.GetCheck(nItem);
	//		m_lstCtrl.SetCheck(nItem,bChecked);
	//	}
	//}else if ( dwID==ID_CHECK_SELECTED ){
	//	while ( pos!=NULL ){
	//		nItem=m_lstCtrl.GetNextSelectedItem(pos);
	//		m_lstCtrl.SetCheck(nItem,TRUE);
	//	}
	//}else if ( dwID==ID_UNCHECK_SELECTED ){
	//	while ( pos!=NULL ){
	//		nItem=m_lstCtrl.GetNextSelectedItem(pos);
	//		bChecked=!m_lstCtrl.GetCheck(nItem);
	//		m_lstCtrl.SetCheck(nItem,bChecked);
	//	}
	//}

	//m_popmenu.DestroyMenu();

}

void CMainDlg::OnColumnClickListAlbum(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_bSortAsc=!m_bSortAsc;
	m_lstCtrl.SortItems(CompareFunc,pNMListView->iSubItem);

	*pResult=0;
}

int CALLBACK CMainDlg::CompareFunc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	int nResult=0;
	//ITEMINFO* pItem1 = (ITEMINFO*)lParam1;
	//ITEMINFO* pItem2 = (ITEMINFO*)lParam2;

	//switch ( lParamSort )
	//{
	//case 0://���
	//	nResult=pItem1->nIndex-pItem2->nIndex;
	//	break;
	//case 1://�������
	//	nResult=pItem1->strName.CompareNoCase(pItem2->strName);
	//	break;
	//case 2://����
	//	nResult=pItem1->nCount-pItem2->nCount;
	//	break;
	//case 3://��������
	//	nResult=pItem1->strUpdateTime.CompareNoCase(pItem2->strUpdateTime);
	//	break;
	//case 4://����Ȩ��
	//	nResult=pItem1->strAccess.CompareNoCase(pItem2->strAccess);
	//	break;
	//case 5://������Ϣ
	//	nResult=pItem1->strDescription.CompareNoCase(pItem2->strDescription);
	//	break;
	//}

	//if ( m_bSortAsc==FALSE ){
	//	nResult=0-nResult;
	//}

	return nResult;
}

void CMainDlg::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	//CString strText;
	//LV_DISPINFO* pDispInfo = (LV_DISPINFO*) pNMHDR;

	//if ( m_lstCtrl.GetStyle()&LVS_REPORT ){	//��ϸ�б�
	//	if (pDispInfo->item.mask & LVIF_TEXT) {
	//		ITEMINFO* pItem = (ITEMINFO*) pDispInfo->item.lParam;

	//		switch (pDispInfo->item.iSubItem) {

	//	case 0: //���
	//		strText.Format("%d",pItem->nIndex);
	//		::lstrcpy (pDispInfo->item.pszText, strText);
	//		break;

	//	case 1: //�������
	//		::lstrcpy (pDispInfo->item.pszText, pItem->strName);
	//		break;

	//	case 2: //ͼƬ����
	//		if ( pItem->nCount==-1 ){	//ͼƬ����δ֪
	//			::lstrcpy (pDispInfo->item.pszText, "?");
	//		}else{
	//			strText.Format("%d",pItem->nCount);
	//			::lstrcpy (pDispInfo->item.pszText, strText);
	//		}
	//		break;
	//	case 3: //��������
	//		::lstrcpy (pDispInfo->item.pszText, pItem->strUpdateTime);
	//		break;
	//	case 4: //����Ȩ��
	//		::lstrcpy (pDispInfo->item.pszText, pItem->strAccess);
	//		break;
	//	case 5: //������Ϣ
	//		::lstrcpy (pDispInfo->item.pszText, pItem->strDescription);
	//		break;

	//		}
	//	}
	//}else{	//��ͼ��
	//	if (pDispInfo->item.mask & LVIF_TEXT) {
	//		ITEMINFO* pItem = (ITEMINFO*) pDispInfo->item.lParam;
	//		if ( pItem->nCount==-1 ){
	//			strText.Format( _T("%s"), pItem->strName );
	//		}else{
	//			strText.Format( _T("%s\n��%d��"), pItem->strName, pItem->nCount );
	//		}
	//		::lstrcpy (pDispInfo->item.pszText, strText);

	//		//switch (pDispInfo->item.iSubItem) {

	//		// 		case 0: //���
	//		// 			strText.Format("%d",pItem->nIndex);
	//		// 			::lstrcpy (pDispInfo->item.pszText, strText);
	//		// 			break;
	//		// 
	//		// 		case 1: //�������
	//		// 			::lstrcpy (pDispInfo->item.pszText, pItem->strName);
	//		// 			break;
	//		// 
	//		// 		case 2: //ͼƬ����
	//		// 			strText.Format("%d",pItem->nCount);
	//		// 			::lstrcpy (pDispInfo->item.pszText, strText);
	//		// 			break;
	//		// 		case 3: //��������
	//		// 			::lstrcpy (pDispInfo->item.pszText, pItem->strUpdateTime);
	//		// 			break;
	//		// 		case 4: //����Ȩ��
	//		// 			::lstrcpy (pDispInfo->item.pszText, pItem->strAccess);
	//		// 			break;
	//		// 		case 5: //������Ϣ
	//		// 			::lstrcpy (pDispInfo->item.pszText, pItem->strDescription);
	//		// 			break;
	//		// 
	//		// 			}
	//	}

	//}

	*pResult = 0;
}

void CMainDlg::OnDeleteItemList(NMHDR* pNMHDR, LRESULT* pResult)    
{   
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;   
	//ITEMINFO*pItem=(ITEMINFO*)pNMListView->lParam;   
	//if ( pItem!=NULL ){
	//	delete pItem;
	//} 

	*pResult = 0;   
}
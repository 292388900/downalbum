// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <afxmt.h>
#include <atlimage.h>
#include <math.h>
#include <StarLib/File/File.h>
#include "ImageBat.h"
#include "MainDlg.h"
#include "ClipImageDlg.h"
#include "global.h"
#include "Optionsdlg.h"
#include "ControlTracker.h"
#include "FormatTagString.h"
#include "resource.h"

/*
ע�͵����new�Ķ��壬������ִ���error C2660: ��Gdiplus::GdiplusBase::operator new��: ���������� 3 ������
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/


//��ʼ��ȫ������
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
	, m_strImagePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pMainDlg = this;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PHOTOS, m_lstPhotos);
	//DDX_Text(pDX, IDC_EDIT_PHOTO_PATH, m_strImagePath);
	DDX_Text(pDX, IDC_COMBO_IMAGEPATH_HISTORY, m_strImagePath);
	DDX_Control(pDX, IDC_BUTTON_VIEW_MODE, m_ctrlViewMode);
	DDX_Control(pDX, IDC_COMBO_IMAGEPATH_HISTORY, m_cmbImagePathHistory);
	DDX_Control(pDX, IDC_EDIT_PHOTO_PATH, m_edtImagePath);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_MESSAGE(WM_POPMSG,&CMainDlg::OnTrayPopMsg)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_PHOTOS, &CMainDlg::OnGetDispInfo)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PHOTOS, &CMainDlg::OnNMClickListAlbum)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PHOTOS, &CMainDlg::OnNMDBLClickListAlbum)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_PHOTOS, &CMainDlg::OnBnClickedButtonViewPhotos)
	ON_MESSAGE(WM_FINDANDSHOWPHOTOSBEGIN,&CMainDlg::OnFindAndShowPhotosBegin)
	ON_MESSAGE(WM_FINDANDSHOWPHOTOSEND,&CMainDlg::OnFindAndShowPhotosEnd)
	ON_MESSAGE(WM_PROCESSPHOTOSBEGIN,&CMainDlg::OnProcessPhotosBegin)
	ON_MESSAGE(WM_PROCESSPHOTOSEND,&CMainDlg::OnProcessPhotosEnd)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_MODE, &CMainDlg::OnBnClickedButtonViewMode)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGEPATH_HISTORY, &CMainDlg::OnCbnSelchangeComboImagepathHistory)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PHOTOS, &CMainDlg::OnLvnColumnclickListPhotos)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CMainDlg::OnBnClickedCheckAll)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PHOTOS, &CMainDlg::OnNMRclickListPhotos)
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
	ON_COMMAND_RANGE(IDC_BATCHCMD_START,IDC_BATCHCMD_END,&CToolboxDlg::OnBtnClickEditPhotos)
	ON_COMMAND(ID_OPEN_SAVEPATH, &CMainDlg::OnOpenSavepath)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){
		if ( pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ){
			return TRUE;
		}
	} 
	return __super::PreTranslateMessage(pMsg);
}

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

	//////////////////////////////////////////////////////////////////////////
	//����ˮӡģ�壬����ˮӡ�ؼ��������η�Χ����һ���Ի�����ϡ�
	//CControlTracker objControlTracker;	
	//objControlTracker.Create(g_pMainDlg);
	//objControlTracker.Load(g_config.m_strWatermarkTemplatePath+"1",g_pMainDlg);
	//objControlTracker.SelectAll();
	//CRect rcRect = objControlTracker.GetBoundsRect();

	////////////////////////////////////////////////////////////////////////////
	//EXIFINFO exifinfo = {0};
	//Cexif exif(&exifinfo);
	//CImageItem photo;

	//photo.strName = "01.jpg";
	//photo.strFilePath = g_config.m_strImagePath + photo.strName;
	//memset(&exifinfo,0,sizeof(exifinfo));
	//exif.OpenFile(photo.strFilePath);
	//exif.DecodeExif();
	//photo.nWidth = exifinfo.Width;
	//photo.nHeight = exifinfo.Height;
	//exif.CloseFile();

	//������СͼƬ
#if 0
	THUMBNAIL_PARAM stParam1;
	stParam1.bRestoreExif = FALSE;
	stParam1.nPixelWidth = 400;
	stParam1.pItem = &photo;
	photo._Thumbnail(&stParam1);
#endif

	//�������ˮӡ
#if 0
	ADDWATERMARKER_PARAM stParam;
	stParam.bRestoreExif = FALSE;
	stParam.pItem = &photo;
	stParam.pControlTracker = &objControlTracker;
	photo._AddWaterMarker(&stParam);
#endif


	//////////////////////////////////////////////////////////////////////////
	DragAcceptFiles(TRUE);
	//////////////////////////////////////////////////////////////////////////
	InitAllControls();
	AppendText("���ڼ���Ƿ������°汾����",0);
	AfxBeginThread(ThreadCheckVersion,NULL);
	UpdateData(FALSE);
	//////////////////////////////////////////////////////////////////////////

	SetupTrayIcon();
	PostMessage(WM_SHOWREGDLG,0,0);
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMainDlg::PostNcDestroy()
{
	ClearPhotoList();
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

void CMainDlg::OnDropFiles(HDROP hDrop)
{
	TCHAR szFilePath[MAX_PATH] = {0};      

	UINT nCount = DragQueryFile(hDrop, -1, NULL, 0);
	if ( nCount>0 ){
		DragQueryFile(hDrop, 0, szFilePath, MAX_PATH);
		if ( PathIsDirectory(szFilePath)==FALSE ){
			PathRemoveFileSpec(szFilePath);
		}
		m_strImagePath = szFilePath;
		UpdateData(FALSE);
		OnBnClickedButtonViewPhotos();
	}
	__super::OnDropFiles(hDrop);   
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

	//���Ҳഴ�������䴰��
	m_toolboxDlg.Create(CToolboxDlg::IDD,this);

	//�����䴰�ڿ��100
	rcWnd.left = rcWnd.Width() - 100;
	rcWnd.bottom = rcWnd.bottom-22;
	m_toolboxDlg.MoveWindow(&rcWnd,TRUE);
	m_toolboxDlg.ShowWindow(SW_SHOW);

	//////////////////////////////////////////////////////////////////////////

	m_lstPhotos.SetExtendedStyle( m_lstPhotos.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT & (~(LVS_EX_GRIDLINES)) );
	CRect rcList;
	m_lstPhotos.GetClientRect(&rcList);
	m_lstPhotos.InsertColumn( COLUMN_INDEX_NAME, _T("����"), LVCFMT_LEFT, rcList.Width()-80-80-80-150-120 );
	m_lstPhotos.InsertColumn( COLUMN_INDEX_DIMENSION, _T("�ߴ�"), LVCFMT_LEFT, 80 );
	m_lstPhotos.InsertColumn( COLUMN_INDEX_FILESIZE, _T("�ļ���С"), LVCFMT_LEFT, 80 );
	m_lstPhotos.InsertColumn( COLUMN_INDEX_IMAGETYPE, _T("����"), LVCFMT_LEFT, 80 );
	m_lstPhotos.InsertColumn( COLUMN_INDEX_PHOTOTIME, _T("��������"), LVCFMT_LEFT, 150 );
	m_lstPhotos.InsertColumn( COLUMN_INDEX_UPDATETIME, _T("�޸�����"), LVCFMT_LEFT, 150 );

	ResetImageList();
	//if ( g_config.nViewMode==VIEWMODE_BIGICON || g_config.nViewMode==VIEWMODE_LARGEICON ){	//����ͼ
	//	m_lstPhotos.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
	//	m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	//}else{							//��ϸ��Ϣ
	//	m_lstPhotos.ModifyStyle( LVS_LIST | LVS_SMALLICON | LVS_ICON | LVS_NOCOLUMNHEADER, LVS_REPORT);
	//	m_imageList.Create(16,16,ILC_COLOR24,10,1);
	//}
	//m_lstPhotos.SetImageList(&m_imageList,LVSIL_NORMAL);
	//////////////////////////////////////////////////////////////////////////

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_VIEWMODE);
	m_ctrlViewMode.SetIcon(hIcon);
	//////////////////////////////////////////////////////////////////////////
	


	//////////////////////////////////////////////////////////////////////////
	//�ʵ��������֣�������m_cmbImagePathHistory�ı༭��
	m_cmbImagePathHistory.LimitText(MAX_PATH);
	//CWnd *pEdit = m_cmbImagePathHistory.FindWindowEx(m_cmbImagePathHistory.GetSafeHwnd(),0,NULL,NULL);
	//if ( pEdit!=NULL ){
	//	CRect rcEdit;
	//	CRect rcCmb;
	//	CRect rcCmbEdit;
	//	int nMargin = 0;
	//	m_cmbImagePathHistory.GetWindowRect(rcCmb);
	//	m_edtImagePath.GetWindowRect(rcEdit);
	//	pEdit->GetWindowRect(rcCmbEdit);
	//	nMargin = rcCmb.Width() - rcCmbEdit.Width();
	//	if ( nMargin>4 ){
	//		nMargin -= 4;
	//	}else{
	//		nMargin = 13;
	//	}

	//	//ʹ��m_cmbImagePathHistory�պñ�m_edtAccount��һ��nMargin
	//	//m_cmbImagePathHistory�ĸ��ǹ̶��ģ�����m_edtAccount�ĸ߶�Ǩ��m_cmbImagePathHistoryһ��
	//	rcEdit.bottom = rcEdit.top + rcCmb.Height();
	//	rcCmb.top = rcEdit.top;
	//	rcCmb.bottom = rcEdit.bottom;
	//	rcCmb.left = rcEdit.left;
	//	rcCmb.right = rcEdit.right + nMargin;

	//	this->ScreenToClient(rcEdit);
	//	this->ScreenToClient(rcCmb);
	//	m_edtImagePath.MoveWindow(rcEdit);
	//	m_cmbImagePathHistory.MoveWindow(rcCmb);

	//	//pEdit->ShowWindow(SW_HIDE);
	//}

	//������ʷ��¼
	LoadHistoryUrlToFile();
	//////////////////////////////////////////////////////////////////////////



	//���ڲ���
	m_arranger.Init(m_hWnd);
	//////////////////////////////////////////////////////////////////////////
	m_arranger.FixAll(CDlgResizeHelper::kWidthLeft,CDlgResizeHelper::kHeightTop);
	
	//�����¹̶����߶ȹ̶�
	m_arranger.Fix(m_wndStatusbar.GetSafeHwnd(), CDlgResizeHelper::kLeft, CDlgResizeHelper::kHeightBottom);

	//���Ҳ�̶�
	m_arranger.Fix(m_toolboxDlg.GetSafeHwnd(), CDlgResizeHelper::EHFix::kWidthRight, CDlgResizeHelper::EVFix::kTop);

	//��������ײ�
	m_arranger.Fix(m_lstPhotos.GetSafeHwnd(), CDlgResizeHelper::EHFix::kLeftRight, CDlgResizeHelper::EVFix::kTopBottom);
	//////////////////////////////////////////////////////////////////////////

}
void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_arranger.OnSize();
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

UINT CMainDlg::ThreadCheckVersion(LPVOID lpParam)
{
	static CEvent evt(true);    //��ʼΪ����״̬���Զ������¼�
	if  ( ::WaitForSingleObject(evt.m_hObject,0)==WAIT_OBJECT_0 ){
		//�¼�֮ǰΪ����״̬�������Զ�����Ϊ����״̬

		CString strText;
		CMainDlg*pMainDlg = g_pMainDlg;
		//��������Ϣ
		if ( CheckUpdateInfoIni(g_sUPDATE_CONFIG_URL,g_stUpdateInfo )==TRUE ){

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
					strText=g_stUpdateInfo.strDownUrl;
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

//////////////////////////////////////////////////////////////////////////

void CMainDlg::AddHistoryUrl(const CString&str)
{
	BOOL bFound = FALSE;

	CString strText;
	int nCount = m_cmbImagePathHistory.GetCount();
	for ( int i=0; i<nCount; ++i ){
		m_cmbImagePathHistory.GetLBText(i,strText);
		if ( strText.Compare(str)==0 ){
			bFound = TRUE;
			if ( i!=0 ){
				m_cmbImagePathHistory.DeleteString(i);
				m_cmbImagePathHistory.InsertString(0,str);
				m_cmbImagePathHistory.SetWindowText(str);
			}
			break;
		}
	}

	if ( bFound==FALSE ){
		if ( nCount>=10 ){
			m_cmbImagePathHistory.DeleteString(nCount-1);
		}

		m_cmbImagePathHistory.InsertString(0,str);
	}
}

void CMainDlg::SaveHistoryUrlToFile()
{
	int nIndex = 0;
	CString strText;
	CString strTemp;

	for ( int i=0; i<m_cmbImagePathHistory.GetCount(); ++i ){
		m_cmbImagePathHistory.GetLBText(i,strTemp);
		strText = strText + strTemp + _T("|");
	}

	Star::File::SetIniString(g_config.m_strCfgFile,"history","urls",strText);
	g_config.m_strImagePath = m_strImagePath;
	Star::File::SetIniString(g_config.m_strCfgFile,"path","lastview",g_config.m_strImagePath);
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
			if ( GetFileAttributes(strText)!=-1 ){
				m_cmbImagePathHistory.InsertString(-1,strText);
			}
		}//end for
	}

	m_strImagePath = g_config.m_strImagePath;
}
//////////////////////////////////////////////////////////////////////////

void CMainDlg::OnLvnColumnclickListPhotos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_bSortAsc=!m_bSortAsc;
	m_lstPhotos.SortItems(CompareFunc,pNMLV->iSubItem);
	*pResult = 0;
}

int CALLBACK CMainDlg::CompareFunc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	int nResult=0;
	ITEMINFO* pItem1 = (ITEMINFO*)lParam1;
	ITEMINFO* pItem2 = (ITEMINFO*)lParam2;

	switch ( lParamSort )
	{
	case COLUMN_INDEX_NAME:			//���
		nResult = pItem1->nItem - pItem2->nItem;
		break;
	case COLUMN_INDEX_DIMENSION:	//�ߴ�
		nResult = pItem1->nWidth - pItem2->nWidth;
		if ( nResult==0 ){
			nResult = pItem1->nHeight - pItem2->nHeight;
		}
		break;
	case COLUMN_INDEX_FILESIZE:		//�ļ���С
		nResult = pItem1->nFileSize - pItem2->nFileSize;
		break;
	case COLUMN_INDEX_IMAGETYPE:	//����
		nResult = pItem1->nImageType - pItem2->nImageType;
		break;
	case COLUMN_INDEX_PHOTOTIME:	//��������
		nResult = pItem1->strPhotoTime.CompareNoCase(pItem2->strPhotoTime);
		break;
	case COLUMN_INDEX_UPDATETIME:	//�޸�����
		break;
	}

	if ( m_bSortAsc==FALSE ){
		nResult=0-nResult;
	}

	return nResult;
}

void CMainDlg::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strText;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*) pNMHDR;

	if ( m_lstPhotos.GetStyle()&LVS_REPORT ){			//��ϸ�б�
		if (pDispInfo->item.mask & LVIF_TEXT) {
			ITEMINFO* pItem = (ITEMINFO*)pDispInfo->item.lParam;

			switch (pDispInfo->item.iSubItem) {

		case COLUMN_INDEX_NAME:			//����
			::lstrcpy (pDispInfo->item.pszText, pItem->strName);
			break;

		case COLUMN_INDEX_DIMENSION:	//�ߴ�
			strText.Format("%d x %d",pItem->nWidth,pItem->nHeight);
			::lstrcpy (pDispInfo->item.pszText, strText);
			break;

		case COLUMN_INDEX_FILESIZE:		//�ļ���С
			//���Ҫ���Ի�һЩ������M����MBΪ��λ������K����KBΪ��λ
			::lstrcpy (pDispInfo->item.pszText, CImageHelper::FormatImageSize(pItem->nFileSize));
			break;
		case COLUMN_INDEX_IMAGETYPE:	//����
			//::lstrcpy (pDispInfo->item.pszText, pItem->strUpdateTime);
			break;
		case COLUMN_INDEX_PHOTOTIME:	//��������
			::lstrcpy (pDispInfo->item.pszText, pItem->strPhotoTime);
			break;
		case COLUMN_INDEX_UPDATETIME:	//�޸�����
			//::lstrcpy (pDispInfo->item.pszText, pItem->timeLastWrite.Format(_T("%Y-%m-%d %H:%M:%S")));
			//::lstrcpy (pDispInfo->item.pszText, pItem->strAccess);
			break;
			}
		}
	}else{												//��ͼ��
		if (pDispInfo->item.mask & LVIF_TEXT) {
			ITEMINFO* pItem = (ITEMINFO*)pDispInfo->item.lParam;
			::lstrcpy (pDispInfo->item.pszText, pItem->strName);
		}

	}

	*pResult = 0;
}

void CMainDlg::OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = pNMLV->iItem;

	
	if ( nItem!=-1 ){
		//״̬����ʾexif������Ϣ
		ITEMINFO *pItem = (ITEMINFO *)m_lstPhotos.GetItemData(nItem);
		if ( pItem!=NULL && pItem->bIsExif==TRUE ){
			CFormatTagString formattag;
			AppendText("��Ȧ��" + formattag.FormatAperture(pItem) + 
				" ���ţ�"+  formattag.FormatShutterTime(pItem) + 
				" �عⲹ����" + formattag.FormatBias(pItem) + 
				" ISO��" + formattag.FormatISO(pItem) + 
				" - "+pItem->strCameraModel);
		}else{
			AppendText("");
		}

		// Didn't click on an icon
		if ( g_config.m_bCheckIfClickIcon==TRUE ){
			LVHITTESTINFO hitinfo;

			//Copy click point
			hitinfo.pt = pNMLV->ptAction;
			//Make the hit test...
			nItem = m_lstPhotos.HitTest(&hitinfo);

			if( hitinfo.flags != LVHT_ONITEMSTATEICON ){
				BOOL bChecked=!m_lstPhotos.GetCheck(nItem);
				m_lstPhotos.SetCheck(nItem,bChecked);
			}
		}

	}/*else{
		AppendText("");
	}*/

	*pResult = 0;
}

void CMainDlg::OnNMDBLClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO hitinfo;

	//Copy click point
	hitinfo.pt = pNMListView->ptAction;

	//Make the hit test...
	int nItem = m_lstPhotos.HitTest(&hitinfo);
	if ( nItem!=-1 ){
		ITEMINFO *pItem = (ITEMINFO *)m_lstPhotos.GetItemData(nItem);
		// Didn't click on an icon
		if( pItem && hitinfo.flags != LVHT_ONITEMSTATEICON ){
			ShellExecute(GetSafeHwnd(), "open", "rundll32", "shimgvw.dll,ImageView_Fullscreen " + pItem->strFilePath, NULL, SW_SHOW );
		}
	}

	*pResult = 0;
}

void CMainDlg::OnNMRclickListPhotos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO hitinfo;

	//Copy click point
	hitinfo.pt = pNMListView->ptAction;

	//Make the hit test...
	int nItem = m_lstPhotos.HitTest(&hitinfo);
	if ( nItem!=-1 ){
		ITEMINFO *pItem = (ITEMINFO *)m_lstPhotos.GetItemData(nItem);
		if ( pItem ){
			m_lstPhotos.ClientToScreen(&hitinfo.pt);
			Star::Common::PopExplorerMenu(m_lstPhotos.GetSafeHwnd(),pItem->strFilePath,hitinfo.pt);
		}
	}
	*pResult = 0;
}

LRESULT CMainDlg::OnFindAndShowPhotosBegin(WPARAM wParam,LPARAM lParam)
{
	m_toolboxDlg.EnableWindow(FALSE);
	AppendText("���ڼ���ͼƬ�����Եȡ���");

	return 0;
}
LRESULT CMainDlg::OnFindAndShowPhotosEnd(WPARAM wParam,LPARAM lParam)
{
	if ( m_vtPhotos.empty() ){
		AppendTextV("δ�����κ�ͼƬ������Ŀ¼��"+m_strImagePath);
	}else{
		AppendTextV("������ %d ��ͼƬ�����Խ�������������",(int)m_vtPhotos.size());
	}
	m_toolboxDlg.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_VIEW_PHOTOS)->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_ALL))->SetCheck(BST_CHECKED);

	return 0;
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
		m_lstPhotos.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
		g_config.nImageViewWidth = WIDTH_VIEWMODE_BIGICON;
		g_config.nImageViewHeight = HEIGHT_VIEWMODE_BIGICON;
		m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	}else if ( g_config.nViewMode==VIEWMODE_LARGEICON ){	//����ͼ��
		g_config.nImageViewWidth = WIDTH_VIEWMODE_LARGEICON;
		g_config.nImageViewHeight = HEIGHT_VIEWMODE_LARGEICON;
		m_lstPhotos.ModifyStyle( LVS_LIST | LVS_REPORT | LVS_SMALLICON, LVS_ICON);
		m_imageList.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24,10,1);
	}else{							//��ϸ��Ϣ
		m_lstPhotos.ModifyStyle( LVS_LIST | LVS_SMALLICON | LVS_ICON | LVS_NOCOLUMNHEADER, LVS_REPORT);
		m_imageList.Create(16,16,ILC_COLOR24,10,1);
	}
	m_lstPhotos.SetImageList(&m_imageList,LVSIL_NORMAL);
}


//����Ŀ¼�µ�ͼƬ����ʾ����
UINT CMainDlg::ThreadProcFindPhotosAndShow(LPVOID lParam)
{
	CMainDlg *pDlg = (CMainDlg *)lParam;
	pDlg->SendMessage(WM_FINDANDSHOWPHOTOSBEGIN);

	//��¼�������ʷ��ַ
	if ( GetFileAttributes(pDlg->m_strImagePath)!=-1 ){
		pDlg->AddHistoryUrl(pDlg->m_strImagePath);
		pDlg->SaveHistoryUrlToFile();
	}

	//////////////////////////////////////////////////////////////////////////
	pDlg->m_lstPhotos.DeleteAllItems();
	CImageList &imagelist = pDlg->m_imageList;

	// Remove every other image from the image list.
	for ( int i=0; i < imagelist.GetImageCount(); ++i){
		imagelist.Remove(i);
	}

	pDlg->ClearPhotoList();
	//////////////////////////////////////////////////////////////////////////

	pDlg->FindPhothos(pDlg->m_strImagePath);

	pDlg->ShowPhotos();

	pDlg->SendMessage(WM_FINDANDSHOWPHOTOSEND);

	return 0;
}

void UpdateTime(CString strFile)
{
	HANDLE     hFile;
	FILETIME   filetime;
	FILETIME   localtime;
	SYSTEMTIME systemtime;

	hFile = CreateFile(strFile, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		GetFileTime(hFile, NULL, NULL, &filetime);      //ȡ��UTC�ļ�ʱ��
		CTime timeOld(filetime);
		CTimeSpan timeSpan(0,8,0,0);
		CTime timeNew = timeOld - timeSpan;
		timeNew.GetAsSystemTime(systemtime);

		TRACE(timeOld.Format("%Y-%m-%d_%H-%M-%S\r\n"));

		TRACE("%04d-%02d-%02d %02d:%02d:%02d\r\n",
			systemtime.wYear, systemtime.wMonth, systemtime.wDay,
			systemtime.wHour, systemtime.wMinute, systemtime.wSecond);

		SystemTimeToFileTime(&systemtime, &localtime);	//�����ļ�ʱ���ʽ
		LocalFileTimeToFileTime(&localtime, &filetime);	//����UTCʱ��
		SetFileTime(hFile, NULL, NULL, &filetime);		//�趨UTC�ļ�ʱ��
		CloseHandle(hFile);
	}
}

//����Ŀ¼�µ�ͼƬ
void CMainDlg::FindPhothos(CString&strPhotoPath)
{
	//���Ŀ¼�Ƿ����
	if ( GetFileAttributes(strPhotoPath)==-1 ){
		return;
	}

	//β��ȷ��Ϊб��
	CString	strDir = strPhotoPath;
	strDir.Replace(_T("/"),_T("\\"));
	if ( strDir.Right(1)!=_T("\\") ){
		strDir += _T("\\");
	}

	m_strCurPhotoPath = strDir;

	ClearPhotoList();
	PHOTOINFO *pPhoto = NULL;
	EXIFINFO exifinfo = {0};
	Cexif exif(&exifinfo);

	//��ʼ���������ݹ飨����͸�ļ��У�
	int			nFindCount = 0;
	int			nPos = 0;
	CString		strFileName;
	CFileFind	finder;
	BOOL		bWorking = finder.FindFile(strDir+_T("*.*")); 
	while ( bWorking ){
		bWorking = finder.FindNextFile(); 
		//skip . and ..,skip if IsDirectory
		if (finder.IsDots()){
			continue;
		}else if (finder.IsDirectory()){
			continue;
		}else { 
			strFileName = finder.GetFileName();
			nPos = strFileName.ReverseFind('.');
			if ( nPos==-1 ){
				continue;
			}

			strFileName = strFileName.Mid(nPos+1);
			strFileName.MakeLower();
			if ( strFileName==_T("jpg") || strFileName==_T("bmp") || strFileName==_T("jpeg") || strFileName==_T("png") ){
				pPhoto = new PHOTOINFO;
				if ( pPhoto==NULL ){
					continue;
				}
				pPhoto->nFileSize = (int)finder.GetLength();
				pPhoto->strName = finder.GetFileName();
				pPhoto->strFilePath = finder.GetFilePath();
				memset(&exifinfo,0,sizeof(exifinfo));
				exif.OpenFile(pPhoto->strFilePath);
				exif.DecodeExif();
				pPhoto->bIsExif = exifinfo.IsExif;
				pPhoto->strPhotoTime = exifinfo.DateTime;
				pPhoto->nWidth = exifinfo.Width;
				pPhoto->nHeight = exifinfo.Height;
				pPhoto->strCameraMaker = exifinfo.CameraMake;
				pPhoto->strCameraModel = exifinfo.CameraModel;
				pPhoto->fShutterTime = exifinfo.ExposureTime;
				pPhoto->fAperture = exifinfo.ApertureFNumber;
				pPhoto->fBias = exifinfo.ExposureBias;
				pPhoto->nWhitebalance = exifinfo.Whitebalance;
				pPhoto->nExposureProgram = exifinfo.ExposureProgram;
				pPhoto->nISOequivalent = exifinfo.ISOequivalent;
				pPhoto->nFlashUsed = exifinfo.FlashUsed;
				exif.CloseFile();
				m_vtPhotos.push_back(pPhoto);
				AppendTextV("���ҵ� %d ��ͼƬ",++nFindCount);
			}else{
				continue;
			}
		} 
	}
	finder.Close();
}
//��ʾͼƬ
void CMainDlg::ShowPhotos()
{
	if ( m_lstPhotos.GetStyle()&LVS_REPORT ){			//��ϸ�б�
		ShowPhotosByReport();
	}else{												//��ͼ��
		ShowPhotosByIcon();
	}
}
void CMainDlg::ShowPhotosByIcon()
{
	CString strText;
	int nItem = 0;

	m_lstPhotos.SetRedraw(FALSE);
	for ( vector<PHOTOINFO*>::iterator iter=m_vtPhotos.begin(); iter!=m_vtPhotos.end(); ++iter,++nItem ){
		PHOTOINFO *pPhoto = (*iter);
		pPhoto->nItem = nItem;
		///////////////////////////////////////////
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; 
		lvi.iItem = nItem; 
		lvi.iSubItem = 0; 
		lvi.iImage = -1;
		lvi.pszText = LPSTR_TEXTCALLBACK; 
		lvi.lParam = (LPARAM)pPhoto;
		m_lstPhotos.InsertItem(&lvi);
		m_lstPhotos.SetCheck(nItem,TRUE);
		m_lstPhotos.EnsureVisible(nItem,FALSE);
		///////////////////////////////////////////

		//pPhoto->LoadImage();
	}
	m_lstPhotos.SetRedraw(TRUE);

	for ( vector<PHOTOINFO*>::iterator iter=m_vtPhotos.begin(); iter!=m_vtPhotos.end(); ++iter ){
		PHOTOINFO *pPhoto = (*iter);
		pPhoto->LoadImage();
	}
}
void CMainDlg::ShowPhotosByReport()
{
	CString strText;
	int nItem = 0;
	for ( vector<PHOTOINFO*>::iterator iter=m_vtPhotos.begin(); iter!=m_vtPhotos.end(); ++iter,++nItem ){
		PHOTOINFO *pPhoto = (*iter);
		pPhoto->nItem = nItem;
		///////////////////////////////////////////
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM; 
		lvi.iItem = nItem; 
		lvi.iSubItem = 0; 
		lvi.iImage = 0;
		lvi.pszText = LPSTR_TEXTCALLBACK; 
		lvi.lParam = (LPARAM)pPhoto;
		m_lstPhotos.InsertItem(&lvi);
		m_lstPhotos.SetCheck(nItem,TRUE);
		m_lstPhotos.EnsureVisible(lvi.iItem,FALSE);
	}//end for
}
//���ͼƬ�б�ռ�õ��ڴ�
void CMainDlg::ClearPhotoList()
{
	for ( vector<PHOTOINFO *>::iterator i=m_vtPhotos.begin(); i!=m_vtPhotos.end(); ++i ){
		delete (*i);
	}
	m_vtPhotos.clear();
}

void CMainDlg::OnBnClickedButtonViewPhotos()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_VIEW_PHOTOS)->EnableWindow(FALSE);
	AfxBeginThread(ThreadProcFindPhotosAndShow,this);
}


//////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnProcessPhotosBegin(WPARAM wParam,LPARAM lParam)
{
	m_toolboxDlg.EnableWindow(FALSE);
	AppendText("��ʼ��������������");
	return 0;
}
LRESULT CMainDlg::OnProcessPhotosEnd(WPARAM wParam,LPARAM lParam)
{
	CString strText;
	LPCTSTR lpszDirPath = (LPCTSTR)lParam;

	m_toolboxDlg.EnableWindow(TRUE);


#if 0

	//Ϊ�˸��õ���������������Ծ�������
#ifdef _UNICODE
	TCHAR szjuanzhu[] = { 0x4E3A, 0x4E86, 0x66F4, 0x597D, 0x5730, 0x5B8C, 0x5584, 0x8F6F, 0x4EF6, 0xFF0C, 0x60A8, 0x53EF, 0x4EE5, 0x6350, 0x52A9, 0x4F5C, 0x8005, '\0' };
#else
	CHAR szjuanzhu[] = { 0xCE, 0xAA, 0xC1, 0xCB, 0xB8, 0xFC, 0xBA, 0xC3, 0xB5, 0xD8, 0xCD, 0xEA, 0xC9, 0xC6, 0xC8, 0xED, 0xBC, 0xFE, 0xA3, 0xAC, 0xC4, 0xFA, 0xBF, 0xC9, 0xD2, 0xD4, 0xBE, 0xE8, 0xD6, 0xFA, 0xD7, 0xF7, 0xD5, 0xDF, '\0' };
#endif

	//������ַ��
#ifdef _UNICODE
	TCHAR szwangzhi[] = { 0x6350, 0x52A9, 0x7F51, 0x5740, 0xFF1A, '\0' };
#else
	CHAR szwangzhi[] = { 0xBE, 0xE8, 0xD6, 0xFA, 0xCD, 0xF8, 0xD6, 0xB7, 0xA3, 0xBA, '\0' };
#endif

	strText = szjuanzhu;

	if ( lpszDirPath!=NULL && GetFileAttributes(lpszDirPath)!=-1 ){
		CStdioFile file;
		CString strFile = lpszDirPath;
		if ( file.Open(strFile + strText + _T(".txt"),CFile::modeCreate|CFile::modeReadWrite) ){
			file.WriteString(szwangzhi+g_stUpdateInfo.strRegUrl);
			file.Close();
		}
	}

#endif

	AppendText("�����������");

	//�����Ŀ¼
	if ( lpszDirPath!=NULL && GetFileAttributes(lpszDirPath)!=-1 ){
		ShellExecute(NULL,"open",lpszDirPath,NULL,NULL,SW_SHOW);   
	}

	return 0;
}

UINT CMainDlg::ThreadClipImageBatch(LPVOID lpParam)
{
	PROCESS_PARAM_CLIPIMAGE *pParam = (PROCESS_PARAM_CLIPIMAGE *)lpParam;
	if ( pParam==NULL ){
		return 0;
	}

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_CLIPIMAGE);
	CImageHelper helper;
	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;
	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			//
			AppendTextV("���ڲü��� %d ��ͼƬ����",nItem+1);
			helper.ClipImageFile(g_pMainDlg->m_strCurPhotoPath + pItem->strName,
				g_config.strSavePath + pItem->strName,
				pParam->nPosX,pParam->nPosY,pParam->nWidth,pParam->nHeight);
		}
	}

	delete pParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_CLIPIMAGE,(LPARAM)(LPCTSTR)g_config.strSavePath);
	return 0;
}

//�������Ļ�����ԭ�ļ��ϸ�
UINT CMainDlg::ThreadRenameImageBatch(LPVOID lpParam)
{
	PROCESS_PARAM_RENAMEIMAGE *pParam = (PROCESS_PARAM_RENAMEIMAGE *)lpParam;
	if ( pParam==NULL ){
		return 0;
	}

	int nPos = 0;
	CString strSrcFilePath;
	CString strFileName;
	CString strFileExt;
	int nStartNumber = pParam->nStartNumber;
	int nStep = pParam->nIncreaseStep;
	int nFileExtCaseMethod = pParam->nFileExtCaseMethod;

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_RENAMEIMAGE);
	CImageHelper helper;
	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	//Ԥ�����ȸ���һ�£���֤�ڶ��θ���ʱ��������ظ����ļ���
	CString strGuid;
	Star::Common::GenerateGUID(strGuid);

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			//
			AppendTextV("����Ԥ����� %d ��ͼƬ����",nItem+1);
			nPos = pItem->strName.ReverseFind('.');
			if ( nPos==-1 ){
				strFileExt.Empty();
			}else{
				strFileExt = pItem->strName.Mid(nPos);
				if ( nFileExtCaseMethod==FILEEXT_CASE_TOLOWER ){
					strFileExt.MakeLower();
				}else if ( nFileExtCaseMethod==FILEEXT_CASE_TOUPPER ){
					strFileExt.MakeUpper();
				}
			}
			strFileName.Format(_T("%d"),nStartNumber);

			//����������е�ͼƬ��ϢҲҪ�ĵ�
			strSrcFilePath = pItem->strFilePath;
			pItem->strName = strGuid + strFileName + strFileExt;
			pItem->strFilePath = g_pMainDlg->m_strCurPhotoPath + pItem->strName;
			CFile::Rename(strSrcFilePath,pItem->strFilePath);
			nStartNumber += nStep;
		}
	}//end for

	//�����ĸ�����ʼ
	nStartNumber = pParam->nStartNumber;
	CFormatTagString objFormatTagStr;
	objFormatTagStr.Init(ctrlList.GetItemCount(),pParam->strRenameRules);
	
	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			pItem->nItem = nStartNumber;
			//
			AppendTextV("������������ %d ��ͼƬ����",nItem+1);
			strSrcFilePath = g_pMainDlg->m_strCurPhotoPath + pItem->strName;
			nPos = pItem->strName.ReverseFind('.');
			if ( nPos==-1 ){
				strFileExt.Empty();
			}else{
				strFileExt = pItem->strName.Mid(nPos);
				if ( nFileExtCaseMethod==FILEEXT_CASE_TOLOWER ){
					strFileExt.MakeLower();
				}else if ( nFileExtCaseMethod==FILEEXT_CASE_TOUPPER ){
					strFileExt.MakeUpper();
				}
			}
			strFileName = Star::Common::RemoveFileNameInvalidChar( objFormatTagStr.GetFormaString(pItem) );

			//����������е�ͼƬ��ϢҲҪ�ĵ�
			strSrcFilePath = pItem->strFilePath;
			pItem->strName = strFileName + strFileExt;
			pItem->strFilePath = g_pMainDlg->m_strCurPhotoPath + pItem->strName;
			CFile::Rename(strSrcFilePath,pItem->strFilePath);
			nStartNumber += nStep;
		}
	}//end for

	//ˢ���б��ʹ�ø�������ļ�������ˢ�³���
	ctrlList.Invalidate();

	delete pParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_RENAMEIMAGE,(LPARAM)(LPCTSTR)g_pMainDlg->m_strCurPhotoPath);
	return 0;
}

//������תͼƬ
UINT CMainDlg::ThreadRotateFlipImageBatch(LPVOID lpParam)
{
	int nRotateFlip = (int)lpParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_ROTATEFLIPIMAGE);

	vector<CWinThread *>vtThreads;
	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;
	CWinThread *pThread = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				pThread = pItem->RotateFlip(nRotateFlip);
				if ( pThread ){
					vtThreads.push_back(pThread);
				}
			}
		}
	}//end for

	//�ȴ������߳̽���
	for ( vector<CWinThread *>::iterator i=vtThreads.begin(); i!=vtThreads.end(); ++i ){
		WaitForSingleObject((*i)->m_hThread,INFINITE);
	}//end for

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_ROTATEFLIPIMAGE/*,(LPARAM)(LPCTSTR)g_config.strSavePath*/);
	return 0;
}

//����У����������
UINT CMainDlg::ThreadAjustTimeImageBatch(LPVOID lpParam)
{
	if ( lpParam==NULL ){
		return 0;
	}

	PPROCESS_PARAM_UPDATETIME pParam = (PPROCESS_PARAM_UPDATETIME)lpParam;

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_UPDATETIME);

	vector<CWinThread *>vtThreads;
	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;
	CWinThread *pThread = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				pThread = pItem->AjustTime(pParam);
				if ( pThread ){
					vtThreads.push_back(pThread);
				}
			}
		}
	}//end for

	//�ȴ������߳̽���
	for ( vector<CWinThread *>::iterator i=vtThreads.begin(); i!=vtThreads.end(); ++i ){
		WaitForSingleObject((*i)->m_hThread,INFINITE);
	}//end for

	//ˢ���б��ʹ�ø�������ļ�������ˢ�³���
	ctrlList.Invalidate();

	delete pParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_UPDATETIME);
	return 0;
}

//������СͼƬ
UINT CMainDlg::ThreadThumbnailImageBatch(LPVOID lpParam)
{
	if ( lpParam==NULL ){
		return 0;
	}

	PPROCESS_PARAM_THUMBNAIL pParam = (PPROCESS_PARAM_THUMBNAIL)lpParam;

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_THUMBNAIL);

	vector<CWinThread *>vtThreads;
	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;
	CWinThread *pThread = NULL;


	//�Ƿ�ָ�exif��Ϣ
	if ( pParam->bResotreExif ){
		//�����Ϊʹ�õ��������⣬û��ʵ��ͬ�����ƣ���һ����������

		THUMBNAIL_PARAM stThumbnailParam;
		stThumbnailParam.bRestoreExif = pParam->bResotreExif;
		stThumbnailParam.nPixelWidth = pParam->nPixelWidth;

		for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
			if ( ctrlList.GetCheck(nItem) ){
				pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
				if ( pItem ){
					stThumbnailParam.pItem = pItem;
					pItem->_Thumbnail(&stThumbnailParam);
				}
			}
		}//end for

	}else{

		for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
			if ( ctrlList.GetCheck(nItem) ){
				pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
				if ( pItem ){
					pThread = pItem->Thumbnail(pParam);
					if ( pThread ){
						vtThreads.push_back(pThread);
					}
				}
			}
		}//end for

		//�ȴ������߳̽���
		for ( vector<CWinThread *>::iterator i=vtThreads.begin(); i!=vtThreads.end(); ++i ){
			WaitForSingleObject((*i)->m_hThread,INFINITE);
		}//end for
	}

	//ctrlList.Invalidate();

	delete pParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_THUMBNAIL,(LPARAM)(LPCTSTR)g_config.strSavePath);
	return 0;
}

//�������ͼƬˮӡ
UINT CMainDlg::ThreadAddWaterMarkerBatch(LPVOID lpParam)
{
	if ( lpParam==NULL ){
		return 0;
	}

	PPROCESS_PARAM_ADDWATERMARKER pParam = (PPROCESS_PARAM_ADDWATERMARKER)lpParam;

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_ADDWATERMARKER);

	vector<CWinThread *>vtThreads;
	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;
	CWinThread *pThread = NULL;

	//////////////////////////////////////////////////////////////////////////
	//����ˮӡģ�壬����ˮӡ�ؼ��������η�Χ����һ���Ի�����ϡ�
	CControlTracker objControlTracker;	
	objControlTracker.Create(g_pMainDlg);
	objControlTracker.Load(pParam->strWatermarkTemplateFile,g_pMainDlg,FALSE);

	pParam->pControlTracker = &objControlTracker;
	//////////////////////////////////////////////////////////////////////////

	//�Ƿ�ָ�exif��Ϣ
	if ( pParam->bResotreExif ){
		//�����Ϊʹ�õ��������⣬û��ʵ��ͬ�����ƣ���һ����������

		ADDWATERMARKER_PARAM stThumbnailParam;
		stThumbnailParam.bRestoreExif = pParam->bResotreExif;
		stThumbnailParam.pControlTracker = &objControlTracker;

		for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
			if ( ctrlList.GetCheck(nItem) ){
				pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
				if ( pItem ){
					stThumbnailParam.pItem = pItem;
					pItem->_AddWaterMarker(&stThumbnailParam);
				}
			}
		}//end for

	}else{

		for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
			if ( ctrlList.GetCheck(nItem) ){
				pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
				if ( pItem ){
					pThread = pItem->AddWaterMarker(pParam);
					if ( pThread ){
						vtThreads.push_back(pThread);
					}
				}
			}
		}//end for

		//�ȴ������߳̽���
		for ( vector<CWinThread *>::iterator i=vtThreads.begin(); i!=vtThreads.end(); ++i ){
			WaitForSingleObject((*i)->m_hThread,INFINITE);
		}//end for
	}

	//ctrlList.Invalidate();

	delete pParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_ADDWATERMARKER,(LPARAM)(LPCTSTR)g_config.strSavePath);
	return 0;
}

//����ɾ��ͼƬExif��Ϣ�������Ϊʹ�õ��������⣬û��ʵ��ͬ�����ƣ���һ����������
UINT CMainDlg::ThreadDeleteImageExifBatch(LPVOID lpParam)
{
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_DELETEEXIF);

	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				AppendTextV("���ڴ���� %d ��ͼƬ��%s",pItem->nItem+1,pItem->strName);
				pItem->DeleteExif();
			}
		}
	}//end for

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_DELETEEXIF);
	return 0;
}

UINT CMainDlg::ThreadDeleteImageExifNotOrignalBatch(LPVOID lpParam)
{
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_DELETEEXIF_NOT_ORIGINAL);

	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				AppendTextV("���ڴ���� %d ��ͼƬ��%s",pItem->nItem+1,pItem->strName);
				pItem->DeleteExifNotOrignal();
			}
		}
	}//end for

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_DELETEEXIF_NOT_ORIGINAL);
	return 0;
}

UINT CMainDlg::ThreadDeleteRemarksBatch(LPVOID lpParam)
{
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_DELETE_REMARKS);

	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				AppendTextV("���ڴ���� %d ��ͼƬ��%s",pItem->nItem+1,pItem->strName);
				pItem->DeleteRemarks();
			}
		}
	}//end for

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_DELETE_REMARKS);
	return 0;
}

UINT CMainDlg::ThreadPureJpgBatch(LPVOID lpParam)
{
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_PUREJPG);

	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				AppendTextV("���ڴ���� %d ��ͼƬ��%s",pItem->nItem+1,pItem->strName);
				pItem->PureJpg();
			}
		}
	}//end for

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_PUREJPG);
	return 0;
}

UINT CMainDlg::ThreadAddRemarksBatch(LPVOID lpParam)
{
	if ( lpParam==NULL ){
		return 0;
	}

	PROCESS_PARAM_ADDCOMMENTS *pParam = (PROCESS_PARAM_ADDCOMMENTS *)lpParam;

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_ADD_REMARKS);

	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				AppendTextV("���ڴ���� %d ��ͼƬ��%s",pItem->nItem+1,pItem->strName);
				pItem->AddRemarks(pParam->strComments);
			}
		}
	}//end for

	delete pParam;
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_ADD_REMARKS);
	return 0;
}

//����ͼƬ��¼ˮƽ��Ϣ�Զ�ת��ͼƬ��ͼƬ����ʧ�棩�������Ϊʹ�õ��������⣬û��ʵ��ͬ�����ƣ���һ����������
UINT CMainDlg::ThreadAutoRotateImageBatch(LPVOID lpParam)
{
	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSBEGIN,CImageHelper::PROCESSTYPE_AUTOROTATE);

	CListCtrl &ctrlList = g_pMainDlg->m_lstPhotos;
	ITEMINFO *pItem = NULL;

	for ( int nItem=0; nItem<ctrlList.GetItemCount(); ++nItem ){
		if ( ctrlList.GetCheck(nItem) ){
			pItem = (ITEMINFO *)ctrlList.GetItemData(nItem);
			if ( pItem ){
				AppendTextV("���ڴ���� %d ��ͼƬ��%s",pItem->nItem+1,pItem->strName);
				pItem->DeleteExif();
			}
		}
	}//end for

	g_pMainDlg->SendMessage(WM_PROCESSPHOTOSEND,CImageHelper::PROCESSTYPE_AUTOROTATE);
	return 0;
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
			if ( m_vtPhotos.empty()==false ){
				OnBnClickedButtonViewPhotos();
			}
		}
	}
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

void CMainDlg::Finalization()
{
	CRect rcMain;
	this->GetWindowRect(&rcMain);
	g_config.nWndPosLeft=rcMain.left;
	g_config.nWndPosTop=rcMain.top;
	g_config.nWndPosRight=rcMain.right;
	g_config.nWndPosBottom=rcMain.bottom;

	//�������ȥ����ֻ�ڱ�������ʱ�����ļ����û��ڵ�һ��ʹ��ʱ������һ�������ļ����û����鲻��
	//g_config.SaveConfig();

	Shell_NotifyIconA(NIM_DELETE,(PNOTIFYICONDATAA)&m_popMsg);
}

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

//ѡ������
void CMainDlg::OnCommandOptions()
{
	COptionsdlg dlg;
	dlg.DoModal();
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

//��m_cmbImagePathHistory��ѡ����Ϊ�û��������ַ
void CMainDlg::OnCbnSelchangeComboImagepathHistory()
{
	int nItem = m_cmbImagePathHistory.GetCurSel();
	if ( nItem!=-1 ){
		m_cmbImagePathHistory.GetLBText(nItem,m_strImagePath);
		m_cmbImagePathHistory.SetWindowText(m_strImagePath);
	}
	UpdateData(FALSE);
}

void CMainDlg::OnBnClickedCheckAll()
{
	int nCheck = ((CButton *)GetDlgItem(IDC_CHECK_ALL))->GetCheck();
	BOOL bChecked = (nCheck==BST_CHECKED);

	for ( int i=0; i<m_lstPhotos.GetItemCount(); ++i ){
		m_lstPhotos.SetCheck(i,bChecked);
	}
}

void CMainDlg::OnOpenSavepath()
{
	ShellExecute(NULL,"open",g_config.strSavePath,NULL,NULL,SW_SHOW);   
}

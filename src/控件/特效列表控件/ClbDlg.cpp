// ClbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Clb.h"
#include "ClbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_COLOR_LB, m_lbTxt);
	DDX_Control(pDX, IDC_CLB, m_lbTask);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TASKCLICK, OnTaskClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	SetIcon(m_hIcon, TRUE);	

	CImageList m_imgList;
	m_imgList.Create(IDB_BITMAP1,16, 1, RGB(255,255,255));

	m_lbTask.CreateTaskList();
	CString tasks[] = 
	{
		"Explorer 7.0",  "WINRAR 4.0", "MediaPlayer", "Reader 7.0",
		"PowerDesigner", "Word 2000",  "EXCEL 2000",  "RealPlayer 10",
		"AutoCAD 2004",  "ACCESS 2003"  
	};

	ItemInfo item;
	item.type = II_ICONTEXT;
	for(int i = 0; i < sizeof(tasks) / sizeof(tasks[0]); i++)
	{
		item.index = i;
		item.text  = tasks[i];
		item.icon  = m_imgList.ExtractIcon(i % 10);
		m_lbTask.AddItem(item);
	}
	m_lbTask.ReSize();
	
	COLORREF crs[] = 
	{
		RGB(255, 0,   0), RGB(0,   255, 0  ), RGB(0,   0,   255),
		RGB(255, 255, 0), RGB(0,   255, 255), RGB(255, 0,   255),	
		RGB(0,   0,   0), RGB(255, 255, 255), RGB(128, 128, 128),
		RGB(128, 0,   0), RGB(0,   128, 128), RGB(0,   128, 0  )		
	};

	CString crNames[] = 
	{
		"红色",  "绿色",   "蓝色",    "黄色",
		"不明",  "粉红",   "黑色",    "白色",
		"灰色",  "紫色",   "墨绿色",  "草绿色"	
	};

	m_lbTxt.CreateColorList();
	item.type = II_COLOR; 
	for(int i = 0; i < sizeof(crs) / sizeof(crs[0]); i++)
	{
		item.color = crs[i];
		item.text  = crNames[i];
		m_lbTxt.AddItem(item);
	}
	m_lbTxt.ReSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
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

HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CMainDlg::OnTaskClick(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDC_CLB)
	{
		CAboutDlg dlg;
		dlg.DoModal();
	}
	else if(wParam == IDC_COLOR_LB)
	{
		CString* pStr = (CString*)lParam;
		MessageBox(*pStr);
	}

	return 0;
}

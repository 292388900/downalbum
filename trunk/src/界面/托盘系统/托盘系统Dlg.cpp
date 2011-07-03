// 托盘系统Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "托盘系统.h"
#include "托盘系统Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// C托盘系统Dlg 对话框




C托盘系统Dlg::C托盘系统Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C托盘系统Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C托盘系统Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C托盘系统Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_POPMSG,OnPopMsg)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// C托盘系统Dlg 消息处理程序

BOOL C托盘系统Dlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C托盘系统Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	static CHAR szTip[]="模拟QQ的托盘系统";

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) ==SC_MINIMIZE)//最小化窗体
	{
		popMsg.cbSize=sizeof(popMsg);
		popMsg.hWnd=this->m_hWnd;
		popMsg.uID=ID_POPMSG;
		popMsg.uFlags=NIF_ICON | NIF_TIP | NIF_MESSAGE;
		popMsg.uCallbackMessage=WM_POPMSG;
		popMsg.hIcon=this->m_hIcon;
		strcpy((char*)&popMsg.szTip,szTip);
		Shell_NotifyIconA(NIM_ADD,(PNOTIFYICONDATAA)&popMsg);
		this->ShowWindow(SW_HIDE);
		bShowed=FALSE;
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C托盘系统Dlg::OnPaint()
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
HCURSOR C托盘系统Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT C托盘系统Dlg::OnPopMsg(WPARAM wParam,LPARAM lParam)
{
	if (lParam==WM_LBUTTONDBLCLK)
	{
		this->ShowWindow(SW_SHOW);
		this->BringWindowToTop();
		Shell_NotifyIconA(NIM_DELETE,(PNOTIFYICONDATAA)&popMsg);
		return 0;
	}

}

void C托盘系统Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Shell_NotifyIconA(NIM_DELETE,(PNOTIFYICONDATAA)&popMsg);
	CDialog::OnClose();
}

void C托盘系统Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow==TRUE)
	{
		KillTimer(1000);
	}
	else
	{
		::SetTimer(this->m_hWnd,1000,500,NULL);
	}
	bShowed=bShow;
	// TODO: 在此处添加消息处理程序代码
}

void C托盘系统Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (bShowed)
	{
		popMsg.hIcon=this->m_hIcon;
		bShowed=FALSE;
	}
	else
	{
		popMsg.hIcon=0;
		bShowed=TRUE;
	}
	Shell_NotifyIconA(NIM_MODIFY,(PNOTIFYICONDATAA)&popMsg);

	CDialog::OnTimer(nIDEvent);
}

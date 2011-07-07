// 气泡窗体Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "气泡窗体.h"
#include "气泡窗体Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C气泡窗体Dlg 对话框




C气泡窗体Dlg::C气泡窗体Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C气泡窗体Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C气泡窗体Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C气泡窗体Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &C气泡窗体Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// C气泡窗体Dlg 消息处理程序

BOOL C气泡窗体Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C气泡窗体Dlg::OnPaint()
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
HCURSOR C气泡窗体Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C气泡窗体Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	tipDlg=new CTipDlg;
	tipDlg->Create(IDD_DIALOG_TIP,this);
	tipDlg->SetTipString("模拟制作的提示窗口 by singer");
	tipDlg->ShowWindow(SW_SHOW);
}

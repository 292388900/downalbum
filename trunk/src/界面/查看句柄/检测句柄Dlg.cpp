// 检测句柄Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "检测句柄.h"
#include "检测句柄Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C检测句柄Dlg 对话框




C检测句柄Dlg::C检测句柄Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C检测句柄Dlg::IDD, pParent)
	, m_nHandle(0)
	, m_strWndText(_T(""))
	, m_strClsName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C检测句柄Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HANDLE, m_nHandle);
	DDX_Text(pDX, IDC_EDIT_WNDTEXT, m_strWndText);
	DDX_Text(pDX, IDC_EDIT_CLSNAME, m_strClsName);
}

BEGIN_MESSAGE_MAP(C检测句柄Dlg, CDialog)
	ON_WM_TIMER()
// 	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// C检测句柄Dlg 消息处理程序

BOOL C检测句柄Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	/************************************************************************/
	SetTimer(1,500,NULL);
	/************************************************************************/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C检测句柄Dlg::OnPaint()
{
	CDialog::OnPaint();
}

void C检测句柄Dlg::OnTimer(UINT_PTR nIDEvent)
{
	char lpClassName[MAX_PATH];

	POINT pt;
	GetCursorPos(&pt);
	CWnd*pWnd=WindowFromPoint(pt);
	if (pWnd!=NULL)
	{
		m_nHandle=(int)pWnd->m_hWnd;
		pWnd->GetWindowText(m_strWndText);
		GetClassName( pWnd->m_hWnd, lpClassName, sizeof(lpClassName) );
		m_strClsName=lpClassName;
	}

	UpdateData(FALSE);
}

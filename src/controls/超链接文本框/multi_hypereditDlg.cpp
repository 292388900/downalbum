// multi_hypereditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "multi_hyperedit.h"
#include "multi_hypereditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMulti_hypereditDlg dialog

CMulti_hypereditDlg::CMulti_hypereditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMulti_hypereditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMulti_hypereditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMulti_hypereditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMulti_hypereditDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMulti_hypereditDlg, CDialog)
	//{{AFX_MSG_MAP(CMulti_hypereditDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulti_hypereditDlg message handlers

BOOL CMulti_hypereditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	GetDlgItem(IDC_EDIT1)->SetWindowText("\r\n\r\n"
		"1.本周工作计划 http://www.google.cn/\r\n\r\n"
		"2.公司最新产品介绍 \r\n\r\n"
		"3.我的空间 http://hi.baidu.com/asmcvc\r\n\r\n"
		);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMulti_hypereditDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMulti_hypereditDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

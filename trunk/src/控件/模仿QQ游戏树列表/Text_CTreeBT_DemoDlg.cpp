// Text_CTreeBT_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Text_CTreeBT_Demo.h"
#include "Text_CTreeBT_DemoDlg.h"

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
// CText_CTreeBT_DemoDlg dialog

CText_CTreeBT_DemoDlg::CText_CTreeBT_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CText_CTreeBT_DemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CText_CTreeBT_DemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CText_CTreeBT_DemoDlg::~CText_CTreeBT_DemoDlg()
{
}

void CText_CTreeBT_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CText_CTreeBT_DemoDlg)
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CText_CTreeBT_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CText_CTreeBT_DemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ROWLINE, OnRowline)
	ON_BN_CLICKED(IDC_ROOTBK, OnRootbk)
	ON_BN_CLICKED(IDC_UPDATEBITMAP, OnUpdatebitmap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CText_CTreeBT_DemoDlg message handlers

BOOL CText_CTreeBT_DemoDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//m_tree.SetBackgroudBitmap(IDB_BMP_BK);
	m_tree.SetExpandBitmap(IDB_BMP_ADD,IDB_BMP_SUN,RGB(255,0,255));
	InitTree();
	ReSizeTree();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CText_CTreeBT_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CText_CTreeBT_DemoDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CText_CTreeBT_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CText_CTreeBT_DemoDlg::InitTree()
{
	CString strText;
	strText = _T("������Ϸ");
	HTREEITEM hRootItem;
	HTREEITEM hItem;
	
	hRootItem = m_tree.InsertItemEx(NULL,strText,IDB_BITMAP1);
	
	hItem = m_tree.InsertItemEx(hRootItem,_T("���ֶ�������ʥ�������Ͳ�ͣ(38191)"),IDB_BITMAP2);
	m_tree.InsertItemEx(hItem,_T("�����콱̨"),IDB_BITMAP3);
	m_tree.InsertItemEx(hItem,_T("���ֳ�ֵ��"),IDB_BITMAP3);
	
	HTREEITEM hTmp= m_tree.InsertItemEx(hItem,_T("�������г�"),IDB_BITMAP4);
	m_tree.InsertItemEx(hTmp,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hTmp,_T("���ָ�����"),IDB_BITMAP2);
	m_tree.Expand(hItem,TVE_EXPAND);
	
	hItem = m_tree.InsertItemEx(hRootItem,_T("�й�����"),IDB_BITMAP6);
	m_tree.InsertItemEx(hItem,_T("������һ��"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("������ʮ��"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("������һ��"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("����������"),IDB_BITMAP5);
	m_tree.InsertItemEx(hItem,_T("������ʮ��"),IDB_BITMAP5);
	m_tree.Expand(hItem,TVE_EXPAND);
	
	hRootItem = m_tree.GetRootItem();
	
	hRootItem = m_tree.InsertItemEx(NULL,_T("������Ϸ"));
	
	hItem = m_tree.InsertItemEx(hRootItem,_T("��Ƶ������"));
	m_tree.InsertItemEx(hItem,_T("����һ��"));
	m_tree.InsertItemEx(hItem,_T("�㶫һ��"));
	
	hTmp= m_tree.InsertItemEx(hItem,_T("������"));
	m_tree.InsertItemEx(hTmp,_T("���е���"));
	m_tree.InsertItemEx(hTmp,_T("���ϵ���"));
	m_tree.Expand(hItem,TVE_EXPAND);
	
	hItem = m_tree.InsertItemEx(hRootItem,_T("QQ���"));
	m_tree.InsertItemEx(hItem,_T("QQ������"));
	m_tree.InsertItemEx(hItem,_T("QQ����"));
	m_tree.InsertItemEx(hItem,_T("QQӰ��"));
	m_tree.InsertItemEx(hItem,_T("QQ����"));
	m_tree.Expand(hItem,TVE_EXPAND);

	//m_tree.EnsureVisible(hRootItem);
	//m_tree.SelectItem(hRootItem);
}

void CText_CTreeBT_DemoDlg::ReSizeTree()
{
	if ( !m_tree.GetSafeHwnd() )
	{
		return;
	}

	CRect rect;
	GetClientRect(rect);
	rect.left   += 20;
	rect.top    += 20;
	rect.right   = rect.left+240;
	rect.bottom -= 20;

	m_tree.MoveWindow(rect);
}
void CText_CTreeBT_DemoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CText_CTreeBT_DemoDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}

void CText_CTreeBT_DemoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ReSizeTree();
}

void CText_CTreeBT_DemoDlg::OnRowline() //�Ƿ���ʾ����
{
	// TODO: Add your control notification handler code here
	m_tree.EnableRowLine( !m_tree.IsEnableRowLine());
	m_tree.Invalidate(FALSE);
}

void CText_CTreeBT_DemoDlg::OnRootbk() //�Ƿ���ʾ�ڵ㱳��
{
	// TODO: Add your control notification handler code here
	m_tree.EnableRootBk( !m_tree.IsEnableRootBk() );
	m_tree.Invalidate(FALSE);
}

void CText_CTreeBT_DemoDlg::OnUpdatebitmap() //�޸�λͼ
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItme = m_tree.GetSelectedItem();
	if ( hItme )
	{
		m_tree.SetItemBitmap(hItme,IDB_BITMAP5);
		m_tree.Invalidate(FALSE);
	}
}

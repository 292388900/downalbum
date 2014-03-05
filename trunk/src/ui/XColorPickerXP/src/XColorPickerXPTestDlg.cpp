// XColorPickerXPTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XColorPickerXPTest.h"
#include "XColorPickerXPTestDlg.h"
#include "about.h"
#include "FontSize.h"
#include "XBalloonMsg.h"

#pragma warning(disable : 4996)	// disable bogus deprecation warning

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestDlg dialog

BEGIN_MESSAGE_MAP(CXColorPickerXPTestDlg, CDialog)
	//{{AFX_MSG_MAP(CXColorPickerXPTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(CPN_SELCHANGE, OnColorChange)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXColorPickerXPTestDlg::CXColorPickerXPTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXColorPickerXPTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXColorPickerXPTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_rgbText = RGB(0,0,0);
	//m_rgbBackground = RGB(255, 153, 0);
	m_rgbText = RGB(255,255,255);
	m_rgbBackground = RGB(0, 128, 255);
	m_strTextCustomColors = _T("");
	m_strBackgroundCustomColors = _T("");
	LoadColors();
}

CXColorPickerXPTestDlg::~CXColorPickerXPTestDlg()
{
	if (m_font.GetSafeHandle())
		m_font.DeleteObject();
}

void CXColorPickerXPTestDlg::DoDataExchange(CDataExchange* pDX)
{
	TRACE(_T("in CXColorPickerXPTestDlg::DoDataExchange\n"));
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXColorPickerXPTestDlg)
	DDX_Control(pDX, IDC_COLOR_SAMPLE, m_stcColorSample);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR, m_ctlBackgroundColor);
	DDX_Control(pDX, IDC_TEXT_COLOR, m_ctlTextColor);
	//}}AFX_DATA_MAP

	DDX_XColorPickerXP(pDX, IDC_TEXT_COLOR, m_rgbText);
	DDX_XColorPickerXP(pDX, IDC_BACKGROUND_COLOR, m_rgbBackground);

	DDX_XColorPickerXPCustom(pDX, IDC_TEXT_COLOR, m_strTextCustomColors);
	DDX_XColorPickerXPCustom(pDX, IDC_BACKGROUND_COLOR, m_strBackgroundCustomColors);
}

/////////////////////////////////////////////////////////////////////////////
// CXColorPickerXPTestDlg message handlers

BOOL CXColorPickerXPTestDlg::OnInitDialog()
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
	
	LOGFONT lf;

	memset(&lf, 0, sizeof(lf));
	lf.lfWeight = 400;
	lf.lfHeight = GetFontHeight(14);
	_tcscpy(lf.lfFaceName, _T("Arial"));
	m_font.CreateFontIndirect(&lf);
	m_stcColorSample.SetFont(&m_font);

	m_stcColorSample.SetWindowText(_T("Sample text"));
	m_stcColorSample.SetTextColor(m_rgbText);
	m_stcColorSample.SetBackgroundColor(m_rgbBackground);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// Load colors from registry
void CXColorPickerXPTestDlg::LoadColors()
{
	TRACE(_T("in CXColorPickerXPTestDlg::LoadColors\n"));

	m_rgbText = AfxGetApp()->GetProfileInt(_T("Colors"), _T("Text"), RGB(255,255,255));
	m_rgbBackground = AfxGetApp()->GetProfileInt(_T("Colors"), _T("Background"), RGB(0, 128, 255));

	m_strTextCustomColors = AfxGetApp()->GetProfileString(_T("Colors"), _T("TextCustom"), _T(""));
	m_strBackgroundCustomColors = AfxGetApp()->GetProfileString(_T("Colors"), _T("BackgroundCustom"), _T(""));

	TRACE(_T("m_rgbText=RGB(%d,%d,%d)\n"), GetRValue(m_rgbText), GetGValue(m_rgbText), GetBValue(m_rgbText));
	TRACE(_T("m_rgbBackground=RGB(%d,%d,%d)\n"), GetRValue(m_rgbBackground), GetGValue(m_rgbBackground), GetBValue(m_rgbBackground));
	TRACE(_T("m_strTextCustomColors=%s\n"), m_strTextCustomColors);
	TRACE(_T("m_strBackgroundCustomColors=%s\n"), m_strBackgroundCustomColors);
}

// Save colors to registry
void CXColorPickerXPTestDlg::SaveColors()
{
	TRACE(_T("in CXColorPickerXPTestDlg::SaveColors\n"));

	TRACE(_T("m_rgbText=RGB(%d,%d,%d)\n"), GetRValue(m_rgbText), GetGValue(m_rgbText), GetBValue(m_rgbText));
	TRACE(_T("m_rgbBackground=RGB(%d,%d,%d)\n"), GetRValue(m_rgbBackground), GetGValue(m_rgbBackground), GetBValue(m_rgbBackground));
	TRACE(_T("m_strTextCustomColors=%s\n"), m_strTextCustomColors);
	TRACE(_T("m_strBackgroundCustomColors=%s\n"), m_strBackgroundCustomColors);

	AfxGetApp()->WriteProfileInt(_T("Colors"), _T("Text"), m_rgbText);
	AfxGetApp()->WriteProfileInt(_T("Colors"), _T("Background"), m_rgbBackground);

	AfxGetApp()->WriteProfileString(_T("Colors"), _T("TextCustom"), m_strTextCustomColors);
	AfxGetApp()->WriteProfileString(_T("Colors"), _T("BackgroundCustom"), m_strBackgroundCustomColors);
}

void CXColorPickerXPTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXColorPickerXPTestDlg::OnPaint() 
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

HCURSOR CXColorPickerXPTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CXColorPickerXPTestDlg::OnColorChange(WPARAM, LPARAM lParam)
{
	TRACE(_T("in CXColorPickerXPTestDlg::OnColorChange\n"));

	if (lParam == IDC_TEXT_COLOR)
	{
		TRACE(_T("IDC_TEXT_COLOR\n"));
	}
	else if (lParam == IDC_BACKGROUND_COLOR)
	{
		TRACE(_T("IDC_BACKGROUND_COLOR\n"));
	}

	if (m_ctlTextColor.GetColor() == m_ctlBackgroundColor.GetColor())
	{
		UpdateWindow();
		CXBalloonMsg::Show(_T("Unreadable Colors"),
						   _T("The text and background colors are identical.\r\n")
						   _T("The text will not be readable."),
						   m_ctlBackgroundColor.m_hWnd, 
						   m_hWnd,
						   AfxGetInstanceHandle(),
						   TTI_ERROR);
	}
	else
	{
		m_rgbText = m_ctlTextColor.GetColor();
		m_rgbBackground = m_ctlBackgroundColor.GetColor();
	}

	m_ctlTextColor.SetColor(m_rgbText);
	m_ctlBackgroundColor.SetColor(m_rgbBackground);
	m_stcColorSample.SetTextColor(m_rgbText);
	m_stcColorSample.SetBackgroundColor(m_rgbBackground);

	return 0;
}

void CXColorPickerXPTestDlg::OnDestroy() 
{
	TRACE(_T("in CXColorPickerXPTestDlg::OnDestroy\n"));
	UpdateData(TRUE);
	SaveColors();
	CDialog::OnDestroy();
}

// GradientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Gradient.h"
#include "GradientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradientDlg dialog

//just a helper function to create nicer fade effect.
COLORREF DarkenColor(COLORREF col,double factor)
{
	if(factor>0.0&&factor<=1.0){
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(col);
		green = GetGValue(col);
		blue = GetBValue(col);
		lightred = (BYTE)(red-(factor*red));
		lightgreen = (BYTE)(green-(factor*green));
		lightblue = (BYTE)(blue-(factor*blue));
		col = RGB(lightred,lightgreen,lightblue);
	}
	return(col);
}

CGradientDlg::CGradientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGradientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGradientDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pBoldFont = NULL;
}

void CGradientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGradientDlg)
	DDX_Control(pDX, IDC_BAR2, m_cBar2);
	DDX_Control(pDX, IDC_BAR1, m_cBar1);
	DDX_Control(pDX, IDC_EXAMPLE2, m_cExample2);
	DDX_Control(pDX, IDC_TH, m_cTH);
	DDX_Control(pDX, IDC_EXAMPLE, m_cExample);
	DDX_Control(pDX, IDC_TITLE, m_cTitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGradientDlg, CDialog)
	//{{AFX_MSG_MAP(CGradientDlg)
	ON_BN_CLICKED(IDC_TH, OnTh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradientDlg message handlers

BOOL CGradientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_pBoldFont = new CFont;
	m_pBoldFont->CreateFont(25,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"Arial");

	//Use big font and standard colors
	m_cTitle.SetFont(m_pBoldFont);
	m_cTitle.SetWindowText("This is CGradientStatic example :)");

	//Use font of dialog controls, centered text and default colors
	m_cExample.SetWindowText("This text is in the center !");
	m_cExample.SetTextAlign(1);

	//Use big font, custom colors and centered text
	m_cExample2.SetFont(m_pBoldFont);
	m_cExample2.SetWindowText("RED - CENTERED - BLACK");
	m_cExample2.SetTextAlign(1);
	m_cExample2.SetColor(RGB(255,0,0));
	m_cExample2.SetGradientColor(RGB(0,0,0));
	m_cExample2.SetTextColor(RGB(255,255,255));


	m_cBar1.SetColor( GetSysColor(COLOR_BTNFACE) );
	m_cBar1.SetGradientColor( DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) );
	m_cBar1.SetVerticalGradient(); //set gradient to be vertical

	m_cBar2.SetGradientColor( GetSysColor(COLOR_BTNFACE) );
	m_cBar2.SetColor( DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) );
	m_cBar2.SetVerticalGradient(); //set gradient to be vertical
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGradientDlg::OnTh() 
{
	ShellExecute(NULL,"open","http://www.trayhelper.com",NULL,NULL,SW_SHOWNORMAL);
}

CGradientDlg::~CGradientDlg()
{
	delete m_pBoldFont;
	m_pBoldFont = NULL;
}

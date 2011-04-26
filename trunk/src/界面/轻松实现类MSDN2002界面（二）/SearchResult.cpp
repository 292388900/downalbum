// SearchResult.cpp : implementation file
//

#include "stdafx.h"
#include "dotNET界面.h"
#include "SearchResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchResult dialog
IMPLEMENT_DYNCREATE(CSearchResult, CDialog)

CSearchResult::CSearchResult(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSearchResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchResult)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchResult, CDialog)
	//{{AFX_MSG_MAP(CSearchResult)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchResult message handlers

BOOL CSearchResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LV_COLUMN lvcol;

	
	//向列表控制中添加表列
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中

	int i=0;
	lvcol.pszText="Title";
	lvcol.iSubItem=i;
	lvcol.cx=200;
	m_ListCtrl.InsertColumn(i++,&lvcol);
	
	lvcol.pszText="Location";
	lvcol.iSubItem=i;
	lvcol.cx=200;
	m_ListCtrl.InsertColumn(i++,&lvcol);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchResult::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(IsWindow(m_ListCtrl))
		m_ListCtrl.MoveWindow (0,0,cx,cy);
	
}

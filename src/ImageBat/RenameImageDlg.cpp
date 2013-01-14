// RenameImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageBat.h"
#include "RenameImageDlg.h"
#include "global.h"
#include "ImageInfo.h"
#include "FormatTagString.h"
#include "TagAdder.h"


// CRenameImageDlg 对话框

IMPLEMENT_DYNAMIC(CRenameImageDlg, CDialog)

CRenameImageDlg::CRenameImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameImageDlg::IDD, pParent)
	, m_nStartNumber(0)
	, m_nIncreaseStep(0)
	, m_bRenameAddIndex(FALSE)
	, m_bRenameAddCustomText(FALSE)
	, m_bRenameAddPhotoDate(FALSE)
	, m_bRenameAddOrignName(FALSE)
	, m_strRenameRules(_T(""))
	, m_strRenamePreview(_T(""))
{

}

CRenameImageDlg::~CRenameImageDlg()
{
}

void CRenameImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RENAME_BEGINWITH, m_nStartNumber);
	DDV_MinMaxInt(pDX, m_nStartNumber, 1, 2147483647);
	DDX_Text(pDX, IDC_EDIT_INCREASE_STEP, m_nIncreaseStep);
	DDV_MinMaxInt(pDX, m_nIncreaseStep, 1, 100000);
	DDX_Text(pDX, IDC_EDIT_RENAME_RULES, m_strRenameRules);
	DDX_Text(pDX, IDC_EDIT_RENAME_PREVIEW, m_strRenamePreview);
	DDX_Control(pDX, IDC_STATIC_RENAME_PREVIEW, m_stcRenamePreview);
}


BEGIN_MESSAGE_MAP(CRenameImageDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameImageDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_RENAME_PREVIEW, &CRenameImageDlg::OnStnClickedStaticRenamePreview)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TAG, &CRenameImageDlg::OnBnClickedButtonAddTag)
END_MESSAGE_MAP()


// CRenameImageDlg 消息处理程序

BOOL CRenameImageDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();
	UpdateData(FALSE);

	return TRUE;
}

void CRenameImageDlg::InitAllControls()
{
	CTime time = CTime::GetCurrentTime();

	m_nIncreaseStep = 1;
	m_nStartNumber = 1;

	//////////////////////////////////////////////////////////////////////////
	m_strRenameRules = g_config.m_strRenameRules;
	m_stParam.nFileExtCaseMethod = g_config.m_nRenameWordCase;
	((CButton *)GetDlgItem(IDC_RADIO_RENAME_FILEEXT_NOCHANGE+g_config.m_nRenameWordCase))->SetCheck(BST_CHECKED);
	//////////////////////////////////////////////////////////////////////////
	m_stcRenamePreview.SetUnderline(CStaticLink::ulAlways);
	SetWindowText("文件重命名设置");
	//////////////////////////////////////////////////////////////////////////
}
void CRenameImageDlg::OnBnClickedOk()
{
	UpdateData();
	m_stParam.strRenameRules = m_strRenameRules;
	m_stParam.nStartNumber = m_nStartNumber;
	m_stParam.nIncreaseStep = m_nIncreaseStep;

	int nID = GetCheckedRadioButton(IDC_RADIO_RENAME_FILEEXT_NOCHANGE,IDC_RADIO_RENAME_FILEEXT_TOUPPER);
	if ( nID==IDC_RADIO_RENAME_FILEEXT_TOLOWER ){
		m_stParam.nFileExtCaseMethod = FILEEXT_CASE_TOLOWER;
	}else if ( nID==IDC_RADIO_RENAME_FILEEXT_TOUPPER ){
		m_stParam.nFileExtCaseMethod = FILEEXT_CASE_TOUPPER;
	}else{
		m_stParam.nFileExtCaseMethod = FILEEXT_CASE_NOCHANGE;
	}

	//////////////////////////////////////////////////////////////////////////
	g_config.m_strRenameRules = m_strRenameRules;
	g_config.m_nRenameWordCase = m_stParam.nFileExtCaseMethod;

	g_config.SaveConfig();
	//////////////////////////////////////////////////////////////////////////

	OnOK();
}

/*
因为<>不能作为文件名，当做tag比较合适
0.<no>编号
1.<photodate>
2.<originalname>
*/
//通过解析重命名规则来显示重命名预览效果
void CRenameImageDlg::OnStnClickedStaticRenamePreview()
{
	UpdateData();
	CFormatTagString obj;
	obj.Init(0,m_strRenameRules);
	m_strRenamePreview = obj.GetPreviewText();
	UpdateData(FALSE);
}

void CRenameImageDlg::OnBnClickedButtonAddTag()
{
	UpdateData();
	CTagAdder tagAdder;
	tagAdder.AddTagTo(m_strRenameRules);
	UpdateData(FALSE);
}

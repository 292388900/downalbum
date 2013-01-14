// Optionsdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageBat.h"
#include "Optionsdlg.h"
#include <StarLib/PathDialog/PathDialog.h>
#include "global.h"


// COptionsdlg 对话框

IMPLEMENT_DYNAMIC(COptionsdlg, CDialog)

COptionsdlg::COptionsdlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsdlg::IDD, pParent)
	, m_strSavePath(_T(""))
	, m_bCheckIfClickIcon(FALSE)
{

}

COptionsdlg::~COptionsdlg()
{
}

void COptionsdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	DDV_MaxChars(pDX, m_strSavePath, 260);
	DDX_Check(pDX, IDC_CHECK_CHECK_WHEN_CLICK_PREVIEWICON, m_bCheckIfClickIcon);
}


BEGIN_MESSAGE_MAP(COptionsdlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET_SAVEPATH, &COptionsdlg::OnBnClickedButtonSetSavepath)
	ON_BN_CLICKED(IDOK, &COptionsdlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COptionsdlg 消息处理程序
BOOL COptionsdlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();
	UpdateData(FALSE);

	return TRUE;
}

void COptionsdlg::InitAllControls()
{
	m_strSavePath = g_config.strSavePath;
	m_bCheckIfClickIcon = g_config.m_bCheckIfClickIcon;
}


void COptionsdlg::OnBnClickedButtonSetSavepath()
{
	CPathDialog dlg(g_sAPPNAME_CH, "请选择你要保存图片的目录", m_strSavePath);

	if (dlg.DoModal()==IDOK){
		m_strSavePath=dlg.GetPathName();
		if ( m_strSavePath.Right(1)!="\\" ){
			m_strSavePath=m_strSavePath+"\\";
		}
		UpdateData(FALSE);
	}
}

void COptionsdlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if ( m_strSavePath.Right(1)!="\\" ){
		m_strSavePath=m_strSavePath+"\\";
	}
	g_config.strSavePath = m_strSavePath;
	g_config.m_bCheckIfClickIcon = m_bCheckIfClickIcon;
	g_config.SaveConfig();
	OnOK();
}

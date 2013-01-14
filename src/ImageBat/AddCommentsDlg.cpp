// AddCommentsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AddCommentsDlg.h"


// CAddCommentsDlg 对话框

IMPLEMENT_DYNAMIC(CAddCommentsDlg, CDialog)

CAddCommentsDlg::CAddCommentsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddCommentsDlg::IDD, pParent)
	, m_strAddComments(_T(""))
{

}

CAddCommentsDlg::~CAddCommentsDlg()
{
}

void CAddCommentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADD_COMMENTS, m_strAddComments);
}


BEGIN_MESSAGE_MAP(CAddCommentsDlg, CDialog)
END_MESSAGE_MAP()


// CAddCommentsDlg 消息处理程序

BOOL CAddCommentsDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();

	return TRUE;
}

void CAddCommentsDlg::InitAllControls()
{
	SetWindowText("添加图片备注信息");

	//加载上次的信息，如果没有，则默认为photo by xxx
	m_strAddComments = "photo by xxx";
}
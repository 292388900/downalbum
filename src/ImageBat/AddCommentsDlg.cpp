// AddCommentsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AddCommentsDlg.h"


// CAddCommentsDlg �Ի���

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


// CAddCommentsDlg ��Ϣ�������

BOOL CAddCommentsDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();

	return TRUE;
}

void CAddCommentsDlg::InitAllControls()
{
	SetWindowText("���ͼƬ��ע��Ϣ");

	//�����ϴε���Ϣ�����û�У���Ĭ��Ϊphoto by xxx
	m_strAddComments = "photo by xxx";
}
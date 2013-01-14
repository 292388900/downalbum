// UpdateTimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageBat.h"
#include "UpdateTimeDlg.h"


// CUpdateTimeDlg �Ի���

IMPLEMENT_DYNAMIC(CUpdateTimeDlg, CDialog)

CUpdateTimeDlg::CUpdateTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateTimeDlg::IDD, pParent)
	, m_nHoursToAdd(0)
{

}

CUpdateTimeDlg::~CUpdateTimeDlg()
{
}

void CUpdateTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADD_HOUR, m_nHoursToAdd);
}


BEGIN_MESSAGE_MAP(CUpdateTimeDlg, CDialog)
END_MESSAGE_MAP()


// CUpdateTimeDlg ��Ϣ�������

BOOL CUpdateTimeDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();

	return TRUE;
}

void CUpdateTimeDlg::InitAllControls()
{
	SetWindowText("ͼƬ����ʱ��У��");

	CSpinButtonCtrl *pSpinCtrl = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ADD_HOURS);
	pSpinCtrl->SetBuddy(GetDlgItem(IDC_EDIT_ADD_HOUR));

	pSpinCtrl->SetRange(-10000,10000);
	pSpinCtrl->SetPos(-8);
}
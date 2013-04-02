// ThumbnailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageBat.h"
#include "ThumbnailDlg.h"
#include "global.h"


// CThumbnailDlg �Ի���

IMPLEMENT_DYNAMIC(CThumbnailDlg, CDialog)

CThumbnailDlg::CThumbnailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThumbnailDlg::IDD, pParent)
	, m_strImageSrcPath(_T(""))
	, m_strImageOutPath(_T(""))
	, m_bResotreExif(FALSE)
	, m_nLimitWidth(200)
	, m_nLimitHeight(200)
{
	m_nPixelWidth = 1600;
}

CThumbnailDlg::~CThumbnailDlg()
{
}

void CThumbnailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PREDEFINED, m_cmbThumnailPredefined);
	DDX_Text(pDX, IDC_EDIT_IMAGE_SRC_PATH, m_strImageSrcPath);
	DDX_Text(pDX, IDC_EDIT_IMAGE_OUT_PATH, m_strImageOutPath);
	DDX_Check(pDX, IDC_CHECK_RESTORE_EXIF, m_bResotreExif);
	DDX_Text(pDX, IDC_EDIT_LIMIT_WIDTH, m_nLimitWidth);
	DDV_MinMaxInt(pDX, m_nLimitWidth, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_LIMIT_HEIGHT, m_nLimitHeight);
	DDV_MinMaxInt(pDX, m_nLimitHeight, 0, 10000);
}


BEGIN_MESSAGE_MAP(CThumbnailDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CThumbnailDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CThumbnailDlg ��Ϣ�������

BOOL CThumbnailDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();
	UpdateData(FALSE);

	return TRUE;
}


void CThumbnailDlg::InitAllControls()
{
	this->SetWindowText("������СͼƬ�ߴ�");

	m_bResotreExif = g_config.m_bResotreExif;

	THUMBNAIL_PREDEFINED_OPTIONS stThumnail;
	
	m_vtThumbnailOptions.clear();
	stThumnail.nPixelWidth = stThumnail.nPixelHeight = 1600;
	stThumnail.strDescription = "1600 ���أ�����ͼ��������ֽ��";
	m_vtThumbnailOptions.push_back(stThumnail);

	stThumnail.nPixelWidth = stThumnail.nPixelHeight = 800;
	stThumnail.strDescription = "800 ���أ��ĵ���ҳ��ͼ��";
	m_vtThumbnailOptions.push_back(stThumnail);

	stThumnail.nPixelWidth = stThumnail.nPixelHeight = 640;
	stThumnail.strDescription = "640 ���أ��ĵ���ҳ��ͼ��";
	m_vtThumbnailOptions.push_back(stThumnail);

	stThumnail.nPixelWidth = stThumnail.nPixelHeight = 320;
	stThumnail.strDescription = "320 ���أ�����ͼ��Сͼ��";
	m_vtThumbnailOptions.push_back(stThumnail);

	stThumnail.nPixelWidth = stThumnail.nPixelHeight = 160;
	stThumnail.strDescription = "160 ���أ�����ͼ��Сͼ��";
	m_vtThumbnailOptions.push_back(stThumnail);

	m_cmbThumnailPredefined.ResetContent();
	for ( int i=0; i<(int)m_vtThumbnailOptions.size(); ++i ){
		m_cmbThumnailPredefined.InsertString(-1,m_vtThumbnailOptions[i].strDescription);
	}

	//Ĭ�ϵ�һ��
	((CButton *)GetDlgItem(IDC_RADIO_PREDEFINED))->SetCheck(BST_CHECKED);
	m_cmbThumnailPredefined.SetCurSel(g_config.m_nIndexPredefinedThumnailSeleted);
}


void CThumbnailDlg::OnBnClickedOk()
{
	UpdateData();

	int nSelItem = m_cmbThumnailPredefined.GetCurSel();
	if ( nSelItem==-1 ){
		AfxMessageBox("��ѡ��һ�ַ���!");
		return;
	}

	if ( nSelItem>(int)m_vtThumbnailOptions.size() ){
		nSelItem = 0;
	}

	m_nPixelWidth = m_vtThumbnailOptions[nSelItem].nPixelWidth;

	g_config.m_bResotreExif = m_bResotreExif;
	g_config.m_nIndexPredefinedThumnailSeleted = nSelItem;
	g_config.SaveConfig();
	OnOK();
}

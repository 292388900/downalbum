// ClipImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageBat.h"
#include "ClipImageDlg.h"


// CClipImageDlg �Ի���

IMPLEMENT_DYNAMIC(CClipImageDlg, CDialog)

CClipImageDlg::CClipImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClipImageDlg::IDD, pParent)
{
	m_hdcBack = NULL;
	m_pSrcImg = NULL;
}

CClipImageDlg::~CClipImageDlg()
{
	if ( m_hdcBack!=NULL ){
		DeleteDC(m_hdcBack);
	}
}

void CClipImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIPIMAGE_SETTING, m_lstClipSettings);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_stcPreview);
}


BEGIN_MESSAGE_MAP(CClipImageDlg, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CLIPIMAGE_SETTING, &CClipImageDlg::OnNMClickList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_CLIPIMAGE_SETTING, &CClipImageDlg::OnLvnEndlabeleditList)
	ON_BN_CLICKED(IDOK, &CClipImageDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CClipImageDlg ��Ϣ�������

BOOL CClipImageDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();
	this->SetWindowText(_T("�ü�ͼƬ"));

	return TRUE;
}

void CClipImageDlg::PostNcDestroy()
{
	if ( m_pSrcImg!=NULL ){
		delete m_pSrcImg;
	}
}

void CClipImageDlg::InitAllControls()
{
	//////////////////////////////////////////////////////////////////////////

	m_lstClipSettings.SetExtendedStyle( m_lstClipSettings.GetExtendedStyle() | 
		/*LVS_EX_CHECKBOXES|*/LVS_EX_FULLROWSELECT & (~LVS_EX_GRIDLINES) &(~LVS_EX_CHECKBOXES) );
	m_lstClipSettings.ModifyStyle( LVS_LIST | LVS_SMALLICON | LVS_ICON | LVS_NOCOLUMNHEADER, LVS_REPORT | LVS_EDITLABELS);
	CRect rcList;
	m_lstClipSettings.GetClientRect(&rcList);
	m_lstClipSettings.InsertColumn( 0, _T("��������"), LVCFMT_LEFT, 70 );
	m_lstClipSettings.InsertColumn( 1, _T("X(����ͼƬ��ߵ�������������Ϊ����)"), LVCFMT_LEFT, 70 );
	m_lstClipSettings.InsertColumn( 2, _T("Y(����ͼƬ�ϱߵ�������������Ϊ����)"), LVCFMT_LEFT, 70 );
	m_lstClipSettings.InsertColumn( 3, _T("�ü����(������ʾ����ͼƬ�ұߵ�������)"), LVCFMT_LEFT, 80 );
	m_lstClipSettings.InsertColumn( 4, _T("�ü��߶�(������ʾ����ͼƬ�±ߵ�������)"), LVCFMT_LEFT, 80 );
	//////////////////////////////////////////////////////////////////////////

	if ( m_lstClipSettings.GetItemCount()==0 ){
		int nItem = m_lstClipSettings.InsertItem(0,_T("�Զ���1"));
		m_lstClipSettings.SetItemText(nItem,1,_T("0"));
		m_lstClipSettings.SetItemText(nItem,2,_T("0"));
		m_lstClipSettings.SetItemText(nItem,3,_T("800"));
		m_lstClipSettings.SetItemText(nItem,4,_T("600"));
	}

	InitPreviewCtrl();
}

void CClipImageDlg::InitPreviewCtrl()
{
	//��ʾԤ��ͼ
	CRect rcWnd;
	HDC hdc = NULL;
	int nPreviewWidth = 0;
	int nPreviewHeight = 0;

	m_stcPreview.GetClientRect(rcWnd);
	nPreviewWidth = rcWnd.Width();
	nPreviewHeight = rcWnd.Height();

	//����б������ͼƬ�����ͼƬ
	if ( GetFileAttributes(m_strPreviewImageFile)!=-1 ){
		CStringW strFileNameW;
		strFileNameW = m_strPreviewImageFile;
		m_pSrcImg = Gdiplus::Image::FromFile(strFileNameW);
	}

	//ͼƬδ���سɹ�������һ����ɫλͼ��ΪԤ��ͼ
	if ( m_pSrcImg==NULL ){
		m_pSrcImg = new Bitmap(nPreviewWidth, nPreviewHeight,PixelFormat32bppARGB );
		Graphics *g = Graphics::FromImage(m_pSrcImg);
		SolidBrush brush(Color::White);
		g->FillRectangle(&brush,0,0,nPreviewWidth, nPreviewHeight);
		delete g;
	}

	HDC hDC = ::GetDC(NULL);
	m_hdcBack = CreateCompatibleDC(hDC);
	HBITMAP hBmp = CreateCompatibleBitmap(hDC, m_pSrcImg->GetWidth(), m_pSrcImg->GetHeight());
	SelectObject(m_hdcBack, hBmp);
	Gdiplus::Graphics graphics(m_hdcBack);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBilinear);
	graphics.DrawImage(m_pSrcImg, 0, 0, m_pSrcImg->GetWidth(), m_pSrcImg->GetHeight());
	DeleteObject(hBmp);
	::ReleaseDC(NULL,hDC);
}
void CClipImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcWnd;
	int nPreviewWidth = 0;
	int nPreviewHeight = 0;

	m_stcPreview.GetClientRect(rcWnd);
	nPreviewWidth = rcWnd.Width();
	nPreviewHeight = rcWnd.Height();

	CDC *pDC = m_stcPreview.GetDC();
	pDC->SetStretchBltMode(HALFTONE);	//COLORONCOLOR
	StretchBlt(pDC->m_hDC,0,0,nPreviewWidth,nPreviewHeight,m_hdcBack,0,0,m_pSrcImg->GetWidth(), m_pSrcImg->GetHeight(),SRCCOPY);
	m_stcPreview.ReleaseDC(pDC);
}

void CClipImageDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO hitinfo;

	//Copy click point
	hitinfo.pt = pNMListView->ptAction;

	//Make the hit test...
	int nItem = m_lstClipSettings.HitTest(&hitinfo);
	if ( nItem!=-1 ){
		InvalidatePreview(nItem);
	}

	*pResult = 0;
}

//ˢ��Ԥ������
void CClipImageDlg::InvalidatePreview(int nItem)
{
	int nPosX = 0;
	int nPosY = 0;
	int nWidth = 0;
	int nHeight = 0;
	nPosX = StrToInt(m_lstClipSettings.GetItemText(nItem,1));
	nPosY = StrToInt(m_lstClipSettings.GetItemText(nItem,2));
	nWidth = StrToInt(m_lstClipSettings.GetItemText(nItem,3));
	nHeight = StrToInt(m_lstClipSettings.GetItemText(nItem,4));
	CImageHelper imgHelper;
	imgHelper.AjustClipArea(m_pSrcImg->GetWidth(),m_pSrcImg->GetHeight(),nPosX,nPosY,nWidth,nHeight);

	//�Ȱ�ԭͼ����
	Gdiplus::Graphics graphics(m_hdcBack);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBilinear);
	graphics.DrawImage(m_pSrcImg, 0, 0, m_pSrcImg->GetWidth(), m_pSrcImg->GetHeight());

	//�ٻ��ϴ���ü�����ľ���
	int nPenWidth = m_pSrcImg->GetWidth();
	if ( m_pSrcImg->GetWidth()>m_pSrcImg->GetHeight() ){
		nPenWidth = m_pSrcImg->GetHeight();
	}
	nPenWidth = nPenWidth/400;
	if ( nPenWidth==0 ){
		nPenWidth=1;
	}else{
		nPenWidth*=4;
	}
	Gdiplus::Pen pen(Gdiplus::Color::Red,Gdiplus::REAL(nPenWidth));
	graphics.DrawRectangle(&pen,nPosX,nPosY,nWidth,nHeight);

	//ֻˢ��Ԥ�����ڲ���
	CRect rcWnd;
	m_stcPreview.GetWindowRect(rcWnd);
	this->ScreenToClient(rcWnd);
	this->InvalidateRect(rcWnd,TRUE);
}
void CClipImageDlg::OnLvnEndlabeleditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = TRUE;   //TRUEֵ��ʾ�����޸����FALSEֵ��ʾ���޸�����
}

void CClipImageDlg::OnBnClickedOk()
{
	int nItem = -1;
	CString strText;

	UpdateData();
	POSITION pos = m_lstClipSettings.GetFirstSelectedItemPosition();
	if ( pos!=NULL ){
		nItem = m_lstClipSettings.GetNextSelectedItem(pos);
	}

	if ( nItem==-1 ){
		AfxMessageBox("��ѡ��ü�������");
		return;
	}

	strText = m_lstClipSettings.GetItemText(nItem,1);
	m_stParam.nPosX = StrToInt(strText);
	strText = m_lstClipSettings.GetItemText(nItem,2);
	m_stParam.nPosY = StrToInt(strText);
	strText = m_lstClipSettings.GetItemText(nItem,3);
	m_stParam.nWidth = StrToInt(strText);
	strText = m_lstClipSettings.GetItemText(nItem,4);
	m_stParam.nHeight = StrToInt(strText);
	OnOK();
}

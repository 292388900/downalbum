// ClipImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageBat.h"
#include "ClipImageDlg.h"


// CClipImageDlg 对话框

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


// CClipImageDlg 消息处理程序

BOOL CClipImageDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitAllControls();
	this->SetWindowText(_T("裁剪图片"));

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
	m_lstClipSettings.InsertColumn( 0, _T("方案名称"), LVCFMT_LEFT, 70 );
	m_lstClipSettings.InsertColumn( 1, _T("X(距离图片左边的像素数，不能为负数)"), LVCFMT_LEFT, 70 );
	m_lstClipSettings.InsertColumn( 2, _T("Y(距离图片上边的像素数，不能为负数)"), LVCFMT_LEFT, 70 );
	m_lstClipSettings.InsertColumn( 3, _T("裁剪宽度(负数表示距离图片右边的像素数)"), LVCFMT_LEFT, 80 );
	m_lstClipSettings.InsertColumn( 4, _T("裁剪高度(负数表示距离图片下边的像素数)"), LVCFMT_LEFT, 80 );
	//////////////////////////////////////////////////////////////////////////

	if ( m_lstClipSettings.GetItemCount()==0 ){
		int nItem = m_lstClipSettings.InsertItem(0,_T("自定义1"));
		m_lstClipSettings.SetItemText(nItem,1,_T("0"));
		m_lstClipSettings.SetItemText(nItem,2,_T("0"));
		m_lstClipSettings.SetItemText(nItem,3,_T("800"));
		m_lstClipSettings.SetItemText(nItem,4,_T("600"));
	}

	InitPreviewCtrl();
}

void CClipImageDlg::InitPreviewCtrl()
{
	//显示预览图
	CRect rcWnd;
	HDC hdc = NULL;
	int nPreviewWidth = 0;
	int nPreviewHeight = 0;

	m_stcPreview.GetClientRect(rcWnd);
	nPreviewWidth = rcWnd.Width();
	nPreviewHeight = rcWnd.Height();

	//如果列表框中有图片则加载图片
	if ( GetFileAttributes(m_strPreviewImageFile)!=-1 ){
		CStringW strFileNameW;
		strFileNameW = m_strPreviewImageFile;
		m_pSrcImg = Gdiplus::Image::FromFile(strFileNameW);
	}

	//图片未加载成功，创建一个白色位图作为预览图
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
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

//刷新预览窗口
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

	//先把原图画上
	Gdiplus::Graphics graphics(m_hdcBack);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBilinear);
	graphics.DrawImage(m_pSrcImg, 0, 0, m_pSrcImg->GetWidth(), m_pSrcImg->GetHeight());

	//再画上代表裁剪区域的矩形
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

	//只刷新预览窗口部分
	CRect rcWnd;
	m_stcPreview.GetWindowRect(rcWnd);
	this->ScreenToClient(rcWnd);
	this->InvalidateRect(rcWnd,TRUE);
}
void CClipImageDlg::OnLvnEndlabeleditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = TRUE;   //TRUE值表示可以修改主项，FALSE值表示不修改主项
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
		AfxMessageBox("请选择裁剪方案！");
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

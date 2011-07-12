/************************************************************************
文 件 名：QQFaceDlgDlg.cpp
创建日期：4/11/2009
创 建 者：zhuxingxing
说    明：
************************************************************************/

#include "stdafx.h"
#include "QQFaceDlg.h"
#include "QQFaceDlgDlg.h"

/************************************************************************
注释掉以下三句，否则编译出错，原因：new操作符与gdi中的new操作符有冲突！

 #ifdef _DEBUG
 #define new DEBUG_NEW
 #endif
************************************************************************/

//必须包含gdiplus.lib库
#pragma  comment (lib ,"gdiplus.lib")



CQQFaceDlgDlg::CQQFaceDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQFaceDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQFaceDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picPreview);
}

BEGIN_MESSAGE_MAP(CQQFaceDlgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CQQFaceDlgDlg 消息处理程序
BOOL CQQFaceDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	

	GpInput.GdiplusVersion=1;
	if (GdiplusStartup(&gdip_Token,&GpInput,0)!=0) 
	{
		MessageBox(_T("加载GDI+失败！"));
		return TRUE;
	}
		
	if (GdipCreateFromHDC(::GetDC(this->m_hWnd),&gdip_Graphics)!=0)
	{
		MessageBox(_T("GdipCreateFromHDC失败！"));
		GdiplusShutdown((ULONG_PTR)&gdip_Token);
		return TRUE;
	}
	char szPath[MAX_PATH];
	GetModuleFileName(0,szPath,MAX_PATH);
	strcpy(strrchr(szPath,'\\')+1,"chatemotion\\");
	CString strPath=szPath;
	m_strEmotionPath=strPath;
	GdipLoadImageFromFile ((strPath+"page1.bmp").AllocSysString(),&gdip_ImagePage[0]);
	GdipLoadImageFromFile ((strPath+"page2.bmp").AllocSysString(),&gdip_ImagePage[1]);
	GdipLoadImageFromFile ((strPath+"page3.bmp").AllocSysString(),&gdip_ImagePage[2]);

	GdipLoadImageFromFile ((strPath+"bg.bmp").AllocSysString(),&gdip_ImageBg);

	GdipLoadImageFromFile ((strPath+"tab1.bmp").AllocSysString(),&gdip_ImageTopTab[0]);
	GdipLoadImageFromFile ((strPath+"tab2.bmp").AllocSysString(),&gdip_ImageTopTab[1]);
	GdipLoadImageFromFile ((strPath+"tab3.bmp").AllocSysString(),&gdip_ImageTopTab[2]);

	GdipLoadImageFromFile ((strPath+"left.bmp").AllocSysString(),&gdip_ImageBorder[0]);
	GdipLoadImageFromFile ((strPath+"middle.bmp").AllocSysString(),&gdip_ImageBorder[1]);
	GdipLoadImageFromFile ((strPath+"right.bmp").AllocSysString(),&gdip_ImageBorder[2]);

	for (int i=0;i<3;i++)
	{
		GdipGetImageWidth(gdip_ImageTopTab[i],&nTopTabWidth[i]);
		GdipGetImageHeight(gdip_ImageTopTab[i],&nTopTabHeight[i]);
	}
	

	GdipGetImageWidth(gdip_ImageBorder[0],&nBorderWidth);
	GdipGetImageHeight(gdip_ImageBorder[0],&nBorderHeight);

	GdipGetImageWidth(gdip_ImagePage[0],&nPageWidth);
	GdipGetImageHeight(gdip_ImagePage[0],&nPageHeight);

	GdipGetImageHeight(gdip_ImageBg,&nBgHeight);

	CRect rect;
	GetWindowRect(&rect);
	rect.right=rect.left+nPageWidth+2*nBorderWidth;
	rect.bottom=rect.top+nBgHeight;//nPageHeight+nTopTabHeight;
	MoveWindow(&rect,TRUE);

	nSelectedTab=1;	//默认显示第一个页面，即“默认”页面
	m_imgGif=NULL;
	m_nCurGif=0;

	m_rtTabRect[0].left=nBorderWidth;
	m_rtTabRect[0].top=0;
	m_rtTabRect[0].right=m_rtTabRect[0].left+nTopTabWidth[0]/3;
	m_rtTabRect[0].bottom=m_rtTabRect[0].top+nTopTabHeight[0];

	m_rtTabRect[1].left=nBorderWidth+nTopTabWidth[0]/3;
	m_rtTabRect[1].top=0;
	m_rtTabRect[1].right=m_rtTabRect[1].left+nTopTabWidth[1]/3;
	m_rtTabRect[1].bottom=m_rtTabRect[1].top+nTopTabHeight[1];

	m_rtTabRect[2].left=nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3;
	m_rtTabRect[2].top=0;
	m_rtTabRect[2].right=m_rtTabRect[2].left+nTopTabWidth[2]/3;
	m_rtTabRect[2].bottom=m_rtTabRect[2].top+nTopTabHeight[2];

	m_rtPageRect.left=nBorderWidth;
	m_rtPageRect.top=nTopTabHeight[0];
	m_rtPageRect.right=nPageWidth+nBorderWidth;
	m_rtPageRect.bottom=nPageHeight+nTopTabHeight[0];
	m_rtPageRect.DeflateRect(3,3);

	m_picPreview.ptLeft.x=nBorderWidth+3;
	m_picPreview.ptLeft.y=nTopTabHeight[0]+3;
	m_picPreview.ptRight.x=nPageWidth+nBorderWidth-56;
	m_picPreview.ptRight.y=nTopTabHeight[0]+3;
	m_picPreview.nWidth=m_picPreview.nHeight=53;

	GetWindowRect(&rect);
	CPoint pt;
	GetCursorPos(&pt);
	rect.OffsetRect(pt);
	MoveWindow(&rect);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQQFaceDlgDlg::OnPaint()
{
	CRect rect;
	GetWindowRect(&rect);	

	//画背景
	GdipDrawImageRect(gdip_Graphics,gdip_ImageBg,0,0,rect.Width()*2,rect.Height());

	//画左右边框
	GdipDrawImageRect(gdip_Graphics,gdip_ImageBorder[0],0,0,nBorderWidth,nBorderHeight);
	GdipDrawImageRect(gdip_Graphics,gdip_ImageBorder[1],nPageWidth+nBorderWidth,0,nBorderWidth,nBorderHeight);

	//画顶部的Tab按钮
	if (nSelectedTab==1)//默认页面
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			2*nTopTabWidth[0]/3,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			0,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			0,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (nSelectedTab==2)//拇指人页面
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			0,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			2*nTopTabWidth[1]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			0,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (nSelectedTab==3)//超级蓝钻页面
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			0,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			0,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			2*nTopTabWidth[2]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}

	//画表情页
	GdipDrawImageRect(gdip_Graphics,gdip_ImagePage[nSelectedTab-1],nBorderWidth,nTopTabHeight[0],nPageWidth,nPageHeight);
	
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CQQFaceDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQQFaceDlgDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i=0;i<3;i++)
	{
		GdipDisposeImage(gdip_ImagePage[i]);
		GdipDisposeImage(gdip_ImageTopTab[i]);
		GdipDisposeImage(gdip_ImageBorder[i]);
	}
	GdipDisposeImage(gdip_ImageBg);

	GdipDeleteGraphics(gdip_Graphics);
	GdiplusShutdown((ULONG_PTR)&gdip_Token);
	if (m_imgGif)
	{
		//delete m_imgGif;
		m_imgGif=NULL;
	}
}

void CQQFaceDlgDlg::OnClose()
{
	CDialog::OnClose();
}

void CQQFaceDlgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT pt;
	pt.x=point.x;
	pt.y=point.y;

	if (PtInRect(&m_rtPageRect,point))
	{	
		int nRow;
		int nCol;
// 		rect.right=rect.left+26;
// 		rect.bottom=rect.top+26;
// 		CClientDC dc(this);
// 		dc.Rectangle(&rect);

		nRow=(pt.y-m_rtPageRect.top)/26;
		nCol=(pt.x-m_rtPageRect.left)/26;
		int nGifIndex=9*nRow+nCol;
		if (nSelectedTab==2)
		{
			nGifIndex+=54;
			if (nGifIndex>=72)
			{
				m_picPreview.ShowWindow(SW_HIDE);
				return;
			}
		}
		else if (nSelectedTab==3)
		{
			nGifIndex+=72;
		}
		

		if (nGifIndex!=m_nCurGif/* || !m_picPreview.IsWindowVisible()*/)
		{
			m_nCurGif=nGifIndex;
			
			CString strText;
			strText.Format(_T("%d.gif"),nGifIndex);
			//m_picPreview.SetWindowText(strText);

			// GDI+
			if (m_imgGif)
			{
				delete m_imgGif;
				m_imgGif=NULL;
				//m_picPreview.Invalidate(TRUE);
			}
			CString strTemp;
			strTemp=m_strEmotionPath+strText;
			
			m_imgGif =new ImageEx(strTemp.AllocSysString());
			CRect rc;
			m_picPreview.GetClientRect(rc);

			int cx = (rc.Width() - m_imgGif->GetWidth()) / 2;
			int cy = (rc.Height() - m_imgGif->GetHeight()) / 2;
			m_imgGif->InitAnimation(m_picPreview.m_hWnd, CPoint(cx,cy));
			if (!m_picPreview.IsWindowVisible())
			{
				m_picPreview.ShowWindow(SW_SHOW);
			}
			
		}
		return;
		
	}
	else
	{
		m_picPreview.ShowWindow(SW_HIDE);
	}

	if (PtInRect(&m_rtTabRect[0],pt)&&nSelectedTab!=1)//默认页面
	{	
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			nTopTabWidth[0]/3,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (PtInRect(&m_rtTabRect[1],pt)&&nSelectedTab!=2)//拇指人页面
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			nTopTabWidth[1]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (PtInRect(&m_rtTabRect[2],pt)&&nSelectedTab!=3)//超级蓝钻页面
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			nTopTabWidth[2]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}

	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CQQFaceDlgDlg::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{	
	return 0;
}


void CQQFaceDlgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	POINT pt;
	pt.x=point.x;
	pt.y=point.y;

	if (PtInRect(&m_rtTabRect[0],pt))//默认页面
	{
		nSelectedTab=1;
		this->OnPaint();
	}
	else if (PtInRect(&m_rtTabRect[1],pt))//拇指人页面
	{
		nSelectedTab=2;
		this->OnPaint();
	}
	else if (PtInRect(&m_rtTabRect[2],pt))//超级蓝钻页面
	{
		nSelectedTab=3;
		this->OnPaint();
	}

	//CDialog::OnLButtonDown(nFlags, point);
}

void CQQFaceDlgDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
}

void CQQFaceDlgDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);
}

BOOL CQQFaceDlgDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//return CDialog::OnEraseBkgnd(pDC);
}
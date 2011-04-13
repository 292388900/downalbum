/************************************************************************
�� �� ����QQFaceDlgDlg.cpp
�������ڣ�4/11/2009
�� �� �ߣ�zhuxingxing
˵    ����
************************************************************************/

#include "stdafx.h"
#include "QQFaceDlg.h"
#include "QQFaceDlgDlg.h"

/************************************************************************
ע�͵��������䣬����������ԭ��new��������gdi�е�new�������г�ͻ��

 #ifdef _DEBUG
 #define new DEBUG_NEW
 #endif
************************************************************************/

//�������gdiplus.lib��
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


// CQQFaceDlgDlg ��Ϣ�������
BOOL CQQFaceDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	

	GpInput.GdiplusVersion=1;
	if (GdiplusStartup(&gdip_Token,&GpInput,0)!=0) 
	{
		MessageBox(_T("����GDI+ʧ�ܣ�"));
		return TRUE;
	}
		
	if (GdipCreateFromHDC(::GetDC(this->m_hWnd),&gdip_Graphics)!=0)
	{
		MessageBox(_T("GdipCreateFromHDCʧ�ܣ�"));
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

	nSelectedTab=1;	//Ĭ����ʾ��һ��ҳ�棬����Ĭ�ϡ�ҳ��
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQQFaceDlgDlg::OnPaint()
{
	CRect rect;
	GetWindowRect(&rect);	

	//������
	GdipDrawImageRect(gdip_Graphics,gdip_ImageBg,0,0,rect.Width()*2,rect.Height());

	//�����ұ߿�
	GdipDrawImageRect(gdip_Graphics,gdip_ImageBorder[0],0,0,nBorderWidth,nBorderHeight);
	GdipDrawImageRect(gdip_Graphics,gdip_ImageBorder[1],nPageWidth+nBorderWidth,0,nBorderWidth,nBorderHeight);

	//��������Tab��ť
	if (nSelectedTab==1)//Ĭ��ҳ��
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			2*nTopTabWidth[0]/3,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			0,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			0,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (nSelectedTab==2)//Ĵָ��ҳ��
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			0,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			2*nTopTabWidth[1]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			0,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (nSelectedTab==3)//��������ҳ��
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			0,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			0,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[2],nBorderWidth+nTopTabWidth[0]/3+nTopTabWidth[1]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],
			2*nTopTabWidth[2]/3,0,nTopTabWidth[2]/3,nTopTabHeight[2],GpUnit::UnitPixel,NULL,NULL,NULL);
	}

	//������ҳ
	GdipDrawImageRect(gdip_Graphics,gdip_ImagePage[nSelectedTab-1],nBorderWidth,nTopTabHeight[0],nPageWidth,nPageHeight);
	
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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

	if (PtInRect(&m_rtTabRect[0],pt)&&nSelectedTab!=1)//Ĭ��ҳ��
	{	
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[0],nBorderWidth,0,nTopTabWidth[0]/3,nTopTabHeight[0],
			nTopTabWidth[0]/3,0,nTopTabWidth[0]/3,nTopTabHeight[0],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (PtInRect(&m_rtTabRect[1],pt)&&nSelectedTab!=2)//Ĵָ��ҳ��
	{
		GdipDrawImageRectRectI(gdip_Graphics,gdip_ImageTopTab[1],nBorderWidth+nTopTabWidth[0]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],
			nTopTabWidth[1]/3,0,nTopTabWidth[1]/3,nTopTabHeight[1],GpUnit::UnitPixel,NULL,NULL,NULL);
	}
	else if (PtInRect(&m_rtTabRect[2],pt)&&nSelectedTab!=3)//��������ҳ��
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

	if (PtInRect(&m_rtTabRect[0],pt))//Ĭ��ҳ��
	{
		nSelectedTab=1;
		this->OnPaint();
	}
	else if (PtInRect(&m_rtTabRect[1],pt))//Ĵָ��ҳ��
	{
		nSelectedTab=2;
		this->OnPaint();
	}
	else if (PtInRect(&m_rtTabRect[2],pt))//��������ҳ��
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
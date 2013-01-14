
#include "stdafx.h"
#include "WorkareaStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CWorkareaStatic

CWorkareaStatic::CWorkareaStatic()
{
}

CWorkareaStatic::~CWorkareaStatic()
{
}

void CWorkareaStatic::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    //::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    __super::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CWorkareaStatic, CStatic)
    //{{AFX_MSG_MAP(CWorkareaStatic)
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkareaStatic message handlers


void CWorkareaStatic::OnPaint()
{
	CPaintDC dc(this);
	CRect rcRect;
	GetClientRect(rcRect);

	Draw(dc,rcRect);
}

void CWorkareaStatic::Draw(CDC&dc,const CRect&rcRect)
{
	const int nWidth = 15;	//方格宽度
	const COLORREF color1 = RGB(255,255,255);
	const COLORREF color2 = RGB(200,200,200);

	//先创建一个四方格的画布
	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc,nWidth*2,nWidth*2);
	dcMem.SelectObject(&bmp);

	//间隔性地画四个方格
	dcMem.FillSolidRect(0,0,nWidth,nWidth,color1);
	dcMem.FillSolidRect(nWidth,0,nWidth,nWidth,color2);
	dcMem.FillSolidRect(0,nWidth,nWidth,nWidth,color2);
	dcMem.FillSolidRect(nWidth,nWidth,nWidth,nWidth,color1);

	//铺开
	CBrush brush;
	brush.CreatePatternBrush(&bmp); ///创建位图画刷
	dc.FillRect(rcRect,&brush);

	//清除dc
	dcMem.DeleteDC();
}
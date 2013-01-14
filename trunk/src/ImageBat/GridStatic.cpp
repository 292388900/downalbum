
#include "stdafx.h"
#include "GridStatic.h"
#include "ImageInfo.h"


/////////////////////////////////////////////////////////////////////////////
// CGridStatic

CGridStatic::CGridStatic()
{
	m_nLastAlignment = m_nAlignment = PROCESS_PARAM_ADDWATERMARKER::eAlignment_custom;
}

CGridStatic::~CGridStatic()
{
}

void CGridStatic::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    __super::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CGridStatic, CStatic)
    //{{AFX_MSG_MAP(CGridStatic)
    ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridStatic message handlers


void CGridStatic::OnPaint()
{
	const int nRows = 3;	//三横三竖
	const int nSpace = 2;	//间距
	int nPosx = 0;
	int nPosy = 0;
	int nWidth = 0;
	int nHeight = 0;

	CPaintDC dc(this);
	CRect rcRect;
	GetClientRect(rcRect);

	nWidth = ( rcRect.Width() - (nRows+1)*nSpace ) / nRows;
	nHeight = ( rcRect.Height() - (nRows+1)*nSpace ) / nRows;

	//绘制灰色背景
	dc.FillSolidRect(rcRect,RGB(192,192,192));

	//绘制小方块
	CBrush brush(RGB(255,251,240));
	dc.SelectObject(&brush);

	for ( int i=0; i<nRows; ++i ){
		nPosx = (i+1) * nSpace + i * nWidth;
		for ( int j=0; j<nRows; ++j ){
			nPosy = (j+1) * nSpace + j * nHeight;
			dc.FillSolidRect(nPosx,nPosy,nWidth,nHeight,RGB(255,251,240));
		}
	}

	//命中的方块
	if ( m_nAlignment!=PROCESS_PARAM_ADDWATERMARKER::eAlignment_custom ){
		int i = (m_nAlignment % nRows);
		int j = (m_nAlignment / nRows);

		nPosx = (i+1) * nSpace + i * nWidth;
		nPosy = (j+1) * nSpace + j * nHeight;
		dc.FillSolidRect(nPosx,nPosy,nWidth,nHeight,RGB(0,0,0));
	}
}

void CGridStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	const int nRows = 3;	//三横三竖
	const int nSpace = 2;	//间距

	int nWidth = 0;
	int nHeight = 0;

	CRect rcRect;
	GetClientRect(rcRect);

	nWidth = ( rcRect.Width() - (nRows+1)*nSpace ) / nRows;
	nHeight = ( rcRect.Height() - (nRows+1)*nSpace ) / nRows;

	int i = point.x / (nSpace+nWidth);
	int j = point.y / (nSpace+nHeight);

	if ( i>=nRows ){
		i = nRows - 1;
	}
	if ( j>=nRows ){
		j = nRows - 1;
	}

	SetAlignment( i+ j * nRows );

	__super::OnLButtonDown(nFlags,point);
}

void CGridStatic::SetAlignment(int nAlignment)
{
	m_nAlignment = nAlignment;
	if ( m_nLastAlignment!=m_nAlignment ){
		Invalidate();
		m_nLastAlignment = m_nAlignment;
	}
}

int CGridStatic::GetAlignment()
{
	return m_nAlignment;
}
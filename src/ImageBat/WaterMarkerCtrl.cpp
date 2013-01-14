#include "stdafx.h"
#include "WaterMarkerCtrl.h"
#include <math.h>
#include "FormatTagString.h"
#include "ImageInfo.h"

#define M_PI       3.14159265358979323846







//IMPLEMENT_DYNAMIC(CWaterMarkerCtrl, CWnd)

BEGIN_MESSAGE_MAP(CWaterMarkerCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


//串行化需要一个默认构造函数
CWaterMarkerCtrl::CWaterMarkerCtrl()
{
	m_pParentWnd = NULL;
	m_bDestroyed = FALSE;

	m_nGradient = 0;
	m_bShadow = TRUE;
	m_bTransparent = FALSE;
	m_nTransparentDegree = 0;

	m_clrText = RGB(0,0,0);

	m_pWaterMarkerImg = NULL;

	m_ptInBounds.x = m_ptInBounds.y = 0;
	m_nZOrderInBounds = 0;

	//设置字体格式
	memset(&m_lffont, 0, sizeof(LOGFONT));
	m_lffont.lfHeight=-15;
	m_lffont.lfWeight=FW_BOLD;
	m_lffont.lfClipPrecision=CLIP_LH_ANGLES;
	lstrcpy (m_lffont.lfFaceName, _T("Arial"));

	memset(m_nReserved,0,sizeof(m_nReserved));
}

void CWaterMarkerCtrl::Create(CWnd *pParent, BOOL bVisible/* = TRUE*/, LPCRECT pRect/* = NULL*/)
{
	m_pParentWnd = pParent;

	if ( m_rectTracker.m_rect.IsRectEmpty() ){
		m_rectTracker.m_rect.CopyRect(pRect);
	}
	__super::Create( _T("STATIC"), NULL , WS_CHILD /*| WS_VISIBLE*/ | SS_CENTERIMAGE|SS_CENTER, m_rectTracker.m_rect, pParent, 0 );
	if ( bVisible==TRUE ){
		ShowWindow(SW_SHOW);
	}

	Initialize();
}

CWaterMarkerCtrl::~CWaterMarkerCtrl()
{
	Destroy();
}

void CWaterMarkerCtrl::Destroy()
{
	if ( m_bDestroyed==FALSE ){
		::DestroyWindow(m_hWnd);

		if ( m_pWaterMarkerImg ){
			delete m_pWaterMarkerImg;
		}

		m_bDestroyed = TRUE;
	}
}


// 串行化宏实现
IMPLEMENT_SERIAL(CWaterMarkerCtrl,CWnd,0);

void CWaterMarkerCtrl::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
		//写

		//位置
		LPRECT lpRect = &m_rectTracker.m_rect;
		ar << lpRect->left;
		ar << lpRect->top;
		ar << lpRect->right;
		ar << lpRect->bottom;

		ar << m_strText;
		ar << m_strImageFilePath;

		ar << m_nGradient;

		ar << m_bShadow;
		ar << m_bTransparent;
		ar << m_nTransparentDegree;
		ar << m_clrText;

		ar << m_ptInBounds;
		ar << m_nZOrderInBounds;

		ar.Write( &m_lffont,sizeof(m_lffont));

		ar.Write(m_nReserved,sizeof(m_nReserved));
		for ( int i=0; i<sizeof(m_strReserved)/sizeof(CString); ++i ){
			ar << m_strReserved[i];
		}

	}else{
		//读

		//位置
		LPRECT lpRect = &m_rectTracker.m_rect;
		ar >> lpRect->left;
		ar >> lpRect->top;
		ar >> lpRect->right;
		ar >> lpRect->bottom;

		ar >> m_strText;
		ar >> m_strImageFilePath;

		ar >> m_nGradient;

		ar >> m_bShadow;
		ar >> m_bTransparent;
		ar >> m_nTransparentDegree;
		ar >> m_clrText;

		ar >> m_ptInBounds;
		ar >> m_nZOrderInBounds;

		ar.Read( &m_lffont,sizeof(m_lffont) );

		ar.Read( m_nReserved,sizeof(m_nReserved) );
		for ( int i=0; i<sizeof(m_strReserved)/sizeof(CString); ++i ){
			ar >> m_strReserved[i];
		}
	}
}

void CWaterMarkerCtrl::SetText(const CString& strText)
{
	m_strText = strText;
}
CString CWaterMarkerCtrl::GetText()
{
	return m_strText;
}

void CWaterMarkerCtrl::SetImageFilePath(const CString& strImageFilePath)
{
	m_strImageFilePath = strImageFilePath;
	LoadImageFile();
}

void CWaterMarkerCtrl::LoadImageFile()
{
	if ( m_strImageFilePath.IsEmpty()==FALSE ){
		CStringW strFilePathW;
		strFilePathW = m_strImageFilePath;

		if ( m_pWaterMarkerImg ){
			delete m_pWaterMarkerImg;
		}

		m_pWaterMarkerImg = new Image(strFilePathW);
	}
}

void CWaterMarkerCtrl::AjustToImageSize()
{
	if ( m_pWaterMarkerImg ){
		m_rectTracker.m_rect.right = m_rectTracker.m_rect.left + m_pWaterMarkerImg->GetWidth();
		m_rectTracker.m_rect.bottom = m_rectTracker.m_rect.top + m_pWaterMarkerImg->GetHeight();
		MoveWindow(&m_rectTracker.m_rect);
	}
}

CString CWaterMarkerCtrl::GetImageFilePath()
{
	return m_strImageFilePath;
}

void CWaterMarkerCtrl::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
}

BOOL CWaterMarkerCtrl::GetSelected()
{
	return m_bSelected;
}

void CWaterMarkerCtrl::SetGradientAngel(int nAngel)
{
	m_nGradient = nAngel;
}

int CWaterMarkerCtrl::GetGradientAngel()
{
	return m_nGradient;
}

//阴影效果操作
void CWaterMarkerCtrl::SetShadow(BOOL bShadow)
{
	m_bShadow = bShadow;
}

BOOL CWaterMarkerCtrl::GetShadow()
{
	return m_bShadow;
}

//透明效果操作
void CWaterMarkerCtrl::SetTransparent(BOOL bTransparent)
{
	m_bTransparent = bTransparent;
}
BOOL CWaterMarkerCtrl::GetTransparent()
{
	return m_bTransparent;
}
void CWaterMarkerCtrl::SetTransparentDegree(int nTransparentDegree)
{
	m_nTransparentDegree = nTransparentDegree;
}
int CWaterMarkerCtrl::GetTransparentDegree()
{
	return m_nTransparentDegree;
}

//颜色操作
void CWaterMarkerCtrl::SetTextColor(COLORREF clr)
{
	m_clrText = clr;
}
COLORREF CWaterMarkerCtrl::GetTextColor()
{
	return m_clrText;
}

//字体操作
void CWaterMarkerCtrl::SetTextFont(LOGFONT *pLogFont)
{
	if ( pLogFont ){
		memcpy(&m_lffont,pLogFont,sizeof(m_lffont));
	}
}
LOGFONT CWaterMarkerCtrl::GetTextFont()
{
	return m_lffont;
}

CWnd *CWaterMarkerCtrl::GetParentWnd()
{
	return m_pParentWnd;
}

UINT CWaterMarkerCtrl::GetHandleMask() const
{
	return ( 255 );
}

void CWaterMarkerCtrl::Initialize()
{
	m_bSelected = FALSE;
	m_rectTracker.m_nHandleSize = 5;
	m_rectTracker.m_nStyle = CRectTracker::resizeOutside | CRectTracker::solidLine;

	LoadImageFile();
}

void CWaterMarkerCtrl::SetRect( int x1, int y1, int x2, int y2 )
{ 
	m_rectTracker.m_rect.SetRect( x1, y1, x2, y2 ); 
	MoveWindow( & m_rectTracker.m_rect );
}

void CWaterMarkerCtrl::SetRect( const CRect & rect )
{
	m_rectTracker.m_rect.SetRect( rect.left , rect.top , rect.right , rect.bottom ); 
	MoveWindow( & m_rectTracker.m_rect );
}

CRect& CWaterMarkerCtrl::GetRect()
{
	return m_rectTracker.m_rect;
}

void CWaterMarkerCtrl::OffsetWindow(int x,int y)
{
	m_rectTracker.m_rect.OffsetRect(x,y);
	MoveWindow(&m_rectTracker.m_rect);
}

//刷新自身所在父窗口的区域
void CWaterMarkerCtrl::Update()
{
	Invalidate();
	if ( m_pParentWnd ){
		m_pParentWnd->InvalidateRect(m_rectTracker.m_rect);
	}
}

void CWaterMarkerCtrl::Track( CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo )
{
	m_rectTracker.Track( pWnd, point, bAllowInvert, pWndClipTo );
				
	MoveWindow(&m_rectTracker.m_rect);
}

//将效果绘制到指定dc上
void CWaterMarkerCtrl::Draw(HDC hdc, const CRect&rcRect, PVOID pParam/* = NULL*/)
{
	Graphics g(hdc);

	//绘制图片
	if ( m_pWaterMarkerImg!=NULL ){
		g.DrawImage(m_pWaterMarkerImg, rcRect.left + (rcRect.Width() - m_pWaterMarkerImg->GetWidth())/2, 
			rcRect.top + (rcRect.Height() - m_pWaterMarkerImg->GetHeight())/2, 
			0, 0, m_pWaterMarkerImg->GetWidth(), m_pWaterMarkerImg->GetHeight(), UnitPixel);
	}

	//绘制文字
	if ( m_strText.IsEmpty()==FALSE ){

		CStringW strText;
		if ( pParam!=NULL ){
			CFormatTagString tagFormatter;
			tagFormatter.Init(0,m_strText);
			strText = tagFormatter.GetFormaString((ITEMINFO *)pParam);
		}else{
			strText = m_strText;
		}

		// Initialize arguments.
		CStringW strFontNameW;
		strFontNameW = m_lffont.lfFaceName;
		int fontstyle = FontStyleRegular;
		if ( m_lffont.lfWeight==FW_BOLD ){
			fontstyle |= FontStyleBold;
		}
		if ( m_lffont.lfItalic ){
			fontstyle |= FontStyleItalic;
		}
		if ( m_lffont.lfUnderline ){
			fontstyle |= FontStyleUnderline;
		}
		if ( m_lffont.lfStrikeOut ){
			fontstyle |= FontStyleStrikeout;
		}
		Font font(strFontNameW, -(Gdiplus::REAL)m_lffont.lfHeight, fontstyle);
		StringFormat format;
		//format.SetAlignment(StringAlignmentCenter);
		//format.SetLineAlignment(StringAlignmentCenter);
		format.SetAlignment(StringAlignmentNear);
		format.SetLineAlignment(StringAlignmentNear);
		Color clrText;		
		Color clrShadow = Color::Gray;
		clrText.SetFromCOLORREF(m_clrText);
		if ( m_bTransparent ){
			clrText = clrText.GetValue() & 0x00FFFFFF;
			clrShadow = clrShadow.GetValue() & 0x00FFFFFF;
			DWORD Alpha = (m_nTransparentDegree*0xFF/100)<<Color::AlphaShift;
			clrText.SetValue(clrText.GetValue()|Alpha);
			clrShadow.SetValue(clrShadow.GetValue()|Alpha);
		}
		Gdiplus::SolidBrush brushText(clrText);
		Gdiplus::SolidBrush brushShadow(clrShadow);

		PointF point(0,0);
		RectF boundRect;
		g.MeasureString(strText, strText.GetLength(), &font, point, &boundRect);

		double fhudu = m_nGradient*M_PI/180;
		double nNewWidth = ceil( boundRect.Width * cos(fhudu) );
		double nNewHeight = ceil( boundRect.Width * sin(fhudu) );
		double nDeltaWidth = ceil( boundRect.Height * sin(fhudu) );
		double nDeltaHeight = ceil( boundRect.Height * cos(fhudu) );

		//居中显示
		g.TranslateTransform((Gdiplus::REAL)rcRect.left + (Gdiplus::REAL)(rcRect.Width()-nNewWidth-nDeltaWidth)/2, 
			(Gdiplus::REAL)rcRect.top + (Gdiplus::REAL)(rcRect.Height()+nNewHeight-nDeltaHeight)/2);

		//旋转
		g.RotateTransform(-(Gdiplus::REAL)m_nGradient);

		//绘制
		if ( m_bShadow ){
			point.X += 0.5;
			point.Y += 0.5;
			g.DrawString(strText, strText.GetLength(), &font, point, &format, &brushShadow);
			point.X -= 0.5;
			point.Y -= 0.5;
		}
		g.DrawString(strText, strText.GetLength(), &font, point, &format, &brushText);

		g.ResetTransform();
	}

#if 0
	if ( m_strText.IsEmpty()==FALSE ){
		CFont font;

		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(m_clrText);

		m_lffont.lfHeight = MulDiv(14, -dc.GetDeviceCaps(LOGPIXELSY), 72);

		//设定新的旋转角度
		m_lffont.lfEscapement = m_nGradient*10;

		//创建字体并选进设备场景
		font.CreateFontIndirect(&m_lffont);
		CFont* pOldFont=dc.SelectObject(&font);

		CSize fsize = dc.GetTextExtent(m_strText);

		double fhudu = m_nGradient*M_PI/180;
		int nNewWidth = (int)ceil( fsize.cx * cos(fhudu) );
		int nNewHeight = (int)ceil( fsize.cx * sin(fhudu) );
		int nDeltaWidth = (int)ceil( fsize.cy * sin(fhudu) );
		int nDeltaHeight = (int)ceil( fsize.cy * cos(fhudu) );

		dc.TextOut( (rcRect.right-nNewWidth-nDeltaWidth)/2, (rcRect.bottom+nNewHeight-nDeltaHeight)/2, m_strText );

		//分行多行显示
		//int height = dc.DrawText(m_strText,rcRect,DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
		//rcRect.DeflateRect(0,(rcRect.Height() - height) / 2); // 改变rect
		//dc.DrawText(m_strText,rcRect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

		//原来字体
		dc.SelectObject(pOldFont);
		font.DeleteObject();
	}
#endif
}

void CWaterMarkerCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rcRect;
	GetClientRect(&rcRect);

	Draw(dc.m_hDC,rcRect);
}

void CWaterMarkerCtrl::DrawTracker( CDC* pDC ) const
{
	m_rectTracker.Draw(pDC);
}

int CWaterMarkerCtrl::HitTest(CPoint point) const
{
	return m_rectTracker.HitTest(point);
}

BOOL CWaterMarkerCtrl::SetCursor(CWnd* pWnd, UINT nHitTest) const
{
	return m_rectTracker.SetCursor(pWnd,nHitTest);
}


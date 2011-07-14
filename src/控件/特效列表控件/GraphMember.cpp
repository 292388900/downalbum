// GraphMember.cpp: implementation of the CTaskItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Clb.h"
#include "GraphMember.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTaskFrame Class
//////////////////////////////////////////////////////////////////////

CTaskFrame::CTaskFrame(const CString& title) 
			: m_rSize(CRect(0, 0, 0, 0)), m_strTitle(title)
{
	m_FrmColor = RGB(203, 203, 203);
	m_BKColor  = RGB(255, 255, 255);

	LOGFONT logFont;
	logFont.lfHeight         = 12;
	logFont.lfEscapement     = 0;
	logFont.lfOrientation    = 0;
	logFont.lfWeight	     = FW_BOLD;
	logFont.lfItalic         = FALSE;
	logFont.lfUnderline	     = FALSE;
	logFont.lfStrikeOut	     = FALSE;
	logFont.lfOutPrecision   = OUT_STROKE_PRECIS;
	logFont.lfClipPrecision  = CLIP_STROKE_PRECIS;
	logFont.lfQuality		 = DEFAULT_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH|FF_MODERN;
	logFont.lfCharSet	     = GB2312_CHARSET;
	strcpy(logFont.lfFaceName, "ËÎÌו");
	m_Font.CreateFontIndirect(&logFont);

	m_nTitleHeight  = 28;
	m_nLeftMargin   = 10;
	m_nRightMargin  = 10;
	m_nTopMargin    = 4;
	m_nBottomMargin = 5;
}

CTaskFrame::~CTaskFrame()
{
}

CRect CTaskFrame::GetWorkRect() const
{
	CRect rect(m_rSize);
	rect.top     = m_nTopMargin + m_nTitleHeight;
	rect.left    = m_nLeftMargin;
	rect.right  -= m_nRightMargin;
	rect.bottom -= m_nBottomMargin;

	return rect;
}

void CTaskFrame::Draw(CDC* pDC)
{
	CRect rect(m_rSize);

	CBrush brush;
	brush.CreateSolidBrush(m_BKColor);
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();

	rect.bottom = m_nTitleHeight;
	brush.CreateSolidBrush(m_FrmColor);
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();

	LOGBRUSH logBrush = { BS_NULL, NULL, NULL };
	brush.CreateBrushIndirect(&logBrush);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_FrmColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&m_rSize);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	CFont* pOldFont = pDC->SelectObject(&m_Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(6, (m_nTitleHeight - 12) / 2, m_strTitle.Left(14));
	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(pOldFont);
}


//////////////////////////////////////////////////////////////////////
// CIconBtn Class
//////////////////////////////////////////////////////////////////////

CIconBtn::CIconBtn()
		 : m_rSize(CRect(0, 0, 0, 0)), m_bFocusedState(false)
{
}

CIconBtn::~CIconBtn()
{
}

void CIconBtn::SetFocusedState(bool bFocused)
{ 
	m_bFocusedState = bFocused;
	Draw();
}

//////////////////////////////////////////////////////////////////////
// CPuckerBtn Class
//////////////////////////////////////////////////////////////////////

CPuckerBtn::CPuckerBtn(CWnd* pWnd)
		   : m_pWnd(pWnd), m_bPressState(false)
{
}

CPuckerBtn::~CPuckerBtn()
{
}

void CPuckerBtn::Draw()
{
	if(m_pWnd == NULL)
		return;
	CDC* pDC = m_pWnd->GetDC();

	CBrush brush;
	brush.CreateSolidBrush(RGB(203, 203, 203));
	CRect rectFill(m_rSize);
	rectFill.bottom += 1;
	rectFill.right  += 1;
	pDC->FillRect(&rectFill, &brush);
	brush.DeleteObject();

	CRect rect(m_rSize);
	if(GetFocusedState())
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rect.left,  rect.top);
		pDC->LineTo(rect.right, rect.top);
		pDC->MoveTo(rect.left,  rect.top);
		pDC->LineTo(rect.left,  rect.bottom);
		pDC->SelectObject(pOldPen);

		pDC->MoveTo(rect.left,  rect.bottom); 
		pDC->LineTo(rect.right, rect.bottom);
		pDC->MoveTo(rect.right, rect.top);	  
		pDC->LineTo(rect.right, rect.bottom);
	}

	CPoint point = rect.CenterPoint();
	if(!m_bPressState)
	{
		pDC->MoveTo(point.x,     point.y - 1);
		pDC->LineTo(point.x + 4, point.y - 5);
		pDC->MoveTo(point.x,     point.y - 1);
		pDC->LineTo(point.x - 4, point.y - 5);
		pDC->MoveTo(point.x,	 point.y + 3);
		pDC->LineTo(point.x + 4, point.y - 1);
		pDC->MoveTo(point.x,	 point.y + 3);
		pDC->LineTo(point.x - 4, point.y - 1);
	}
	else
	{
		pDC->MoveTo(point.x,	 point.y - 4);
		pDC->LineTo(point.x + 4, point.y);
		pDC->MoveTo(point.x,	 point.y - 4);
		pDC->LineTo(point.x - 4, point.y);
		pDC->MoveTo(point.x,	 point.y);
		pDC->LineTo(point.x + 4, point.y + 4);
		pDC->MoveTo(point.x,	 point.y);
		pDC->LineTo(point.x - 4, point.y + 4);
	}
}

void CPuckerBtn::Click()
{	
	m_bPressState = !m_bPressState;

	if(m_pWnd == NULL)
		return;

	Draw();

	if(m_bPressState)
	{
		if(m_pWnd == NULL) 
			return;
		m_pWnd->GetWindowRect(&m_wndRect);
		m_pWnd->GetParent()->ScreenToClient(&m_wndRect);
			
		CRect rect(m_wndRect);
		rect.bottom = rect.top + 28;
		DWORD dwStyle = m_pWnd->GetExStyle();
		DWORD dwNewStyle = WS_EX_DLGMODALFRAME|WS_EX_CLIENTEDGE;
		if((dwStyle & dwNewStyle) == dwNewStyle)
		{
			rect.bottom += 10;
		}
		else if((dwStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
		{
			rect.bottom += 4;
		}
		else if((dwStyle & WS_EX_DLGMODALFRAME) == WS_EX_DLGMODALFRAME)
		{
			rect.bottom += 6;
		}
		else if((dwStyle & WS_EX_STATICEDGE) == WS_EX_STATICEDGE)
		{
			rect.bottom += 2;
		}
		m_pWnd->MoveWindow(&rect);
	}
	else
		m_pWnd->MoveWindow(&m_wndRect);
}

/////////////////////////////////////////////////////////////////////
// CItem Class
//////////////////////////////////////////////////////////////////////
CItem::CItem()
	  : m_pWnd(NULL), m_nCtrlId(NULL)
{
}

CItem::CItem(CWnd* pWnd, UINT id)
	  : m_pWnd(pWnd), m_nCtrlId(id)	
{
}

CItem::~CItem()
{
}

void CItem::SetItem(const ItemInfo& item)
{
	if(item.type < II_ICONTEXT || item.type > II_COLOR)
	{
		m_ItemInfo.type = -1;
		return;
	}
	
	m_ItemInfo.type  = item.type;
	m_ItemInfo.index = item.index;
	m_ItemInfo.text  = item.text;
	m_ItemInfo.color = item.color;
	m_ItemInfo.icon  = CopyIcon(item.icon);
}
//////////////////////////////////////////////////////////////////////
// CTaskItem Class
//////////////////////////////////////////////////////////////////////

CTaskItem::CTaskItem(CWnd* pWnd, UINT id)
		   : CItem(pWnd, id) 	
{
	m_LogFont.lfHeight         = 12;
	m_LogFont.lfEscapement     = 0;
	m_LogFont.lfOrientation    = 0;
	m_LogFont.lfWeight	       = FW_NORMAL;
	m_LogFont.lfItalic         = FALSE;
	m_LogFont.lfUnderline	   = FALSE;
	m_LogFont.lfStrikeOut	   = FALSE;
	m_LogFont.lfOutPrecision   = OUT_STROKE_PRECIS;
	m_LogFont.lfClipPrecision  = CLIP_STROKE_PRECIS;
	m_LogFont.lfQuality		   = DEFAULT_QUALITY;
	m_LogFont.lfPitchAndFamily = VARIABLE_PITCH|FF_MODERN;
	m_LogFont.lfCharSet	       = GB2312_CHARSET;
	strcpy(m_LogFont.lfFaceName, "ËÎÌו");

	m_Font.CreateFontIndirect(&m_LogFont);
}

CTaskItem::CTaskItem(const CTaskItem& rhs)
{
	CItem::operator=(rhs);
	m_LogFont = rhs.m_LogFont;

	m_Font.CreateFontIndirect(&m_LogFont);
}

CItem* CTaskItem::Clone()
{
	return new CTaskItem(*this);
}

CTaskItem::~CTaskItem()
{
}

void CTaskItem::Draw()
{
	if(m_ItemInfo.type != II_ICONTEXT)
		return;
	
	if(m_pWnd == NULL)
		return;
	CDC* pDC = m_pWnd->GetDC();
	HDC  hdc = pDC->m_hDC;

	CRect rect(m_rSize);
	COLORREF cl = GetFocusedState() ? RGB(128, 255, 255) : RGB(255, 255, 255);
	CBrush brush;
	brush.CreateSolidBrush(cl);
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();

	DrawIconEx(hdc, m_rSize.left, m_rSize.top, m_ItemInfo.icon, 16, 16, NULL, NULL, DI_NORMAL);
	
	CFont* pOldFont = pDC->SelectObject(&m_Font);
	pDC->SetBkMode(TRANSPARENT);
	
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	int max_len = (rect.Width() - 16 - 5) / tm.tmAveCharWidth;

	pDC->TextOut(rect.left + 16 + 5, rect.top + 2, m_ItemInfo.text.Left(max_len));

	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(pOldFont);
}

void CTaskItem::Click()
{	
	if(m_ItemInfo.type == II_ICONTEXT)
		m_pWnd->GetParent()->SendMessage(WM_TASKCLICK, m_nCtrlId, m_ItemInfo.index);
}


//////////////////////////////////////////////////////////////////////
// CColorItem Class
//////////////////////////////////////////////////////////////////////

CColorItem::CColorItem(CWnd* pWnd, UINT id)
		   :CItem(pWnd, id)
{
}

CColorItem::CColorItem(const CColorItem& rhs)
{
	CItem::operator=(rhs);
}

CColorItem::~CColorItem()
{
}

CItem* CColorItem::Clone()
{
	return new CColorItem(*this);
}

void CColorItem::Draw()
{
	if(m_ItemInfo.type != II_COLOR)
		return;
	
	if(m_pWnd == NULL)
		return;
	CDC* pDC = m_pWnd->GetDC();

	CRect rect(m_rSize);
	rect.top    += 1;
	rect.left   += 1;
	rect.right  -= 1;
	rect.bottom -= 1;
	pDC->Rectangle(&m_rSize);

	CBrush brush;
	brush.CreateSolidBrush(m_ItemInfo.color);
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();
}

void CColorItem::Click()
{
	if(m_ItemInfo.type == II_COLOR)
		m_pWnd->GetParent()->SendMessage(WM_TASKCLICK, m_nCtrlId, (LONG)&m_ItemInfo.text);
}
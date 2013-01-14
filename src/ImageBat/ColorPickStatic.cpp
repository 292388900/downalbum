
#include "stdafx.h"
#include "ColorPickStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CColorPickStatic

CColorPickStatic::CColorPickStatic()
{
    m_crBackColor = RGB(0, 0, 0);
}

CColorPickStatic::~CColorPickStatic()
{
}

void CColorPickStatic::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    __super::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CColorPickStatic, CStatic)
    //{{AFX_MSG_MAP(CColorPickStatic)
    ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPickStatic message handlers

HBRUSH CColorPickStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetBkColor(m_crBackColor);
    // transparent text.
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CColorPickStatic::OnEraseBkgnd(CDC* pDC) 
{
    CRect rect;
    GetClientRect(rect);
    pDC->FillSolidRect(rect, m_crBackColor);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CColorPickStatic operations

void CColorPickStatic::SetColor(COLORREF clrBack)
{ 
	m_crBackColor = clrBack;

    if (::IsWindow(m_hWnd))
        Invalidate(); 
}

COLORREF CColorPickStatic::GetColor()
{
	return m_crBackColor;
}
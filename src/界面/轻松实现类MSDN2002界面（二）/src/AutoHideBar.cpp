/////////////////////////////////////////////////////////////////////////
//
// CAutoHideBar            Version 1.2
//
// Created: Mar 30, 2004
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 2004 by Cuick. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// Cuick@163.net
//
// Hint: These classes are intended to be used as base classes. Do not
// simply add your code to these file - instead create a new class
// derived from CDockPageBar classes and put there what you need. See 
// CTestBar classes in the demo projects for examples.
// Modify this file only to fix bugs, and don't forget to send me a copy.
/////////////////////////////////////////////////////////////////////////
// Acknowledgements:
//

#include "stdafx.h"
#include "AutoHideBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAHFloatWnd

CAHFloatWnd::CAHFloatWnd()
{
	m_font.CreatePointFont(85, "Tahoma");
	m_Title = "Test";
}

CAHFloatWnd::~CAHFloatWnd()
{
}


BEGIN_MESSAGE_MAP(CAHFloatWnd, CWnd)
	//{{AFX_MSG_MAP(CAHFloatWnd)
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_MOUSEACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONUP()
	ON_WM_TIMER()
	ON_WM_NCLBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAHFloatWnd message handlers

void CAHFloatWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	CRect rc(&lpncsp->rgrc[0]);
	rc.DeflateRect(0, 20, 0, 0);
	lpncsp->rgrc[0] = rc;
}

void CAHFloatWnd::OnNcPaint() 
{
	//CWnd::OnNcPaint();
    CWindowDC dc(this);

    CRect rcBar;
    GetWindowRect(rcBar);
	rcBar.OffsetRect (-rcBar.TopLeft ());
	CRect rect = rcBar;
	rect.right = ::GetSystemMetrics(SM_CXFRAME);
	dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE));
	rect = rcBar;
	rect.top = rect.bottom - ::GetSystemMetrics(SM_CXFRAME);
	dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE));
	rect = rcBar;
	rect.left = rect.right - ::GetSystemMetrics(SM_CXFRAME);
	dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE));

    rcBar.bottom = rcBar.top + 20 + ::GetSystemMetrics(SM_CXFRAME);
	dc.FillSolidRect(&rcBar, ::GetSysColor(COLOR_BTNFACE));
	
	CRect gripper = rcBar;
    CRect rcbtn = m_biHide.GetRect();


    gripper.DeflateRect(3, 4);
	gripper.right -= 2;
	gripper.bottom -= 0;

	CDC* pDC = &dc;
	HFONT oldFont = (HFONT)pDC->SelectObject (m_font);
	
	int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF crOldText;

	if(TRUE == m_isActive)			// active state
	{
		CBrush brush(RGB(10,36,106));
		pDC->FillRect(&gripper, &brush);

		crOldText = pDC->SetTextColor(RGB(255,255,255));
	}
	else
	{
		CPen pen(PS_SOLID, 1, RGB(128,128,128));
		
		HPEN poldPen = (HPEN)pDC->SelectObject (&pen);
		
		pDC->MoveTo (gripper.TopLeft());
		pDC->LineTo (gripper.right ,gripper.top );
		pDC->LineTo (gripper.BottomRight() );
		pDC->LineTo (gripper.left ,gripper.bottom );
		pDC->LineTo (gripper.TopLeft ());
		pDC->SelectObject (poldPen);

		crOldText = pDC->SetTextColor(RGB(0,0,0));
	}
	
	gripper.left += 4;
	gripper.top += 2;

	// draw caption
	if (!m_Title.IsEmpty())
	{
		CString sText = m_Title;
		int l = sText.GetLength();
		int i;

		for(i=0;i<10 && pDC->GetTextExtent(sText).cx > (gripper.Width() - 30);i++,l-=2)
			sText = sText.Left(l-2);
		if(i > 0)
		{
			sText = sText.Left(l-2);
			sText += "...";
		}
		pDC->TextOut (gripper.left, gripper.top, sText);
	}

	pDC->SetTextColor (crOldText);
	pDC->SetBkMode(nPrevBkMode);
	pDC->SelectObject(oldFont);

	CPoint ptOrgBtn;
    ptOrgBtn = CPoint(gripper.right - 15, gripper.top);
	m_biHide.Move(ptOrgBtn);
    m_biHide.Paint(pDC, m_isActive);

	ptOrgBtn.x -= 17;
	m_stud.Move (ptOrgBtn);
	m_stud.Paint(pDC, m_isActive);
}

void CAHFloatWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CWnd* pWnd = GetWindow(GW_CHILD);
	if (pWnd != NULL)
	{
		pWnd->MoveWindow(0, 0, cx, cy);
	}
	
}


void CAHFloatWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	BOOL bNeedPaint = FALSE;

    CPoint pt;
    ::GetCursorPos(&pt);

	///////////////////////////////////////////////////////////
	// hit close
    BOOL bHit = (OnNcHitTest(pt) == HTHIDE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;

    bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised);

	////////////////////////////////////////////////////////////
	// hit stud
	bHit = (OnNcHitTest(pt) == HTSTUD);
	bWasPushed = m_stud.bPushed;
    m_stud.bPushed = bHit && bLButtonDown;

    bWasRaised = m_stud.bRaised;
    m_stud.bRaised = bHit && !bLButtonDown;

	bNeedPaint |= (m_stud.bPushed ^ bWasPushed) ||
                  (m_stud.bRaised ^ bWasRaised);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);

	CWnd::OnNcMouseMove(nHitTest, point);
}

LRESULT CAHFloatWnd::OnNcHitTest(CPoint point) 
{
	CRect rcBar;
    GetWindowRect(rcBar);

	UINT nRet = CWnd::OnNcHitTest(point);

    CRect rc = m_biHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTHIDE;
	else
	{
		if(TRUE == m_biHide.bRaised)
		{
			m_biHide.bRaised = FALSE;
			SendMessage(WM_NCPAINT);
		}
	}

	rc = m_stud.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTSTUD;
	else
	{
		if(TRUE == m_stud.bRaised)
		{
			m_stud.bRaised = FALSE;
			SendMessage(WM_NCPAINT);
		}
	}

	if (m_dwStyle & CBRS_ALIGN_TOP)
		nRet = (HTBOTTOM == nRet?nRet: HTNOWHERE);
	else if (m_dwStyle & CBRS_ALIGN_BOTTOM)
		nRet = (HTTOP == nRet?nRet: HTNOWHERE);
	else if (m_dwStyle & CBRS_ALIGN_LEFT)
		nRet = (HTRIGHT == nRet?nRet: HTNOWHERE);
	else if (m_dwStyle & CBRS_ALIGN_RIGHT)
		nRet = (HTLEFT == nRet?nRet: HTNOWHERE);
	else
		ASSERT(FALSE);      // can never happen

    return nRet;
}

int CAHFloatWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	m_isActive = TRUE;
	SendMessage(WM_NCPAINT);
	SetFocus();
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CAHFloatWnd::OnNcActivate(BOOL bActive) 
{
	if(!bActive)
	{
		ShowWindow(SW_HIDE);
	}
	
	return CWnd::OnNcActivate(bActive);
}


void CAHFloatWnd::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	CWnd::OnNcLButtonUp(nHitTest, point);
	if (nHitTest == HTHIDE)
	{
        ShowWindow(SW_HIDE);
		m_biHide.bPushed = FALSE;
	}
	if (nHitTest == HTSTUD)
	{
		m_stud.bPushed = FALSE;
		OnStudClick();
	}
}

void CAHFloatWnd::OnStudClick()
{
	m_HideItem->Dock ();
}

void CAHFloatWnd::OnTimer(UINT nIDEvent) 
{
	ShowWindow (SW_HIDE);
	CWnd::OnTimer(nIDEvent);
}

void CAHFloatWnd::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	BOOL bNeedPaint = FALSE;

    CWnd* pFocus = GetFocus();
    BOOL bActiveOld = m_isActive;

    m_isActive = TRUE;
    if (m_isActive != bActiveOld)
        bNeedPaint = TRUE;


	///////////////////////////////////////////////////////////
	// hit close
    BOOL bHit = (nHitTest == HTHIDE);
    BOOL bLButtonDown = TRUE;

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;

    bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised);

	////////////////////////////////////////////////////////////
	// hit stud
	bHit = (nHitTest == HTSTUD);
	bWasPushed = m_stud.bPushed;
    m_stud.bPushed = bHit && bLButtonDown;

    bWasRaised = m_stud.bRaised;
    m_stud.bRaised = bHit && !bLButtonDown;

	bNeedPaint |= (m_stud.bPushed ^ bWasPushed) ||
                  (m_stud.bRaised ^ bWasRaised);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
	
	CWnd::OnNcLButtonDown(nHitTest, point);
}



//////////////////////////////////////////////////////////////////////////////////////////
// CHidePageItem
CHidePageItem::CHidePageItem()
{
}

CHidePageItem::~CHidePageItem()
{
}

void CHidePageItem::Draw(CDC *pDC, BOOL bHorz)
{
	::DrawIconEx(pDC->m_hDC,m_rect.left+3, m_rect.top + 3,m_hIcon,16,16,0,NULL,DI_NORMAL);
	CRect rect = m_rect;
	rect.left += 3;
	rect.top += 3;
	if(TRUE == m_isActive)
	{
		int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
		if(TRUE == bHorz)
		{
			rect.left += 16 + 6; 
			pDC->ExtTextOut(rect.left, rect.top+3, ETO_CLIPPED, NULL, m_sText, NULL);
		}
		else
		{
			rect.top += 16 + 6;
			pDC->ExtTextOut(rect.right-6, rect.top, ETO_CLIPPED, NULL, m_sText, NULL);
		}
		pDC->SetBkMode(nPrevBkMode);
	}
	if(TRUE == bHorz)
	{
		pDC->MoveTo (m_rect.right, m_rect.top );
		pDC->LineTo (m_rect.right, m_rect.bottom );
	}
	else
	{
		pDC->MoveTo (m_rect.left, m_rect.bottom );
		pDC->LineTo (m_rect.right, m_rect.bottom );			
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// CHideItem

CHideItem::CHideItem()
{
	m_nActivePage = -1;
}

CHideItem::~CHideItem()
{	
}

void CHideItem::Draw(CDC *pDC)
{
	CBrush brush(GetSysColor(COLOR_3DFACE));
	pDC->FillRect(m_rect,&brush);

	CPen pen(PS_SOLID, 1, RGB(128,128,128));
	HPEN poldPen = (HPEN)pDC->SelectObject (&pen);

	if(m_dwStyle & CBRS_ORIENT_HORZ)
	{
		pDC->MoveTo (m_rect.left, m_rect.top );
		pDC->LineTo (m_rect.left, m_rect.bottom );
		pDC->MoveTo (m_rect.right, m_rect.top );
		pDC->LineTo (m_rect.right, m_rect.bottom );
		if (m_dwStyle & CBRS_ALIGN_TOP)
		{
			pDC->MoveTo (m_rect.left, m_rect.bottom );
			pDC->LineTo (m_rect.right, m_rect.bottom );
		}
		else if (m_dwStyle & CBRS_ALIGN_BOTTOM)
		{
			pDC->MoveTo (m_rect.left, m_rect.top );
			pDC->LineTo (m_rect.right, m_rect.top );
		}
	}
	else if(m_dwStyle & CBRS_ORIENT_VERT)
	{
		pDC->MoveTo (m_rect.left, m_rect.top );
		pDC->LineTo (m_rect.right, m_rect.top );
		pDC->MoveTo (m_rect.left, m_rect.bottom );
		pDC->LineTo (m_rect.right, m_rect.bottom );
		if (m_dwStyle & CBRS_ALIGN_LEFT)
		{
			pDC->MoveTo (m_rect.right, m_rect.top );
			pDC->LineTo (m_rect.right, m_rect.bottom );
		}
		else if (m_dwStyle & CBRS_ALIGN_RIGHT)
		{
			pDC->MoveTo (m_rect.left, m_rect.top );
			pDC->LineTo (m_rect.left, m_rect.bottom );
		}
	}

	CHidePageItem* pHidePageItem;
	for(int i =0; i< m_arrPags.GetSize(); i++)
	{
		pHidePageItem = (CHidePageItem*)m_arrPags[i];
		

		if(m_dwStyle & CBRS_ORIENT_HORZ)
		{
			pHidePageItem->Draw (pDC, TRUE);
		}
		else if(m_dwStyle & CBRS_ORIENT_VERT)
		{
			pHidePageItem->Draw (pDC, FALSE);
		}
	}
	pDC->SelectObject (poldPen);
}

void CHideItem::AddPageItem(CDockPageBar* pPageBar, CWnd* pWnd, DWORD dwStyle)
{
	m_dwStyle = dwStyle;
	pPageBar->GetWindowRect (m_oldWndRect );
	pPageBar->GetParentFrame()->ScreenToClient (&m_oldWndRect);
	m_pAutoHideBar = pWnd;

	m_pDockPageBar = pPageBar;
	pPageBar->ShowWindow (SW_HIDE);
	m_wndFloat.CreateEx(WS_EX_LEFT,	AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW),
			NULL, (WS_POPUP | WS_BORDER | WS_THICKFRAME|WS_VISIBLE), m_oldWndRect, 
			pWnd, 0);
	m_wndFloat.m_dwStyle = dwStyle;
	m_wndFloat.ShowWindow (SW_HIDE);
	m_wndFloat.m_HideItem = this;
	m_nActivePage = pPageBar->m_nActivePage;

	POSITION	pos;
	CPageItem*  pPageItem;

	for(pos=pPageBar->m_PageList.GetHeadPosition();pos!=NULL;)
	{
		pPageItem=(CPageItem*)pPageBar->m_PageList.GetNext(pos);
		if(pPageItem)
		{
			CHidePageItem* pHidePage = new CHidePageItem;
			pHidePage->m_hIcon = pPageItem->m_hIcon;
			pHidePage->m_pWnd = pPageItem->m_pWnd;
			pHidePage->m_sText = pPageItem->m_sText;
			m_arrPags.Add (pHidePage);
		}
	}

}

void CHideItem::UpDateSize(CDC* pDC, CRect *rect)
{

	CHidePageItem* pHidePageItem;

	m_rect = *rect;
	for(int i =0; i< m_arrPags.GetSize(); i++)
	{
		pHidePageItem = (CHidePageItem*)m_arrPags[i];
		if(NULL != pHidePageItem)
		{
			if(m_dwStyle & CBRS_ORIENT_HORZ)
			{
				pHidePageItem->m_rect = *rect;
				pHidePageItem->m_rect.right =  pHidePageItem->m_rect.left + 24;
				pHidePageItem->m_isActive = FALSE;
				if(m_nActivePage == i)
				{
					pHidePageItem->m_rect.right += 6;
					pHidePageItem->m_rect.right += pDC->GetTextExtent (pHidePageItem->m_sText).cx;
					pHidePageItem->m_rect.right += 26;
					pHidePageItem->m_isActive = TRUE;
				}
				m_rect.right = rect->left = pHidePageItem->m_rect.right;
			}
			else if(m_dwStyle & CBRS_ORIENT_VERT)
			{
				pHidePageItem->m_rect = *rect;
				pHidePageItem->m_rect.bottom =  pHidePageItem->m_rect.top + 24;
				pHidePageItem->m_isActive = FALSE;
				if(m_nActivePage == i)
				{
					pHidePageItem->m_rect.bottom += 6;
					pHidePageItem->m_rect.bottom += pDC->GetTextExtent (pHidePageItem->m_sText).cx;
					pHidePageItem->m_rect.bottom += 26;
					pHidePageItem->m_isActive = TRUE;
				}
				m_rect.bottom = rect->top = pHidePageItem->m_rect.bottom;
			}
		}
	}
	
}

BOOL CHideItem::OnMouseMove(CPoint pt)
{
	CHidePageItem* pHidePageItem;

	for(int i =0; i< m_arrPags.GetSize(); i++)
	{
		pHidePageItem = (CHidePageItem*)m_arrPags[i];
		if(NULL != pHidePageItem)
		{
			if(pHidePageItem->m_rect.PtInRect (pt))
			{
				Show(i);
				if(m_nActivePage != i)
				{
					
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
	
	return FALSE;
}

void CHideItem::Show(int nShow)
{
	if(m_nActivePage == nShow && m_wndFloat.IsWindowVisible ())
		return;
	CRect rect;
	m_pAutoHideBar->GetWindowRect(rect);
	m_wndFloat.GetWindowRect (m_oldWndRect);

	if (m_dwStyle & CBRS_ORIENT_HORZ)
	{
		rect.bottom = rect.top + m_oldWndRect.Height();
		if (m_dwStyle & CBRS_ALIGN_TOP)
		{
			rect.OffsetRect (0, 26);
		}
		else if (m_dwStyle & CBRS_ALIGN_BOTTOM)
		{
			rect.OffsetRect (0, -m_oldWndRect.Height());
		}
	}
	else if (m_dwStyle & CBRS_ORIENT_VERT)
	{
		rect.right = rect.left + m_oldWndRect.Width();
		if (m_dwStyle & CBRS_ALIGN_LEFT)
			rect.OffsetRect (26, 0);
		else if (m_dwStyle & CBRS_ALIGN_RIGHT)
			rect.OffsetRect (- m_oldWndRect.Width(), 0);
	}
	else
	{
		ASSERT(FALSE);      // can never happen
	}

	m_wndFloat.ShowWindow (SW_HIDE);
	m_wndFloat.MoveWindow(0,0,0,0);
	((CHidePageItem*)m_arrPags[m_nActivePage])->m_pWnd->ShowWindow (SW_HIDE);
	((CHidePageItem*)m_arrPags[m_nActivePage])->m_pWnd->SetParent (m_pDockPageBar);
	
	m_nActivePage = nShow;
	CWnd * pchild = ((CHidePageItem*)m_arrPags[m_nActivePage])->m_pWnd;
	pchild->SetParent (&m_wndFloat);
	m_wndFloat.m_Title = ((CHidePageItem*)m_arrPags[m_nActivePage])->m_sText;

	m_wndFloat.MoveWindow (rect.left,rect.top ,rect.Width (), rect.Height ());

	CRect clientRect;
	m_wndFloat.GetClientRect(clientRect);
	pchild->MoveWindow(clientRect);
	pchild->ShowWindow (SW_SHOW);

	m_wndFloat.ShowWindow (SW_SHOW);
}


void CHideItem::Dock()
{
	((CHidePageItem*)m_arrPags[m_nActivePage])->m_pWnd->SetParent (m_pDockPageBar);

	RemoveAll();

	m_wndFloat.DestroyWindow();
	m_pDockPageBar->ShowWindow (SW_SHOW);
	m_pDockPageBar->UpdateSize();
	((CAutoHideBar*)m_pAutoHideBar)->UpdateBar();
}

/////////////////////////////////////////////////////////////////////////////
// remove all item (2004/04/06)
void CHideItem::RemoveAll()
{
	CHidePageItem* pHidePageItem;
	int count = m_arrPags.GetSize();

	for(int i =0; i< count; i++)
	{
		pHidePageItem = (CHidePageItem*)m_arrPags[0];
		delete pHidePageItem;
		pHidePageItem = NULL;
		m_arrPags.RemoveAt(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAutoHideBar

CAutoHideBar::CAutoHideBar()
{
}

CAutoHideBar::~CAutoHideBar()
{
	POSITION	pos;
	CHideItem*	pHideItem;

	for(pos=m_listBars.GetHeadPosition();pos!=NULL;)
	{
		pHideItem = (CHideItem*)m_listBars.GetNext(pos);
		if(NULL != pHideItem)
		{
			pHideItem->RemoveAll ();
			delete pHideItem;
			pHideItem = NULL;
		}
	}
	m_listBars.RemoveAll ();
}


BEGIN_MESSAGE_MAP(CAutoHideBar, CControlBar)
	//{{AFX_MSG_MAP(CAutoHideBar)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoHideBar message handlers

BOOL CAutoHideBar::Create(CWnd *pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT(pParentWnd != NULL);
	ASSERT_KINDOF(CFrameWnd, pParentWnd);

	m_dwStyle = (dwStyle & CBRS_ALL);
	// register and create the window 
    CString wndclass = ::AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
        ::LoadCursor(NULL, IDC_ARROW),
        ::GetSysColorBrush(COLOR_BTNFACE), 0);

    dwStyle &= ~CBRS_ALL; // keep only the generic window styles
    dwStyle |= WS_CLIPCHILDREN; // prevents flashing

	if(m_dwStyle & CBRS_ORIENT_HORZ)
	{
		m_Font.CreateFont(13 ,0, 0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			FF_ROMAN , "Tahoma");
	}
	else if(m_dwStyle & CBRS_ORIENT_VERT)
	{
		m_Font.CreateFont(13,0, -900,-900, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			FF_ROMAN , "Tahoma");
	}

	// Note: Parent must resize itself for control bar to be resized
	return CWnd::Create(wndclass, NULL, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
}

LRESULT CAutoHideBar::OnSizeParent(WPARAM wParam, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	DWORD dwStyle = m_dwStyle;

	if(!(dwStyle & WS_VISIBLE))
	{
		SetWindowPos(NULL, 0, 0, 0, 0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_HIDEWINDOW);
	}

	if ((dwStyle & WS_VISIBLE) && (dwStyle & CBRS_ALIGN_ANY) != 0)
	{
		// align the control bar
		CRect rect;
		rect.CopyRect(&lpLayout->rect);

		CSize sizeAvail = rect.Size();  // maximum size available

		// get maximum requested size
		DWORD dwMode = lpLayout->bStretch ? LM_STRETCH : 0;
		if (dwStyle & CBRS_ORIENT_HORZ)
			dwMode |= LM_HORZ | LM_HORZDOCK;
		else
			dwMode |=  LM_VERTDOCK;

		CSize size (26,28);

		size.cx = min(size.cx, sizeAvail.cx);
		size.cy = min(size.cy, sizeAvail.cy);

		if (dwStyle & CBRS_ORIENT_HORZ)
		{
			lpLayout->sizeTotal.cy += size.cy;
			lpLayout->sizeTotal.cx = max(lpLayout->sizeTotal.cx, size.cx);
			if (dwStyle & CBRS_ALIGN_TOP)
				lpLayout->rect.top += size.cy;
			else if (dwStyle & CBRS_ALIGN_BOTTOM)
			{
				rect.top = rect.bottom - size.cy;
				lpLayout->rect.bottom -= size.cy;
			}
			rect.bottom = rect.top + size.cy;
			rect.right = lpLayout->rect.right;
		}
		else if (dwStyle & CBRS_ORIENT_VERT)
		{
			lpLayout->sizeTotal.cx += size.cx;
			lpLayout->sizeTotal.cy = max(lpLayout->sizeTotal.cy, size.cy);
			if (dwStyle & CBRS_ALIGN_LEFT)
				lpLayout->rect.left += size.cx;
			else if (dwStyle & CBRS_ALIGN_RIGHT)
			{
				rect.left = rect.right - size.cx;
				lpLayout->rect.right -= size.cx;
			}
			rect.right = rect.left + size.cx;
			rect.bottom = lpLayout->rect.bottom ;
		}
		else
		{
			ASSERT(FALSE);      // can never happen
		}
		m_size = CSize(rect.BottomRight() - rect.TopLeft());
		SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
	return 0;
}

void CAutoHideBar::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{

}

void CAutoHideBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	POSITION	pos;
	CHideItem*	pHideItem;

	for(pos=m_listBars.GetHeadPosition();pos!=NULL;)
	{
		pHideItem = (CHideItem*)m_listBars.GetNext(pos);
		if(NULL != pHideItem)
		{
			if(pHideItem->m_rect.PtInRect (point))
			{
				pHideItem->OnMouseMove (point);
			}
			//else
			//	pHideItem->m_wndFloat.SetTimer(1, 1000, NULL);
		}
	}

	Invalidate(FALSE);
	CControlBar::OnMouseMove(nFlags, point);
}

void CAutoHideBar::OnPaint() 
{
	CPaintDC PaintDC(this); // device context for painting
	
	CRect rcBar;
	GetClientRect(&rcBar);

	CDC	dc;
	dc.CreateCompatibleDC(&PaintDC);

	CBitmap bm;
	bm.CreateCompatibleBitmap(&PaintDC, rcBar.Width(), rcBar.Height());
	dc.SelectObject(bm);
	dc.SetBoundsRect(&rcBar, DCB_DISABLE);

    // draw background
	CBrush brush(RGB(247,243,233));
    dc.FillRect(rcBar, &brush);

	dc.SelectObject (m_Font);

	//COLORREF crOldText;
	//crOldText = dc.SetTextColor(RGB(128,128,128));

	UpDateSize();
	DrawItem(&dc);
	//dc.SetTextColor(crOldText);

	PaintDC.BitBlt (rcBar.left, rcBar.top, rcBar.Width(), rcBar.Height(), &dc, 0,0,SRCCOPY);
	dc.DeleteDC ();
}

void CAutoHideBar::DrawItem(CDC *pDC)
{
	POSITION	pos;
	CHideItem* pHideItem;

	for(pos=m_listBars.GetHeadPosition();pos!=NULL;)
	{
		pHideItem = (CHideItem*)m_listBars.GetNext(pos);;
		if(NULL != pHideItem)
			pHideItem->Draw (pDC);
	}
}

void CAutoHideBar::HidePageBar(CDockPageBar *pDockPageBar)
{
	m_dwStyle |= WS_VISIBLE;
	CHideItem* pHideItem = new CHideItem;
	
	pHideItem->AddPageItem(pDockPageBar, this, m_dwStyle);

	m_listBars.AddTail (pHideItem);
	Invalidate();
}

void CAutoHideBar::UpDateSize()
{
	CPaintDC dc(this);
	dc.SelectObject (m_Font);

	CRect rect;
	GetClientRect(&rect);

	if(m_dwStyle & CBRS_ORIENT_HORZ)
	{
		rect.left += 2;
		if (m_dwStyle & CBRS_ALIGN_TOP)
			rect.bottom -= 2;
		else if (m_dwStyle & CBRS_ALIGN_BOTTOM)
			rect.top += 2;

	}
	else if(m_dwStyle & CBRS_ORIENT_VERT)
	{
		rect.top += 2;
		if (m_dwStyle & CBRS_ALIGN_LEFT)
			rect.right -= 2;
		else if (m_dwStyle & CBRS_ALIGN_RIGHT)
			rect.left += 2;
	}
	else
	{
		ASSERT(FALSE);      // can never happen
	}

	POSITION	pos;
	CHideItem* pHideItem;

	for(pos=m_listBars.GetHeadPosition();pos!=NULL;)
	{
		pHideItem = (CHideItem*)m_listBars.GetNext(pos);;
		if(NULL != pHideItem)
		{
			pHideItem->UpDateSize (&dc, &rect);
			if(m_dwStyle & CBRS_ORIENT_HORZ)
			{
				rect.left += 20;
			}
			else if(m_dwStyle & CBRS_ORIENT_VERT)
			{
				rect.top += 20;
			}
		}
	}
}

void CAutoHideBar::GetClientRect(CRect *rect)
{
	CWnd::GetClientRect (rect);
	if(m_dwStyle & CBRS_ORIENT_HORZ)
	{
		if (m_dwStyle & CBRS_ALIGN_TOP)
			rect->bottom -= 2;
		else if (m_dwStyle & CBRS_ALIGN_BOTTOM)
		{
			rect->top += 1;
			rect->bottom -= 2;
		}

	}
	else if(m_dwStyle & CBRS_ORIENT_VERT)
	{
		if (m_dwStyle & CBRS_ALIGN_LEFT)
			rect->right -= 2;
		else if (m_dwStyle & CBRS_ALIGN_RIGHT)
			rect->left += 1;
	}
	else
	{
		ASSERT(FALSE);      // can never happen
	}
}

void CAutoHideBar::UpdateBar()
{
	POSITION	pos, pos2;
	CHideItem* pHideItem;
	

	for(pos=m_listBars.GetHeadPosition();( pos2 = pos ) != NULL;)
	{
		pHideItem = (CHideItem*)m_listBars.GetNext(pos);;
		if(NULL != pHideItem)
		{
			if(pHideItem->m_arrPags.GetSize () == 0)
			{
				delete pHideItem;
				pHideItem = NULL;
				m_listBars.RemoveAt(pos2);
			}
		}
	}
	if(m_listBars.GetCount () == 0)
	{
		m_dwStyle &= ~WS_VISIBLE;
	}
	Invalidate ();
	GetParentFrame()->RecalcLayout();
}

/////////////////////////////////////////////////////////////////////////////
// CAutoHideFrame

#define AHBRS_TOP		0xE82B
#define AHBRS_BOTTOM	0xE82C
#define AHBRS_LEFT		0xE82D
#define AHBRS_RIGHT		0xE82E
const DWORD CAutoHideFrame::dwHideBarMap[4][2] =
{
	{ AHBRS_TOP,      CBRS_TOP    },
	{ AHBRS_BOTTOM,   CBRS_BOTTOM },
	{ AHBRS_LEFT,     CBRS_LEFT   },
	{ AHBRS_RIGHT,    CBRS_RIGHT  },
};

IMPLEMENT_DYNCREATE(CAutoHideFrame, CFrameWnd)

CAutoHideFrame::CAutoHideFrame()
{
}

CAutoHideFrame::~CAutoHideFrame()
{

}


BEGIN_MESSAGE_MAP(CAutoHideFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CAutoHideFrame)
	ON_MESSAGE( WM_AUTOHIDE_ON, OnAutoHideOn)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoHideFrame message handlers

void CAutoHideFrame::EnableDocking(DWORD dwDockStyle)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);

	for (int i = 0; i < 4; i++)
	{
		if (dwHideBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)
		{
			CAutoHideBar* pAutoHide = (CAutoHideBar*)GetControlBar(dwHideBarMap[i][0]);
			if (pAutoHide == NULL)
			{
				pAutoHide = new CAutoHideBar;
				if (!pAutoHide->Create(this,
					WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE |
						dwHideBarMap[i][1], dwHideBarMap[i][0]))
				{
					AfxThrowResourceException();
				}
			}
		}
	}
	CFrameWnd::EnableDocking(dwDockStyle);
}
void CAutoHideFrame::OnDestroy()
{
	for (int i = 0; i < 4; i++)
	{
		CAutoHideBar* pAutoHide = (CAutoHideBar*)GetControlBar(dwHideBarMap[i][0]);
		if (pAutoHide != NULL)
		{
			pAutoHide->DestroyWindow();
			delete pAutoHide;
		}
	}
	CFrameWnd::OnDestroy();
}

LRESULT CAutoHideFrame::OnAutoHideOn(WPARAM wParam, LPARAM lParam)
{
	CDockPageBar * pWnd = (CDockPageBar *)lParam;
	CAutoHideBar * pAutoHideBar;

	switch(wParam)
	{
		case AFX_IDW_DOCKBAR_TOP:
			pAutoHideBar = (CAutoHideBar *)GetControlBar(AHBRS_TOP);
			break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			pAutoHideBar = (CAutoHideBar *)GetControlBar(AHBRS_BOTTOM);
			break;
		case AFX_IDW_DOCKBAR_LEFT:
			pAutoHideBar = (CAutoHideBar *)GetControlBar(AHBRS_LEFT);
			break;
		case AFX_IDW_DOCKBAR_RIGHT:
			pAutoHideBar = (CAutoHideBar *)GetControlBar(AHBRS_RIGHT);
			break;
	}

	pAutoHideBar->HidePageBar(pWnd);
	RecalcLayout();
	return 0;
}


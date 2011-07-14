// TaskList.cpp: implementation of the CGraphMember class.
//

#include "stdafx.h"
#include "Clb.h"
#include "TaskList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTaskList Class
//////////////////////////////////////////////////////////////////////

CTaskList::CTaskList(CWnd* pParent, CTaskFrame* pFrm, CPuckerBtn* pPucker, CItem* pItem)
		  : m_pWnd(pParent), m_rSize(CRect(0, 0, 0, 0))
{
	m_pFrame	 = pFrm;
	m_pPuckerBtn = pPucker;
	m_pFocusBtn  = NULL;
	m_pCloneItem = pItem;
}

CTaskList::~CTaskList()
{
	m_pFocusBtn = NULL;

	std::vector<CItem*>::iterator theIt;
	for(theIt = m_vecItems.begin(); theIt != m_vecItems.end(); ++theIt)
	{
		CItem* pItem = *theIt;	
		m_vecItems.erase(theIt);
		theIt--; 
		delete pItem;
	}
	
	if(m_pCloneItem != NULL)
		delete m_pCloneItem;

	if(m_pPuckerBtn != NULL)
		delete m_pPuckerBtn;
	
	if(m_pFrame != NULL)
		delete m_pFrame;
}

void CTaskList::SetSize(const CRect& rect)
{
	m_rSize = rect;

	if(m_pFrame != NULL)
		m_pFrame->SetSize(rect);
	else
		return;

	if(m_pPuckerBtn != NULL)
	{
		CRect rect(m_rSize);
		int len = 17;
		rect.top    = (m_pFrame->TitleHeight() - len) / 2;
		rect.left   = rect.right - len - 6;
		rect.right  = rect.left + len;
		rect.bottom = rect.top  + len;
		m_pPuckerBtn->SetSize(rect);
	}

	CRect rc(m_pFrame->GetWorkRect());
	std::vector<CItem*>::iterator theIt;
	for(theIt = m_vecItems.begin(); theIt != m_vecItems.end(); ++theIt)
	{
		rc.bottom = rc.top + 16;
		(*theIt)->SetSize(rc);
		rc.top += (16 + 4);
	}
}

void CTaskList::OnDraw(CDC* pDC)
{
	if(m_pFrame != NULL)
		m_pFrame->Draw(pDC);

	if(m_pPuckerBtn != NULL)
		m_pPuckerBtn->Draw();
	
	if(!m_pPuckerBtn->GetPuckerState())
	{
		std::vector<CItem*>::iterator theIt;
		for(theIt = m_vecItems.begin(); theIt != m_vecItems.end(); ++theIt)
			(*theIt)->Draw();
	}
}

void CTaskList::AddItem(const ItemInfo& item)
{
	CItem* pItem = m_pCloneItem->Clone();
	pItem->SetItem(item);
	m_vecItems.push_back(pItem);
}

void CTaskList::ReSize()
{
	CRect rc;
	m_pWnd->GetWindowRect(&rc);
	m_pWnd->GetParent()->ScreenToClient(&rc);
	rc.bottom  = rc.top;
	rc.bottom += m_pFrame->TMargin();
	rc.bottom += m_pFrame->TitleHeight();
	rc.bottom += m_pFrame->BMargin();
	rc.bottom += m_vecItems.size() * (16 + 4);

	DWORD dwStyle = m_pWnd->GetExStyle();
	DWORD dwNewStyle = WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE;
	if((dwStyle & dwNewStyle) == dwNewStyle)
	{
		rc.bottom += 10;
	}
	else if((dwStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
	{
		rc.bottom += 4;
	}
	else if((dwStyle & WS_EX_DLGMODALFRAME) == WS_EX_DLGMODALFRAME)
	{
		rc.bottom += 6;
	}
	else if((dwStyle & WS_EX_STATICEDGE) == WS_EX_STATICEDGE)
	{
		rc.bottom += 2;
	}

	m_pWnd->MoveWindow(&rc);
}

void CTaskList::OnMouseMove(const CPoint& point)
{
	if(m_pWnd == NULL)
		return;

	if(m_pFocusBtn != NULL)
	{
		if(m_pFocusBtn->TestInArea(point))
			return;
		
		m_pFocusBtn->SetFocusedState(false);
	}

	if(m_pPuckerBtn == NULL)
		return;

	if(m_pPuckerBtn->TestInArea(point))
	{
		m_pPuckerBtn->SetFocusedState(true);
		m_pFocusBtn = m_pPuckerBtn;
		SetCursor(LoadCursor(NULL, IDC_HAND));
		return;
	}

	if(!m_pPuckerBtn->GetPuckerState())
	{
		std::vector<CItem*>::iterator theIt;
		for(theIt = m_vecItems.begin(); theIt != m_vecItems.end(); ++theIt)
		{
			CIconBtn* pItem = (*theIt);
			if(pItem->TestInArea(point))
			{
				pItem->SetFocusedState(true);
				m_pFocusBtn = pItem;
				SetCursor(LoadCursor(NULL, IDC_HAND));
				return;
			}
		}
	}

	m_pFocusBtn = NULL;	
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CTaskList::OnLButtonDown(const CPoint& point)
{
	if(m_pPuckerBtn == NULL)
		return;

	if(m_pPuckerBtn->TestInArea(point))
	{
		m_pPuckerBtn->Click();
		m_pFocusBtn = m_pPuckerBtn;
		return;
	}

	if(!m_pPuckerBtn->GetPuckerState())
	{
		std::vector<CItem*>::iterator theIt;
		int i = 0;
		for(theIt = m_vecItems.begin(); theIt != m_vecItems.end(); ++theIt, i++)
		{
			CIconBtn* pItem = (*theIt);
			if(pItem->TestInArea(point))
			{
				pItem->Click();
				m_pFocusBtn = pItem;
				return;;
			}
		}
	}

	m_pFocusBtn = NULL;	
}
// ListControlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListControlEx.h"

#define CONLUMN_LEFTWIDTH   20

// CListControlEx dialog

IMPLEMENT_DYNAMIC(CListControlEx, CListCtrl)

CListControlEx::CListControlEx()
{
	m_nLastItem = -1;
}

CListControlEx::~CListControlEx()
{
}


BEGIN_MESSAGE_MAP(CListControlEx, CListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(ORANGE_LVN_ENDEDIT, OnEndEdit)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CListControlEx::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	// TODO:  添加您的代码以绘制指定项
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	LVITEM lvi = {0};
	lvi.mask = LVIF_STATE;
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED ;
	lvi.iItem = lpDrawItemStruct->itemID;
	BOOL bGet = GetItem(&lvi);

	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)||((lvi.state & LVIS_SELECTED) && ((GetFocus() == this)|| (GetStyle() & LVS_SHOWSELALWAYS))));

	CRect rcBack = lpDrawItemStruct->rcItem;

	if( bHighlight ) //高亮显示
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(rcBack, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
	{

		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->FillRect(rcBack, &CBrush(::GetSysColor(COLOR_WINDOW)));


	}
	//绘制文本
	CString strText;
	CRect rcItem;
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		for (int i = 0; i < GetHeaderCtrl()-> GetItemCount();i++)
		{            
			if ( !GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem ))
				continue;
			strText = GetItemText( lpDrawItemStruct->itemID, i );

			pDC->DrawText(strText,strText.GetLength(), &rcItem, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_BOTTOM);
		}
	}
}

void CListControlEx::OnLButtonDown( UINT nFlags, CPoint point )
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CListCtrl::OnLButtonDown(nFlags, point);
	CRect rcCtrl;
	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	m_nItem =SubItemHitTest(&lvhti);
	if (m_nItem == -1) {
		m_nLastItem = m_nItem;
		return;
	}

	if ( m_nItem!=m_nLastItem ){
		//第一次选中m_nItem时编辑框不出现，要在第一次单击下出现比较合适
		m_nLastItem = m_nItem;
		return;
	}

	m_nSubItem = lvhti.iSubItem;
	GetSubItemRect(m_nItem,m_nSubItem,LVIR_LABEL,rcCtrl);
	if (m_Edit_ctlItem.m_hWnd == NULL)
	{
		m_Edit_ctlItem.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,99);
		m_Edit_ctlItem.ShowWindow(SW_HIDE);

		m_FontItem.CreateStockObject(DEFAULT_GUI_FONT);
		m_Edit_ctlItem.SetFont(&m_FontItem);
		m_FontItem.DeleteObject();

	}
	CString strItem=GetItemText(m_nItem,m_nSubItem);
	m_Edit_ctlItem.MoveWindow(&rcCtrl);
	m_Edit_ctlItem.ShowWindow(SW_SHOW);
	m_Edit_ctlItem.SetWindowText(strItem);
	m_Edit_ctlItem.SetFocus();
	m_Edit_ctlItem.SetSel(-1); 
}

LRESULT CListControlEx::OnEndEdit( WPARAM wParam,LPARAM lParam )
{
	CString strText;
	m_Edit_ctlItem.GetWindowText(strText);
	SetItemText(m_nItem,m_nSubItem,strText);
	m_Edit_ctlItem.ShowWindow(SW_HIDE);

	return 0;
}

void CListControlEx::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	
	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	int nItem = SubItemHitTest(&lvhti);
	if (nItem == -1)//插入一行
	{
		InsertItem(GetItemCount()+1,_T(""));
	}
	
}

void CListControlEx::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	Invalidate();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListControlEx::InsertOneItem()
{
	InsertItem(GetItemCount()+1,_T(""));
}

void CListControlEx::DelCheckedItem()
{
	for (int i = 0; i < GetItemCount();i++)
	{
		BOOL bCheck = GetCheck(i);
		if(bCheck == TRUE)
		{
			DeleteItem(i);
			i--;
		}
	}
}

void CListControlEx::GetColumnData( int nColumn,CStringArray & strArryData )
{
	int nCount = GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		strArryData.Add(GetItemText(i,nColumn));
	}
}
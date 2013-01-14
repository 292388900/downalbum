#include <afxcmn.h>
#pragma once
#include "EditEx.h"

// CListControlEx dialog

enum TYPE_CONLUMN
{
	type_Header,
	type_Image
};

class CListControlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListControlEx)

public:
	CListControlEx();   // standard constructor
	virtual ~CListControlEx();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    void DelCheckedItem();
	void InsertOneItem();

	void GetColumnData(int nColumn,CStringArray & strArryData);

protected:
	int m_nLastItem;
	int m_nItem;
    int m_nSubItem;
	CFont m_FontItem;
	CEditEx m_Edit_ctlItem;

	void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnEndEdit(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};

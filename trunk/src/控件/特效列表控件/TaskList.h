// TaskList.h: interface for the CGraphMember class.
#pragma once

#include "GraphMember.h"

class CTaskList
{
public:
	CTaskList(CWnd*, CTaskFrame* pFrm, CPuckerBtn* pPucker, CItem* pItem);
	virtual ~CTaskList();

	void SetSize(const CRect& rect);

	void OnDraw(CDC* pDC);
	void OnMouseMove(const CPoint& point);
	void OnLButtonDown(const CPoint& point);
	void AddItem(const ItemInfo& item);
	void ReSize();

private:
	CWnd*		m_pWnd;
	CRect		m_rSize;
	CTaskFrame* m_pFrame;
	CPuckerBtn*	m_pPuckerBtn;
	CItem*		m_pCloneItem;
	CIconBtn*	m_pFocusBtn;

	vector<CItem*> m_vecItems;
};

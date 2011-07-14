#pragma once

#include "TaskList.h"

class CTaskListBox : public CStatic
{
// Construction
public:
	CTaskListBox();
	virtual ~CTaskListBox();

// Attributes
private:
	CTaskList* m_pTaskList;
	CRect	   m_rSize;

// Implementation
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskListBox)
	//}}AFX_VIRTUAL

// Operations
public:
	bool CreateTaskList();
	bool CreateColorList();

	void AddItem(const ItemInfo& item);
	void ReSize();

protected:
// Generated message map functions
	//{{AFX_MSG(CTaskListBox)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

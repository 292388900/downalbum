
#pragma once

#include <afx.h>
#include <afxwin.h>
/////////////////////////////////////////////////////////////////////////////
// CGridStatic window

class CGridStatic : public CStatic
{
// Construction/destruction
public:
    CGridStatic();
    virtual ~CGridStatic();

// Attributes
public:
	void SetAlignment(int nAlignment);
	int GetAlignment();

protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Protected attributes
protected:
	int m_nAlignment;
	int m_nLastAlignment;

    // Generated message map functions
protected:
    //{{AFX_MSG(CGridStatic)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

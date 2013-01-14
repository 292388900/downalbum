
#pragma once

#include <afx.h>
#include <afxwin.h>
/////////////////////////////////////////////////////////////////////////////
// CColorPickStatic window

class CColorPickStatic : public CStatic
{
// Construction/destruction
public:
    CColorPickStatic();
    virtual ~CColorPickStatic();

// Attributes
public:
    void SetColor(COLORREF clrBack);
	COLORREF GetColor();

protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Protected attributes
protected:
    COLORREF m_crBackColor;

    // Generated message map functions
protected:
    //{{AFX_MSG(CColorPickStatic)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

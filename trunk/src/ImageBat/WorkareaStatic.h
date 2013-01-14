
#pragma once

#include <afx.h>
#include <afxwin.h>
#include "ControlTracker.h"
/////////////////////////////////////////////////////////////////////////////
// CWorkareaStatic window

class CWorkareaStatic : public CStatic
{
// Construction/destruction
public:
    CWorkareaStatic();
    virtual ~CWorkareaStatic();

	void Draw(CDC&dc,const CRect&rcRect);

protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

    // Generated message map functions
protected:
    //{{AFX_MSG(CWorkareaStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#pragma once

#include "ImageEx.h"
// CPreviewPicture

class CPreviewPicture : public CEdit
{
	DECLARE_DYNAMIC(CPreviewPicture)

private:
	BOOL m_bTopLeft;
public:
	POINT	ptLeft;
	POINT	ptRight;
	
	int nWidth;
	int nHeight;
	CString strPreviewPicFile;
	//ImageEx *m_imgGif;
public:
	CPreviewPicture();
	virtual ~CPreviewPicture();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) ;
	afx_msg void OnShowWindow( BOOL, UINT );
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};



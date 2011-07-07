#if !defined _BDIALOG_H__
#define _BDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif 

#define StyleTile 0   //∆Ω∆Ã
#define StyleCenter 1 //æ”÷–
#define StyleStretch 2  //¿≠…Ï

class CBDialog : public CDialog
{
private:
	CBitmap m_bitmap;
	int m_style;

public:
	CBDialog(UINT nIDTemplate, CWnd* pParent = NULL);  

	int SetBitmap(UINT nIDResource);
	void SetBitmapStyle(int style);


protected:
	//{{AFX_MSG(CBDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif 
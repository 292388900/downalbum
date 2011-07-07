/************************************************************************
�� �� ����CSundyButton.h
�������ڣ�3/26/2009
�� �� �ߣ�zhuxingxing
˵    ����
************************************************************************/

#pragma once


// CSundyButton

class CSundyButton : public CButton
{
	DECLARE_DYNAMIC(CSundyButton)
private:
	BOOL m_bMouseOver;
	BOOL bDown;
	CBitmap bmpDefault;
	CBitmap bmpHover;
	CBitmap bmpDown;
	CDC		mdcDefault; 
	CDC		mdcHover;
	CDC		mdcDown;
public:
	CSundyButton();
	virtual ~CSundyButton();


protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow( );

protected:
	DECLARE_MESSAGE_MAP()
};



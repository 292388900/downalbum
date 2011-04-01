/////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
class TabDialog : public CDialog
{private:
	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedButton1();
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
struct TabCtrlComplex : public TabCtrl
{	TabCtrlStyle_base styleBase;
		// 
	TabCtrlStyle_VS2003_client styleVS2003_client;
	TabCtrlStyle_VS2003_client_custom1 styleVS2003_client_custom1;
	TabCtrlStyle_VS2003_bars styleVS2003_bars;
	TabCtrlStyle_VS2003_bars_custom1 styleVS2003_bars_custom1;
		// 
	TabCtrlStyle_VS2008_client_classic styleVS2008_client_classic;
	TabCtrlStyle_VS2008_client_blue styleVS2008_client_blue;
	TabCtrlStyle_VS2008_client_silver styleVS2008_client_silver;
	TabCtrlStyle_VS2008_client_olive styleVS2008_client_olive;
	TabCtrlStyle_VS2008_bars_classic styleVS2008_bars_classic;
	TabCtrlStyle_VS2008_bars_classic_custom1 styleVS2008_bars_classic_custom1;
	TabCtrlStyle_VS2008_bars_blue styleVS2008_bars_blue;
	TabCtrlStyle_VS2008_bars_blue_custom1 styleVS2008_bars_blue_custom1;
	TabCtrlStyle_VS2008_bars_silver styleVS2008_bars_silver;
	TabCtrlStyle_VS2008_bars_silver_custom1 styleVS2008_bars_silver_custom1;
	TabCtrlStyle_VS2008_bars_olive styleVS2008_bars_olive;
	TabCtrlStyle_VS2008_bars_olive_custom1 styleVS2008_bars_olive_custom1;
		// 
	TabCtrlStyle_VS2010_client styleVS2010_client;
	TabCtrlStyle_VS2010_client_custom1 styleVS2010_client_custom1;
	TabCtrlStyle_VS2010_client_custom2 styleVS2010_client_custom2;
	TabCtrlStyle_VS2010_bars styleVS2010_bars;
};
/////////////////////////////////////////////////////////////////////////////
// 
class DemoDlg : public CDialog, 
	public TabCtrlNotify
{
public:
	DemoDlg();

private: // TabCtrlNotify.
	virtual void OnCloseButtonClicked(TabCtrl *pCtrl, CRect const *pRect, CPoint ptScr);
	virtual void OnMenuButtonClicked(TabCtrl *pCtrl, CRect const *pRect, CPoint ptScr);
	virtual void OnDrag(TabCtrl *pCtrl, HTAB hTab, CPoint ptScr, bool outside);

private:
	TabCtrlComplex m_TabCtrl;
		// 
	CListCtrl m_List1, m_List2, m_List3;
	CTreeCtrl m_Tree1, m_Tree2, m_Tree3;
	CEdit m_Edit1, m_Edit2;
	TabDialog m_Dlg1;
		// 
	bool m_bDelDragOutside;

private:
	void SetTabsPosition();
	void SetImages();
	bool IsExist(CWnd *pWnd) const;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedBut1();
	afx_msg void OnBnClickedBut2();
	afx_msg void OnBnClickedBut3();
	afx_msg void OnBnClickedBut4();
	afx_msg void OnBnClickedBut5();
	afx_msg void OnBnClickedBut6();
	afx_msg void OnBnClickedBut7();
	afx_msg void OnBnClickedBut8();
	afx_msg void OnBnClickedBut9();
	afx_msg void OnBnClickedBut10();
	afx_msg void OnBnClickedBut11();
	afx_msg void OnBnClickedBut12();
	afx_msg void OnBnClickedBut13();
	afx_msg void OnBnClickedBut14();
	afx_msg void OnBnClickedBut15();
	afx_msg void OnBnClickedBut16();
	afx_msg void OnBnClickedBut17();
	afx_msg void OnBnClickedBut20();
	afx_msg void OnBnClickedBut21();
	afx_msg void OnBnClickedBut22();
	afx_msg void OnBnClickedBut23();
	afx_msg void OnBnClickedBut24();
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////









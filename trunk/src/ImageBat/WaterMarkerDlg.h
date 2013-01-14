#pragma once
#include "WaterMarkerCtrl.h"
#include "ControlTracker.h"
#include <vector>
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;
#include "ColorPickStatic.h"
#include "GridStatic.h"
#include "WorkareaStatic.h"

// CWaterMarkerDlg 对话框

class CWaterMarkerDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaterMarkerDlg)

public:
	CWaterMarkerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaterMarkerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WATERMARKER };


	void InitAllControls();
	void Finalization();

	void Cut();
	void Copy();
	CWaterMarkerCtrl *Paste();
	void Delete();

	//自定义剪贴板格式
	int m_nClipboardFormat;

	//刷新边框设置
	void UpdateFrameOptions();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedButtonAddWatermarkerCtrl();

	DECLARE_MESSAGE_MAP()

private:
	CControlTracker m_ControlTracker;	
	//vector<CWaterMarkerCtrl *>m_vtCtrls;
	void ClearCtrls();

	//当前保存路径
	CString m_strSavePath;

	COLORREF m_clrText;
	LOGFONT m_lfontText;

	COLORREF m_clrFrame;

	//该水印模板是否被修改了
	BOOL m_bModified;

public:
	CString m_strWaterMarkerText;
	CString m_strWaterMarkerImageFile;
	int m_nGradientAngel;
	CSliderCtrl m_SliderTransparent;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnEnChangeEditGradientAngel();
	afx_msg void OnEnChangeEditWatermarkerText();
	BOOL m_bShadow;
	BOOL m_bTransparent;
	afx_msg void OnBnClickedCheckShadow();
	afx_msg void OnBnClickedCheckTransparent();
	afx_msg void OnNMReleasedcaptureSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSelectWatermarkerPicture();
	afx_msg void OnBnClickedButtonSelectFont();
	afx_msg void OnStnClickedStaticFrameColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CColorPickStatic m_ctrlFramClr;
	CGridStatic m_ctrlAlignmentGrid;
	int m_nFrameTopHeight;
	int m_nFrameBottomHeight;
	int m_nFrameLeftWidth;
	int m_nFrameRightWidth;
	CWorkareaStatic m_stcWorkarea;
	BOOL m_bAddFrame;
public:
	afx_msg void OnBnClickedCheckAddFrame();
	afx_msg void OnBnClickedButtonAddTag();
};

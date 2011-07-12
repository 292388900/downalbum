// QQFaceDlgDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "PreviewPicture.h"
#include "ImageEx.h"

// CQQFaceDlgDlg 对话框
class CQQFaceDlgDlg : public CDialog
{
private:
	int nSelectedTab;

	CRect m_rtTabRect[3];
	CRect m_rtPageRect;

	ImageEx *m_imgGif;
	CString m_strEmotionPath;

	int m_nCurGif;
	BOOL m_bMouseOver;
private:
	ULONG gdip_Token;
	GdiplusStartupInput GpInput;
	GpImage *gdip_ImagePage[3];
	GpImage	*gdip_ImageTopTab[3];
	GpImage *gdip_ImageBorder[3];
	GpImage *gdip_ImageBg;
	GpGraphics *gdip_Graphics;
	UINT nPageWidth;
	UINT nPageHeight;
	UINT nTopTabWidth[3];
	UINT nTopTabHeight[3];
	UINT nBorderWidth;
	UINT nBorderHeight;

	UINT nBgHeight;

// 构造
public:
	CQQFaceDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QQFACEDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	CPreviewPicture m_picPreview;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnDestroy();
};

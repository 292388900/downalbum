// QQFaceDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "PreviewPicture.h"
#include "ImageEx.h"

// CQQFaceDlgDlg �Ի���
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

// ����
public:
	CQQFaceDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQFACEDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <atlimage.h>
#include "ListControlEx.h"
#include "ImageHelper.h"


// CClipImageDlg 对话框

class CClipImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CClipImageDlg)

public:
	CClipImageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClipImageDlg();

// 对话框数据
	enum { IDD = IDC_CLIPIMAGE };


	void InitAllControls();
	void InitPreviewCtrl();

	CListControlEx	m_lstClipSettings;
	CStatic			m_stcPreview;

	//用来显示预览效果的图片文件，如果没有设置或者文件不存在则显示一张白色图片
	//默认为图片列表框中第一个被选中的图片文件
	CString		m_strPreviewImageFile;	
	Image		*m_pSrcImg;
	HDC			m_hdcBack;

	int	m_nPosX;
	int	m_nPosY;
	int m_nWidth;
	int m_nHeight;

	PROCESS_PARAM_CLIPIMAGE m_stParam;

	//刷新预览窗口
	void InvalidatePreview(int nItem);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

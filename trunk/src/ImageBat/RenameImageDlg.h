#pragma once
#include "ImageHelper.h"
#include "afxwin.h"
#include <StarLib/StaticLink/StaticLink.h>


// CRenameImageDlg 对话框

class CRenameImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameImageDlg)

public:
	CRenameImageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenameImageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RENAMEIMAGE };

	void InitAllControls();
	PROCESS_PARAM_RENAMEIMAGE m_stParam;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nStartNumber;
	int m_nIncreaseStep;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddTag();
	//刷新重命名预览效果
	afx_msg void OnStnClickedStaticRenamePreview();
	BOOL m_bRenameAddIndex;
	BOOL m_bRenameAddCustomText;
	BOOL m_bRenameAddPhotoDate;
	BOOL m_bRenameAddOrignName;
	CString m_strRenameRules;
	CString m_strRenamePreview;
	CStaticLink m_stcRenamePreview;
};

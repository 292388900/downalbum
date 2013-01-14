#pragma once
#include "resource.h"
#include "afxwin.h"
#include <starlib/StaticLink/StaticLink.h>


// CToolboxDlg 对话框

class CToolboxDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolboxDlg)

public:
	CToolboxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolboxDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOLBOX };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnClickEditPhotos(UINT uCtrlID);
	afx_msg void OnBnClickedEditWatermarker();
	CStaticLink m_stcShowOperateMenu;
	afx_msg void OnStnClickedStaticShowOprateMenu();
};

#pragma once
#include "afxcmn.h"


// CUpdateTimeDlg 对话框

class CUpdateTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateTimeDlg)

public:
	CUpdateTimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateTimeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_AJUSTTIME };

	void InitAllControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// 增加或减小的小时数：整数代表加，负数代表减
	int m_nHoursToAdd;
};

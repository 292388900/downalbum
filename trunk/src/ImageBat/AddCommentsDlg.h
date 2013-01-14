#pragma once
#include "resource.h"


// CAddCommentsDlg 对话框

class CAddCommentsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddCommentsDlg)

public:
	CAddCommentsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddCommentsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD_COMMENTS };

	void InitAllControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAddComments;
};

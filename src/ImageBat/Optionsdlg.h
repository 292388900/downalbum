#pragma once


// COptionsdlg 对话框

class COptionsdlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsdlg)

public:
	COptionsdlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COptionsdlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OPTIONS };


	void InitAllControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// 图片输出保存目录
	CString m_strSavePath;
public:
	// 点击预览图标时是否勾选check框
	BOOL m_bCheckIfClickIcon;
public:
	afx_msg void OnBnClickedButtonSetSavepath();
public:
	afx_msg void OnBnClickedOk();
};

// 检测句柄Dlg.h : 头文件
//

#pragma once


// C检测句柄Dlg 对话框
class C检测句柄Dlg : public CDialog
{
// 构造
public:
	C检测句柄Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	int m_nHandle;
public:
	CString m_strWndText;
public:
	CString m_strClsName;
};

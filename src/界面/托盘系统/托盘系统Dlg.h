// 托盘系统Dlg.h : 头文件
//

#pragma once

const INT ID_POPMSG=100;
const INT WM_POPMSG=WM_USER+100;


// C托盘系统Dlg 对话框
class C托盘系统Dlg : public CDialog
{
private:
	NOTIFYICONDATA popMsg;
	BOOL bShowed;
	// 构造
public:
	C托盘系统Dlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnPopMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

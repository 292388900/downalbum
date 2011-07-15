// CppClassToCSharpDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CCppClassToCSharpDlg 对话框
class CCppClassToCSharpDlg :public  CDialog
{
// 构造
public:
	CCppClassToCSharpDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CPPCLASSTOCSHARP_DIALOG };

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
	CEdit m_edtCpp;
	CEdit m_edtCSharp;
public:
	afx_msg void OnBnClickedBtntranslate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//LRESULT CALLBACK NewCallWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
};

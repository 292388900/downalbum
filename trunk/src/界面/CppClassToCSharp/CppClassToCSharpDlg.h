// CppClassToCSharpDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CCppClassToCSharpDlg �Ի���
class CCppClassToCSharpDlg :public  CDialog
{
// ����
public:
	CCppClassToCSharpDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CPPCLASSTOCSHARP_DIALOG };

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
	CEdit m_edtCpp;
	CEdit m_edtCSharp;
public:
	afx_msg void OnBnClickedBtntranslate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//LRESULT CALLBACK NewCallWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
};

// StealerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

CString getWindowText(HWND hWnd);
void AppendText(const CString&strText);
extern CEdit *g_edtMsg;


// CStealerDlg �Ի���
class CStealerDlg : public CDialog
{
// ����
public:
	CStealerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STEALER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void AddHook();
	void RemoveHook();
	CEdit m_edtMsg;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
};



// ����ϵͳDlg.h : ͷ�ļ�
//

#pragma once

const INT ID_POPMSG=100;
const INT WM_POPMSG=WM_USER+100;


// C����ϵͳDlg �Ի���
class C����ϵͳDlg : public CDialog
{
private:
	NOTIFYICONDATA popMsg;
	BOOL bShowed;
	// ����
public:
	C����ϵͳDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

// �����Dlg.h : ͷ�ļ�
//

#pragma once


// C�����Dlg �Ի���
class C�����Dlg : public CDialog
{
// ����
public:
	C�����Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

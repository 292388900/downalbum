#pragma once


// COptionsdlg �Ի���

class COptionsdlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsdlg)

public:
	COptionsdlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COptionsdlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_OPTIONS };


	void InitAllControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// ͼƬ�������Ŀ¼
	CString m_strSavePath;
public:
	// ���Ԥ��ͼ��ʱ�Ƿ�ѡcheck��
	BOOL m_bCheckIfClickIcon;
public:
	afx_msg void OnBnClickedButtonSetSavepath();
public:
	afx_msg void OnBnClickedOk();
};

#pragma once
#include "afxcmn.h"


// CUpdateTimeDlg �Ի���

class CUpdateTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateTimeDlg)

public:
	CUpdateTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpdateTimeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AJUSTTIME };

	void InitAllControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// ���ӻ��С��Сʱ������������ӣ����������
	int m_nHoursToAdd;
};

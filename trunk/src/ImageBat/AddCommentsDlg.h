#pragma once
#include "resource.h"


// CAddCommentsDlg �Ի���

class CAddCommentsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddCommentsDlg)

public:
	CAddCommentsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddCommentsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADD_COMMENTS };

	void InitAllControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAddComments;
};

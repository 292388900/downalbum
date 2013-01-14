#pragma once
#include "ImageHelper.h"
#include "afxwin.h"
#include <StarLib/StaticLink/StaticLink.h>


// CRenameImageDlg �Ի���

class CRenameImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameImageDlg)

public:
	CRenameImageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRenameImageDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RENAMEIMAGE };

	void InitAllControls();
	PROCESS_PARAM_RENAMEIMAGE m_stParam;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nStartNumber;
	int m_nIncreaseStep;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddTag();
	//ˢ��������Ԥ��Ч��
	afx_msg void OnStnClickedStaticRenamePreview();
	BOOL m_bRenameAddIndex;
	BOOL m_bRenameAddCustomText;
	BOOL m_bRenameAddPhotoDate;
	BOOL m_bRenameAddOrignName;
	CString m_strRenameRules;
	CString m_strRenamePreview;
	CStaticLink m_stcRenamePreview;
};

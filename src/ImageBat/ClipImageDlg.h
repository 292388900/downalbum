#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <atlimage.h>
#include "ListControlEx.h"
#include "ImageHelper.h"


// CClipImageDlg �Ի���

class CClipImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CClipImageDlg)

public:
	CClipImageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClipImageDlg();

// �Ի�������
	enum { IDD = IDC_CLIPIMAGE };


	void InitAllControls();
	void InitPreviewCtrl();

	CListControlEx	m_lstClipSettings;
	CStatic			m_stcPreview;

	//������ʾԤ��Ч����ͼƬ�ļ������û�����û����ļ�����������ʾһ�Ű�ɫͼƬ
	//Ĭ��ΪͼƬ�б���е�һ����ѡ�е�ͼƬ�ļ�
	CString		m_strPreviewImageFile;	
	Image		*m_pSrcImg;
	HDC			m_hdcBack;

	int	m_nPosX;
	int	m_nPosY;
	int m_nWidth;
	int m_nHeight;

	PROCESS_PARAM_CLIPIMAGE m_stParam;

	//ˢ��Ԥ������
	void InvalidatePreview(int nItem);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

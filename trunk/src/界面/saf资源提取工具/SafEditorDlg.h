// SafEditorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#pragma pack(1)

typedef struct SAFHEAD 
{
	DWORD nSign;			// equal 'SAFF'
	DWORD nUnknow;			// always equal 0x00000001
	DWORD nOffset;			// the start postion of files info from the SAF begin 
}*PSAFHEAD;

typedef struct SAFTAIL
{
	DWORD nUnknow1;			// always equal 0x00000001
	DWORD nUnknow2[4];		// may be include checknum or Hash key
	DWORD nFileCount;		// how many files included in this pack 
}*PSAFTAIL;

typedef struct SUBFILEINFO
{
	DWORD nFileOffset;
	DWORD nFileSize;
	DWORD nUnknow[4];
	WORD nFileNameLen;		// the length of the file name
	//char* pFileName;		// a string 
}*PSUBFILEINFO;

// CSafEditorDlg �Ի���
class CSafEditorDlg : public CDialog
{
public:
	void AppendMsg(CString strMsg);

// ����
public:
	CSafEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SAFEDITOR_DIALOG };

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
	CEdit m_edtMsg;
public:
	CEdit m_edtSafFile;
public:
	afx_msg void OnBnClickedButtonExtract();
};

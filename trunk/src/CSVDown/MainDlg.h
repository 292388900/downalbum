// MainDlg.h : ͷ�ļ�
//

#pragma once
#include <afxwin.h>
#include <StarLib/DlgResizeHelper/DlgResizeHelper.h>


// CMainDlg �Ի���
class CMainDlg : public CDialog
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CSVDOWN_DIALOG, ID_POPMSG };

	enum { 
		WM_VISIT_WEB = WM_USER+1, WM_POPMSG, WM_SENDMESSAGEBOX, WM_SHOWREGDLG,
		WM_FINDANDSHOWPHOTOSBEGIN = WM_USER+100, WM_FINDANDSHOWPHOTOSEND, WM_PROCESSPHOTOSBEGIN, WM_PROCESSPHOTOSEND, 
	};

	//�б�����ͷ���
	enum {
		COLUMN_INDEX_NAME=0,
		COLUMN_INDEX_DIMENSION=1,
		COLUMN_INDEX_FILESIZE=2,
		COLUMN_INDEX_IMAGETYPE=3,
		COLUMN_INDEX_PHOTOTIME=4,
		COLUMN_INDEX_UPDATETIME=5,
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	void InitAllControls();
	void Finalization();
	void ResetImageList();


	CListCtrl			m_lstCtrl;		//��ʾ�������б��
	CButton				m_ctrlViewMode;	//����ͼƬԤ��ģʽ

	CImageList			m_imageList;		//
	CStatusBarCtrl		m_wndStatusbar;
	CDlgResizeHelper	m_arranger;

public:
	NOTIFYICONDATA m_popMsg;
	void MinimizeToTrayArea();
	void SetupTrayIcon();

public:
	static UINT ThreadCheckVersion(LPVOID lpParam);
	static UINT ThreadGetAdNews(LPVOID lpParam);

	static int CALLBACK CompareFunc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);
	static bool m_bSortAsc;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonViewMode();

public:
	//�˵�����
	afx_msg LRESULT OnTrayPopMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSendMessageBox(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCommandOptions();
	afx_msg LRESULT OnShowRegDlg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBuynow();
	afx_msg void OnCheckUpdate();
	afx_msg void OnViewAllsofts();
	afx_msg void OnViewHelp();
	afx_msg void OnShareToOthers(UINT uCtrlID);
	afx_msg void OnBbs();
	afx_msg void OnEmailMe();
	afx_msg void OnExit();
};

extern CMainDlg *g_pMainDlg;

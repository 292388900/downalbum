// MainDlg.h : 头文件
//

#pragma once
#include <afxwin.h>
#include <StarLib/DlgResizeHelper/DlgResizeHelper.h>


// CMainDlg 对话框
class CMainDlg : public CDialog
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CSVDOWN_DIALOG, ID_POPMSG };

	enum { 
		WM_VISIT_WEB = WM_USER+1, WM_POPMSG, WM_SENDMESSAGEBOX, WM_SHOWREGDLG,
		WM_FINDANDSHOWPHOTOSBEGIN = WM_USER+100, WM_FINDANDSHOWPHOTOSEND, WM_PROCESSPHOTOSBEGIN, WM_PROCESSPHOTOSEND, 
	};

	//列表框的列头序号
	enum {
		COLUMN_INDEX_NAME=0,
		COLUMN_INDEX_DIMENSION=1,
		COLUMN_INDEX_FILESIZE=2,
		COLUMN_INDEX_IMAGETYPE=3,
		COLUMN_INDEX_PHOTOTIME=4,
		COLUMN_INDEX_UPDATETIME=5,
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void InitAllControls();
	void Finalization();
	void ResetImageList();


	CListCtrl			m_lstCtrl;		//显示宝贝的列表框
	CButton				m_ctrlViewMode;	//设置图片预览模式

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

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//菜单命令
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

// MainDlg.h : 头文件
//

#pragma once
#include <afxwin.h>
#include <vector>
using namespace std;
#include <StarLib/DlgResizeHelper/DlgResizeHelper.h>


class CBaseAlbumInfo
{
public:
	int		nIndex;			//编号
	CString strName;		//相册名称
	CString	strTotal;		//相册含有的照片张数
	int		nTotal;
	CString strPreviewUrl;	//相册封面图片链接
	CString strCreateTime;	//创建时间描述
	CString strUpdateTime;	//更新时间描述
	int		nPrivate;		//访问权限
	CString strAccess;		//访问权限说明
	CString strDescription;	//其他描述信息
	CString strAlbumId;		//相册id
	CString strAlbumUrl;	//相册的具体链接，打开后可获得其照片信息
	CString strAccount;		//相册主人编号
	CString strQuestion;	//如果有问题，需要回答的问题
};

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
		WM_GETITEMSBEGIN = WM_USER+100, WM_GETITEMSEND, WM_DOWNCSVBEGIN, WM_DOWNCSVEND, 
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
	void AddHistoryUrl(const CString&str);
	void LoadHistoryUrlToFile();
	void SaveHistoryUrlToFile();
	void StartGetItems();
	void StartDownCSV();
	static UINT ThreadGetItems(LPVOID lpParam);


	CListCtrl			m_lstCtrl;		//显示宝贝的列表框
	CButton				m_ctrlViewMode;	//设置图片预览模式
	CComboBox			m_cmbHistory;
	CString				m_strAccount;

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
	static vector<CBaseAlbumInfo*>m_vtAlbumList;	//相册

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

	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnClickListAlbum(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnDeleteItemList(NMHDR* pNMHDR, LRESULT* pResult); 

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
	afx_msg void OnBnClickedButtonGetitems();
	afx_msg void OnBnClickedButtonDownCsv();
	afx_msg LRESULT OnGetItemsBegin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetItemsEnd(WPARAM wParam,LPARAM lParam);
};

extern CMainDlg *g_pMainDlg;
void ClearAlbumList(vector<CBaseAlbumInfo*>&vtAlbum);

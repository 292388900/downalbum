// MainDlg.h : 头文件
//

#pragma once
#include "ToolboxDlg.h"
#include <StarLib/DlgResizeHelper/DlgResizeHelper.h>
#include <afxcmn.h>
#include "ImageInfo.h"
#include <vector>
#include "afxwin.h"
using namespace std;


// CMainDlg 对话框
class CMainDlg : public CDialog
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG, ID_POPMSG };
//指示线程开始、结束的消息
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


public:
	void InitAllControls();
	void Finalization();

	CButton				m_ctrlViewMode;	//设置图片预览模式
	CListCtrl			m_lstPhotos;	//显示照片的列表框
	CString	m_strCurPhotoPath;			//当前正在显示的图片目录
	vector<PHOTOINFO *>m_vtPhotos;		//当前正在显示的图片列表

	//查找目录下的图片并显示出来
	static UINT ThreadProcFindPhotosAndShow(LPVOID lParam);
	//查找目录下的图片
	void FindPhothos(CString&strPhotoPath);
	//显示图片
	void ShowPhotos();
	void ShowPhotosByIcon();
	void ShowPhotosByReport();
	//清空图片列表占用的内存
	void ClearPhotoList();

	//图片预览模式发生变化，重新创建imagelist
	void ResetImageList();

public:
	NOTIFYICONDATA m_popMsg;
	void MinimizeToTrayArea();
	void SetupTrayIcon();

public:
	static UINT ThreadCheckVersion(LPVOID lpParam);
	static UINT ThreadGetAdNews(LPVOID lpParam);

public:
	afx_msg LRESULT OnProcessPhotosBegin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnProcessPhotosEnd(WPARAM wParam,LPARAM lParam);

	static UINT ThreadRenameImageBatch(LPVOID lpParam);
	static UINT ThreadClipImageBatch(LPVOID lpParam);
	static UINT ThreadRotateFlipImageBatch(LPVOID lpParam);
	static UINT ThreadAjustTimeImageBatch(LPVOID lpParam);
	static UINT ThreadThumbnailImageBatch(LPVOID lpParam);
	static UINT ThreadAddWaterMarkerBatch(LPVOID lpParam);
	static UINT ThreadDeleteImageExifBatch(LPVOID lpParam);
	static UINT ThreadDeleteImageExifNotOrignalBatch(LPVOID lpParam);
	static UINT ThreadDeleteRemarksBatch(LPVOID lpParam);
	static UINT ThreadPureJpgBatch(LPVOID lpParam);
	static UINT ThreadAutoRotateImageBatch(LPVOID lpParam);
	static UINT ThreadAddRemarksBatch(LPVOID lpParam);
	


	static int CALLBACK CompareFunc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);
	static bool m_bSortAsc;

// 实现
	HICON				m_hIcon;
	CDlgResizeHelper	m_arranger;
	CToolboxDlg			m_toolboxDlg;
	//int					m_nImageWith;	//图片预览宽度
	//int					m_nImageHeight;	//图片预览高度
	CImageList			m_imageList;		//
	CStatusBarCtrl		m_wndStatusbar;
	CString				m_strImagePath;
	
	CEdit				m_edtImagePath;			// 图片预览目录
	CComboBox			m_cmbImagePathHistory;	// 图片历史目录

	void AddHistoryUrl(const CString&str);
	void SaveHistoryUrlToFile();
	void LoadHistoryUrlToFile();

protected:

	// 生成的消息映射函数
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDropFiles(HDROP hDrop);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnFindAndShowPhotosBegin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnFindAndShowPhotosEnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDBLClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonViewPhotos();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonViewMode();
	afx_msg void OnCbnSelchangeComboImagepathHistory();
	afx_msg void OnLvnColumnclickListPhotos(NMHDR *pNMHDR, LRESULT *pResult);
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

public:
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnNMRclickListPhotos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOpenSavepath();
};

extern CMainDlg *g_pMainDlg;

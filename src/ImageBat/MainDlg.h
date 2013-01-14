// MainDlg.h : ͷ�ļ�
//

#pragma once
#include "ToolboxDlg.h"
#include <StarLib/DlgResizeHelper/DlgResizeHelper.h>
#include <afxcmn.h>
#include "ImageInfo.h"
#include <vector>
#include "afxwin.h"
using namespace std;


// CMainDlg �Ի���
class CMainDlg : public CDialog
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAIN_DIALOG, ID_POPMSG };
//ָʾ�߳̿�ʼ����������Ϣ
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


public:
	void InitAllControls();
	void Finalization();

	CButton				m_ctrlViewMode;	//����ͼƬԤ��ģʽ
	CListCtrl			m_lstPhotos;	//��ʾ��Ƭ���б��
	CString	m_strCurPhotoPath;			//��ǰ������ʾ��ͼƬĿ¼
	vector<PHOTOINFO *>m_vtPhotos;		//��ǰ������ʾ��ͼƬ�б�

	//����Ŀ¼�µ�ͼƬ����ʾ����
	static UINT ThreadProcFindPhotosAndShow(LPVOID lParam);
	//����Ŀ¼�µ�ͼƬ
	void FindPhothos(CString&strPhotoPath);
	//��ʾͼƬ
	void ShowPhotos();
	void ShowPhotosByIcon();
	void ShowPhotosByReport();
	//���ͼƬ�б�ռ�õ��ڴ�
	void ClearPhotoList();

	//ͼƬԤ��ģʽ�����仯�����´���imagelist
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

// ʵ��
	HICON				m_hIcon;
	CDlgResizeHelper	m_arranger;
	CToolboxDlg			m_toolboxDlg;
	//int					m_nImageWith;	//ͼƬԤ�����
	//int					m_nImageHeight;	//ͼƬԤ���߶�
	CImageList			m_imageList;		//
	CStatusBarCtrl		m_wndStatusbar;
	CString				m_strImagePath;
	
	CEdit				m_edtImagePath;			// ͼƬԤ��Ŀ¼
	CComboBox			m_cmbImagePathHistory;	// ͼƬ��ʷĿ¼

	void AddHistoryUrl(const CString&str);
	void SaveHistoryUrlToFile();
	void LoadHistoryUrlToFile();

protected:

	// ���ɵ���Ϣӳ�亯��
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
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

public:
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnNMRclickListPhotos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOpenSavepath();
};

extern CMainDlg *g_pMainDlg;

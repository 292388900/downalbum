// MainDlg.h : ͷ�ļ�
//

#pragma once
#include <afxwin.h>
#include <vector>
using namespace std;
#include <StarLib/DlgResizeHelper/DlgResizeHelper.h>


class CBaseAlbumInfo
{
public:
	int		nIndex;			//���
	CString strName;		//�������
	CString	strTotal;		//��Ậ�е���Ƭ����
	int		nTotal;
	CString strPreviewUrl;	//������ͼƬ����
	CString strCreateTime;	//����ʱ������
	CString strUpdateTime;	//����ʱ������
	int		nPrivate;		//����Ȩ��
	CString strAccess;		//����Ȩ��˵��
	CString strDescription;	//����������Ϣ
	CString strAlbumId;		//���id
	CString strAlbumUrl;	//���ľ������ӣ��򿪺�ɻ������Ƭ��Ϣ
	CString strAccount;		//������˱��
	CString strQuestion;	//��������⣬��Ҫ�ش������
};

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
		WM_GETITEMSBEGIN = WM_USER+100, WM_GETITEMSEND, WM_DOWNCSVBEGIN, WM_DOWNCSVEND, 
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
	void AddHistoryUrl(const CString&str);
	void LoadHistoryUrlToFile();
	void SaveHistoryUrlToFile();
	void StartGetItems();
	void StartDownCSV();
	static UINT ThreadGetItems(LPVOID lpParam);


	CListCtrl			m_lstCtrl;		//��ʾ�������б��
	CButton				m_ctrlViewMode;	//����ͼƬԤ��ģʽ
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
	static vector<CBaseAlbumInfo*>m_vtAlbumList;	//���

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

	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnClickListAlbum(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnDeleteItemList(NMHDR* pNMHDR, LRESULT* pResult); 

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
	afx_msg void OnBnClickedButtonGetitems();
	afx_msg void OnBnClickedButtonDownCsv();
	afx_msg LRESULT OnGetItemsBegin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetItemsEnd(WPARAM wParam,LPARAM lParam);
};

extern CMainDlg *g_pMainDlg;
void ClearAlbumList(vector<CBaseAlbumInfo*>&vtAlbum);

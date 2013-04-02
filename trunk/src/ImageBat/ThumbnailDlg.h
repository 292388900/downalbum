#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

//
typedef struct THUMBNAIL_PREDEFINED_OPTIONS{
	int nPixelWidth;
	int nPixelHeight;
	CString strDescription;
	BOOL bResotreExif;
}*PTHUMBNAIL_PREDEFINED_OPTIONS;


// CThumbnailDlg 对话框

class CThumbnailDlg : public CDialog
{
	DECLARE_DYNAMIC(CThumbnailDlg)

public:
	CThumbnailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThumbnailDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Thumbnail_IMAGES };


	void InitAllControls();

	int m_nPixelWidth;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbThumnailPredefined;
	CString m_strImageSrcPath;
	CString m_strImageOutPath;
	BOOL m_bResotreExif;
	int m_nLimitWidth;
	int m_nLimitHeight;

private:
	vector<THUMBNAIL_PREDEFINED_OPTIONS>m_vtThumbnailOptions;

};

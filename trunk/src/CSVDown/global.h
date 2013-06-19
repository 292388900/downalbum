

#pragma once

#include <afx.h>
#include <vector>
using namespace std;
#include <StarLib/Update/Update.h>
#include <StarLib/htmldown/htmldown.h>
#include <StarLib/Common/common.h>


//ȫ������
//////////////////////////////////////////////////////////////////////////
extern CString g_sAPPNAME;
extern CString g_sAPPNAME_CH;
extern CString g_sSTRFILEVER;
extern CString g_sFIRST_PAGE_NAME;
extern CString g_sHARDCODEPREFIX;

extern CString g_sUPDATE_CONFIG_URL;
extern CString g_sUPDATE_CONFIG_URL2;
extern CString g_sLOGIN_URL;

extern CString g_sREG_URL;
extern CString g_sHELP_URL;
extern CString g_sHOME_URL;
extern CString g_sBBS_URL;

extern int g_nMAIN_WIDTH;
extern int g_nMAIN_HEIGHT;
extern int g_nIMAGE_WIDTH;
extern int g_nIMAGE_HEIGHT;
//////////////////////////////////////////////////////////////////////////

enum{
	FILEEXT_CASE_NOCHANGE,
	FILEEXT_CASE_TOLOWER,
	FILEEXT_CASE_TOUPPER,
};


enum{
	VIEWMODE_LARGEICON,	
	VIEWMODE_BIGICON,
	VIEWMODE_DETAILS,

	WIDTH_VIEWMODE_LARGEICON = 200,
	HEIGHT_VIEWMODE_LARGEICON = 160,

	WIDTH_VIEWMODE_BIGICON = 100,
	HEIGHT_VIEWMODE_BIGICON = 80,
};

//Ӧ�ó���������Ϣ
class APPCONFIG 
{
public:
	APPCONFIG();
	void LoadConfig();
	void SaveConfig();

public:
	//���ñ���ͼƬĿ¼��ÿ����������һ�ε�ʱ����Ŀ¼�Ƿ���ڣ�����������򴴽�֮
	void SetSavePath(CString&strPath);


	CString m_strStartPath;
	CString m_strCfgFile;
	CString m_strRegFile;
	CString m_strTempPath;	//tmpĿ¼�������Ŀ¼�¹�Ȼ�ܺã������û�������Ϊ�Ǿ������ص�ͼƬ�����鲻��
	CString m_strImagePath;	//��ǰ���ϴ������ͼƬĿ¼

	int nViewMode;			//
	int nImageViewWidth;	//ͼƬԤ�����
	int nImageViewHeight;	//ͼƬԤ���߶�
	BOOL m_bCheckIfClickIcon;// ���Ԥ��ͼ��ʱ�Ƿ�ѡcheck��

	int nDownMode;			//����ģʽ����������ģʽ��ƽ������ģʽ
	int nDownTimeOut;		//���س�ʱʱ��
	CString strSavePath;	//��Ƭ�ı���Ŀ¼
	BOOL bUseSkin;			//�Ƿ�ʹ��Ƥ��
	BOOL bNeedNo;			//�Ƿ���Ҫ���
	BOOL bAutoShutPC;		//�Ƿ�������ɺ��Զ��ػ�
	BOOL bDupFilter;		//��ǰ���ع����Ƿ����أ�Ĭ�ϲ�����
	int nDownloadSleepCnt;	//�������һ�ź��Ƿ��ӳٺ�������������һ�ţ�0Ϊ���ӳ�
	BOOL bShowEditMsg;		//�Ƿ���ʾ��Ϣ�����Ĭ����ʾ

	BOOL bShowScrollAds;	//�Ƿ���ʾ�������

	int nWndPosTop;
	int nWndPosLeft;
	int nWndPosRight;
	int nWndPosBottom;

	int m_nIndexPredefinedThumnailSeleted;	//��Сͼ�ߴ緽��ѡ������
	BOOL m_bResotreExif;	//��СͼƬʱ�Ƿ�ָ�ԭ�е�exif��Ϣ


	//////////////////////////////////////////////////////////////////////////
	//����������Ԥ����
	CString m_strRenameRules;		//����������
	int  m_nRenameWordCase;			//�ļ�����Сд��0���ı䣬1Сд��2��д
	//////////////////////////////////////////////////////////////////////////
};

extern APPCONFIG g_config;


extern BOOL g_bNeedUpdate;
extern bool g_bRegistered;
extern UPDATEINFO g_stUpdateInfo;


extern CStatusBarCtrl*g_pStatusbar;
void AppendText(const CString&strText,int nIndex=1);
void AppendTextV(LPCTSTR lpszFormat,...);

extern NOTIFYICONDATA*g_pNotifyIconData;
extern HWND g_hMainWnd;
void ShowToolTip(LPCTSTR szMsg,LPCTSTR szTitle=NULL/*=g_sAPPNAME_CH*/);

void LogError(const CString&strText);

//���߳��е����ĶԻ���Ҳ��Ƥ������ǰ
namespace MSGBOXTIP{
	enum { 
		LatestVersion,
		CheckVersionError,
		TrialVersion,
		InvalidSavePath,
		SelectNoAlbumToDown,
		PhotoDownOver
	};
};

namespace SOFTSITE{
	enum {
		Unknown=0,
		Huajun=1,
		Tiankong=2,
		Duote=3
	};
};

BOOL VoteSoft(int nSiteNo,const CString&strSoftid);




#pragma once

#include <afx.h>
#include <vector>
using namespace std;
#include <StarLib/Update/Update.h>
#include <StarLib/htmldown/htmldown.h>
#include <StarLib/Common/common.h>


//全局配置
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

//应用程序配置信息
class APPCONFIG 
{
public:
	APPCONFIG();
	void LoadConfig();
	void SaveConfig();

public:
	//设置保存图片目录，每当重新设置一次的时候检查目录是否存在，如果不存在则创建之
	void SetSavePath(CString&strPath);


	CString m_strStartPath;
	CString m_strCfgFile;
	CString m_strRegFile;
	CString m_strTempPath;	//tmp目录放在软件目录下固然很好，但是用户会误认为那就是下载的图片，体验不好
	CString m_strImagePath;	//当前或上次浏览的图片目录

	int nViewMode;			//
	int nImageViewWidth;	//图片预览宽度
	int nImageViewHeight;	//图片预览高度
	BOOL m_bCheckIfClickIcon;// 点击预览图标时是否勾选check框

	int nDownMode;			//下载模式：缓存下载模式，平稳下载模式
	int nDownTimeOut;		//下载超时时间
	CString strSavePath;	//照片的保存目录
	BOOL bUseSkin;			//是否使用皮肤
	BOOL bNeedNo;			//是否需要编号
	BOOL bAutoShutPC;		//是否下载完成后自动关机
	BOOL bDupFilter;		//以前下载过的是否下载，默认不过滤
	int nDownloadSleepCnt;	//下载完成一张后是否延迟毫秒数再下载下一张，0为不延迟
	BOOL bShowEditMsg;		//是否显示信息输出框，默认显示

	BOOL bShowScrollAds;	//是否显示滚动广告

	int nWndPosTop;
	int nWndPosLeft;
	int nWndPosRight;
	int nWndPosBottom;

	int m_nIndexPredefinedThumnailSeleted;	//缩小图尺寸方案选择索引
	BOOL m_bResotreExif;	//缩小图片时是否恢复原有的exif信息


	//////////////////////////////////////////////////////////////////////////
	//批量重命名预配置
	CString m_strRenameRules;		//重命名规则
	int  m_nRenameWordCase;			//文件名大小写：0不改变，1小写，2大写
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

//让线程中弹出的对话框也有皮肤，置前
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


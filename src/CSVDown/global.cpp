
#include "stdafx.h"
#include "global.h"
#include <afxinet.h>
#include <shlwapi.h>
#include <ObjBase.h>
#include <RpcDce.h>
#include <StarLib/MD5/Md5.h>
#include <StarLib/File/File.h>


BOOL g_bNeedUpdate=FALSE;
#ifdef _FREESOFT 
bool g_bRegistered=true;
#else
bool g_bRegistered=false;
#endif

APPCONFIG g_config;

UPDATEINFO g_stUpdateInfo;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

APPCONFIG::APPCONFIG(){

	//初始化随机数种子
	Star::Common::InitializeSeed();

	nViewMode=VIEWMODE_BIGICON;
	nDownMode=0;
	nDownTimeOut=5;
	nWndPosTop=0;
	nWndPosLeft=0;
	nWndPosRight=0;
	nWndPosBottom=0;
	nDownloadSleepCnt=0;
	bNeedNo=TRUE;
	bDupFilter=FALSE;

	bShowScrollAds = TRUE;

	m_strStartPath=Star::Common::GetStartPath();
	//创建一个临时目录
	m_strTempPath=Star::Common::GetSysTempPath()+"tmp\\";
	if ( GetFileAttributes(m_strTempPath)==-1 ){
		::CreateDirectory(m_strTempPath,NULL);
	}

	m_bResotreExif = TRUE;
	m_nIndexPredefinedThumnailSeleted = 0;

	LoadConfig();
}

void APPCONFIG::LoadConfig()
{
	m_strCfgFile.Format( "%s%s.ini", m_strStartPath, g_sAPPNAME );
	m_strRegFile.Format( "%s%s.usr", m_strStartPath, g_sAPPNAME );

	nViewMode=Star::File::GetIniInt(m_strCfgFile,"main","viewmode",VIEWMODE_BIGICON);
	if ( nViewMode==VIEWMODE_LARGEICON ){
		nImageViewWidth = WIDTH_VIEWMODE_LARGEICON;
		nImageViewHeight = HEIGHT_VIEWMODE_LARGEICON;
	}else if ( nViewMode==VIEWMODE_DETAILS ){
		nImageViewWidth = 0;
		nImageViewHeight = 0;
	}else{
		nViewMode = VIEWMODE_BIGICON;
		nImageViewWidth = WIDTH_VIEWMODE_BIGICON;
		nImageViewHeight = HEIGHT_VIEWMODE_BIGICON;
	}

	m_bCheckIfClickIcon = Star::File::GetIniInt(m_strCfgFile,"main","checkifclickicon",0);

	nDownMode=Star::File::GetIniInt(m_strCfgFile,"main","downstyle",0);
	nDownTimeOut=Star::File::GetIniInt(m_strCfgFile,"main","downtimeout",5);
	strSavePath=Star::File::GetIniString(m_strCfgFile,"main","savepath");
	if ( strSavePath.IsEmpty() || GetFileAttributes(strSavePath)==-1 ){
		strSavePath = m_strStartPath;	// + _T("out\\")
	}
	SetSavePath(strSavePath);
	if ( GetFileAttributes(strSavePath)==-1 ){
		CreateDirectory(strSavePath,NULL);
	}

	bUseSkin=Star::File::GetIniInt(m_strCfgFile,"main","useskin",1);
	bNeedNo=Star::File::GetIniInt(m_strCfgFile,"main","needno",1);
	bAutoShutPC=Star::File::GetIniInt(m_strCfgFile,"main","autoshutpc",0);
	bDupFilter=Star::File::GetIniInt(m_strCfgFile,"main","dupfilter",0);
	nDownloadSleepCnt=Star::File::GetIniInt(m_strCfgFile,"main","downsleep",0);
	bShowEditMsg=Star::File::GetIniInt(m_strCfgFile,"main","showedtmsg",1);

	nWndPosTop=Star::File::GetIniInt(m_strCfgFile,"window","top");
	nWndPosLeft=Star::File::GetIniInt(m_strCfgFile,"window","left");
	nWndPosRight=Star::File::GetIniInt(m_strCfgFile,"window","right");
	nWndPosBottom=Star::File::GetIniInt(m_strCfgFile,"window","bottom");

	m_nIndexPredefinedThumnailSeleted = Star::File::GetIniInt(m_strCfgFile,"thumbnail","preindex",0);
	m_bResotreExif = Star::File::GetIniInt(m_strCfgFile,"thumbnail","resotreexif",TRUE);

	m_strImagePath = Star::File::GetIniString(m_strCfgFile,"path","lastview");
	if ( GetFileAttributes(m_strImagePath)==-1 ){
		m_strImagePath = m_strStartPath;
	}
	if ( m_strImagePath.Right(1)!="\\" ){
		m_strImagePath += "\\";
	}

	//////////////////////////////////////////////////////////////////////////
	//批量重命名预配置
	m_strRenameRules = Star::File::GetIniString(m_strCfgFile,"rename","rules");
	if ( m_strRenameRules.IsEmpty()==TRUE ){
		m_strRenameRules = "<no>";
	}
	m_nRenameWordCase = Star::File::GetIniInt(m_strCfgFile,"rename","wordcase",FILEEXT_CASE_TOLOWER);
	//////////////////////////////////////////////////////////////////////////
}

void APPCONFIG::SaveConfig()
{
	Star::File::SetIniInt(m_strCfgFile,"main","viewmode",nViewMode);
	Star::File::SetIniInt(m_strCfgFile,"main","checkifclickicon",m_bCheckIfClickIcon);

	Star::File::SetIniInt(m_strCfgFile,"main","downstyle",nDownMode);
	Star::File::SetIniInt(m_strCfgFile,"main","downtimeout",nDownTimeOut);
	Star::File::SetIniString(m_strCfgFile,"main","savepath",strSavePath);
	Star::File::SetIniInt(m_strCfgFile,"main","useskin",bUseSkin);
	Star::File::SetIniInt(m_strCfgFile,"main","needno",bNeedNo);
	Star::File::SetIniInt(m_strCfgFile,"main","autoshutpc",bAutoShutPC);
	Star::File::SetIniInt(m_strCfgFile,"main","dupfilter",bDupFilter);
	Star::File::SetIniInt(m_strCfgFile,"main","downsleep",nDownloadSleepCnt);
	Star::File::SetIniInt(m_strCfgFile,"main","showedtmsg",bShowEditMsg);

	Star::File::SetIniInt(m_strCfgFile,"window","top",nWndPosTop);
	Star::File::SetIniInt(m_strCfgFile,"window","left",nWndPosLeft);
	Star::File::SetIniInt(m_strCfgFile,"window","right",nWndPosRight);
	Star::File::SetIniInt(m_strCfgFile,"window","bottom",nWndPosBottom);

	Star::File::SetIniInt(m_strCfgFile,"thumbnail","resotreexif",m_bResotreExif);
	Star::File::SetIniInt(m_strCfgFile,"thumbnail","preindex",m_nIndexPredefinedThumnailSeleted);

	Star::File::SetIniString(m_strCfgFile,"path","lastview",m_strImagePath);

	//////////////////////////////////////////////////////////////////////////
	//批量重命名预配置
	Star::File::SetIniString(m_strCfgFile,"rename","rules",m_strRenameRules);
	Star::File::SetIniInt(m_strCfgFile,"rename","wordcase",m_nRenameWordCase);
	//////////////////////////////////////////////////////////////////////////
}


void APPCONFIG::SetSavePath(CString&strPath)
{
	strSavePath = strPath;
	if ( strSavePath.Right(1)!="\\" ){
		strSavePath += "\\";
	}
	if ( GetFileAttributes(strSavePath)==-1 ){
		CreateDirectory(strSavePath,NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


CStatusBarCtrl*g_pStatusbar=NULL;
void AppendText(const CString&strText,int nIndex)
{
	if ( g_pStatusbar!=NULL ){
		g_pStatusbar->SetText(strText,nIndex,0);
	}
}

void AppendTextV(LPCTSTR lpszFormat,...)
{
	if ( g_pStatusbar!=NULL ){
		CString strText;
		va_list argList;

		va_start(argList, lpszFormat);
		strText.FormatV(lpszFormat, argList);
		va_end(argList);

		g_pStatusbar->SetText(strText,1,0);
	}
}

NOTIFYICONDATA*g_pNotifyIconData=NULL;
HWND g_hMainWnd=NULL;
void ShowToolTip(LPCTSTR szMsg,LPCTSTR szTitle)
{
	if ( g_pNotifyIconData!=NULL ){
		g_pNotifyIconData->hWnd=g_hMainWnd;
		g_pNotifyIconData->hIcon=NULL;	
		g_pNotifyIconData->cbSize=sizeof(NOTIFYICONDATA);
		g_pNotifyIconData->uFlags = NIF_INFO;
		g_pNotifyIconData->uVersion = NOTIFYICON_VERSION;
		g_pNotifyIconData->uTimeout = 0;
		g_pNotifyIconData->dwInfoFlags = NIIF_INFO;
		if ( szTitle==NULL ){
			lstrcpy(g_pNotifyIconData->szInfoTitle,g_sAPPNAME_CH);	//气泡标题
		}else{
			lstrcpy(g_pNotifyIconData->szInfoTitle,szTitle);	//气泡标题
		}
		lstrcpy(g_pNotifyIconData->szInfo,szMsg);			//气泡内容
		Shell_NotifyIcon( NIM_MODIFY, g_pNotifyIconData );
	}
}

void LogError(const CString&strText)
{
	static int nIndex = 1;
	CString str;
	str.Format("%d_",nIndex++);
	Star::File::SetIniString(g_config.m_strCfgFile,"error",str+Star::Common::GetCurrentTimeStr(),strText);
}

BOOL VoteSoft(int nSiteNo,const CString&strSoftid)
{
	BOOL bSuccess=FALSE;
	CString strUrl;
	CString strHtml;
	
	switch ( nSiteNo )
	{
	case SOFTSITE::Huajun:
		{
			CString strSendData;
			CString strHeaders;

			strSendData.Format("do=digg&action=1&id=%s",strSoftid);
			strHeaders.Format(
				"Accept: */*\n"
				"Accept-Language: zh-cn\n"
				"Referer: http://digg.newhua.com/topinfo.php?id=%s\n"
				"Content-Type: application/x-www-form-urlencoded\n"
				"Connection: Keep-Alive\n"
				"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; InfoPath.2; .NET4.0C)\n"
				"Host: digg.newhua.com\n"
				"Content-Length: %d\n"
				"Pragma: no-cache\n",
				strSoftid,strSendData.GetLength());

			strHtml=SendHttpData("digg.newhua.com","ajax_top.php",strHeaders,strSendData,strSendData.GetLength());
			Star::Common::ConvertUtf8ToGBK(strHtml);
			if ( strHtml.Find(strSoftid) !=-1 ){
				bSuccess=TRUE;
			}
		}
		break;
	case SOFTSITE::Tiankong:
		{
			strUrl="http://123.103.68.178:81/web/softeval.php?et=1&id=" + strSoftid;
			GetHttpFileContent(strUrl,strHtml);
			//var re=new Array();re[0]=57830;re[1]=1;re[2]="success";re[3]="投票成功！";
			//var re=new Array();re[0]=59457;re[1]=1;re[2]="fail";re[3]="您已经投过票了，感谢您的参与！";
			if ( strHtml.Find("success")!=-1 || strHtml.Find("fail")!=-1 ){
				bSuccess=TRUE;
			}
		}
		break;
	case SOFTSITE::Duote:
		{
			/*
			function pj(st,softId)
			{
				if(pjed==true)
				{
				return;
				}
				var dt = new Date();
				var pjUrl = "/soft_pj.php?st="+st+"&id="+softId+"&dt="+dt.getTime();
				ajx.fget(pjUrl,pjBack);
				pjed=true;
			}
			*/

			//CTime tmNow = CTime::GetCurrentTime();
			//strUrl.Format("http://www.duote.com/soft_pj.php?st=1&id=%s&dt=%I64d",
			//	strSoftid,tmNow.GetTime());
			//GetHttpFileContent(strUrl,strHtml);
			//Star::Common::ConvertUtf8ToGBK(strHtml);
		}
		break;
	default:
		break;
	}

	return bSuccess;
}
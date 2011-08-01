
#pragma once
#include <afx.h>


typedef struct UPDATEINFO
{
	CString strName;		//该程序的名称
	CString strVersion;		//最新版本信息
	CString strNews;		//最新版本信息，可以提示修复的错误，广告，新特征等
	CString strDownUrl;		//试用版下载地址
	CString strDownUrl2;	//注册版下载地址
	CString strRegUrl;		//购买网址
	CString strHelpUrl;		//帮助网页地址
	CString strHome;		//官方网站
	CString strBBS;			//官方论坛
	CString strPswdUrl;		//用户密码修改页面
	CString strUrgent;		//
}*PUPDATEINFO;

BOOL CheckUpdateInfo(CString strAppName,CString strUrl,UPDATEINFO&stUpdateInfo);
void StartNewVersionApp(CString strNewApp);

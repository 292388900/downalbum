
#pragma once
#include <atlstr.h>
#include <WinInet.h>

UINT GetHttpFileSaveAs(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);
UINT GetHttpFileSaveAs2(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);

UINT GetHttpFileContent(LPCTSTR lpszUrl,CString&strHtml,int nTimeOutSeconds=10);
UINT GetHttpFileContentEx(CString strUrl,CString&strHtml,int nTimeOutSeconds=10);
BOOL URLDownloadToString(CString strUrl,CString&strHtml,int nTimeOutSeconds=10);

CString SendHttpData(const CString&strHost, const CString&strPath, const CString&strHeaders, const CString&strSendData, 
					 int nMethod=0, BOOL bNeedDocode=FALSE );

#pragma once
#include <atlstr.h>
#include <WinInet.h>

UINT GetHttpFileSaveAs(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);
UINT GetHttpFileSaveAs2(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);

UINT GetHttpFileContent(LPCTSTR lpszUrl,CString&strHtml,int nTimeOutSeconds=10);
UINT GetHttpFileContentEx(CString strUrl,CString&strHtml,int nTimeOutSeconds=10);
BOOL URLDownloadToString(CString strUrl,CString&strHtml,int nTimeOutSeconds=10);

CString SendHttpData(LPCTSTR szHost, LPCTSTR szPath, LPCTSTR szHeaders, LPCTSTR szSendData, int nSendDataSize, 
					 int nMethod=0, BOOL bNeedDocode=FALSE );
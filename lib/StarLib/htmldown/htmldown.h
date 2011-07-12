
#pragma once
#include <afx.h>

UINT GetHttpFileSaveAs(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);
UINT GetHttpFileSaveAs2(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);
UINT GetHttpFileSaveAs3(CString lpszUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds=10);

UINT GetHttpFileContent(LPCTSTR lpszUrl,CString&strHtml,int nTimeOutSeconds=10);
UINT GetHttpFileContentUseProxy(LPCTSTR lpszUrl,CString&strHtml,const CString&strProxy,int nTimeOutSeconds=10);
UINT GetHttpFileContentEx(CString strUrl,CString&strHtml,int nTimeOutSeconds=10);
BOOL URLDownloadToString(CString strUrl,CString&strHtml,int nTimeOutSeconds=10);


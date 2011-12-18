#include "stdafx.h"
#include "htmldown.h"
#include <afxinet.h>
#include <StarLib/Common/common.h>


UINT GetHttpFileSaveAs(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds)
{
	DWORD dwHttpStatus=0x80000000;

	HRESULT hRet=URLDownloadToFile(NULL,strUrl,lpszSaveAs,0,NULL);
	if ( hRet==S_OK ){
		dwHttpStatus=0;
	}else{//INET_E_DOWNLOAD_FAILURE 0x800C0008L
		CString strText;
		strText.Format("下载错误：%08x\n图片可能不存在：%s\n，点击确定继续下载",hRet,strUrl);
		//::AfxMessageBox(strText);
	}
	return   dwHttpStatus;  
}

UINT GetHttpFileSaveAs2(CString strUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds)
{
	DWORD dwHttpStatus=0x80000000;

	CString       strServer; 
	CString       strObject; 
	INTERNET_PORT nPort;
	DWORD         dwServiceType;
	HINTERNET hInternetSession=NULL;
	HINTERNET hHttpConnection=NULL;
	HINTERNET     hHttpFile=NULL;
	CFile		  fileToWrite;

	if (!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//Try sticking "http://" before it
		if (!AfxParseURL(_T("http://") + strUrl, dwServiceType, strServer, strObject, nPort))
		{
			TRACE(_T("Failed to parse the URL: %s\n"), strUrl);
			return dwHttpStatus;
		}
	}

	//Create the Internet session handle
	hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternetSession == NULL)
	{
		TRACE(_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}

	//Make the connection to the HTTP server
	hHttpConnection = ::InternetConnect(hInternetSession, strServer, nPort, NULL, 
		NULL, dwServiceType, 0, NULL);
	if (hHttpConnection == NULL)
	{
		TRACE(_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}

	//设置下载超时时间，默认10秒
	nTimeOutSeconds*=1000;
	//nTimeOutSeconds=1;
	InternetSetOption(hHttpConnection,INTERNET_OPTION_RECEIVE_TIMEOUT,&nTimeOutSeconds,sizeof(nTimeOutSeconds));


	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;
	ASSERT(hHttpFile == NULL);
	hHttpFile = HttpOpenRequest(hHttpConnection, NULL, strObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | 
		INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, NULL);
	if (hHttpFile == NULL)
	{
		TRACE(_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}

	//label used to jump to if we need to resend the request
resend:

	//////////////////////////////////////////////////////////////////////////
	//Issue the request
	BOOL bSend = ::HttpSendRequestEx(hHttpFile, NULL, NULL, 0, 0);
	if (!bSend){
		TRACE(_T("Failed in call to HttpSendRequestEx, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}
	bSend = HttpEndRequest(hHttpFile,NULL,0,0);
	if (!bSend){
		TRACE(_T("Failed in call to HttpEndRequest, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}
	//////////////////////////////////////////////////////////////////////////

	//Check the HTTP status code
	TCHAR szStatusCode[32];
	DWORD dwInfoSize = 32;
	if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		TRACE(_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);

		//Handle any authentication errors
		if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
			// We have to read all outstanding data on the Internet handle
			// before we can resubmit request. Just discard the data.
			char szData[51];
			DWORD dwSize;
			do
			{
				::InternetReadFile(hHttpFile, (LPVOID)szData, 50, &dwSize);
			}
			while (dwSize != 0);

			//Bring up the standard authentication dialog
			if (::InternetErrorDlg(AfxGetMainWnd()->GetSafeHwnd(), hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
				goto resend;
		}
		else if (nStatusCode != HTTP_STATUS_OK)
		{
			TRACE(_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
			return dwHttpStatus;
		}
	}

	// Get the length of the file.            
	TCHAR szContentLength[32];
	dwInfoSize = 32;
	DWORD dwFileSize = 0;
	BOOL bGotFileSize = FALSE;
	if (::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
	{
		//Set the progress control range
		bGotFileSize = TRUE;
		dwFileSize = (DWORD) _ttol(szContentLength);
	}

	//Now do the actual read of the file
	if (!fileToWrite.Open(lpszSaveAs, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite)){
		TRACE(_T("Failed to create file :%s\n"), lpszSaveAs);
		return dwHttpStatus;
	}
	DWORD dwStartTicks = ::GetTickCount();
	DWORD dwCurrentTicks = dwStartTicks;
	DWORD dwBytesRead = 0;
	char szReadBuf[1024*10];
	DWORD dwBytesToRead = 1024*10;
	DWORD dwTotalBytesRead = 0;
	DWORD dwLastTotalBytes = 0;
	DWORD dwLastPercentage = 0;
	do
	{
		if (!::InternetReadFile(hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead))
		{
			TRACE(_T("Failed in call to InternetReadFile, Error:%d\n"), ::GetLastError());
			return dwHttpStatus;
		}
		else if (dwBytesRead)
		{
			//Write the data to file
			TRY
			{
				fileToWrite.Write(szReadBuf, dwBytesRead);
			}
			CATCH(CFileException, e);                                          
			{
				TRACE(_T("An exception occured while writing to the download file\n"));
				e->Delete();
				return dwHttpStatus;
			}
			END_CATCH

				//Increment the total number of bytes read
				dwTotalBytesRead += dwBytesRead;  
		}
	}while (dwBytesRead);

	//Delete the file being downloaded to if it is present and the download was aborted
	fileToWrite.Close();
	dwHttpStatus=0;

	//We're finished
	return dwHttpStatus;
}

UINT GetHttpFileSaveAs3(CString lpszUrl,LPCTSTR lpszSaveAs,int nTimeOutSeconds)
{
	int		nRead=0;  
	CString strText;
	DWORD	dwHttpStatus;  
	try{  
		CInternetSession session;
		session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT,nTimeOutSeconds*1000);
		DWORD dwFlags=INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_RELOAD;  
		CHttpFile*pHttpFile=(CHttpFile*)session.OpenURL(lpszUrl,1,dwFlags);  

		if ( pHttpFile!=NULL && pHttpFile->QueryInfoStatusCode(dwHttpStatus)!=0 ){  
			if ( dwHttpStatus>=200 && dwHttpStatus<300 ){
				//Success
				char*pBuff=new char[1024*1024];
				CFile file(lpszSaveAs,CFile::modeCreate|CFile::modeReadWrite);
				pHttpFile->SeekToBegin();
				while ( nRead=pHttpFile->Read(pBuff,1024*1024) ){
					file.Write(pBuff,nRead);
				}
				SetEndOfFile(file.m_hFile);
				delete[] pBuff;
			}else{
				//读取图片失败了，请先看看其他照片吧
				dwHttpStatus=0x80000000;
			}

			pHttpFile->Close();
			delete pHttpFile;
		}  
	}catch(CInternetException*e){ 
		//e->ReportError();
		e->Delete();  
		//可能是超时引起的
		dwHttpStatus=0x80000000;
	}

	return   dwHttpStatus;  
}

UINT GetHttpFileContentEx(CString strUrl,CString&strHtml,int nTimeOutSeconds)
{
	DWORD dwHttpStatus=0x80000000;
	strHtml.Empty();

	CString       strServer; 
	CString       strObject; 
	INTERNET_PORT nPort;
	DWORD         dwServiceType;
	HINTERNET hInternetSession=NULL;
	HINTERNET hHttpConnection=NULL;
	HINTERNET     hHttpFile=NULL;

	if (!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//Try sticking "http://" before it
		if (!AfxParseURL(_T("http://") + strUrl, dwServiceType, strServer, strObject, nPort))
		{
			TRACE(_T("Failed to parse the URL: %s\n"), strUrl);
			return dwHttpStatus;
		}
	}

	//Create the Internet session handle
	hInternetSession = ::InternetOpen(/*AfxGetAppName()+*/Star::Common::RandFloatNum(10), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternetSession == NULL)
	{
		TRACE(_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}

	//Make the connection to the HTTP server
	hHttpConnection = ::InternetConnect(hInternetSession, strServer, nPort, NULL, 
		NULL, dwServiceType, 0, NULL);
	if (hHttpConnection == NULL)
	{
		TRACE(_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}

	nTimeOutSeconds*=1000;
	InternetSetOption(hHttpConnection,INTERNET_OPTION_RECEIVE_TIMEOUT,&nTimeOutSeconds,sizeof(nTimeOutSeconds));

	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;
	ASSERT(hHttpFile == NULL);
	hHttpFile = HttpOpenRequest(hHttpConnection, NULL, strObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | 
		INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, NULL);
	if (hHttpFile == NULL)
	{
		TRACE(_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}

	//label used to jump to if we need to resend the request
resend:

	//////////////////////////////////////////////////////////////////////////
	//Issue the request
	BOOL bSend = ::HttpSendRequestEx(hHttpFile, NULL, NULL, 0, 0);
	if (!bSend){
		TRACE(_T("Failed in call to HttpSendRequestEx, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}
	bSend = HttpEndRequest(hHttpFile,NULL,0,0);
	if (!bSend){
		TRACE(_T("Failed in call to HttpEndRequest, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}
	//////////////////////////////////////////////////////////////////////////

	//Check the HTTP status code
	TCHAR szStatusCode[32];
	DWORD dwInfoSize = 32;
	if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		TRACE(_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
		return dwHttpStatus;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);

		//Handle any authentication errors
		if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
			// We have to read all outstanding data on the Internet handle
			// before we can resubmit request. Just discard the data.
			char szData[51]={0};
			DWORD dwSize;
			do
			{
				::InternetReadFile(hHttpFile, (LPVOID)szData, 50, &dwSize);
			}
			while (dwSize != 0);

			//Bring up the standard authentication dialog
			if (::InternetErrorDlg(AfxGetMainWnd()->GetSafeHwnd(), hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
				goto resend;
		}
		else if (nStatusCode != HTTP_STATUS_OK)
		{
			TRACE(_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
			return dwHttpStatus;
		}
	}

	// Get the length of the file.            
	TCHAR szContentLength[32];
	dwInfoSize = 32;
	DWORD dwFileSize = 0;
	BOOL bGotFileSize = FALSE;
	if (::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
	{
		//Set the progress control range
		bGotFileSize = TRUE;
		dwFileSize = (DWORD) _ttol(szContentLength);
	}

	DWORD dwStartTicks = ::GetTickCount();
	DWORD dwCurrentTicks = dwStartTicks;
	DWORD dwBytesRead = 0;
	DWORD dwBytesToRead = 1024;
	DWORD dwTotalLen=0;
	BOOL bRet = TRUE;
	DWORD dwMaxDataLength =1024*1024;
	char*pBuf=strHtml.GetBuffer(dwMaxDataLength);
	do
	{
		bRet = InternetReadFile(hHttpFile,pBuf,dwBytesToRead,&dwBytesRead);
		pBuf[dwBytesRead]=0;
		strHtml.ReleaseBuffer();
		if ( bRet==FALSE || dwBytesRead==0 ){
			break;
		}
		dwTotalLen=strHtml.GetLength();
		pBuf=strHtml.GetBuffer(dwTotalLen+dwMaxDataLength)+dwTotalLen;
	} while ( TRUE );

	//Delete the file being downloaded to if it is present and the download was aborted
	dwHttpStatus=0;

	//We're finished
	return dwHttpStatus;
}

UINT GetHttpFileContent(LPCTSTR lpszUrl,CString&strHtml,int nTimeOutSeconds) 
{
	int		nRead=0;  
	CString strText;
	DWORD	dwHttpStatus=0x80000000;
	strHtml=_T("");

	CInternetSession session(Star::Common::RandFloatNum(10));
	session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,nTimeOutSeconds*1000);
	try{  
		CHttpFile*pHttpFile=(CHttpFile*)session.OpenURL(lpszUrl);  //使用默认的flag即可 否则肯会卡

		if ( pHttpFile!=NULL && pHttpFile->QueryInfoStatusCode(dwHttpStatus)!=0 ){  
			if ( dwHttpStatus>=200 && dwHttpStatus<300 ){
				//Success
				dwHttpStatus=0;
#ifdef _UNICODE
#error Unicode 模式下CHttpFile::ReadString读入乱码
#endif
				while( pHttpFile->ReadString(strText)!=0 ){  
					strHtml=strHtml+strText+_T("\n");   
				}
			}

			pHttpFile->Close();
			delete pHttpFile;
		}  
	}catch(CInternetException*e){ 
		CString s;
		TCHAR szCause[MAX_PATH];
		e->GetErrorMessage(szCause,MAX_PATH);
		s.Format("InternetException：\n%s\n m_dwError%u,m_dwContextError%u",szCause,e->m_dwError,e->m_dwContext);
		//AfxMessageBox(s);
		//e->ReportError();
		e->Delete();  
		//可能是超时引起的
		dwHttpStatus=0x80000000;
	}

	return   dwHttpStatus;  
}

UINT GetHttpFileContentUseProxy(LPCTSTR lpszUrl,CString&strHtml,const CString&strProxy,int nTimeOutSeconds)
{
	int		nRead=0;  
	CString strText;
	DWORD	dwHttpStatus=0x80000000;
	strHtml=_T("");

	CInternetSession session("HttpClient");
	session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,nTimeOutSeconds*1000);
	try{  
		CHttpFile*pHttpFile=(CHttpFile*)session.OpenURL(lpszUrl,1);  //使用默认的flag即可 否则肯会卡
		Star::Common::SetProxy(session,pHttpFile,(char*)(LPCTSTR)strProxy,"","");

		if ( pHttpFile!=NULL && pHttpFile->QueryInfoStatusCode(dwHttpStatus)!=0 ){  
			if ( dwHttpStatus>=200 && dwHttpStatus<300 ){
				//Success
#ifdef _UNICODE
#error Unicode 模式下CHttpFile::ReadString读入乱码
#endif
				while( pHttpFile->ReadString(strText)!=0 ){  
					strHtml=strHtml+strText+_T("\n");   
				}
			}  

			pHttpFile->Close();
			delete pHttpFile;
			dwHttpStatus=0;
		}  
	}catch(CInternetException*e){ 
		CString s;
		TCHAR szCause[MAX_PATH];
		e->GetErrorMessage(szCause,MAX_PATH);
		s.Format("InternetException：\n%s\n m_dwError%u,m_dwContextError%u",szCause,e->m_dwError,e->m_dwContext);
		AfxMessageBox(s);
		//e->ReportError();
		e->Delete();  
		//可能是超时引起的
		dwHttpStatus=0x80000000;
	}

	return   dwHttpStatus;  
}

//不采用改函数，不稳定，InternetReadFile随机返回： ERROR_INTERNET_CONNECTION_RESET导致页面读取的不全
UINT GetHttpFileContent2(LPCTSTR lpszUrl,CString&strHtml)  
{  
	CString strText;
	DWORD dwHttpStatus=0x80000000;
	strHtml.Empty();
	HINTERNET hInternet1=InternetOpen(NULL,PRE_CONFIG_INTERNET_ACCESS,NULL,INTERNET_INVALID_PORT_NUMBER,NULL);
	if (NULL == hInternet1){
		InternetCloseHandle(hInternet1);
		return dwHttpStatus;
	}
	HINTERNET hInternet2=InternetOpenUrl(hInternet1,lpszUrl,NULL,NULL,
		INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_RELOAD,1);
	if (NULL == hInternet2){
		InternetCloseHandle(hInternet2);
		InternetCloseHandle(hInternet1);
		return dwHttpStatus;
	}
	DWORD dwMaxDataLength =1024*1024*20;
	//char*pBuf = new char[dwMaxDataLength];
	// 	if (NULL == pBuf){
	// 		InternetCloseHandle(hInternet2);
	// 		InternetCloseHandle(hInternet1);
	// 		return dwHttpStatus;
	// 	}
	DWORD dwReadDataLength = NULL;
	DWORD dwTotalLen=0;
	BOOL bRet = TRUE;
	char*pBuf=strHtml.GetBuffer(dwMaxDataLength);
	do
	{
		bRet = InternetReadFile(hInternet2,pBuf,dwMaxDataLength-1,&dwReadDataLength);
		pBuf[dwReadDataLength]=0;
		strHtml.ReleaseBuffer();
		if ( bRet==FALSE || dwReadDataLength==0 ){
			break;
		}
		dwTotalLen=strHtml.GetLength();
		pBuf=strHtml.GetBuffer(dwTotalLen+dwMaxDataLength)+dwTotalLen;
	} while ( TRUE );
	//delete[]pBuf;
	dwHttpStatus=0;

	dwTotalLen=strHtml.GetLength();
	return   dwHttpStatus;  
}

BOOL URLDownloadToString(CString strUrl,CString&strHtml,int nTimeOutSeconds)
{
	CString strTempFile;
	BOOL bOK=FALSE;
	int nFileSize=0;

	strHtml.Empty();
	//strTempFile=Star::Common::GetMyTempPath();

	HRESULT hRet=URLDownloadToCacheFile(NULL,strUrl,strTempFile.GetBuffer(MAX_PATH),URLOSTRM_GETNEWESTVERSION,0,NULL);
	strTempFile.ReleaseBuffer();
	if ( hRet!=S_OK ){
		strTempFile = Star::Common::GetSysTempFileName();
		hRet=URLDownloadToFile(NULL,strUrl,strTempFile,0,NULL);
		if ( hRet!=S_OK ){
			return bOK;  
		}
	}

	CFile file;
	if ( file.Open(strTempFile,CFile::modeReadWrite) ){
		nFileSize=(int)file.GetLength();
		char*pBuf=strHtml.GetBuffer(nFileSize+1);
		file.Read(pBuf,nFileSize);
		pBuf[nFileSize]=0;
		strHtml.ReleaseBuffer();
		file.Close();
		::DeleteFile(strTempFile);

		bOK = TRUE;
	}

	return bOK;  
}
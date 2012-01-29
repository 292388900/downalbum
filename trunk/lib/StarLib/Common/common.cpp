#include "stdafx.h"
#include "Common.h"
#include <atltime.h>
#include <ShellAPI.h>
#include <shlwapi.h>
#include <ShlObj.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable:4996)


void _Trace(const CHAR* lpszFormat, ...)
{
	if( lpszFormat ){
		va_list vlArgs;
		CHAR tzText[1024];
		va_start(vlArgs, lpszFormat);
		if(_vscprintf(lpszFormat,vlArgs) < 1024){
			vsprintf(tzText, lpszFormat, vlArgs);
		}else{
			strcpy_s(tzText, 1024, "_Trace: too long...");
		}
		OutputDebugStringA(tzText);
		va_end(vlArgs);
	}
}

void _TraceW(const WCHAR* lpszFormat, ...)
{
	if( lpszFormat ){
		va_list vlArgs;
		WCHAR tzText[1024];
		va_start(vlArgs, lpszFormat);
		if(_vscwprintf(lpszFormat,vlArgs) < 1024){
			vswprintf_s(tzText, 1024, lpszFormat, vlArgs);
		}else{
			wcscpy_s(tzText, 1024, L"_Trace: too long...");
		}
		OutputDebugStringW(tzText);
		va_end(vlArgs);
	}
}

BOOL Star::Common::MsgSleep(IN DWORD dwMillseconds)
{
	MSG msg;
	UINT_PTR nTimerID=SetTimer(NULL,NULL,dwMillseconds,NULL);
	while(GetMessage(&msg,NULL,NULL,NULL))
	{
		if(msg.message==WM_TIMER && msg.wParam==nTimerID)
		{
			KillTimer(NULL,nTimerID);
			return TRUE;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return FALSE;
}

LONG Star::Common::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS) {
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

LONG Star::Common::GetRegValueDWord(HKEY key, LPCTSTR subkey, LPCTSTR szValue,DWORD*pData)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS) {
		DWORD dwType=REG_DWORD;
		DWORD dwDataSize=sizeof(DWORD);
		retval=RegQueryValueEx(hkey, szValue,NULL,&dwType, (LPBYTE)pData, &dwDataSize);
		RegCloseKey(hkey);
	}

	return retval;
}

LONG Star::Common::SetRegValueDWord(HKEY key, LPCTSTR subkey, LPCTSTR szValue,DWORD dwData,BOOL bForce)
{
	HKEY hkey;
	LONG retval;

	if ( bForce==TRUE ){
		char szString[25];
		DWORD dwordbuffer;
		retval = RegCreateKeyEx(key,subkey,NULL,szString,REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,NULL,&hkey,&dwordbuffer); 
	}else{
		retval = RegOpenKeyEx(key, subkey, 0, KEY_WRITE, &hkey);
	}

	if (retval == ERROR_SUCCESS) {
		DWORD dwTemp=dwData;
		retval=RegSetValueEx(hkey,szValue,0,REG_DWORD,(LPBYTE)&dwTemp,sizeof(DWORD));
		RegCloseKey(hkey);
	}

	return retval;
}

HINSTANCE Star::Common::OpenUrl(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ( result<=(HINSTANCE)HINSTANCE_ERROR ) {
		if ( GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS ) {
			lstrcat(key, _T("\\shell\\open\\command"));
			if ( GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS ) {
				TCHAR *pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {                     // No quotes found
					pos = _tcsstr(key, _T("%1"));       // Check for %1, without quotes 
					if (pos == NULL){                   // No parameter at all...
						pos = key+lstrlen(key)-1;
					}else{
						*pos = _T('\0');                   // Remove the parameter
					}
				}else{
					*pos = _T('\0');                       // Remove the parameter
				}

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);

				result = (HINSTANCE)NULL + WinExec((LPCSTR)key,showcmd);
			}
		}
	}

	return result;
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明: 提高本进程权限,以取得系统进程的信息
------------------------------------------------------------------------*/
BOOL WINAPI Star::Common::EnableDebugPrivilege(BOOL bEnable) 
{
	// 附给本进程特权，用以取得系统进程的信息
	BOOL bOk = FALSE;    // Assume function fails
	HANDLE hToken;

	// 打开一个进程的访问令牌
	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, 
		&hToken)) 
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}
	return(bOk);
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明: dll远程注入到进程
------------------------------------------------------------------------*/
BOOL WINAPI Star::Common::RemoteLoadLibrary(LPCTSTR pszDllName, DWORD dwProcessId)
{
	// 试图打开目标进程
	HANDLE hProcess = ::OpenProcess(
		PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if(hProcess == NULL)
		return FALSE;


	// 在目标进程申请空间，存放字符串pszDllName，作为远程线程的参数
	int cbSize = (::lstrlen(pszDllName) + 1);
	LPVOID lpRemoteDllName = ::VirtualAllocEx(hProcess, NULL, cbSize, MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(hProcess, lpRemoteDllName, pszDllName, cbSize, NULL);

	// 取得LoadLibraryA函数的地址，我们将以它作为远程线程函数启动
	HMODULE hModule=::GetModuleHandle (_T("kernel32.dll"));
	LPTHREAD_START_ROUTINE pfnStartRoutine = 
		(LPTHREAD_START_ROUTINE)::GetProcAddress(hModule, "LoadLibraryA");


	// 启动远程线程
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnStartRoutine, lpRemoteDllName, 0, NULL);
	if(hRemoteThread == NULL){
		::CloseHandle(hProcess);
		return FALSE;
	}

	::CloseHandle(hRemoteThread);
	::CloseHandle(hProcess);

	return TRUE;
}


CString Star::Common::GetStartPath()
{
	TCHAR szTemp[MAX_PATH];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	_tcsrchr(szTemp,'\\')[1]=0;
	return szTemp;
}

CString Star::Common::GetMyTempPath()
{
	TCHAR szTemp[MAX_PATH];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	lstrcpy( _tcsrchr(szTemp,'\\')+1, _T("tmp\\") );
	if ( GetFileAttributes(szTemp)==-1 ){
		CreateDirectory(szTemp,NULL);
	}
	return szTemp;
}

//获取系统临时目录
CString Star::Common::GetSysTempPath()
{
	TCHAR szPath[MAX_PATH]={0};

	::GetTempPath( _countof(szPath), szPath );
	//::GetTempFileName(szPath,_T("tmp"),0,szFileName);
	return szPath;
}

//获取系统临时文件名
CString Star::Common::GetSysTempFileName()
{
	TCHAR szPath[MAX_PATH]={0};
	TCHAR szFileName[MAX_PATH]={0};

	::GetTempPath( _countof(szPath), szPath );
	::GetTempFileName(szPath,_T("tmp"),0,szFileName);
	return szFileName;
}

CString Star::Common::GetModuleFilePath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	return szTemp;
}

CString Star::Common::GetWindowsPath()
{
	TCHAR str[MAX_PATH]={};
	GetWindowsDirectory(str, MAX_PATH);
	return str;
}

CString Star::Common::GetSystemPath()
{
	TCHAR str[MAX_PATH]={0};
	int nLen=GetSystemDirectory(str, MAX_PATH);
	if ( nLen ){
		str[nLen]='\\';
	}
	return str;
}

/************************************************************************/  
/* 函数说明：释放资源中某类型的文件                                      
/* 参    数：新文件名、资源ID、资源类型                                  
/* 返 回 值：成功返回TRUE，否则返回FALSE   
/* By:Koma   2009.07.24 23:30                               
/************************************************************************/  
BOOL Star::Common::ReleaseRes(LPCTSTR szFileName, DWORD dwResID, LPCTSTR lpType)  
{  
	// 资源大小  
	DWORD dwWritten = 0; 
	BOOL bResult = FALSE;

	// 创建文件  
	HANDLE hFile=CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );  
	if ( hFile==INVALID_HANDLE_VALUE ){  
		return (::GetFileAttributes(szFileName)!=-1); 
	}  

	// 查找资源文件中、加载资源到内存、得到资源大小  
	HRSRC   hrsc = FindResource(NULL, MAKEINTRESOURCE(dwResID), lpType);  
	HGLOBAL hG = LoadResource(NULL, hrsc);  
	DWORD   dwSize = SizeofResource( NULL,  hrsc);  

	// 写入文件
	if ( hG && dwSize>0 ){	  
		WriteFile(hFile,hG,dwSize,&dwWritten,NULL);
		bResult = TRUE;
	}
	CloseHandle( hFile );

	return bResult;  
}


#pragma comment(lib,"Rpcrt4.lib")
int Star::Common::GenerateGUID(CString& sGUID)
{
	int status = 1;
	sGUID.Empty();

	// Create GUID

	UCHAR *pszUuid = 0; 
	GUID *pguid = NULL;
	pguid = new GUID;
	if(pguid!=NULL){
		HRESULT hr = CoCreateGuid(pguid);
		if(SUCCEEDED(hr)){
			// Convert the GUID to a string
			hr = UuidToString(pguid, &pszUuid);
			if(SUCCEEDED(hr) && pszUuid!=NULL){ 
				status = 0;
				sGUID = pszUuid;
				RpcStringFree(&pszUuid);
			}
		}
		delete pguid; 
	}

	return status;
}

void Star::Common::ConvertUtf8ToGBK(CString& strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	WCHAR * wszGBK = new WCHAR[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;

	return;
}

void Star::Common::ConvertGBKToUtf8(CString& strText)
{
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strText, -1, NULL,0);
	WCHAR * wszGBK = new WCHAR[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strText, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, wszGBK, -1, szGBK, len, NULL,NULL);

	strText = szGBK;
	delete[] szGBK;
	delete[] wszGBK;

	return;
}

CString Star::Common::BrowseFolder(HWND hWnd,LPCTSTR lpszTitle)  
{   
	char szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner=hWnd;
	bi.pidlRoot=NULL;
	bi.pszDisplayName=szDir;
	bi.lpszTitle=lpszTitle;
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn=NULL;
	bi.lParam=0;
	bi.iImage=0;
	pidl=SHBrowseForFolder(&bi);
	if(pidl=NULL){ 
		return _T("");
	}
	if( SHGetPathFromIDList(pidl, szDir)==FALSE ){
		return _T("");
	}

	return CString(szDir);  
}

//一个字符串将要作为文件名，去除这个字符串中非法的字符
//可能返回为空串，也可能这个字符串很合法但是太长了，这两条需要外部处理
CString Star::Common::RemoveFileNameInvalidChar(CString strText)
{
	if ( strText.IsEmpty()==TRUE ){
		return strText;
	}

	CStringW strFileNameW;
	strFileNameW=strText;
	strFileNameW.Replace(L"\r",L"");
	strFileNameW.Replace(L"\n",L"");
	strFileNameW.Replace(L"\t",L"");
	strFileNameW.Replace(L"\\",L"");
	strFileNameW.Replace(L"/",L"");
	strFileNameW.Replace(L":",L"");
	strFileNameW.Replace(L"*",L"");
	strFileNameW.Replace(L"?",L"");
	strFileNameW.Replace(L"\"",L"");
	strFileNameW.Replace(L"<",L"");
	strFileNameW.Replace(L">",L"");
	strFileNameW.Replace(L"|",L"");
	while ( strFileNameW.IsEmpty()==FALSE && strFileNameW.Right(1)=="." ){
		strFileNameW=strFileNameW.Left(strFileNameW.GetLength()-1);
	}
	strFileNameW.Trim();

	return (CString)strFileNameW;
}

//转义网页中的一些字符UrlEscape和UrlUnescape只能转换%的转义字符，如&-%26 空格-%20
//参考：http://www.cnblogs.com/anjou/archive/2007/03/15/676476.html
void Star::Common::EscapeXmlHtmlChar(CString&strText)
{
	strText.Replace("&amp;","&");
	strText.Replace("&lt;","<");
	strText.Replace("&gt;",">");
	strText.Replace("&quot;","\"");
	strText.Replace("&apos;","\'");
	strText.Replace("&nbsp;"," ");
	strText.Replace("&copy;"," ");
	strText.Replace("&reg;"," ");
	strText.Replace("&#47;","/");
	strText.Replace("&#60;","<");
	strText.Replace("&#62;",">");
	strText.Replace("&#38;","&");
}


//解析\u65F6\u5149\u5F71\u50CF\u9986
//参考jsoncpp中Reader::decodeUnicodeEscapeSequence
CStringW Star::Common::DecodeUnicodeEscapeSequence(const char *lpszText)
{
	CStringW strResult;
	if ( lpszText==NULL ){
		return strResult;
	}

	WCHAR unicode = 0;
	const char *pCurrent = lpszText;
	while ( *pCurrent ){
		if ( *pCurrent=='\\' ){
			switch ( pCurrent[1] )
			{
			case 'u':
				{
					pCurrent+=2;
					unicode = 0;
					for ( int index =0; index < 4; ++index ){
						char c = *pCurrent++;
						unicode *= 16;
						if ( c >= '0'  &&  c <= '9' )
							unicode += c - '0';
						else if ( c >= 'a'  &&  c <= 'f' )
							unicode += c - 'a' + 10;
						else if ( c >= 'A'  &&  c <= 'F' )
							unicode += c - 'A' + 10;
						else
							return strResult;
					}

					strResult.AppendChar(unicode);
				}
				break;
			case '"': 
			case '/': 
			case '\\':
			case 'b': 
			case 'f': 
			case 'n': 
			case 'r': 
			case 't':
				strResult.AppendChar(pCurrent[1]);
				pCurrent+=2;
				break;
			default:
				strResult.AppendChar(*pCurrent);
				++pCurrent;
				break;
			}

			
		}else{
			strResult.AppendChar(*pCurrent);
			++pCurrent;
		}
	}

	return strResult;
}

//转换百度账号中的中文字符，注意如果是中文和英文的混合则英文不变
CString Star::Common::GBKEncodeURI(CString strText)
{
	CString strResult;
	CString strTemp;
	char ch;
	for ( int i=0; i<strText.GetLength(); ++i ){
		ch=strText.GetAt(i);
		if ( ch<0 ){
			strTemp.Format("%X",ch);
			strResult+="%"+strTemp.Right(2);
		}else{
			strResult+=ch;
		}
	}

	return strResult;
}

//转换百度账号中的中文字符，一个中文字符转换为三个%。注意如果是中文和英文的混合则英文不变
CString Star::Common::UTF8EncodeURI(CString strText)
{
	if ( strText.IsEmpty()==TRUE ){
		return strText;
	}

	CString strResult;
	CString strTemp;
	char ch;

	Star::Common::ConvertGBKToUtf8(strText);
	for ( int i=0; i<strText.GetLength(); ++i ){
		ch=strText.GetAt(i);
		if ( ch<0 ){
			strTemp.Format("%X",ch);
			strResult+="%"+strTemp.Right(2);
			++i;
			ch=strText.GetAt(i);
			strTemp.Format("%X",ch);
			strResult+="%"+strTemp.Right(2);
			++i;
			ch=strText.GetAt(i);
			strTemp.Format("%X",ch);
			strResult+="%"+strTemp.Right(2);

		}else{
			strResult+=ch;
		}
	}

	return strResult;
}

BOOL Star::Common::IsSignalToStop(HANDLE hEvent)
{
	return ( ::WaitForSingleObject(hEvent,0)!=WAIT_OBJECT_0 );
}

void Star::Common::InitializeSeed()  
{  
	srand((unsigned)time(0));  
} 

//产生一个随机数范围：[Low，High]
int Star::Common::RandIntNum(int Low,int High)    
{  
	if(Low > High){  
		int nTemp=Low;
		Low=High;
		High=nTemp;
	}  

	int IntervalSize=High-Low+1;  
	int RandomOffset=rand()%IntervalSize;  
	return Low+RandomOffset;   
}

//产生一个0-1之间的随机浮点数字符串，小数点后面的位数由nCount定,不得超过MAX_PATH位
CString Star::Common::RandFloatNum(int nCount)
{
	CString strFloat;
	CString strText;
	if ( nCount<=0 || nCount>MAX_PATH ){
		return strFloat;
	}

	strFloat="0.";
	for ( int i=0; i<nCount; ++i ){
		strText.Format( "%d", Star::Common::RandIntNum(0,9) );
		strFloat+=strText;
	}

	return strFloat;
}

CString Star::Common::RandNum(int nCount)
{
	CString strFloat;
	CString strText;
	if ( nCount<=0 || nCount>MAX_PATH ){
		return strFloat;
	}

	for ( int i=0; i<nCount; ++i ){
		strText.Format( "%d", Star::Common::RandIntNum(0,9) );
		strFloat+=strText;
	}

	return strFloat;
}
//
//CString Star::Common::GetCpuId()
//{
//	unsigned long s0a,s0b,s0c,s0d;     
//	unsigned long s1a,s1b,s1c,s1d;     
//	unsigned long s2a,s2b,s2c,s2d;     
//	unsigned long s3a,s3b,s3c,s3d;     
//	unsigned long cpuid1,cpuid2;     
//	CString strCpuID;   
//	__asm{   
//		mov eax,00h   
//			cpuid   
//			mov s0a,eax   
//			mov s0b,ebx   
//			mov s0c,ecx   
//			mov s0d,edx   
//	}   
//	__asm{   
//		mov eax,01h   
//			cpuid   
//			mov s1a,eax   
//			mov s1b,ebx   
//			mov s1c,ecx   
//			mov s1d,edx   
//	}   
//	__asm{   
//		mov eax,02h   
//			cpuid   
//			mov s2a,eax   
//			mov s2b,ebx   
//			mov s2c,ecx   
//			mov s2d,edx   
//	}   
//	__asm{   
//		mov eax,03h   
//			cpuid   
//			mov s3a,eax   
//			mov s3b,ebx   
//			mov s3c,ecx   
//			mov s3d,edx   
//	}   
//
//	cpuid1=s0a+s0b+s1a+s1b+s2a+s2b+s3a+s3b;
//	cpuid2=s0c+s0d+s1c+s1d+s2c+s2d+s3c+s3d;
//	strCpuID.Format(_T("%08X%08X"),cpuid1,cpuid2);
//
//
//	//////////////////////////////////////////////////////////////////////////
//	DWORD veax;
//	DWORD deax;
//	DWORD debx;
//	DWORD decx;
//	DWORD dedx;
//
//	veax=0;
//	__asm
//	{
//		mov eax, veax
//			cpuid
//			mov deax, eax
//			mov debx, ebx
//			mov decx, ecx
//			mov dedx, edx
//	}
//
//	//获得CPU的制造商信息(Vender ID String)
//	char cVID[13]={0};			// 字符串，用来存储制造商信息
//	memcpy(cVID, &debx, 4); // 复制前四个字符到数组
//	memcpy(cVID+4, &dedx, 4); // 复制中间四个字符到数组
//	memcpy(cVID+8, &decx, 4); // 复制最后四个字符到数组
//
//	//获得CPU商标信息(Brand String)
//	veax=0x80000002;
//	__asm
//	{
//		mov eax, veax
//			cpuid
//			mov deax, eax
//			mov debx, ebx
//			mov decx, ecx
//			mov dedx, edx
//	}
//	veax=0x80000003;
//	__asm
//	{
//		mov eax, veax
//			cpuid
//			mov deax, eax
//			mov debx, ebx
//			mov decx, ecx
//			mov dedx, edx
//	}	
//	veax=0x80000004;
//	__asm
//	{
//		mov eax, veax
//			cpuid
//			mov deax, eax
//			mov debx, ebx
//			mov decx, ecx
//			mov dedx, edx
//	}
//
//	return strCpuID;
//}

void Star::Common::RestartApp()
{
	char szExePath[MAX_PATH];
	GetModuleFileName(NULL,szExePath,MAX_PATH);

	CString strCmdLine;
	strCmdLine.Format("\"%s\"",szExePath);

	STARTUPINFO si={};
	PROCESS_INFORMATION pi={};
	si.cb=sizeof(si);
	CreateProcess(NULL,(LPSTR)(LPCTSTR)strCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	::ExitProcess(0);
}

int Star::Common::SplitString(CString strSrc,CString strSep,vector<CString>&vtStr)
{	
	vtStr.clear();
	TCHAR *pToken=_tcstok( (LPTSTR)(LPCTSTR)strSrc, (LPCTSTR)strSep ); 
	while( pToken!=NULL ){ 
		vtStr.push_back(pToken);
		pToken=_tcstok( NULL, (LPCTSTR)strSep );
	}

	return (int)vtStr.size();
}

//http://zhidao.baidu.com/question/272885337.html
//设置代理       
//参数分别为：代理服务器地址，端口号，用户名，密码   
//用到的变量说明：   
//sess为CInternetSession变量   
//pConnect为CHttpConnection变量   
//pFile   为CHttpFile变量   
//int Star::Common::SetProxy(CInternetSession&sess,CHttpFile*pFile,char*szproxy, char*proxyUser, char* proxyPassword)   
//{   
//	INTERNET_PROXY_INFO   proxyinfo;   
//
//	try   
//	{   
//		proxyinfo.dwAccessType=INTERNET_OPEN_TYPE_PROXY;   
//		proxyinfo.lpszProxy=szproxy;   
//		proxyinfo.lpszProxyBypass=NULL;   
//
//		if   (!   sess.SetOption(INTERNET_OPTION_PROXY,   (LPVOID)&proxyinfo,   sizeof(INTERNET_PROXY_INFO)))   
//		{   
//			return   0;   
//		}   
//
//		if   (!pFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,   proxyUser,   strlen(proxyUser)   +   1))   
//		{   
//			return   0;   
//		}   
//
//		if   (!pFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,   proxyPassword,   strlen(proxyPassword)   +   1))   
//		{   
//			return   0;   
//		}   
//
//		return   1;   
//	}   
//	catch(...)   
//	{   
//		return   0;   
//	}   
//
//}

//在字符串中查找字符，pSub含有多个字符
int Star::Common::StrFindOneOf(char*pMain,char*pSub)
{
	int nRet=-1;
	char ch=0;

	for ( int i=0; ; ++i ){
		ch=pMain[i];
		if ( ch==0 ){
			break;
		}
		for ( int j=0; ; ++j ){
			if ( pSub[j]==0 ){
				break;
			}else if ( pSub[j]==ch ){
				return i;
			}
		}
	}

	return nRet;
}

//查找直到不是数字的字符，实际效率太慢啊啊啊啊，尽量少用，不要在循环中使用
int Star::Common::StrFindCharNotDigit(char*pMain)
{
	int nRet=-1;
	char ch=0;

	for ( int i=0; ; ++i ){
		ch=pMain[i];
		if ( ch==0 ){
			break;
		}

		if ( ch<'0' || ch>'9' ){
			return i;
		}
	}

	return nRet;
}


//实现关机的函数
//摘自：http://www.google.com/codesearch/p?hl=zh-CN#X_1VXq0XKpU/trunk/shareaza/Scheduler.cpp&q=ExitWindowsEx%20lang:c%2B%2B&sa=N&cd=3&ct=rc
BOOL Star::Common::SetShutdownRights()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process.
	if ( !OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return( FALSE );

	// Get the LUID for the shutdown privilege.
	LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid );

	tkp.PrivilegeCount = 1;  // One privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process.
	AdjustTokenPrivileges( hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0 );

	if ( GetLastError() != ERROR_SUCCESS )
		return FALSE;

	return TRUE;
}

BOOL Star::Common::ShutDownComputer(UINT ShutdownFlags)
{
	int ShutdownSuccess = ExitWindowsEx( ShutdownFlags, 
		SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED );
	return (ShutdownSuccess != 0);

	/*
	int ShutdownSuccess = 0;

	// Try 2000/XP way first
	ShutdownSuccess = InitiateSystemShutdownEx( NULL,_T("Shareaza Scheduled Shutdown\n\nA system shutdown was scheduled using Shareaza. The system will now shut down."), 30, FALSE, FALSE, SHTDN_REASON_FLAG_USER_DEFINED );

	// Fall back to 9x way if this does not work
	if ( !ShutdownSuccess && GetLastError() != ERROR_SHUTDOWN_IN_PROGRESS )
	{
	UINT ShutdownFlags = EWX_POWEROFF;
	DWORD dReason;
	dReason = ( SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED );
	ShutdownSuccess = ExitWindowsEx( ShutdownFlags, dReason );
	}
	return (ShutdownSuccess != 0);
	*/
}

CString Star::Common::GetCurrentTimeStr()
{
	//COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d_%H-%M-%S"));
	CTime time = CTime::GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %H:%M:%S");
	return str;
}


//////////////////////////////////////////////////////////////////////////

int Star::Common::WaitData(SOCKET hSocket,DWORD dwTime)
{
	fd_set stFdSet;
	timeval stTimeval;

	stFdSet.fd_count=1;
	stFdSet.fd_array[0]=hSocket;
	stTimeval.tv_usec=dwTime;
	stTimeval.tv_sec=0;
	return select(0,&stFdSet,NULL,NULL,&stTimeval);
}

//lpszHostAddress填:"time-a.timefreq.bldrdoc.gov"或者"132.163.4.101"都可以; 外部不需要调用htons(nHostPort)
//返回0表示失败,成功返回:从1900年1月1日起的秒数
ULONG Star::Common::GetInternetTime(LPCTSTR lpszHostAddress, UINT nHostPort,int nTimeOut/*=10000*/)
{
	ULONG ulTime = 0;

	WSADATA WSAData;
	::WSAStartup (MAKEWORD(2,0), &WSAData);	


	//创建套接字

	SOCKET hSocket = socket(AF_INET, SOCK_STREAM, 0);   
	if(hSocket == INVALID_SOCKET){   
		//OutputDebugString(_T("创建套接字失败\n"));
		WSACleanup();
		return ulTime;   
	}  


	//连接服务器

	SOCKADDR_IN sockAddr;   
	memset(&sockAddr,0,sizeof(sockAddr));   

	LPSTR lpszAscii = (LPTSTR)lpszHostAddress;   
	sockAddr.sin_family = AF_INET;   
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);   

	if (sockAddr.sin_addr.s_addr == INADDR_NONE){   
		LPHOSTENT lphost;   
		lphost = gethostbyname(lpszAscii);   
		if (lphost != NULL){  
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;   
		}else{   
			WSACleanup();
			return ulTime;   
		}   
	}   
	sockAddr.sin_port = htons((u_short)nHostPort);   

	if( connect(hSocket,(SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR ){   
		if ( hSocket != INVALID_SOCKET ){   
			closesocket(hSocket);   
			hSocket = INVALID_SOCKET;   
		}   

		//OutputDebugString("连接服务器失败\n");
		WSACleanup();
		return ulTime;   
	}   


	//接受数据

	while ( TRUE ){
		int ret= WaitData(hSocket,nTimeOut);
		if ( ret==SOCKET_ERROR ){
			break;
		}

		if ( ret && !recv(hSocket, (LPSTR)&ulTime, sizeof(ulTime), 0) ){	//与服务器断开链接
			//OutputDebugString("服务器断开连接\n");
			break;
		}

		if (ret){	//没有退出且ret不为零，说明是正常的发送与接收
			//ProcessServerCmd
			ulTime = ::ntohl (ulTime);	//大小端转换	
		}
	}

	WSACleanup();
	return ulTime; 
}


ULONG Star::Common::GetInternetTime(int nTimeOut/*=10000*/)
{
	ULONG ulTime = 0;
	TCHAR szTimeHosts[]={
		//_T("time-a.nist.gov\0")
		//_T("time-b.nist.gov\0")
		_T("time-a.timefreq.bldrdoc.gov\0")
		//_T("time-b.timefreq.bldrdoc\0")
		//_T("time-c.timefreq.bldrdoc.gov\0")
		_T("utcnist.colorado.edu\0")
		_T("time.nist.gov\0")
		//_T("time-nw.nist.gov\0")
		_T("nist1.datum.com\0")
		//_T("nist1-dc.glassey.com\0")
		//_T("nist1-ny.glassey.com\0")
		//_T("nist1-sj.glassey.com\0")
		//_T("nist1.aol-ca.truetime.com\0")
		//_T("nist1.aol-va.truetime.com\0")
		_T("\0")
	};

	TCHAR *p=szTimeHosts;
	while ( *p ){
		ulTime = GetInternetTime(p,IPPORT_TIMESERVER,nTimeOut);
		if ( ulTime ){
			break;
		}

		p+=_tcslen(p)+1;
	}



	return ulTime; 
}

//////////////////////////////////////////////////////////////////////////

//换行要用\r\n
void Star::Common::CopyToClipboard(LPCTSTR szText,int nCbSize/*=-1*/)
{
	if ( szText==NULL ){
		return;
	}

	if ( nCbSize==-1 ){
		nCbSize = lstrlen(szText)*sizeof(TCHAR);
	}

	HGLOBAL hClip = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,nCbSize+1);
	if ( hClip==NULL ){
		return;
	}

	char *pBuff = (char*)GlobalLock(hClip);
	if( pBuff==NULL ){
		GlobalFree(hClip);
		return;
	}

	memcpy(pBuff,szText,nCbSize);
	pBuff[nCbSize] = 0;
	GlobalUnlock(hClip);

	if ( OpenClipboard(NULL) ){
		EmptyClipboard();
		SetClipboardData( CF_TEXT,hClip);
		CloseClipboard();
	}
	
	GlobalFree(hClip);
}



#define OID_802_3_PERMANENT_ADDRESS        0x01010101
//#define OID_802_3_CURRENT_ADDRESS        0x01010102
#define IOCTL_NDIS_QUERY_GLOBAL_STATS    0x00170002

int Star::Common::GetMac(IN OUT TCHAR *szMac,IN int nBuffSize)
{
	int nError = -1;
	if ( szMac==NULL || nBuffSize<18 ){
		return nError;
	}

	HKEY hKey = NULL;
	HKEY hKey2 = NULL;
	TCHAR szKey[MAX_PATH],szBuffer[MAX_PATH];
	TCHAR szServiceName[MAX_PATH];
	TCHAR szFileName[MAX_PATH] = {0};
	DWORD dwRet = 0;
	DWORD dwType = 0;
	DWORD cbData = 0;
	DWORD cName = _countof(szBuffer);
	if ( RegOpenKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\"),&hKey)!=ERROR_SUCCESS ){
		return nError;
	}

	for ( int i=0; RegEnumKeyEx(hKey,i,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS; ++i, cName = _countof(szBuffer) ){
		_tcscpy_s(szKey,MAX_PATH,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\"));
		_tcscat_s(szKey,MAX_PATH,szBuffer);
		if ( RegOpenKey(HKEY_LOCAL_MACHINE,szKey,&hKey2)!=ERROR_SUCCESS ){
			continue;
		}

		dwType = REG_SZ;
		cbData = MAX_PATH*sizeof(TCHAR);
		if ( RegQueryValueEx(hKey2,_T("ServiceName"),NULL,&dwType,(LPBYTE)szServiceName,&cbData)==ERROR_SUCCESS ){
			//RegQueryValueEx(hKey2,_T("Description"),NULL,&dwType,(LPBYTE)szDescription,&cbData)

			//读取成功后关闭句柄
			RegCloseKey(hKey2);

			_tcscpy_s(szFileName,MAX_PATH,_T("\\\\.\\"));
			_tcscat_s(szFileName,MAX_PATH,szServiceName);
			HANDLE hFile = CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
			if ( hFile!=INVALID_HANDLE_VALUE ){
				DWORD dwInBuff = OID_802_3_PERMANENT_ADDRESS;
				BYTE outBuff[MAX_PATH];
				dwRet = DeviceIoControl(hFile,IOCTL_NDIS_QUERY_GLOBAL_STATS,&dwInBuff,sizeof(dwInBuff),outBuff,sizeof(outBuff),&cbData,NULL);

				//无论成功失败关闭文件句柄
				CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;

				if ( dwRet ){
					_stprintf_s(szMac,nBuffSize/sizeof(TCHAR),_T("%02X:%02X:%02X:%02X:%02X:%02X"),outBuff[0],outBuff[1],outBuff[2],outBuff[3],outBuff[4],outBuff[5]);
					nError = 0;
					break;
				}
			}
		}else{
			//读取失败关闭句柄
			RegCloseKey(hKey2);
		}        

	}//end for

	if ( hKey!=NULL ){
		RegCloseKey(hKey);
	}

	return nError;
}

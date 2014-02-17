#include "stdafx.h"
#include "Common.h"
#include <afxwin.h>
#include <atltime.h>
#include <ShellAPI.h>
#include <shlwapi.h>
#include <ShlObj.h>
#include <atlimage.h>
#include <sys/timeb.h>
#include <IPHlpApi.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

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
		TCHAR szString[25];
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

CString Star::Common::GetDesktopPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	SHGetSpecialFolderPath(0, szPath, CSIDL_DESKTOPDIRECTORY, FALSE );
	return szPath;
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

	WCHAR *pszUuid = 0; 
	GUID *pguid = NULL;
	pguid = new GUID;
	if(pguid!=NULL){
		HRESULT hr = CoCreateGuid(pguid);
		if(SUCCEEDED(hr)){
			// Convert the GUID to a string
			hr = UuidToStringW(pguid, (RPC_WSTR *)&pszUuid);
			if(SUCCEEDED(hr) && pszUuid!=NULL){ 
				status = 0;
				sGUID = pszUuid;
				RpcStringFreeW((RPC_WSTR *)&pszUuid);
			}
		}
		delete pguid; 
	}

	return status;
}

void Star::Common::ConvertUtf8ToGBK(CString& strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCTSTR)strUtf8, -1, NULL,0);
	WCHAR * wszGBK = new WCHAR[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCTSTR)strUtf8, -1, wszGBK, len);

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
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(LPCTSTR)strText, -1, NULL,0);
	WCHAR * wszGBK = new WCHAR[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(LPCTSTR)strText, -1, wszGBK, len);

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
	TCHAR szDir[MAX_PATH];
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
void Star::Common::unescapexml(CString&strText)
{
	strText.Replace(_T("&amp;"),_T("&"));
	strText.Replace(_T("&lt;"),_T("<"));
	strText.Replace(_T("&gt;"),_T(">"));
	strText.Replace(_T("&quot;"),_T("\""));
	strText.Replace(_T("&apos;"),_T("\'"));
	strText.Replace(_T("&nbsp;"),_T(" "));
	strText.Replace(_T("&copy;"),_T(" "));
	strText.Replace(_T("&reg;"),_T(" "));
	strText.Replace(_T("&#47;"),_T("/"));
	strText.Replace(_T("&#60;"),_T("<"));
	strText.Replace(_T("&#62;"),_T(">"));
	strText.Replace(_T("&#38;"),_T("&"));
}


//解析\u65F6\u5149\u5F71\u50CF\u9986
//参考jsoncpp中Reader::decodeUnicodeEscapeSequence
CString Star::Common::DecodeUnicodeEscapeSequence(const char *lpszText)
{
	CString strResult;
	CStringW strTemp;
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

					strTemp = unicode;
					strResult += strTemp;
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
	TCHAR ch;
	for ( int i=0; i<strText.GetLength(); ++i ){
		ch = strText.GetAt(i);
		if ( ch<0 ){
			strTemp.Format(_T("%X"),ch);
			strResult += _T("%") + strTemp.Right(2);
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
	TCHAR ch;

	Star::Common::ConvertGBKToUtf8(strText);
	for ( int i=0; i<strText.GetLength(); ++i ){
		ch=strText.GetAt(i);
		if ( ch<0 ){
			strTemp.Format(_T("%X"),ch);
			strResult+=_T("%")+strTemp.Right(2);
			++i;
			ch=strText.GetAt(i);
			strTemp.Format(_T("%X"),ch);
			strResult+=_T("%")+strTemp.Right(2);
			++i;
			ch=strText.GetAt(i);
			strTemp.Format(_T("%X"),ch);
			strResult+=_T("%")+strTemp.Right(2);

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

	strFloat=_T("0.");
	for ( int i=0; i<nCount; ++i ){
		strText.Format( _T("%d"), Star::Common::RandIntNum(0,9) );
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
		strText.Format( _T("%d"), Star::Common::RandIntNum(0,9) );
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
	TCHAR szExePath[MAX_PATH] = {0};
	GetModuleFileName(NULL,szExePath,MAX_PATH);

	CString strCmdLine;
	strCmdLine.Format(_T("\"%s\""),szExePath);

	STARTUPINFO si={};
	PROCESS_INFORMATION pi={};
	si.cb=sizeof(si);
	CreateProcess(NULL,(LPTSTR)(LPCTSTR)strCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	::ExitProcess(0);
}

int Star::Common::SplitString(const CString&strSrc,const CString&strSep,vector<CString>&vtStr)
{	
	vtStr.clear();
	TCHAR *pContext = NULL;

	TCHAR *pToken=_tcstok_s( (LPTSTR)(LPCTSTR)strSrc, (LPCTSTR)strSep, &pContext ); 
	while( pToken!=NULL ){ 
		vtStr.push_back(pToken);
		pToken=_tcstok_s( NULL, (LPCTSTR)strSep, &pContext );
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

CString Star::Common::GetCurrentTimeTotalSeconds()
{
	char szBuff[MAX_PATH] = {0};
	CTime time = CTime::GetCurrentTime();
	__int64 nTime = time.GetTime();
	return _i64toa(nTime,szBuff,10);
}

CString Star::Common::GetCurrentTimeTotalMiliSeconds()
{
	CString strText;
	CString strTemp;
	char szBuff[MAX_PATH] = {0};

	struct timeb t1;
	ftime(&t1);

	strText = _i64toa(t1.time,szBuff,10);
	strTemp.Format("%d",t1.millitm);

	return strText+strTemp;
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
ULONG Star::Common::GetInternetTime(char *lpszHostAddress, UINT nHostPort,int nTimeOut/*=10000*/)
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

	LPSTR lpszAscii = lpszHostAddress;   
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
	CHAR szTimeHosts[]={
		//_T("time-a.nist.gov\0")
		//_T("time-b.nist.gov\0")
		"time-a.timefreq.bldrdoc.gov\0"
		//_T("time-b.timefreq.bldrdoc\0")
		//_T("time-c.timefreq.bldrdoc.gov\0")
		"utcnist.colorado.edu\0"
		"time.nist.gov\0"
		//_T("time-nw.nist.gov\0")
		"nist1.datum.com\0"
		//_T("nist1-dc.glassey.com\0")
		//_T("nist1-ny.glassey.com\0")
		//_T("nist1-sj.glassey.com\0")
		//_T("nist1.aol-ca.truetime.com\0")
		//_T("nist1.aol-va.truetime.com\0")
		"\0"
	};

	CHAR *p=szTimeHosts;
	while ( *p ){
		ulTime = GetInternetTime(p,IPPORT_TIMESERVER,nTimeOut);
		if ( ulTime ){
			break;
		}

		p+=strlen(p)+1;
	}



	return ulTime; 
}

//////////////////////////////////////////////////////////////////////////

//换行要用\r\n
void Star::Common::CopyToClipboard(LPCTSTR szText,int nCbSize/*=-1*/,HWND hWndNewOwner/*=NULL*/)
{
	if ( szText==NULL ){
		return;
	}

	if ( nCbSize==-1 ){
		nCbSize = lstrlen(szText)*sizeof(TCHAR);
	}

	HGLOBAL hClip = GlobalAlloc(GHND|GMEM_SHARE,nCbSize+1);
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

	if ( OpenClipboard(hWndNewOwner) ){
		EmptyClipboard();
		SetClipboardData( CF_TEXT,hClip);
		CloseClipboard();
	}
	
	//注意：这里不能释放，否则会每隔一次设置不成功的。
	//GlobalFree(hClip);
}



#define OID_802_3_PERMANENT_ADDRESS        0x01010101
//#define OID_802_3_CURRENT_ADDRESS        0x01010102
#define IOCTL_NDIS_QUERY_GLOBAL_STATS    0x00170002




CString GetMacByAdapterName(const CString&strAdapterName)
{
	int nError = 0;
	DWORD dwRet = 0;
	DWORD cbData = 0;
	CString strMac;
	CString strDriverName;
	TCHAR szFormat[] = { '%', '0', '2', 'X', '%', '0', '2', 'X', '%', '0', '2', 'X', '%', '0', '2', 'X', '%', '0', '2', 'X', '%', '0', '2', 'X', '\0' };

	strDriverName = _T("\\\\.\\") + strAdapterName;
	HANDLE hFile = CreateFile(strDriverName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if ( hFile!=INVALID_HANDLE_VALUE ){
		DWORD dwInBuff = OID_802_3_PERMANENT_ADDRESS;
		BYTE outBuff[MAX_PATH] = {0};
		dwRet = DeviceIoControl(hFile,IOCTL_NDIS_QUERY_GLOBAL_STATS,&dwInBuff,sizeof(dwInBuff),outBuff,sizeof(outBuff),&cbData,NULL);

		//无论成功失败关闭文件句柄
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;

		if ( dwRet ){
			strMac.Format(szFormat,outBuff[0],outBuff[1],outBuff[2],outBuff[3],outBuff[4],outBuff[5]);
		}else{
			nError = GetLastError();
		}
	}else{
		nError = GetLastError();
	}

	return strMac;
}


CString GetMacByReg()
{
    int nError = 0;
	CString strMac;
	CString strText;

    HKEY hKey = NULL;
    HKEY hKey2 = NULL;
    TCHAR szKey[MAX_PATH],szBuffer[MAX_PATH];
	TCHAR szServiceName[MAX_PATH];
    TCHAR szDescription[MAX_PATH];
    DWORD dwType = 0;
	DWORD cbData = 0;
    DWORD cName = _countof(szBuffer);
	TCHAR szRegPath1[] = { 'S', 'O', 'F', 'T', 'W', 'A', 'R', 'E', '\\', 'M', 'i', 'c', 'r', 'o', 's', 'o', 'f', 't', '\\', 'W', 'i', 'n', 'd', 'o', 'w', 's', ' ', 'N', 'T', '\\', 'C', 'u', 'r', 'r', 'e', 'n', 't', 'V', 'e', 'r', 's', 'i', 'o', 'n', '\\', 'N', 'e', 't', 'w', 'o', 'r', 'k', 'C', 'a', 'r', 'd', 's', '\\', '\0' };
	TCHAR szServiceNameKey[] = { 'S', 'e', 'r', 'v', 'i', 'c', 'e', 'N', 'a', 'm', 'e', '\0' };
	TCHAR szDescriptionKey[] = { 'D', 'e', 's', 'c', 'r', 'i', 'p', 't', 'i', 'o', 'n', '\0' };
	TCHAR szvirtual[] = { 'v', 'i', 'r', 't', 'u', 'a', 'l', '\0' };

	if ( RegOpenKey(HKEY_LOCAL_MACHINE,szRegPath1,&hKey)!=ERROR_SUCCESS ){
        return strMac;
    }

    for ( int i=0; RegEnumKeyEx(hKey,i,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS; ++i, cName = _countof(szBuffer) ){
		_tcscpy_s(szKey,MAX_PATH,szRegPath1);
		_tcscat_s(szKey,MAX_PATH,szBuffer);
        if ( RegOpenKey(HKEY_LOCAL_MACHINE,szKey,&hKey2)!=ERROR_SUCCESS ){
            continue;
        }

        dwType = REG_SZ;
		cbData = sizeof(szDescription);
		memset(szDescription,0,sizeof(szDescription));
		RegQueryValueEx(hKey2,szDescriptionKey,NULL,&dwType,(LPBYTE)szDescription,&cbData);
		strText = szDescription;
		strText.MakeLower();
		if ( strText.Find(szvirtual)!=-1 ){
			//虚拟网卡
			RegCloseKey(hKey2);
			continue;
		}

		dwType = REG_SZ;
		cbData = sizeof(szServiceName);
        if ( RegQueryValueEx(hKey2,szServiceNameKey,NULL,&dwType,(LPBYTE)szServiceName,&cbData)==ERROR_SUCCESS ){

            //读取成功后关闭句柄
            RegCloseKey(hKey2);
			strMac = GetMacByAdapterName(szServiceName);
			if ( strMac.IsEmpty()==FALSE ){
				break;
			}
        }else{
			nError = GetLastError();
			//读取失败关闭句柄
			RegCloseKey(hKey2);
		}        

    }//end for

	if ( hKey!=NULL ){
		RegCloseKey(hKey);
	}

    return strMac;
}

/*
根据IP地址获取MAC
*/
CString GetMacByIP()
{
	CString strMac;
	WSADATA wsaData;
	if ( WSAStartup(MAKEWORD(2,0),&wsaData)!=0 ){
		return strMac;
	}

	char szName[MAX_PATH] = {0};
	int err = gethostname(szName, MAX_PATH);

	if( err==SOCKET_ERROR ) {
		WSACleanup();
		return strMac;
	}

	hostent *pHostent = gethostbyname(szName);
	ULONG IP = *(ULONG*)pHostent->h_addr_list[0];

	ULONG ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);
	IP_ADAPTER_INFO *pAdapterInfoEnum = NULL;
	IP_ADAPTER_INFO *pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
	if ( pAdapterInfo==NULL ){
		WSACleanup();
		return strMac;
	}

	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW ){
		// 缓冲区不够大
		delete[] pAdapterInfo;
		pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
		if ( pAdapterInfo==NULL ){
			WSACleanup();
			return strMac;
		}
	}

	pAdapterInfoEnum = pAdapterInfo;
	if( GetAdaptersInfo(pAdapterInfoEnum, &ulAdapterInfoSize) == ERROR_SUCCESS ){
		do{     //遍历所有适配器
			if( pAdapterInfoEnum->Type == MIB_IF_TYPE_ETHERNET ){
				// 判断是否为以太网接口
				//pAdapterInfoEnum->Description 是适配器描述
				//pAdapterInfoEnum->AdapterName 是适配器名称
				//pAdapterInfoEnum->Address      是Mac地址
				if ( inet_addr(pAdapterInfoEnum->IpAddressList.IpAddress.String) == IP ){
					strMac = GetMacByAdapterName(pAdapterInfoEnum->AdapterName);
					break;
				}
			}
			pAdapterInfoEnum = pAdapterInfoEnum->Next;
		}while(pAdapterInfoEnum);
	}

	delete []pAdapterInfo;

	WSACleanup();
	return strMac;
}

CString Star::Common::GetMac()
{
	CString strMac;

	strMac = GetMacByReg();
	if ( strMac.IsEmpty()==TRUE ){
		strMac = GetMacByIP();
	}

	return strMac;
}


//截屏 如果hwnd为NULL则截取整个桌面，否则只截取该窗口,文件保存到szFilePath
typedef BOOL (__stdcall *TPrintWindow)(
									   HWND hwnd,               // Window to copy
									   HDC  hdcBlt,             // HDC to print into
									   UINT nFlags              // Optional flags
									   );

DWORD Star::Common::CaptureScreen(HWND hWnd, LPCTSTR sSavePath)
{
	if ( sSavePath==NULL ){
		return -1;
	}

	BOOL bSuccess=FALSE;
	DWORD dwError = 0;

	int nWidth=0;
	int nHeight=0;
	HDC hDC = NULL;
	HDC hCompatDC = NULL;
	HBITMAP hBitmap = NULL;
	RECT rcWnd = {0};
	TCHAR szUser32dll[] = {'u','s','e','r','3','2','.','d','l','l','\0'};
	CHAR szPrintWindow[] = {'P','r','i','n','t','W','i','n','d','o','w','\0'};

	HMODULE hModule = GetModuleHandle(szUser32dll);
	TPrintWindow pfnPrintWindow = (TPrintWindow)GetProcAddress(hModule,szPrintWindow);

	if ( hWnd==0 || IsIconic(hWnd) || IsWindowVisible(hWnd)==FALSE ){
		nWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		int nLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
		int nTop = GetSystemMetrics(SM_YVIRTUALSCREEN);

		if ( nWidth==0 || nHeight==0 ){
			dwError = -2;
		}

		hDC = ::GetDC(NULL);
		hCompatDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
		SelectObject(hCompatDC, hBitmap);
		bSuccess = BitBlt(hCompatDC, 0, 0, nWidth, nHeight, hDC, nLeft, nTop, SRCCOPY|CAPTUREBLT);
		if ( bSuccess==FALSE ){
			dwError = -3;
		}
		ReleaseDC(NULL,hDC);
	}else{
		::GetWindowRect(hWnd,&rcWnd);
		nWidth=rcWnd.right - rcWnd.left;
		nHeight=rcWnd.bottom - rcWnd.top;
		if ( nWidth==0 || nHeight==0 ){
			dwError = -4;
		}

		hDC = ::GetWindowDC(hWnd);
		hCompatDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
		SelectObject(hCompatDC, hBitmap);
		if ( pfnPrintWindow!=NULL ){
			bSuccess = pfnPrintWindow(hWnd, hCompatDC, 0);	//此处用PrintWindow不会被遮挡，用BitBlt截图会被遮挡的
		}

		if ( bSuccess==FALSE ){
			bSuccess = BitBlt(hCompatDC, 0, 0, nWidth, nHeight, hDC, 0, 0, SRCCOPY|CAPTUREBLT);
		}

		if ( bSuccess==FALSE ){
			dwError = -5;
		}
		ReleaseDC(hWnd,hDC);
	}

	if ( hBitmap!=NULL ){
		CImage image;
		image.Attach(hBitmap);
		image.Save(sSavePath,Gdiplus::ImageFormatJPEG);
		dwError = 0;
	}else{
		dwError = -6;
	}

	return dwError;
}

//转换路径,\Device\HarddiskVolume2\ -》 D
CString Star::Common::DosDevicePath2LogicalPath(LPCTSTR lpszDosPath)
{
	CString strResult;

	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';

	if ( lpszDosPath==NULL || !GetLogicalDriveStrings(_countof(szTemp)-1, szTemp) ){
		return strResult;
	}


	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;

	do{
		// Copy the drive letter to the template string
		*szDrive = *p;

		// Look up each device name
		if ( QueryDosDevice(szDrive, szName, _countof(szName)) ){
			UINT uNameLen = (UINT)_tcslen(szName);

			if (uNameLen < MAX_PATH) 
			{
				bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

				if ( bFound ){
					// Reconstruct pszFilename using szTemp
					// Replace device path with DOS path
					TCHAR szTempFile[MAX_PATH];
					_stprintf_s(szTempFile,	TEXT("%s%s"), szDrive, lpszDosPath+uNameLen);
					strResult = szTempFile;
				}
			}
		}

		// Go to the next NULL character.
		while (*p++);
	} while (!bFound && *p); // end of string

	return strResult;
}

//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
//在win7 64系统下会自动映射到：HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run
//举例：CreateAutoRun(_T("myautoruntest"),_T("/auto"));
int Star::Common::CreateAutoRun( LPCTSTR szDispInfo, LPCTSTR szAutoRunCmd, BOOL bAdd )
{
	int nError = 0;
	HKEY hKey = NULL;   
	CString strFilePath;   
	GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	strFilePath.ReleaseBuffer(); 

	strFilePath = _T("\"") + strFilePath + _T("\" ") + szAutoRunCmd;

	RegOpenKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),&hKey);
	if ( hKey!=NULL ){
		if ( bAdd ){
			RegSetValueEx(hKey,szDispInfo,0,REG_SZ,(const BYTE *)(LPCTSTR)strFilePath,strFilePath.GetLength());
		}else{
			RegDeleteValue(hKey,szDispInfo);
		}
		RegCloseKey(hKey);
	}

	return 0;
} 


//////////////////////////////////////////////////////////////////////////
//功能:弹出文件的系统菜单
//strFilePath文件完整路径
//pt 弹出菜单的坐标
//////////////////////////////////////////////////////////////////////////
void Star::Common::PopExplorerMenu(HWND hWnd, const CString&strFilePath, const CPoint&pt)
{
	LPSHELLFOLDER	pDesktop = NULL;	//桌面IShellFolder接口
	LPSHELLFOLDER	pParent = NULL;		//父目录的IShellFolder接口
	LPITEMIDLIST	pid = NULL;			//目标文件PIDL
	LPITEMIDLIST	pidEnum = NULL;		//枚举文件用的
	CStringW		strFolder;
	CString			strFileName;

	int nPos = strFilePath.ReverseFind('\\');
	if ( nPos==-1 ){
		return;
	}
	strFolder = strFilePath.Left(nPos);
	strFileName = strFilePath.Mid(nPos+1);

	//获取桌面的IShellFolder接口
	if( FAILED(::SHGetDesktopFolder(&pDesktop)) ){		
		return;
	}

	//获取父目录的LPITEMIDLIST
	if( FAILED(pDesktop->ParseDisplayName(hWnd,NULL,(LPWSTR)(LPCWSTR)strFolder,0,&pid,0)) || pid == NULL ){		
		pDesktop->Release();
		return;
	}

	//获取父目录的IShellFolder接口
	if( FAILED(pDesktop->BindToObject(pid,NULL,IID_IShellFolder,(LPVOID*)&pParent)) ){			
		pDesktop->Release();
		return;
	}

	IEnumIDList*        pEnum = NULL;            //枚举文件夹用的
	BOOL                bFind = NULL;            //是否找到文件
	SHFILEINFO          sfi;                     //获取文件信息的结构体

	pParent->EnumObjects(hWnd,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN,&pEnum);
	while( pEnum->Next(1,&pidEnum,0)==S_OK ){			//枚举文件(因为直接调用ParseDisplayName获得LPITEMIDLIST的话,后面是不会成功的
		::SHGetFileInfo(LPCTSTR(pidEnum),0,&sfi,sizeof(SHFILEINFO),SHGFI_PIDL | SHGFI_DISPLAYNAME);
		if( strFileName.CompareNoCase(sfi.szDisplayName)==0 ){
			bFind = TRUE;								//找到了就为真
			break;
		}
	}

	if( bFind==FALSE ){           //没找到就返回
		pDesktop->Release();
		pParent->Release();
		return;
	}

	IContextMenu*    pContextMenu = NULL;
	if(!SUCCEEDED(pParent->GetUIObjectOf(hWnd,1,(LPCITEMIDLIST*)&pidEnum,
		IID_IContextMenu,NULL,(LPVOID*)&pContextMenu)))                //获取文件的IContextMenu接口
	{
		pDesktop->Release();
		pParent->Release();
		return;
	}

	CMenu menu;
	menu.CreatePopupMenu();
	if( !SUCCEEDED(pContextMenu->QueryContextMenu(menu.m_hMenu,menu.GetMenuItemCount(),
		1,0x7ff,CMF_NORMAL | CMF_EXPLORE)) )
	{
		pDesktop->Release();
		pParent->Release();
		pContextMenu->Release();
		return;
	}

	UINT icmd = 0;
	icmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,pt.x,pt.y,CWnd::FromHandle(hWnd));
	if( icmd ){
		CMINVOKECOMMANDINFO ici;
		ici.cbSize          = sizeof (CMINVOKECOMMANDINFO);
		ici.fMask           = 0;
		ici.hwnd            = hWnd;
		ici.lpVerb          = MAKEINTRESOURCE (icmd - 1);
		ici.lpParameters    = NULL;
		ici.lpDirectory     = NULL;
		ici.nShow           = SW_SHOWNORMAL;
		ici.dwHotKey        = 0;
		ici.hIcon           = NULL;
		pContextMenu->InvokeCommand(&ici);
	}
	menu.DestroyMenu();
	pContextMenu->Release();
	pDesktop->Release();
	pParent->Release();
}

CString Star::Common::FormatLastError(DWORD dwError/* = -1*/)
{
	CString strError;
	if ( dwError==-1 ) {
		dwError = ::GetLastError();
	}

	HLOCAL hlocal = NULL;   // Buffer that gets the error message string

	// Use the default system locale since we look for Windows messages.
	// Note: this MAKELANGID combination has 0 as value
	DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	// Get the error code's textual description
	BOOL fOk = FormatMessage(
							 FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
							 FORMAT_MESSAGE_ALLOCATE_BUFFER, 
							 NULL, dwError, systemLocale, 
							 (PTSTR) &hlocal, 0, NULL);

	if (!fOk) {
		// Is it a network-related error?
		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
			DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL) {
			fOk = FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				hDll, dwError, systemLocale,
				(PTSTR) &hlocal, 0, NULL);
			FreeLibrary(hDll);
		}
	}

	if (fOk && (hlocal != NULL)) {
		strError = (PCTSTR) LocalLock(hlocal);
		LocalFree(hlocal);
	} else {
		strError = TEXT("No text found for this error number.");
	}

	return strError;
}

void Star::Common::PathWithSlash(CString&strPath)
{
	if ( strPath.GetLength()>1 ) {
		strPath.Replace('/', '\\');
		if ( strPath.Right(1)!="\\" ) {
			strPath += "\\";	
		}
	}
}

CString Star::Common::GetFileExt(const CString&strFilePath)
{
	CString strExt;
	int nPos = strFilePath.ReverseFind('.');
	if ( nPos!=-1 ) {
		strExt = strFilePath.Mid(nPos);
	}

	return strExt;
}

void Star::Common::CreateDirRecursively(const CString&strRoot,const CString&strSubDir)
{
	CString strTemp;
	CString strPath = strSubDir;
	Star::Common::PathWithSlash(strPath);

	int nPos = 0;
	int nStart = strRoot.GetLength();
	while ( nStart<strPath.GetLength() ){
		nPos = strPath.Find('\\',nStart);
		if ( nPos==-1 ) {
			break;
		}

		strTemp = strPath.Left(nPos);
		if ( GetFileAttributes(strTemp)==-1 ) {
			CreateDirectory(strTemp,NULL);
		}
		nStart = nPos + 1;
	}
}

void Star::Common::DeleteDirectory(LPCTSTR lpszDirPath)
{
	CString strDirPath = lpszDirPath;
	Star::Common::PathWithSlash(strDirPath);

	WIN32_FIND_DATA stFindData = {0};
	HANDLE hFind = ::FindFirstFile(strDirPath + "*.*", &stFindData);
	if ( hFind!=INVALID_HANDLE_VALUE ) {

		do 
		{
			if ( stFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY ) {
				if ( stFindData.cFileName[0]!='.' ) {
					DeleteDirectory(strDirPath + stFindData.cFileName);
				}
			}else{
				::DeleteFile(strDirPath + stFindData.cFileName);
			}
		} while ( ::FindNextFile(hFind, &stFindData) );

		::FindClose(hFind);
	}

	::RemoveDirectory(lpszDirPath);
}

void Star::Common::Run(LPCTSTR lpszCmdLine, DWORD dwMilliseconds/* = INFINITE*/)
{
	CString strCmdLine = lpszCmdLine;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	::CreateProcess(NULL,(LPSTR)(LPCSTR)strCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	CloseHandle(pi.hThread);

	WaitForSingleObject(pi.hProcess, dwMilliseconds);   
	CloseHandle(pi.hProcess);
}

void Star::Common::FindFileRecursively(OUT vector<CString>&vtFiles, IN LPCTSTR lpszDirPath, IN LPCTSTR lpszFilter/* = NULL*/)
{
	CString strDirPath = lpszDirPath;
	CString strFindFilter;

	Star::Common::PathWithSlash(strDirPath);
	if ( lpszFilter==NULL ) {
		strFindFilter = "*.*";
	}else{
		strFindFilter = lpszFilter;
	}

	WIN32_FIND_DATA stFindData = {0};
	HANDLE hFind = ::FindFirstFile(strDirPath + strFindFilter, &stFindData);
	if ( hFind!=INVALID_HANDLE_VALUE ) {

		do 
		{
			if ( stFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY ) {
				if ( stFindData.cFileName[0]!='.' ) {
					FindFileRecursively(vtFiles, strDirPath + stFindData.cFileName, lpszFilter);
				}
			}else{
				vtFiles.push_back(strDirPath + stFindData.cFileName);
			}
		} while ( ::FindNextFile(hFind, &stFindData) );

		::FindClose(hFind);
	}

}

LONGLONG Star::Common::GetFileSize(LPCTSTR lpszFileName)
{
	LONGLONG llSize = 0;

	if ( lpszFileName==NULL ){
		return llSize;
	}

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,NULL,NULL);
	if ( hFile!=INVALID_HANDLE_VALUE ){
		GetFileSizeEx(hFile,(PLARGE_INTEGER)&llSize);
		CloseHandle(hFile);
	}

	return llSize;
}

DWORD Star::Common::GetFileSize(HANDLE hFile,LPDWORD lpFileSizeHigh)
{
	DWORD dwSize = 0;

	if ( hFile!=NULL && hFile!=INVALID_HANDLE_VALUE ){
		dwSize = ::GetFileSize(hFile,lpFileSizeHigh);
		if ( dwSize==INVALID_FILE_SIZE ){
			dwSize = 0;
		}
	}

	return dwSize;
}
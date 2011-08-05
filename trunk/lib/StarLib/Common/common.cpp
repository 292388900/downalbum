#include "stdafx.h"
#include "Common.h"
#include <ShellAPI.h>
#include <shlwapi.h>
#include <iphlpapi.h>

#pragma comment(lib,"Iphlpapi.lib")

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明:   打开指定的网页
------------------------------------------------------------------------*/
void Star::Common::OpenUrl(CString strHtmlFile,bool bShow)
{
	ShellExecute(NULL,_T("open"),_T("IEXPLORE"),strHtmlFile,NULL,bShow?SW_SHOWNORMAL:SW_HIDE);
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

HINSTANCE Star::Common::GotoURL(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ( result<= (HINSTANCE)HINSTANCE_ERROR ) 
	{
		if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
		{
			lstrcat(key, _T("\\shell\\open\\command"));

			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {                     // No quotes found
					pos = _tcsstr(key, _T("%1"));       // Check for %1, without quotes 
					if (pos == NULL)                   // No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = _T('\0');                   // Remove the parameter
				}
				else
					*pos = _T('\0');                       // Remove the parameter

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
说明:   Sleep一段时间返回,这段时间内仍然处理消息.
------------------------------------------------------------------------*/
BOOL Star::Common::SleepPumpMsg(IN DWORD dwMillseconds)
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

int Star::Common::MsgBox(TCHAR*szText,HWND hWnd,TCHAR*szCaption,UINT uType)
{
	return ::MessageBox(hWnd,szText,szCaption,uType);
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
	if(hRemoteThread == NULL)
	{
		::CloseHandle(hProcess);
		return FALSE;


	}

	::CloseHandle(hRemoteThread);
	::CloseHandle(hProcess);

	return TRUE;
}


CString Star::Common::GetStartPath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	StrRChr(szTemp,NULL,'\\')[1]=0;
	return szTemp;
}

CString Star::Common::GetMyTempPath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	lstrcpy( StrRChr(szTemp,NULL,'\\')+1, _T("tmp\\") );
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
	TCHAR str[MAX_PATH];
	if(0 == GetWindowsDirectory(str, MAX_PATH))
		return _T("");
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

CString Star::Common::GetCurrentPath()
{
	TCHAR str[MAX_PATH];
	if(0 == GetCurrentDirectory(MAX_PATH, str))
		return _T("");
	return str;
}



/*------------------------------------------------------------------------
[7/25/2009 xiaolin]
说明: 取类成员函数的地址.vc6版本.
------------------------------------------------------------------------*/
template <class ToType, class FromType>
void Star::Common::GetMemberFuncAddr_VC6(ToType&addr,FromType f)
{
	union 
	{
		FromType _f;
		ToType   _t;
	}ut;

	ut._f = f;

	addr = ut._t;
}

DWORD Star::Common::strtolSafely( const char* _str, int _Radix )
{
	DWORD dwRet = -1;
	__try{
		dwRet = strtol(_str, NULL, _Radix);
	}
	__except(1){
		dwRet = -1;
	}
	return dwRet;
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


/************************************************************************/  
/* 函数说明：释放资源中某类型的文件                                      
/* 参    数：新文件名、资源ID、资源类型                                  
/* 返 回 值：成功返回TRUE，否则返回FALSE   
/* By:Koma   2009.07.24 23:30                               
/************************************************************************/  
BOOL Star::Common::ReleaseRes(CString strFileName,WORD wResID,CString strFileType)  
{  
	// 资源大小  
	DWORD dwWrite=0;        

	// 创建文件  
	HANDLE hFile=CreateFile( strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );  
	if ( hFile == INVALID_HANDLE_VALUE ){  
		return (::GetFileAttributes(strFileName)!=-1); 
	}  

	// 查找资源文件中、加载资源到内存、得到资源大小  
	HRSRC   hrsc=FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);  
	HGLOBAL hG=LoadResource(NULL, hrsc);  
	DWORD   dwSize=SizeofResource( NULL,  hrsc);  

	// 写入文件  
	WriteFile(hFile,hG,dwSize,&dwWrite,NULL);     
	CloseHandle( hFile );  
	return TRUE;  
}


/************************************************************************
[3/23/2009 xiaolin]
函数名：DeleteDirectory
参数：  DirName:目录名(需要后面带\);bDeleteSelf:最终是否把这个目录也删除.
返回值：该目录下的文件总数
说明：	删除DirName路径下的所有文件和此文件夹(取决于bDeleteSelf)。
创建者:	朱星星
************************************************************************/
int Star::Common::DeleteDirectory(CString DirName,BOOL bDeleteSelf) 
{ 
	int i=0;
	CString strDir;
	CString strExistFile;
	CFileFind finder; 

	strDir.Format("%s*.*",DirName);
	BOOL bWorking = finder.FindFile(strDir); 
	while (bWorking){
		bWorking = finder.FindNextFile(); 
		//skip . and ..,skip if IsDirectory
		if (finder.IsDots()){
			continue;
		}else if (finder.IsDirectory()){
			DeleteDirectory(finder.GetFilePath()+"\\",true);
		}else { 
			i++;
			DeleteFile(finder.GetFilePath());
		} 
	}
	finder.Close();
	if (bDeleteSelf){
		RemoveDirectory(DirName);
	}
	return i; 
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

//转义网页中的一些字符
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

BOOL Star::Common::IsSignalToStop(CEvent *pEvent)
{
	if ( pEvent==NULL ){
		return FALSE;
	}

	return ( ::WaitForSingleObject(pEvent->m_hObject,0)!=WAIT_OBJECT_0 );
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

////////////////////////////////////////////////////////////////////////////
// 函数名： GetMacByCmd(char *lpszMac)
// 参数：
//      输入： void
//      输出： lpszMac,返回的MAC地址串
// 返回值：
//      TRUE:  获得MAC地址。
//      FALSE: 获取MAC地址失败。
// 过程：
//      1. 创建一个无名管道。
//      2. 创建一个IPCONFIG 的进程，并将输出重定向到管道。
//      3. 从管道获取命令行返回的所有信息放入缓冲区lpszBuffer。
//      4. 从缓冲区lpszBuffer中获得抽取出MAC串。
//
//  提示：可以方便的由此程序获得IP地址等其他信息。
//        对于其他的可以通过其他命令方式得到的信息只需改变strFetCmd 和
//        str4Search的内容即可。
///////////////////////////////////////////////////////////////////////////
CString Star::Common::GetMacByCmd()
{
	BOOL bOK=FALSE;
	CString strMac;
	CString strText;
	int nPos1=0;
	int nPos2=0;
	SECURITY_ATTRIBUTES sa;
	HANDLE hReadPipe,hWritePipe;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//创建管道
	bOK=CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if( bOK==FALSE ){
		return strMac;
	}

	//控制命令行窗口信息
	STARTUPINFO si;
	//返回进程信息
	PROCESS_INFORMATION pi;
	DWORD dwRead;

	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //隐藏命令行窗口
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//创建获取命令行进程
	bOK=CreateProcess(NULL,"ipconfig /all",NULL, NULL, TRUE, 0, NULL,
		NULL, &si, &pi );


	const int MAX_COMMAND_SIZE=10240;	//命令行输出缓冲大小
	char szBuffer[MAX_COMMAND_SIZE]={0};//放置命令行输出缓冲区

	if ( bOK==TRUE ){
		WaitForSingleObject (pi.hProcess, INFINITE);
		bOK=ReadFile(hReadPipe,szBuffer,MAX_COMMAND_SIZE,&dwRead,  0);
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if( bOK==TRUE ){
			strText=szBuffer;
			char*p=strstr(szBuffer,"本地连接:");
			if ( p==NULL ){
				p=StrRStrI(szBuffer,NULL,"Physical Address");
			}else{
				p=strstr(p,"Physical Address");
			}

			if ( p==NULL ){
				return strMac;
			}
			nPos1=strText.Find(":",p-szBuffer);
			if ( nPos1==-1 ){
				return strMac;
			}
			nPos2=strText.Find("\n",nPos1);
			if ( nPos2==-1 ){
				return strMac;
			}
			strMac=strText.Mid(nPos1+1,nPos2-nPos1-1);
			strMac.Trim();
		}

	}else{
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
	}

	return strMac;
}

/*
摘自网络：
GetAdaptersInfo 方法也不是十全十美，也存在些问题：
1）如何区分物理网卡和虚拟网卡；
2）如何区分无线网卡和有线网卡；
3）“禁用”的网卡获取不到。

关于问题1和问题2的处理办法是：
区分物理网卡和虚拟网卡：pAdapter->Description中包含"PCI"是：物理网卡。（试了3台机器可以）
区分无线网卡和有线网卡：pAdapter->Type为71的是：无线网卡。（试了2个无线网卡也可以）

sing解释：
区分区分物理网卡和虚拟网卡不能靠"PCI"，因为网卡型号有很多，如下以为网友的网卡型号是：Broadcom NetLink (TM) Fast Ethernet
下面的信息是我远程获取的。
而且pAdapter->Type为71也不是指无线网卡，如下该用户有无线网卡但是没获取到。

C:\Documents and Settings\Administrator>ipconfig/all

Windows IP Configuration

Host Name . . . . . . . . . . . . : GULL
Primary Dns Suffix  . . . . . . . :
Node Type . . . . . . . . . . . . : Broadcast
IP Routing Enabled. . . . . . . . : No
WINS Proxy Enabled. . . . . . . . : No
DNS Suffix Search List. . . . . . : domain

Ethernet adapter Bluetooth Network:

Media State . . . . . . . . . . . : Media disconnected
Description . . . . . . . . . . . : 蓝牙局域网接入服务器驱动程序
Physical Address. . . . . . . . . : 00-24-2C-F9-DF-6B

Ethernet adapter 无线网络连接:

Connection-specific DNS Suffix  . :
Description . . . . . . . . . . . : Broadcom 802.11g 网络适配器
Physical Address. . . . . . . . . : 00-21-00-D7-9D-67
Dhcp Enabled. . . . . . . . . . . : Yes
Autoconfiguration Enabled . . . . : Yes
IP Address. . . . . . . . . . . . : 192.168.1.203
Subnet Mask . . . . . . . . . . . : 255.255.255.0
Default Gateway . . . . . . . . . : 192.168.1.1
DHCP Server . . . . . . . . . . . : 192.168.1.1
DNS Servers . . . . . . . . . . . : 60.191.134.196
60.191.134.206
192.168.1.1
Lease Obtained. . . . . . . . . . : 2010年11月7日 星期日 下午 12:34:25
Lease Expires . . . . . . . . . . : 2010年11月7日 星期日 下午 15:34:25

Ethernet adapter 本地连接:

Connection-specific DNS Suffix  . : domain
Description . . . . . . . . . . . : Broadcom NetLink (TM) Fast Ethernet
Physical Address. . . . . . . . . : 00-1F-16-1A-68-3D
Dhcp Enabled. . . . . . . . . . . : Yes
Autoconfiguration Enabled . . . . : Yes
IP Address. . . . . . . . . . . . : 192.168.1.100
Subnet Mask . . . . . . . . . . . : 255.255.255.0
Default Gateway . . . . . . . . . : 192.168.1.1
DHCP Server . . . . . . . . . . . : 192.168.1.1
DNS Servers . . . . . . . . . . . : 60.191.134.196
60.191.134.206
Lease Obtained. . . . . . . . . . : 2010年11月7日 星期日 下午 12:56:43
Lease Expires . . . . . . . . . . : 2010年11月7日 星期日 下午 14:56:43

C:\Documents and Settings\Administrator>
//////////////////////////////////////////////////////////////////////////



顺 时针  21:48:53
Microsoft Windows [版本 6.1.7600]
版权所有 (c) 2009 Microsoft Corporation。保留所有权利。

C:\Users\admin> ipconfig/all

Windows IP 配置

主机名  . . . . . . . . . . . . . : PC
主 DNS 后缀 . . . . . . . . . . . :
节点类型  . . . . . . . . . . . . : 混合
IP 路由已启用 . . . . . . . . . . : 否
WINS 代理已启用 . . . . . . . . . : 否

以太网适配器 Bluetooth 网络连接 3:

媒体状态  . . . . . . . . . . . . : 媒体已断开
连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : Bluetooth 设备(个人区域网) #3
物理地址. . . . . . . . . . . . . : 11-11-11-11-11-11
DHCP 已启用 . . . . . . . . . . . : 是
自动配置已启用. . . . . . . . . . : 是

以太网适配器 本地连接:

连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : Realtek RTL8168C(P)/8111C(P) 系列 PCI-E
千兆以太网 NIC (NDIS 6.20)
物理地址. . . . . . . . . . . . . : 6C-F0-49-0B-2F-96
DHCP 已启用 . . . . . . . . . . . : 否
自动配置已启用. . . . . . . . . . : 是
本地链接 IPv6 地址. . . . . . . . : fe80::a815:54f4:27a8:43a1%11(首选)
IPv4 地址 . . . . . . . . . . . . : 10.10.8.2(首选)
子网掩码  . . . . . . . . . . . . : 255.0.0.0
默认网关. . . . . . . . . . . . . : 10.10.8.254
DHCPv6 IAID . . . . . . . . . . . : 242020425
DHCPv6 客户端 DUID  . . . . . . . : 00-01-00-01-13-58-E7-A3-6C-F0-49-0B-2F-96

DNS 服务器  . . . . . . . . . . . : 172.168.1.9
203.95.1.2
TCPIP 上的 NetBIOS  . . . . . . . : 已启用

以太网适配器 VirtualBox Host-Only Network:

连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : VirtualBox Host-Only Ethernet Adapter
物理地址. . . . . . . . . . . . . : 08-00-27-00-54-AF
DHCP 已启用 . . . . . . . . . . . : 否
自动配置已启用. . . . . . . . . . : 是
本地链接 IPv6 地址. . . . . . . . : fe80::4551:1a00:48e2:a95c%34(首选)
IPv4 地址 . . . . . . . . . . . . : 192.168.56.1(首选)
子网掩码  . . . . . . . . . . . . : 255.255.255.0
默认网关. . . . . . . . . . . . . :
DHCPv6 IAID . . . . . . . . . . . : 654835751
DHCPv6 客户端 DUID  . . . . . . . : 00-01-00-01-13-58-E7-A3-6C-F0-49-0B-2F-96

DNS 服务器  . . . . . . . . . . . : fec0:0:0:ffff::1%1
fec0:0:0:ffff::2%1
fec0:0:0:ffff::3%1
TCPIP 上的 NetBIOS  . . . . . . . : 已启用

隧道适配器 isatap.{CDD5BF42-C7D8-4691-8489-E54479C3DEEB}:

媒体状态  . . . . . . . . . . . . : 媒体已断开
连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : Microsoft ISATAP Adapter
物理地址. . . . . . . . . . . . . : 00-00-00-00-00-00-00-E0
DHCP 已启用 . . . . . . . . . . . : 否
自动配置已启用. . . . . . . . . . : 是

隧道适配器 isatap.{BB877FA4-700D-4927-8529-7CE6BDA0F110}:

媒体状态  . . . . . . . . . . . . : 媒体已断开
连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : Microsoft ISATAP Adapter #2
物理地址. . . . . . . . . . . . . : 00-00-00-00-00-00-00-E0
DHCP 已启用 . . . . . . . . . . . : 否
自动配置已启用. . . . . . . . . . : 是

隧道适配器 Teredo Tunneling Pseudo-Interface:

媒体状态  . . . . . . . . . . . . : 媒体已断开
连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : Teredo Tunneling Pseudo-Interface
物理地址. . . . . . . . . . . . . : 00-00-00-00-00-00-00-E0
DHCP 已启用 . . . . . . . . . . . : 否
自动配置已启用. . . . . . . . . . : 是

隧道适配器 isatap.{A99DF0AF-8CCA-49FC-863A-619A0E928BC9}:

媒体状态  . . . . . . . . . . . . : 媒体已断开
连接特定的 DNS 后缀 . . . . . . . :
描述. . . . . . . . . . . . . . . : Microsoft ISATAP Adapter #4
物理地址. . . . . . . . . . . . . : 00-00-00-00-00-00-00-E0
DHCP 已启用 . . . . . . . . . . . : 否
自动配置已启用. . . . . . . . . . : 是

//////////////////////////////////////////////////////////////////////////
C:\Documents and Settings\Administrator>ipconfig/all

Windows IP Configuration

Host Name . . . . . . . . . . . . : PC-201005091403
Primary Dns Suffix  . . . . . . . :
Node Type . . . . . . . . . . . . : Unknown
IP Routing Enabled. . . . . . . . : No
WINS Proxy Enabled. . . . . . . . : No

Ethernet adapter 本地连接:

Connection-specific DNS Suffix  . :
Description . . . . . . . . . . . : NVIDIA nForce 10/100 Mbps Ethernet
Physical Address. . . . . . . . . : 48-5B-39-D1-7A-58
Dhcp Enabled. . . . . . . . . . . : Yes
Autoconfiguration Enabled . . . . : Yes
Autoconfiguration IP Address. . . : 169.254.206.240
Subnet Mask . . . . . . . . . . . : 255.255.0.0
Default Gateway . . . . . . . . . :

PPP adapter 独立:

Connection-specific DNS Suffix  . :
Description . . . . . . . . . . . : WAN (PPP/SLIP) Interface
Physical Address. . . . . . . . . : 00-53-45-00-00-00
Dhcp Enabled. . . . . . . . . . . : No
IP Address. . . . . . . . . . . . : 112.111.186.9
Subnet Mask . . . . . . . . . . . : 255.255.255.255
Default Gateway . . . . . . . . . : 112.111.186.9
DNS Servers . . . . . . . . . . . : 218.104.128.106
58.22.96.66
NetBIOS over Tcpip. . . . . . . . : Disabled

收集的却是：Physical Address. . . . . . . . . : 00-53-45-00-00-00
这个应该过滤掉。
//////////////////////////////////////////////////////////////////////////

我想的方法是第一个IP_ADAPTER_INFO信息就作为用户的物理网卡,优先收集的是无线网卡mac地址，
其次是有线网卡的mac地址，最后才是虚拟网卡mac地址。
*/
CString Star::Common::GetMacByAdapter()
{
	CString strMac;
	CString strText;

	IP_ADAPTER_INFO AdapterInfo;
	IP_ADAPTER_INFO*pBuff=NULL;
	IP_ADAPTER_INFO*pAdapter=NULL;
	DWORD dwRetVal=0;
	ULONG ulOutBufLen=0;
	if( GetAdaptersInfo(&AdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS ){
		pBuff=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
		pAdapter=pBuff;
	}

	if( (dwRetVal=GetAdaptersInfo(pAdapter,&ulOutBufLen))==NO_ERROR ){
		while(pAdapter){
			if( StrStrI(pAdapter->Description,"VMware")==NULL		//vmware虚拟机网卡
				&& StrStrI(pAdapter->Description,"Microsoft")==NULL	//
				&& StrStrI(pAdapter->Description,"Win32")==NULL		//TAP-Win32 Adapter V9,某防火墙的虚拟网卡
				&& StrStrI(pAdapter->Description,"USB")==NULL		//电脑插上手机后多的无线网卡
				&& StrStrI(pAdapter->Description,"Virtual")==NULL	//Microsoft Virtual PC 虚拟机网卡
				&& StrStrI(pAdapter->Description,"PPP/SLIP")==NULL	//某个电视软件的虚拟网卡？
				&& StrStrI(pAdapter->Description,"Mobile")==NULL	//电脑插上手机后多个手机网卡
				&& pAdapter->AddressLength==6
				){//VMware的虚拟网卡
					//printf("------------------------------------------------------------n");
					//printf("AdapterName:t%sn",pAdapter->AdapterName);
					//printf("AdapterDesc:t%sn",pAdapter->Description);
					//printf("AdapterAddr:t");
					if (!( pAdapter->Address[0]==pAdapter->Address[1] &&
						pAdapter->Address[0]==pAdapter->Address[2] && 
						pAdapter->Address[0]==pAdapter->Address[3] )){
							for(UINT i=0;i<pAdapter->AddressLength;i++){
								strText.Format("%02X",pAdapter->Address[i]);
								strMac+=strText;
							}
							break;
					}
					//printf("AdapterType:t%dn",pAdapter->Type);
					//printf("IPAddress:t%sn",pAdapter->IpAddressList.IpAddress.String);
					//printf("IPMask:t%sn",pAdapter->IpAddressList.IpMask.String);

			}
			pAdapter=pAdapter->Next;
		}
	}else{
		//printf("CalltoGetAdaptersInfofailed.n");
	}

	if ( pBuff!=NULL ){
		free(pBuff);
	}

	return strMac;
}

BOOL IsLocalAdapter(IP_ADAPTER_INFO *pAdapter,DWORD dwFindType,BOOL bCheckPCI)
{
	BOOL ret_value = FALSE;

	if( StrStrI(pAdapter->Description,"VMware")!=NULL		//vmware虚拟机网卡
		|| StrStrI(pAdapter->Description,"Microsoft")!=NULL	//
		|| StrStrI(pAdapter->Description,"Win32")!=NULL		//TAP-Win32 Adapter V9,某防火墙的虚拟网卡
		|| StrStrI(pAdapter->Description,"USB")!=NULL		//电脑插上手机后多的无线网卡
		|| StrStrI(pAdapter->Description,"Virtual")!=NULL	//Microsoft Virtual PC 虚拟机网卡
		|| StrStrI(pAdapter->Description,"PPP/SLIP")!=NULL	//某个电视软件的虚拟网卡？
		|| StrStrI(pAdapter->Description,"Mobile")!=NULL	//电脑插上手机后多个手机网卡
		|| pAdapter->AddressLength!=6
		){//VMware的虚拟网卡
		return ret_value;
	}

#define NET_CARD_KEY _T("System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}")
	char szDataBuf[MAX_PATH+1];
	DWORD dwDataLen = MAX_PATH;
	DWORD dwType = REG_SZ;
	HKEY hNetKey = NULL;
	HKEY hLocalNet = NULL;

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NET_CARD_KEY, 0, KEY_READ, &hNetKey))
		return FALSE;
	wsprintf(szDataBuf, "%s\\Connection", pAdapter->AdapterName);
	if(ERROR_SUCCESS != RegOpenKeyEx(hNetKey ,szDataBuf ,0 ,KEY_READ, &hLocalNet))
	{
		RegCloseKey(hNetKey);
		return FALSE;
	}
	if (ERROR_SUCCESS != RegQueryValueEx(hLocalNet, "MediaSubType", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen))
	{
		goto ret;
	}
	if (*((DWORD *)szDataBuf)!=dwFindType)
		goto ret;
	dwDataLen = MAX_PATH;
	if (ERROR_SUCCESS != RegQueryValueEx(hLocalNet, "PnpInstanceID", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen))
	{
		goto ret;
	}

	//这个仍然不可靠，有个用户获取的是；{1A3E09BE-1E45-494B-9174-D7385B45BBF5}\NVNET_DEV0373\4&224BFD2D&0&00
	if ( bCheckPCI==TRUE ){
		if (strncmp(szDataBuf, "PCI", strlen("PCI")))
			goto ret;
	}

	ret_value = TRUE;

ret:
	RegCloseKey(hLocalNet);
	RegCloseKey(hNetKey);

	return ret_value;
}

CString Star::Common::GetLocalMAC()
{
	CString strMac;
	IP_ADAPTER_INFO *IpAdaptersInfo =NULL;
	IP_ADAPTER_INFO *IpAdaptersInfoHead =NULL;

	IpAdaptersInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GPTR, sizeof(IP_ADAPTER_INFO ));

	if (IpAdaptersInfo == NULL)
	{
		return strMac;
	}

	DWORD dwDataSize = sizeof( IP_ADAPTER_INFO );
	DWORD dwRetVal = GetAdaptersInfo(IpAdaptersInfo,&dwDataSize);

	if ( ERROR_SUCCESS != dwRetVal)
	{
		GlobalFree( IpAdaptersInfo );
		IpAdaptersInfo = NULL;

		if( ERROR_BUFFER_OVERFLOW == dwRetVal)
		{
			IpAdaptersInfo =(IP_ADAPTER_INFO *) GlobalAlloc( GPTR, dwDataSize );
			if (IpAdaptersInfo == NULL)
			{
				return strMac;
			}
			if ( ERROR_SUCCESS != GetAdaptersInfo( IpAdaptersInfo, &dwDataSize ))
			{
				GlobalFree( IpAdaptersInfo );
				return strMac;
			}

		}
		else
		{
			return strMac;
		}

	}

	//Save the head pointer of IP_ADAPTER_INFO structures list.
	IpAdaptersInfoHead = IpAdaptersInfo;

	do{
		if (IsLocalAdapter(IpAdaptersInfo,0x01,TRUE))	//本地连接物理网卡
		{
			strMac.Format("%02X%02X%02X%02X%02X%02X",
				IpAdaptersInfo->Address[0],
				IpAdaptersInfo->Address[1],
				IpAdaptersInfo->Address[2],
				IpAdaptersInfo->Address[3],
				IpAdaptersInfo->Address[4],
				IpAdaptersInfo->Address[5]
			);
			//
			break;
		}

		IpAdaptersInfo = IpAdaptersInfo->Next;

	}while (IpAdaptersInfo);

	if ( strMac.IsEmpty()==TRUE ){	//没有本地连接，只有无线连接的情况
		IpAdaptersInfo=IpAdaptersInfoHead;

		do{
			if (IsLocalAdapter(IpAdaptersInfo,0x02,TRUE))	//无线连接物理网卡
			{
				strMac.Format("%02X%02X%02X%02X%02X%02X",
					IpAdaptersInfo->Address[0],
					IpAdaptersInfo->Address[1],
					IpAdaptersInfo->Address[2],
					IpAdaptersInfo->Address[3],
					IpAdaptersInfo->Address[4],
					IpAdaptersInfo->Address[5]
				);
				//
				break;
			}

			IpAdaptersInfo = IpAdaptersInfo->Next;

		}while (IpAdaptersInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	//特殊意外
	//////////////////////////////////////////////////////////////////////////

	if ( strMac.IsEmpty()==TRUE ){
		IpAdaptersInfo=IpAdaptersInfoHead;

		do{
			if (IsLocalAdapter(IpAdaptersInfo,0x01,FALSE))	//本地连接物理网卡
			{
				strMac.Format("%02X%02X%02X%02X%02X%02X",
					IpAdaptersInfo->Address[0],
					IpAdaptersInfo->Address[1],
					IpAdaptersInfo->Address[2],
					IpAdaptersInfo->Address[3],
					IpAdaptersInfo->Address[4],
					IpAdaptersInfo->Address[5]
				);
				//
				break;
			}

			IpAdaptersInfo = IpAdaptersInfo->Next;

		}while (IpAdaptersInfo);
	}

	if ( strMac.IsEmpty()==TRUE ){	//没有本地连接，只有无线连接的情况
		IpAdaptersInfo=IpAdaptersInfoHead;

		do{
			if (IsLocalAdapter(IpAdaptersInfo,0x02,FALSE))	//无线连接物理网卡
			{
				strMac.Format("%02X%02X%02X%02X%02X%02X",
					IpAdaptersInfo->Address[0],
					IpAdaptersInfo->Address[1],
					IpAdaptersInfo->Address[2],
					IpAdaptersInfo->Address[3],
					IpAdaptersInfo->Address[4],
					IpAdaptersInfo->Address[5]
				);
				//
				break;
			}

			IpAdaptersInfo = IpAdaptersInfo->Next;

		}while (IpAdaptersInfo);
	}

	if (IpAdaptersInfoHead != NULL)
	{
		GlobalFree( IpAdaptersInfoHead );
	}

	if ( strMac.IsEmpty()==TRUE ){
		strMac=Star::Common::GetMacByAdapter();
	}

	return strMac;
}

CString Star::Common::GetCpuId()
{
	unsigned long s0a,s0b,s0c,s0d;     
	unsigned long s1a,s1b,s1c,s1d;     
	unsigned long s2a,s2b,s2c,s2d;     
	unsigned long s3a,s3b,s3c,s3d;     
	unsigned long cpuid1,cpuid2;     
	CString strCpuID;   
	__asm{   
		mov eax,00h   
			cpuid   
			mov s0a,eax   
			mov s0b,ebx   
			mov s0c,ecx   
			mov s0d,edx   
	}   
	__asm{   
		mov eax,01h   
			cpuid   
			mov s1a,eax   
			mov s1b,ebx   
			mov s1c,ecx   
			mov s1d,edx   
	}   
	__asm{   
		mov eax,02h   
			cpuid   
			mov s2a,eax   
			mov s2b,ebx   
			mov s2c,ecx   
			mov s2d,edx   
	}   
	__asm{   
		mov eax,03h   
			cpuid   
			mov s3a,eax   
			mov s3b,ebx   
			mov s3c,ecx   
			mov s3d,edx   
	}   

	cpuid1=s0a+s0b+s1a+s1b+s2a+s2b+s3a+s3b;
	cpuid2=s0c+s0d+s1c+s1d+s2c+s2d+s3c+s3d;
	strCpuID.Format(_T("%08X%08X"),cpuid1,cpuid2);


	//////////////////////////////////////////////////////////////////////////
	DWORD veax;
	DWORD deax;
	DWORD debx;
	DWORD decx;
	DWORD dedx;

	veax=0;
	__asm
	{
		mov eax, veax
			cpuid
			mov deax, eax
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
	}

	//获得CPU的制造商信息(Vender ID String)
	char cVID[13]={0};			// 字符串，用来存储制造商信息
	memcpy(cVID, &debx, 4); // 复制前四个字符到数组
	memcpy(cVID+4, &dedx, 4); // 复制中间四个字符到数组
	memcpy(cVID+8, &decx, 4); // 复制最后四个字符到数组

	//获得CPU商标信息(Brand String)
	veax=0x80000002;
	__asm
	{
		mov eax, veax
			cpuid
			mov deax, eax
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
	}
	veax=0x80000003;
	__asm
	{
		mov eax, veax
			cpuid
			mov deax, eax
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
	}	
	veax=0x80000004;
	__asm
	{
		mov eax, veax
			cpuid
			mov deax, eax
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
	}

	return strCpuID;
}

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

int Star::Common::SplitString(CString strSrc,CString strSep,CStringList&strList)
{	
	int nCnt=0;
	strList.RemoveAll();
	TCHAR *pToken=_tcstok( (LPTSTR)(LPCTSTR)strSrc, (LPCTSTR)strSep ); 
	while( pToken!=NULL ){ 
		strList.AddTail(pToken);
		++nCnt; 
		pToken=_tcstok( NULL, (LPCTSTR)strSep );
	}

	return nCnt;
}

int Star::Common::SplitString(CString strSrc,CString strSep,CStringArray&strArr)
{
	int nCnt=0;
	strArr.RemoveAll();
	TCHAR *pToken=_tcstok( (LPTSTR)(LPCTSTR)strSrc, (LPCTSTR)strSep ); 
	while( pToken!=NULL ){ 
		strArr.Add(pToken);
		++nCnt; 
		pToken=_tcstok( NULL, (LPCTSTR)strSep );
	}

	return nCnt;
}

//http://zhidao.baidu.com/question/272885337.html
//设置代理       
//参数分别为：代理服务器地址，端口号，用户名，密码   
//用到的变量说明：   
//sess为CInternetSession变量   
//pConnect为CHttpConnection变量   
//pFile   为CHttpFile变量   
int Star::Common::SetProxy(CInternetSession&sess,CHttpFile*pFile,char*szproxy, char*proxyUser, char* proxyPassword)   
{   
	INTERNET_PROXY_INFO   proxyinfo;   

	try   
	{   
		proxyinfo.dwAccessType=INTERNET_OPEN_TYPE_PROXY;   
		proxyinfo.lpszProxy=szproxy;   
		proxyinfo.lpszProxyBypass=NULL;   

		if   (!   sess.SetOption(INTERNET_OPTION_PROXY,   (LPVOID)&proxyinfo,   sizeof(INTERNET_PROXY_INFO)))   
		{   
			return   0;   
		}   

		if   (!pFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,   proxyUser,   strlen(proxyUser)   +   1))   
		{   
			return   0;   
		}   

		if   (!pFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,   proxyPassword,   strlen(proxyPassword)   +   1))   
		{   
			return   0;   
		}   

		return   1;   
	}   
	catch(...)   
	{   
		return   0;   
	}   

}

CString PostData_common(CString server,CString dir,CString strHeaders,CString strSendData)
{
	CInternetSession m_InetSession("session");
	CHttpConnection*pServer =NULL;
	CHttpFile*pFile =NULL;
	CString m_strHtml="";

	try{
		pServer =m_InetSession.GetHttpConnection(server);
		pFile =pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,dir);
		pFile->AddRequestHeaders(strHeaders);

		pFile->SendRequestEx(strSendData.GetLength());
		pFile->WriteString(strSendData);
		pFile->EndRequest();
		//pFile->SendRequest(NULL,0,(LPVOID)(LPCTSTR)strSendData,strSendData.GetLength()); 

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet==HTTP_STATUS_OK){
			ULONGLONG nLen=pFile->GetLength();//不可靠，返回的大小太小了
			char *buff=new char[1024];
			memset(buff,0,1024);
			int nReaded=0;
			int nTotalSize=0;
			while ( TRUE ){
				nReaded=pFile->Read(&buff[nTotalSize],1024);
				nTotalSize+=nReaded;
				if ( nReaded<1024 ){
					break;
				}
			}

			buff[nTotalSize]=0;
			m_strHtml=buff;
			//Star::Common::ConvertUtf8ToGBK(m_strHtml);
			delete[] buff;
		}
		delete pFile;
		delete pServer;

	}catch (CInternetException*e){
		CString s;
		TCHAR szCause[MAX_PATH];
		e->GetErrorMessage(szCause,MAX_PATH);
		s.Format("InternetException：\n%s\n m_dwError%u,m_dwContextError%u",szCause,e->m_dwError,e->m_dwContext);
		//AfxMessageBox(s);
		e->Delete();
		m_strHtml.Empty();
	}

	m_strHtml.Trim();
	return m_strHtml;
}

CString Star::Common::Certify(CString strHostServer,CString strPage,CString strName,CString strPswd,CString strAppName)
{
	CString strSendData;
	CString strHeaders;
	CString strReferer;
	CString strCookies;
	CString strText;

	strAppName.MakeLower();
	strSendData.Format("name=%s&pswd=%s&app=%s",strName,strPswd,strAppName);
	strReferer="";
	strCookies="";
	strHeaders.Format(
		"Host: %s \n"
		"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.12) Gecko/20101026 Firefox/3.6.12 \n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \n"
		"Accept-Language: zh-cn,zh;q=0.5 \n"
		"Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7 \n"
		"Keep-Alive: 115 \n"
		"Connection: keep-alive \n"
		"Referer: http://%s/url/certify.html \n"
		"Content-Type: application/x-www-form-urlencoded \n"
		"Content-Length: %d \n"
		"Accept-Encoding:  \n"
		"Cookie:  \n",
		strHostServer,strHostServer,strSendData.GetLength());

	strText=PostData_common(strHostServer,strPage,strHeaders,strSendData);
	return strText;
}

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
	CTime time = CTime::GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %H:%M:%S");
	return str;
}
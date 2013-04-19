 
#pragma once
#include <afx.h>
#include <atlstr.h>
#include <vector>
using namespace std;



// _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
// _CrtSetBreakAlloc(118);	

/*------------------------------------------------------------------------
[7/25/2009 xiaolin]
说明: 取类成员函数的地址.vc8版本.可以取私有成员函数地址.
使用示例:

DWORD dwAddr;
#if _MSC_VER >1200
GetMemberFuncAddr_VC8(dwAddr,obj::FuncMember);
#else
Star::Common::GetMemberFuncAddr_VC6(dwAddr,&obj::FuncMember);
#endif
------------------------------------------------------------------------*/
// 
// #define GetMemberFuncAddr_VC8(FuncAddr,FuncType)\
// {	\
// 	__asm{mov eax,offset FuncType};\
// 	__asm{mov FuncAddr, eax};\
// }

void _Trace(const CHAR* lpszFormat, ...);
void _TraceW(const CHAR* lpszFormat, ...);
#define Trace _Trace
#define TraceW _TraceW

namespace Star
{
	namespace Common
	{
		HINSTANCE OpenUrl(LPCTSTR url, int showcmd=SW_SHOWNORMAL);
		BOOL WINAPI EnableDebugPrivilege(BOOL bEnable);
		BOOL WINAPI RemoteLoadLibrary(LPCTSTR pszDllName, DWORD dwProcessId);
		CString GetStartPath();
		CString GetMyTempPath();
		CString GetSysTempPath();
		CString GetSysTempFileName();
		CString GetModuleFilePath();
		CString GetWindowsPath();
		CString GetSystemPath();
		CString GetDesktopPath();

		BOOL MsgSleep(IN DWORD dwMillseconds);
		LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
		LONG GetRegValueDWord(HKEY key, LPCTSTR subkey, LPCTSTR szValue,DWORD*pData);
		LONG SetRegValueDWord(HKEY key, LPCTSTR subkey, LPCTSTR szValue,DWORD dwData,BOOL bForce=TRUE);

		//释放资源中某类型的文件 
		BOOL ReleaseRes(LPCTSTR szFileName, DWORD dwResID, LPCTSTR lpType);  

		int GenerateGUID(CString& sGUID);
		void ConvertUtf8ToGBK(CString& strUtf8);
		void ConvertGBKToUtf8(CString& strText);
		CString BrowseFolder(HWND hWnd,LPCTSTR lpszTitle);

		//一个字符串将要作为文件名，去除这个字符串中非法的字符
		//可能返回为空串，也可能这个字符串很合法但是太长了，这两条需要外部处理
		CString RemoveFileNameInvalidChar(CString strText);

		//转义网页中的一些字符
		void unescapexml(CString&strText);

		//解析\u65F6\u5149\u5F71\u50CF\u9986
		CString DecodeUnicodeEscapeSequence(const char *lpszText);

		//转换百度账号中的中文字符，一个中文字符转换为两个%。注意如果是中文和英文的混合则英文不变
		CString GBKEncodeURI(CString strText);
		//转换百度账号中的中文字符，一个中文字符转换为三个%。注意如果是中文和英文的混合则英文不变
		CString UTF8EncodeURI(CString strText);

		BOOL IsSignalToStop(HANDLE hEvent);

		void InitializeSeed();
		//产生一个随机数范围：[Low，High]
		int RandIntNum(int Low,int High);    

		//产生一个0-1之间的随机浮点数字符串，小数点后面的位数由nCount定,不得超过MAX_PATH位
		CString RandFloatNum(int nCount);
		CString RandNum(int nCount);

		void RestartApp();

		int SplitString(const CString&strSrc,const CString&strSep,vector<CString>&vtStr);

		//int SetProxy(CInternetSession&sess,CHttpFile*pFile,char*szproxy, char*proxyUser, char* proxyPassword);   
		
		//在字符串中查找字符，pSub含有多个字符
		int StrFindOneOf(char*pMain,char*pSub);
		//查找直到不是数字的字符
		int StrFindCharNotDigit(char*pMain);

		//实现关机的函数,借鉴：http://shareaza.svn.sourceforge.net/svnroot/shareaza
		BOOL SetShutdownRights();
		//默认关机
		BOOL ShutDownComputer(UINT ShutdownFlags=EWX_POWEROFF);
		
		CString GetCurrentTimeStr();
		CString GetCurrentTimeTotalSeconds();
		CString GetCurrentTimeTotalMiliSeconds();

		//////////////////////////////////////////////////////////////////////////
		int WaitData(SOCKET hSocket,DWORD dwTime);
		ULONG GetInternetTime(char *lpszHostAddress, UINT nHostPort,int nTimeOut=10000);
		ULONG GetInternetTime(int nTimeOut=10000);
		//////////////////////////////////////////////////////////////////////////
		void CopyToClipboard(LPCTSTR szText,int nCbSize=-1,HWND hWndNewOwner=NULL);

		CString GetMac();

		//截屏 如果hwnd为NULL则截取整个桌面，否则只截取该窗口,文件保存到szFilePath
		DWORD CaptureScreen(HWND hWnd, LPCTSTR sSavePath);

		//转换路径,\Device\HarddiskVolume2\ -》 D
		CString DosDevicePath2LogicalPath(LPCTSTR lpszDosPath);

		int CreateAutoRun( LPCTSTR szDispInfo, LPCTSTR szAutoRunCmd, BOOL bAdd=TRUE );

		//弹出文件的系统菜单
		void PopExplorerMenu(HWND hWnd, const CString&strFilePath, const CPoint&pt);
	}
}
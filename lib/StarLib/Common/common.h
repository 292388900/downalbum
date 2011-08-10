 
#pragma once
#include <afx.h>
#include <afxmt.h>
#include <afxinet.h>


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

namespace Star
{
	namespace Common
	{
		void OpenUrl(CString strHtmlFile,bool bShow=true);
		HINSTANCE GotoURL(LPCTSTR url, int showcmd=SW_SHOWNORMAL);
		BOOL SleepPumpMsg(IN DWORD dwMillseconds);
		int MsgBox(TCHAR*szText=NULL,HWND hWnd=NULL,TCHAR*szCaption=NULL,UINT uType=NULL);
		BOOL WINAPI EnableDebugPrivilege(BOOL bEnable);
		BOOL WINAPI RemoteLoadLibrary(LPCTSTR pszDllName, DWORD dwProcessId);
		CString GetStartPath();
		CString GetMyTempPath();
		CString GetSysTempPath();
		CString GetSysTempFileName();
		CString GetModuleFilePath();
		CString GetWindowsPath();
		CString GetSystemPath();
		CString GetCurrentPath();
		
		
		template <class ToType, class FromType>
		void GetMemberFuncAddr_VC6(ToType&addr,FromType f);

		DWORD strtolSafely( const char* _str, int _Radix );

		BOOL MsgSleep(IN DWORD dwMillseconds);

		LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
		LONG GetRegValueDWord(HKEY key, LPCTSTR subkey, LPCTSTR szValue,DWORD*pData);
		LONG SetRegValueDWord(HKEY key, LPCTSTR subkey, LPCTSTR szValue,DWORD dwData,BOOL bForce=TRUE);

		//释放资源中某类型的文件 
		BOOL ReleaseRes(CString strFileName,WORD wResID,CString strFileType);  

		//删除DirName路径下的所有文件和此文件夹(取决于bDeleteSelf)
		int DeleteDirectory(CString DirName,BOOL bDeleteSelf=false);

		int GenerateGUID(CString& sGUID);
		void ConvertUtf8ToGBK(CString& strUtf8);
		void ConvertGBKToUtf8(CString& strText);
		CString BrowseFolder(HWND hWnd,LPCTSTR lpszTitle);

		//一个字符串将要作为文件名，去除这个字符串中非法的字符
		//可能返回为空串，也可能这个字符串很合法但是太长了，这两条需要外部处理
		CString RemoveFileNameInvalidChar(CString strText);

		//转义网页中的一些字符
		void EscapeXmlHtmlChar(CString&strText);

		//转换百度账号中的中文字符，一个中文字符转换为两个%。注意如果是中文和英文的混合则英文不变
		CString GBKEncodeURI(CString strText);
		//转换百度账号中的中文字符，一个中文字符转换为三个%。注意如果是中文和英文的混合则英文不变
		CString UTF8EncodeURI(CString strText);

		BOOL IsSignalToStop(CEvent *pEvent);

		void InitializeSeed();
		//产生一个随机数范围：[Low，High]
		int RandIntNum(int Low,int High);    

		//产生一个0-1之间的随机浮点数字符串，小数点后面的位数由nCount定,不得超过MAX_PATH位
		CString RandFloatNum(int nCount);
		CString RandNum(int nCount);
			
		CString GetLocalMAC();
		CString GetMacByCmd();
		CString GetMacByAdapter();
		CString GetCpuId();

		void RestartApp();

		int SplitString(CString strSrc,CString strSep,CStringList&strList);
		int SplitString(CString strSrc,CString strSep,CStringArray&strArr);

		int SetProxy(CInternetSession&sess,CHttpFile*pFile,char*szproxy, char*proxyUser, char* proxyPassword);   
		
		CString PostData_common(const CString&server,const CString&dir,const CString&strHeaders,const CString&strSendData);
		CString Certify(const CString&strHostServer,const CString&strPage,const CString&strName,const CString&strPswd,CString strAppName);

		//在字符串中查找字符，pSub含有多个字符
		int StrFindOneOf(char*pMain,char*pSub);
		//查找直到不是数字的字符
		int StrFindCharNotDigit(char*pMain);

		//实现关机的函数,借鉴：http://shareaza.svn.sourceforge.net/svnroot/shareaza
		BOOL SetShutdownRights();
		//默认关机
		BOOL ShutDownComputer(UINT ShutdownFlags=EWX_POWEROFF);
		
		CString GetCurrentTimeStr();
	}
}
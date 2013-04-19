 
#pragma once
#include <afx.h>
#include <atlstr.h>
#include <vector>
using namespace std;



// _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
// _CrtSetBreakAlloc(118);	

/*------------------------------------------------------------------------
[7/25/2009 xiaolin]
˵��: ȡ���Ա�����ĵ�ַ.vc8�汾.����ȡ˽�г�Ա������ַ.
ʹ��ʾ��:

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

		//�ͷ���Դ��ĳ���͵��ļ� 
		BOOL ReleaseRes(LPCTSTR szFileName, DWORD dwResID, LPCTSTR lpType);  

		int GenerateGUID(CString& sGUID);
		void ConvertUtf8ToGBK(CString& strUtf8);
		void ConvertGBKToUtf8(CString& strText);
		CString BrowseFolder(HWND hWnd,LPCTSTR lpszTitle);

		//һ���ַ�����Ҫ��Ϊ�ļ�����ȥ������ַ����зǷ����ַ�
		//���ܷ���Ϊ�մ���Ҳ��������ַ����ܺϷ�����̫���ˣ���������Ҫ�ⲿ����
		CString RemoveFileNameInvalidChar(CString strText);

		//ת����ҳ�е�һЩ�ַ�
		void unescapexml(CString&strText);

		//����\u65F6\u5149\u5F71\u50CF\u9986
		CString DecodeUnicodeEscapeSequence(const char *lpszText);

		//ת���ٶ��˺��е������ַ���һ�������ַ�ת��Ϊ����%��ע����������ĺ�Ӣ�ĵĻ����Ӣ�Ĳ���
		CString GBKEncodeURI(CString strText);
		//ת���ٶ��˺��е������ַ���һ�������ַ�ת��Ϊ����%��ע����������ĺ�Ӣ�ĵĻ����Ӣ�Ĳ���
		CString UTF8EncodeURI(CString strText);

		BOOL IsSignalToStop(HANDLE hEvent);

		void InitializeSeed();
		//����һ���������Χ��[Low��High]
		int RandIntNum(int Low,int High);    

		//����һ��0-1֮�������������ַ�����С��������λ����nCount��,���ó���MAX_PATHλ
		CString RandFloatNum(int nCount);
		CString RandNum(int nCount);

		void RestartApp();

		int SplitString(const CString&strSrc,const CString&strSep,vector<CString>&vtStr);

		//int SetProxy(CInternetSession&sess,CHttpFile*pFile,char*szproxy, char*proxyUser, char* proxyPassword);   
		
		//���ַ����в����ַ���pSub���ж���ַ�
		int StrFindOneOf(char*pMain,char*pSub);
		//����ֱ���������ֵ��ַ�
		int StrFindCharNotDigit(char*pMain);

		//ʵ�ֹػ��ĺ���,�����http://shareaza.svn.sourceforge.net/svnroot/shareaza
		BOOL SetShutdownRights();
		//Ĭ�Ϲػ�
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

		//���� ���hwndΪNULL���ȡ�������棬����ֻ��ȡ�ô���,�ļ����浽szFilePath
		DWORD CaptureScreen(HWND hWnd, LPCTSTR sSavePath);

		//ת��·��,\Device\HarddiskVolume2\ -�� D
		CString DosDevicePath2LogicalPath(LPCTSTR lpszDosPath);

		int CreateAutoRun( LPCTSTR szDispInfo, LPCTSTR szAutoRunCmd, BOOL bAdd=TRUE );

		//�����ļ���ϵͳ�˵�
		void PopExplorerMenu(HWND hWnd, const CString&strFilePath, const CPoint&pt);
	}
}
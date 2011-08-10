 
#pragma once
#include <afx.h>
#include <afxmt.h>
#include <afxinet.h>


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

		//�ͷ���Դ��ĳ���͵��ļ� 
		BOOL ReleaseRes(CString strFileName,WORD wResID,CString strFileType);  

		//ɾ��DirName·���µ������ļ��ʹ��ļ���(ȡ����bDeleteSelf)
		int DeleteDirectory(CString DirName,BOOL bDeleteSelf=false);

		int GenerateGUID(CString& sGUID);
		void ConvertUtf8ToGBK(CString& strUtf8);
		void ConvertGBKToUtf8(CString& strText);
		CString BrowseFolder(HWND hWnd,LPCTSTR lpszTitle);

		//һ���ַ�����Ҫ��Ϊ�ļ�����ȥ������ַ����зǷ����ַ�
		//���ܷ���Ϊ�մ���Ҳ��������ַ����ܺϷ�����̫���ˣ���������Ҫ�ⲿ����
		CString RemoveFileNameInvalidChar(CString strText);

		//ת����ҳ�е�һЩ�ַ�
		void EscapeXmlHtmlChar(CString&strText);

		//ת���ٶ��˺��е������ַ���һ�������ַ�ת��Ϊ����%��ע����������ĺ�Ӣ�ĵĻ����Ӣ�Ĳ���
		CString GBKEncodeURI(CString strText);
		//ת���ٶ��˺��е������ַ���һ�������ַ�ת��Ϊ����%��ע����������ĺ�Ӣ�ĵĻ����Ӣ�Ĳ���
		CString UTF8EncodeURI(CString strText);

		BOOL IsSignalToStop(CEvent *pEvent);

		void InitializeSeed();
		//����һ���������Χ��[Low��High]
		int RandIntNum(int Low,int High);    

		//����һ��0-1֮�������������ַ�����С��������λ����nCount��,���ó���MAX_PATHλ
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

		//���ַ����в����ַ���pSub���ж���ַ�
		int StrFindOneOf(char*pMain,char*pSub);
		//����ֱ���������ֵ��ַ�
		int StrFindCharNotDigit(char*pMain);

		//ʵ�ֹػ��ĺ���,�����http://shareaza.svn.sourceforge.net/svnroot/shareaza
		BOOL SetShutdownRights();
		//Ĭ�Ϲػ�
		BOOL ShutDownComputer(UINT ShutdownFlags=EWX_POWEROFF);
		
		CString GetCurrentTimeStr();
	}
}
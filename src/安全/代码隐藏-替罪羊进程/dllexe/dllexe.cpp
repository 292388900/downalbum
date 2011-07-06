
/*
ע�⣺Ԥ������һ�����ܺ���_USRDLL��DLLEXE_EXPORTS
*/

#include <afx.h>
#include <afxwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <afxstat_.h>
#include "MainDlg.h"
#include "exeApp.h"

#define  __MAKEDLL
#undef __MAKEDLL

#pragma comment(linker, "/entry:myDllMain") 



__declspec(naked) HMODULE WINAPI GetSelfModuleHandle()
{
	_asm{
		sub esp, 28;
		mov eax, esp;
		push 28;
		push eax;
		call label0;
		label0:
		call dword ptr[VirtualQuery];
		test eax, eax;
		jz label1;
		mov eax, [esp + 4];
		label1:
		add esp, 28;
		ret;
	}
}

#ifdef __MAKEDLL
extern "C" BOOL WINAPI _DllMainCRTStartup(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved);
extern "C" BOOL WINAPI InternalDllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved);
#else
extern "C" BOOL WINAPI _tmainCRTStartup(void);
extern "C" int WinMainCRTStartup(void);
#endif

//#ifndef _UNICODE
//extern "C" int WinMainCRTStartup(void);
//#else
//extern "C" int wWinMainCRTStartup(void);
//#endif



// CinjectApp
// �йش���ʵ�ֵ���Ϣ������� inject.cpp
//

class CinjectApp : public CWinApp
{
public:
	CinjectApp(){
		m_pdlgMain=NULL;
		//::MessageBox(0,"CinjectApp ����",0,0);
	}
	CMainDlg*m_pdlgMain;

	// ��д
public:
	virtual BOOL InitInstance(){

		CWinApp::InitInstance();
		m_pdlgMain=new CMainDlg;
		m_pdlgMain->Create(CMainDlg::IDD,AfxGetMainWnd());
		m_pdlgMain->ShowWindow(SW_SHOW);

		return TRUE;
	}

	virtual int ExitInstance(){
		if (m_pdlgMain){
			delete m_pdlgMain;
			m_pdlgMain=NULL;
		}

		return CWinApp::ExitInstance();
	}

	DECLARE_MESSAGE_MAP()
};


// CinjectApp

BEGIN_MESSAGE_MAP(CinjectApp, CWinApp)
END_MESSAGE_MAP()


// Ψһ��һ�� CinjectApp ����



#ifdef __MAKEDLL
 CinjectApp theApp;
#else
 CWinApp theApp;
#endif




int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int nCmdShow)
{
	//��仰�Ǳ���ģ���Ϊmfc��VC\atlmfc\src\mfc\appinit.cpp��87�е�����SHLWAPI.DLL�ĺ���::PathFindExtension
	//�ɴ��뵽�����Ӧ����loader����������Ҫ�Լ�ȥ����
	::LoadLibrary("SHLWAPI.DLL");

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if ( !AfxWinInit(GetModuleHandle(NULL), NULL, NULL, 0) ){
		 // TODO: ���Ĵ�������Է���������Ҫ
		 //_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		 return 0;
	}

	AfxMessageBox(_T("AfxWinInit OK"));

	//��ʼ���ɹ���������Դ���Ϊ���ǵ���ʵ��ַ
	AfxSetResourceHandle(GetSelfModuleHandle());

	CMainDlg dlg;
	dlg.DoModal();

	return 0;
}



//CWinApp theApp;
BOOL WINAPI myDllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	HANDLE hRealModule=GetSelfModuleHandle();
	if ( hModule==hRealModule ){
#ifdef __MAKEDLL
		_DllMainCRTStartup(hRealModule,ul_reason_for_call,NULL);
#endif
	}else{
#ifndef __MAKEDLL
		::MessageBox(0,"myDllMain",0,0);
		WinMainCRTStartup();
#endif
	}
	return TRUE;

	//���û�У�ֻ���ó�������_DllMainCRTStartup������ѣ�������Դ˾�
	//�������dll���سɹ��󲻻���ִ���
#ifdef __MAKEDLL
	InternalDllMain(hModule,ul_reason_for_call,lpReserved);
#endif
}

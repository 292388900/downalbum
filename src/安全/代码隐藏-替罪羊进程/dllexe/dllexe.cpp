
/*
注意：预处理器一定不能含有_USRDLL　DLLEXE_EXPORTS
*/

#include <afx.h>
#include <afxwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <afxstat_.h>
#include "MainDlg.h"
#include "exeApp.h"
#include "../Common.h"

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


// CinjectApp
// 有关此类实现的信息，请参阅 inject.cpp
//

class CinjectApp : public CWinApp
{
public:
	CinjectApp(){
		m_pdlgMain=NULL;
		//::MessageBox(0,"CinjectApp 构造",0,0);
	}
	CMainDlg*m_pdlgMain;

	// 重写
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


// 唯一的一个 CinjectApp 对象



#ifdef __MAKEDLL
 CinjectApp theApp;
#else
 CWinApp theApp;
#endif




int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int nCmdShow)
{
	//注：如果模块不是自己处理输入表，而是让loader处理的话，旺旺有些dll不能正确加载。
	//例如mfc的VC\atlmfc\src\mfc\appinit.cpp第87行调用了SHLWAPI.DLL的函数::PathFindExtension
	//loader处理后即便获得了地址，但是模块并没有加载该dll，一样不能正确调用。
	//由此想到输入表不应该由loader来处理，而是要自己去处理。
	//::LoadLibrary("SHLWAPI.DLL");

	// 初始化 MFC 并在失败时显示错误
	if ( !AfxWinInit(GetModuleHandle(NULL), NULL, NULL, 0) ){
		 // TODO: 更改错误代码以符合您的需要
		 //_tprintf(_T("错误: MFC 初始化失败\n"));
		 return 0;
	}

	AfxMessageBox(_T("AfxWinInit OK"));

	//初始化成功后，设置资源句柄为我们的真实基址
	AfxSetResourceHandle(GetSelfModuleHandle());

	CMainDlg dlg;
	dlg.DoModal();

	return 0;
}



//CWinApp theApp;
BOOL WINAPI myDllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	hModule=::GetModuleHandle(NULL);
	HMODULE hRealModule=GetSelfModuleHandle();

	if ( hModule==hRealModule ){
#ifdef __MAKEDLL
		_DllMainCRTStartup(hRealModule,ul_reason_for_call,NULL);
#endif
	}else{
#ifndef __MAKEDLL
		//::MessageBox(0,"myDllMain",0,0);
		//__asm{__emit 0xCC}
		//加载dll并修复输入表
		ProcessImportTable(hRealModule);
		WinMainCRTStartup();
#endif
	}
	return TRUE;

	//这句没有，只是让程序链接_DllMainCRTStartup代码而已，如果忽略此句
	//编译出的dll加载成功后不会出现窗口
#ifdef __MAKEDLL
	InternalDllMain(hModule,ul_reason_for_call,lpReserved);
#endif
}

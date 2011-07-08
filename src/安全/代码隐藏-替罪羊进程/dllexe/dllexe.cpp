
/*
注意：无论哪种编译方法，预处理器一定不能含有_USRDLL，DLLEXE_EXPORTS

三种编译方法四种用途：

1.编译exe程序，需要设置【属性】－【配置属性】－【常规】－【配置类型】为“应用程序(.exe)”，
注释掉：#pragma comment(linker, "/entry:myMain") 
注释掉：#define  __MAKEDLL  也就是关闭__MAKEDLL宏
编译出来的可以直接运行，成功后弹出MFC对话框。

2.编译dll程序，需要设置【属性】－【配置属性】－【常规】－【配置类型】为“动态库(.dll)”，
开启__MAKEDLL宏，编译出来的dll可以使用loaddll.exe加载，成功后弹出MFC对话框。

3.编译patch程序，需要设置【属性】－【配置属性】－【常规】－【配置类型】为“动态库(.dll)”，
注释掉：#define  __MAKEDLL  也就是关闭__MAKEDLL宏
编译出来的文件使用PeLoad.exe加载到notepad.exe（替罪羊）进程中，成功后弹出MFC对话框。

我们的patch程序是一个dll文件，但是如果把pe属性改为exe，并将文件后缀改为.exe，那么直接双击可运行。


by sing
2011-7-6 20:09
*/

#include <afx.h>
#include <afxwin.h>
#include "MainDlg.h"
#include "../Common.h"

//#define  __MAKEDLL
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
CinjectApp theApp;




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

	AfxMessageBox(_T("start as normal exe"));

	//初始化成功后，设置资源句柄为我们的真实基址
	AfxSetResourceHandle(GetSelfModuleHandle());

	CMainDlg dlg;
	dlg.DoModal();

	return 0;
}



//CWinApp theApp;
BOOL WINAPI myDllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	//hModule=::GetModuleHandle(NULL);
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(LPCTSTR)myDllMain,(HMODULE*)&hModule);
	HMODULE hRealModule=GetSelfModuleHandle();

	if ( hModule==hRealModule ){
#ifdef __MAKEDLL
		_DllMainCRTStartup(hRealModule,ul_reason_for_call,NULL);
#else
		//_tmainCRTStartup();
		WinMainCRTStartup();
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

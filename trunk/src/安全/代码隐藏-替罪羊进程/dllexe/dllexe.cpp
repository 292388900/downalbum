
/*
ע�⣺�������ֱ��뷽����Ԥ������һ�����ܺ���_USRDLL��DLLEXE_EXPORTS

���ֱ��뷽��������;��

1.����exe������Ҫ���á����ԡ������������ԡ��������桿�����������͡�Ϊ��Ӧ�ó���(.exe)����
ע�͵���#pragma comment(linker, "/entry:myMain") 
ע�͵���#define  __MAKEDLL  Ҳ���ǹر�__MAKEDLL��
��������Ŀ���ֱ�����У��ɹ��󵯳�MFC�Ի���

2.����dll������Ҫ���á����ԡ������������ԡ��������桿�����������͡�Ϊ����̬��(.dll)����
����__MAKEDLL�꣬���������dll����ʹ��loaddll.exe���أ��ɹ��󵯳�MFC�Ի���

3.����patch������Ҫ���á����ԡ������������ԡ��������桿�����������͡�Ϊ����̬��(.dll)����
ע�͵���#define  __MAKEDLL  Ҳ���ǹر�__MAKEDLL��
����������ļ�ʹ��PeLoad.exe���ص�notepad.exe�������򣩽����У��ɹ��󵯳�MFC�Ի���

���ǵ�patch������һ��dll�ļ������������pe���Ը�Ϊexe�������ļ���׺��Ϊ.exe����ôֱ��˫�������С�


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
CinjectApp theApp;




int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int nCmdShow)
{
	//ע�����ģ�鲻���Լ����������������loader����Ļ���������Щdll������ȷ���ء�
	//����mfc��VC\atlmfc\src\mfc\appinit.cpp��87�е�����SHLWAPI.DLL�ĺ���::PathFindExtension
	//loader����󼴱����˵�ַ������ģ�鲢û�м��ظ�dll��һ��������ȷ���á�
	//�ɴ��뵽�����Ӧ����loader����������Ҫ�Լ�ȥ����
	//::LoadLibrary("SHLWAPI.DLL");

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if ( !AfxWinInit(GetModuleHandle(NULL), NULL, NULL, 0) ){
		 // TODO: ���Ĵ�������Է���������Ҫ
		 //_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		 return 0;
	}

	AfxMessageBox(_T("start as normal exe"));

	//��ʼ���ɹ���������Դ���Ϊ���ǵ���ʵ��ַ
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
		//����dll���޸������
		ProcessImportTable(hRealModule);
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

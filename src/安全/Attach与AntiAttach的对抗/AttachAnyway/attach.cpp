///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                     AttachAnyway Plugin for OllyDbg                       //
//                                                              			 //
// This plugin allows you to attach to a process in OllyDbg even when        //
// it is attempting to prevent the attach by hooking NtContinue				 //
// /DbgUiRemoteBreakin/DbgBreakPoint.					     				 //
//																			 //
///////////////////////////////////////////////////////////////////////////////

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR!

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
//#include <string.h>

#include "plugin.h"

#define	TEB	0x7ffde000		// only works for versions < XP SP2
#define	BYTES_TO_RW	15

// stuff from winternl.h
DWORD ProcessBasicInformation = 0;

typedef unsigned long * ULONG_PTR; //added...

typedef struct _PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PVOID PebBaseAddress; // normally it's a PPEB, but we don't care
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

HANDLE	hinst;
HWND	hwmain;

/*
NtContinue:			7C92D040
7C92D040 >  B8 20000000        mov     eax, 20
7C92D045    BA 0003FE7F        mov     edx, 7FFE0300
7C92D04A    FF12               call    dword ptr [edx]
7C92D04C    C2 0800            retn    8

DbgUiRemoteBreakin:	7C96FFE3
7C96FFE3 >  6A 08              push    8
7C96FFE5    68 3000977C        push    7C970030
7C96FFEA    E8 BCE8FBFF        call    7C92E8AB
7C96FFEF    64:A1 18000000     mov     eax, dword ptr fs:[18]
7C96FFF5    8B40 30            mov     eax, dword ptr [eax+30]
7C96FFF8    8078 02 00         cmp     byte ptr [eax+2], 0
7C96FFFC    75 09              jnz     short 7C970007
7C96FFFE    F605 D402FE7F 02   test    byte ptr [7FFE02D4], 2
7C970005    74 20              je      short 7C970027
7C970007    8365 FC 00         and     dword ptr [ebp-4], 0
7C97000B    E8 FE11FBFF        call    DbgBreakPoint

DbgBreakPoint:		7C92120E
7C92120E >  CC              int3
7C92120F    C3              retn
*/

char NtContinue_sb[] =			"\xB8\x20\x00\x00\x00"\
								"\xBA\x00\x03\xFE\x7F"\
								"\xFF\x12"\
								"\xC2\x08\x00";
const  dwNtContinue_Size = 15;

char DbgUiRemoteBreakin_sb[] =	"\x6A\x08"\
								"\x68\x30\x00\x97\x7C"\
								"\xE8\xBC\xE8\xFB\xFF"\
								"\x64\xA1\x18\x00\x00\x00"\
								"\x8B\x40\x30"\
								"\x80\x78\x02\x00"\
								"\x75\x09"\
								"\xF6\x05\xD4\x02\xFE\x7F\x02"\
								"\x74\x20"\
								"\x83\x65\xFC\x00"\
								"\xE8\xFE\x11\xFB\xFF";
const  dwDbgUiRemoteBreakin_Size = 45;

char DbgBreakPoint_sb[] =		"\xCC"\
								"\xC3";
const  dwDbgBreakPoint_Size = 2;

void AttachList();
void PEB_Restore_Permissions();

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call==DLL_PROCESS_ATTACH)
		hinst=hModule;
	return true;
}

//OD插件的必要元素,用来显示在OD的插件菜单里面,表示插件名称（32字节长）
extc int _export cdecl ODBG_Plugindata(char *shortname) 
{
	strcpy(shortname,"AttachAnyway");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) 
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	hwmain=hw;
	_Addtolist(0,0,"AttachAnyway plugin v0.4");
	_Addtolist(0,-1,"  Copyright (C) 2008 softsing");
	return 0;
}

//插件的子菜单的显示
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin)//调用该回调函数的窗口编号 
	{  
		case PM_MAIN:	 //子菜单显示在主窗口的插件菜单中,而PM_DISASM表示子菜单显示在反汇编窗口中
			strcpy(data,"0 Attach to Process Hooked on NtContinue/DbgUiRemoteBreakin/DbgBreakPoint,"
      					"1 Attach to Process With Altered PEB_LDR_DATA Permissions|"
						"2 About");
			return 1;
		default: 
			return 0;
	}
}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	if (origin == PM_MAIN) 
	{
		switch (action) {
			case 0://第一个子菜单响应
				AttachList();
				break;
			case 1://第二个子菜单响应
				PEB_Restore_Permissions();
				break;
			case 2://第三个子菜单响应
				MessageBox(hwmain,
					"AttachAnyway plugin v0.4\n"
					"Attach to NtContinue/DbgUiRemoteBreakin/DbgBreakPoint-Hooked\n" 
					"And unreadable PEB_LDR_DATA Processes\n"
					"Copyright (C) 2008 softsing",
					"AttachAnyway Plugin",
					MB_OK|MB_ICONINFORMATION);
				break;
			default: 
				break;
		}//end switch
	} 
}

extc void _export cdecl ODBG_Pluginreset(void) 
{
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void) 
{
}

void AttachList() 
{

	typedef 
	BOOL (WINAPI * PEnumProcesses)(
	  DWORD* pProcessIds,
	  DWORD cb,
	  DWORD* pBytesReturned
	);

	typedef 
	BOOL (WINAPI *PEnumProcessModules)(
	  HANDLE hProcess,
	  HMODULE* lphModule,
	  DWORD cb,
	  LPDWORD lpcbNeeded
	);

	typedef 
	DWORD (WINAPI * PGetModuleFileNameEx)(
	  HANDLE hProcess,
	  HMODULE hModule,
	  LPTSTR lpFilename,
	  DWORD nSize
	);

	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	char hookedProc[MAX_PATH*2];
	char szProcessName[MAX_PATH];
	LPCVOID NtContinue,DbgUiRemoteBreakin,DbgBreakPoint;
	char buffer[sizeof(DbgUiRemoteBreakin_sb)];
	HANDLE hProcess;
	HMODULE hModPSAPI;
	HMODULE hModNTDLL;
	PEnumProcesses EnumProcesses;
	PEnumProcessModules EnumProcessModules;
	PGetModuleFileNameEx GetModuleFileNameExA;
	
	int bNtContinueHooked = 0;
	int bDbgUiRemoteBreakinHooked = 0;
	int bDbgBreakPointHooked = 0;
	bool bApiHooked =false;

	hModPSAPI = LoadLibrary("psapi.dll");
	if (!hModPSAPI) 
	{ 
		MessageBox(hwmain, "Couldn't load psapi.dll","Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}

	EnumProcesses = (PEnumProcesses)GetProcAddress(hModPSAPI,"EnumProcesses");
	EnumProcessModules = (PEnumProcessModules)GetProcAddress(hModPSAPI, "EnumProcessModules");
	GetModuleFileNameExA = (PGetModuleFileNameEx)GetProcAddress(hModPSAPI, "GetModuleFileNameExA");
	if (!EnumProcesses || !EnumProcessModules || !GetModuleFileNameExA) 
	{
		MessageBox(hwmain,"Couldn't find needed API entries in psapi.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		_Addtolist(0,0,"EnumProcesses = %x",EnumProcesses);
		_Addtolist(0,0,"EnumProcessModules = %x",EnumProcessModules);
		_Addtolist(0,0,"GetModuleFileNameExA = %x",GetModuleFileNameExA);
		return;
	}

	hModNTDLL = GetModuleHandle("ntdll.dll");
	if (!hModNTDLL) 
	{
		MessageBox(hwmain,"Couldn't get handle for ntdll.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}

	NtContinue = (LPCVOID)GetProcAddress(hModNTDLL, "NtContinue");
	if (!NtContinue) 
	{
		MessageBox(hwmain,"Couldn't locate NtContinue in ntdll.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}
	DbgUiRemoteBreakin = (LPCVOID)GetProcAddress(hModNTDLL, "DbgUiRemoteBreakin");
	if (!DbgUiRemoteBreakin) 
	{
		MessageBox(hwmain,"Couldn't locate DbgUiRemoteBreakin in ntdll.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}
	DbgBreakPoint = (LPCVOID)GetProcAddress(hModNTDLL, "DbgBreakPoint");
	if (!DbgBreakPoint) 
	{
		MessageBox(hwmain,"Couldn't locate DbgBreakPoint in ntdll.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}

	if (!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded)) 
	{
		MessageBox(hwmain,"Couldn't enumerate processes!",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}
	cProcesses = cbNeeded / sizeof(DWORD);
	for ( DWORD i = 0; i < cProcesses; i++ ) 
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,aProcesses[i]); 
		if ( !hProcess )
			continue;
		//获取进程的路径名
		if (!GetModuleFileNameExA(hProcess, NULL, szProcessName, MAX_PATH)) 
			strcpy(szProcessName, "<unknown>");

		memset(buffer,0,dwNtContinue_Size+1);
		ReadProcessMemory(hProcess, NtContinue, buffer, dwNtContinue_Size, NULL);
		bNtContinueHooked = strcmp(NtContinue_sb,buffer);
		
		memset(buffer,0,dwDbgUiRemoteBreakin_Size+1);
		ReadProcessMemory(hProcess, DbgUiRemoteBreakin, buffer, dwDbgUiRemoteBreakin_Size, NULL);
		bDbgUiRemoteBreakinHooked = strcmp(DbgUiRemoteBreakin_sb,buffer);

		memset(buffer,0,dwDbgBreakPoint_Size+1);
		ReadProcessMemory(hProcess, DbgBreakPoint, buffer, dwDbgBreakPoint_Size, NULL);
		bDbgBreakPointHooked = strcmp(DbgBreakPoint_sb,buffer);

		if(bNtContinueHooked||bDbgUiRemoteBreakinHooked||bDbgBreakPointHooked)
		{
			bApiHooked = true;
			sprintf(hookedProc,
					"Hooked process found:\nPID: %08x (%d)\nName:  %s\n\n"
					"Click OK to attach to this process\n",
					aProcesses[i], aProcesses[i], szProcessName);
			if(bNtContinueHooked)
				strcat(hookedProc,"\nNtContinue Hooked");
			if(bDbgUiRemoteBreakinHooked)
				strcat(hookedProc,"\nDbgUiRemoteBreakin Hooked");
			if(bDbgBreakPointHooked)
				strcat(hookedProc,"\nDbgBreakPoint Hooked");

			if (MessageBox(hwmain, hookedProc, "Hooked API Found",
				MB_OKCANCEL|MB_ICONINFORMATION) != IDCANCEL) 
			{
				CloseHandle ( hProcess );
				hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE,FALSE,aProcesses[i]);
				if (hProcess) 
				{		
					if(!WriteProcessMemory(hProcess,(void *) NtContinue,NtContinue_sb,dwNtContinue_Size,NULL))				  
						MessageBox(hwmain,"Couldn't write to process virtual memory\nHook Removal Failed in NtContinue",
										  "Hook Removal Failed", MB_OK|MB_ICONERROR);
					if(!WriteProcessMemory(hProcess,(void *) DbgUiRemoteBreakin,DbgUiRemoteBreakin_sb,dwDbgUiRemoteBreakin_Size,NULL))				  
						MessageBox(hwmain,"Couldn't write to process virtual memory\nHook Removal Failed in DbgUiRemoteBreakin",
										  "Hook Removal Failed", MB_OK|MB_ICONERROR); 
					if(!WriteProcessMemory(hProcess,(void *) DbgBreakPoint,DbgBreakPoint_sb,dwDbgBreakPoint_Size,NULL))				  
						MessageBox(hwmain,"Couldn't write to process virtual memory\nHook Removal Failed in DbgBreakPoint",
										  "Hook Removal Failed", MB_OK|MB_ICONERROR); 
					
					CloseHandle( hProcess );
					_Attachtoactiveprocess(aProcesses[i]);		  
					return;		
				} 
				else 
					MessageBox(hwmain,"Couldn't open process virtual memory for write",		  
									  "Hook Removal Failed", MB_OK|MB_ICONERROR);
			}
		}

		if (hProcess)
			CloseHandle( hProcess );
	} // end process read loop
	if(!bApiHooked)
	{//no hooked API found
			MessageBox(hwmain,"Done reading processes, no hooked API found", 
						  "Done", MB_OK|MB_ICONINFORMATION);
	}
	
}

void PEB_Restore_Permissions() 
{
	typedef 
	BOOL (WINAPI * PEnumProcesses)(
		DWORD* pProcessIds,
		DWORD cb,
		DWORD* pBytesReturned
	);

	typedef 
	BOOL (WINAPI *PEnumProcessModules)(
		HANDLE hProcess,
		HMODULE* lphModule,
		DWORD cb,
		LPDWORD lpcbNeeded
	);

	typedef 
	DWORD (WINAPI * PGetModuleFileNameEx)(
		HANDLE hProcess,
		HMODULE hModule,
		LPTSTR lpFilename,
		DWORD nSize
	);

	typedef 
	DWORD (WINAPI *PNtQueryInformationProcess)( 
		HANDLE ProcessHandle,
		DWORD  ProcessInformationClass,
		PVOID  ProcessInformation,
		ULONG  ProcessInformationLength,
		PULONG ReturnLength
	);

	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	MEMORY_BASIC_INFORMATION pMem;
	char hookedProc[MAX_PATH*2];
	char szProcessName[MAX_PATH];
	DWORD res;
	DWORD oldProt;
	DWORD peb;
	DWORD peb_ldr_data;
	HANDLE hProcess;
	HMODULE hModPSAPI;
	HMODULE hModNTDLL;
	TOKEN_PRIVILEGES tPriv;
	TOKEN_PRIVILEGES tPrivOld;
	HANDLE hToken;
	DWORD cbPriv;
	DWORD red;
	PNtQueryInformationProcess NtQueryInformationProcess;
	PROCESS_BASIC_INFORMATION pbi;
	PEnumProcesses EnumProcesses;
	PEnumProcessModules EnumProcessModules;
	PGetModuleFileNameEx GetModuleFileNameExA;

	int found = 0;

	hModPSAPI = LoadLibrary("psapi.dll");
	if (!hModPSAPI) 
	{ 
		MessageBox(hwmain, "Couldn't load psapi.dll","Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}

	EnumProcesses = (PEnumProcesses)GetProcAddress(hModPSAPI,"EnumProcesses");
	EnumProcessModules = (PEnumProcessModules)GetProcAddress(hModPSAPI, "EnumProcessModules");
	GetModuleFileNameExA = (PGetModuleFileNameEx)GetProcAddress(hModPSAPI, "GetModuleFileNameExA");
	if (!EnumProcesses || !EnumProcessModules || !GetModuleFileNameExA) 
	{
		MessageBox(hwmain,"Couldn't find needed API entries in psapi.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		_Addtolist(0,0,"EnumProcesses = %x",EnumProcesses);
		_Addtolist(0,0,"EnumProcessModules = %x",EnumProcessModules);
		_Addtolist(0,0,"GetModuleFileNameExA = %x",GetModuleFileNameExA);
		return;
	}

	hModNTDLL = GetModuleHandle("ntdll.dll");
	if (!hModNTDLL) 
	{
		MessageBox(hwmain,"Couldn't get handle for ntdll.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}

	NtQueryInformationProcess = (PNtQueryInformationProcess)
			GetProcAddress(hModNTDLL, "NtQueryInformationProcess");
	if (!NtQueryInformationProcess) 
	{
		MessageBox(hwmain,"Couldn't find needed API entry in ntdll.dll",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);
		return;
	}

	if (!OpenThreadToken(GetCurrentThread(),TOKEN_QUERY |TOKEN_ADJUST_PRIVILEGES,false,&hToken))
	{
		if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES,&hToken))
			hToken=NULL;
	}

	if (hToken)
	{
		tPriv.PrivilegeCount = 1;
		tPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tPriv.Privileges[0].Luid);
		if (AdjustTokenPrivileges(hToken,false,&tPriv,sizeof(tPriv),&tPrivOld,&cbPriv))
		{
			  if(GetLastError()==ERROR_NOT_ALL_ASSIGNED)
			  {
					CloseHandle(hToken);
					hToken=NULL;
			  }
		}
		else
		{
			CloseHandle(hToken);
			hToken=NULL;
		}
	}

	if (!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded)) 
		MessageBox(hwmain,"Couldn't enumerate processes!",
						  "Error Loading DLL",MB_OK|MB_ICONERROR);

	cProcesses = cbNeeded / sizeof(DWORD);
	for (DWORD i = 0; i < cProcesses; i++ ) 
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | 
					PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE,aProcesses[i]);   
		if ( !hProcess )
			continue;

		if (!GetModuleFileNameExA(hProcess, NULL, szProcessName, MAX_PATH)) 
			strcpy(szProcessName, "<unknown>");
  
		oldProt = 0;
		peb_ldr_data = 0;	
		res = 0;
		_Addtolist(0,-1,"---");
		_Addtolist(0,0,"Checking process %d - %s for altered PEB_LDR_DATA permissions",
					 aProcesses[i], szProcessName);

		memset(&pMem,0,sizeof(pMem));
		memset(&pbi, 0, sizeof(pbi));

		// only works for pre-SP2
		// ReadProcessMemory(hProcess, (void *) (TEB + 0x30), &peb, 4, &read);

		// better method to find PEB suggested by anonymouse:
			  
		NtQueryInformationProcess(hProcess,ProcessBasicInformation,&pbi,sizeof(pbi),NULL);
		peb = (DWORD) pbi.PebBaseAddress;

		if (peb == 0) 
		{
			_Addtolist(0,1,"Failed to locate PEB from NtQueryInformationProcess");
		}
		else 
		{
			SetLastError(ERROR_SUCCESS);
			_Addtolist(0,0,"Found PEB at 0x%x", peb);
			red = ReadProcessMemory(hProcess, (void *) (peb + 0x0c),&peb_ldr_data, 4, NULL);
			if (red != 0)
			{
				if (peb_ldr_data == 0)
					_Addtolist(0,0,"peb_ldr_data could not be found at 0x%x",peb + 0x0c);
				else if (VirtualQueryEx(hProcess, (void *) peb_ldr_data, &pMem,sizeof(pMem)) > 0)
				{
					_Addtolist(0,0,"Current PEB_LDR_DATA is at 0x%x and has perms: 0x%x",peb_ldr_data, pMem.Protect);
					if (pMem.Protect == PAGE_NOACCESS) 
					{
						res = VirtualProtectEx(hProcess, (void *) peb_ldr_data,BYTES_TO_RW,PAGE_READWRITE,&oldProt);
						_Addtolist(0,0,"VirtualProtectEx returned %d, peb_ldr_data is 0x%x,"
							"oldProt = 0x%x, last error is %d",res,peb_ldr_data,oldProt,GetLastError());
					}
				}
				else
					_Addtolist(0,1,"VirtualQueryEx failed with error code %d",GetLastError());

				if ((res != 0) && (oldProt == PAGE_NOACCESS)) 
				{
					found = 1;
					// try again to get the process name
					if (!GetModuleFileNameExA(hProcess, NULL, szProcessName, MAX_PATH)) 
						strcpy(szProcessName, "<unknown>");

					sprintf(hookedProc,"PEB with altered permissions found in process:\n"
									   "PID: %08x (%d)\nName:  %s\n\n"
									   "Click OK to attach to this process\n",
									   aProcesses[i], aProcesses[i], szProcessName);
					if (MessageBox(hwmain, hookedProc, "Altered PEB Permissions Found",MB_OKCANCEL|MB_ICONINFORMATION) != IDCANCEL) 
					{
      					CloseHandle( hProcess );
						_Attachtoactiveprocess(aProcesses[i]);
						return;
					}
				} // end of if res !=0 &&
			} // end of if ReadProcessMemory
			else 
				_Addtolist(0,1,"ReadProcessMemory failed with error code 0x%x (0x%x)",red,GetLastError());
		}
		if (hProcess)
			CloseHandle( hProcess );
	} // end process read loop

	if (found) 
	{	
		MessageBox(hwmain,"Done reading processes","Done", MB_OK|MB_ICONINFORMATION);
		found = 0;
	} 
	else 
	{
		MessageBox(hwmain,"Done reading processes, no altered PEB permissions found", 
						  "Done", MB_OK|MB_ICONINFORMATION);
	}
}



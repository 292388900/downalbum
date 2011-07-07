
/************************************************************************
releaseģʽ����Ч,debug�汾�ĵ���Ŀ����̱���
�̺߳���������ò�Ҫ���ó�Kernel32��User32��������к���,
ò��User32����Ĳ��ֺ����ڵ��õ�ʱ��Ҳ����ֱ����ľ���,
���Ҫ����������ĺ������԰�LoadLibrary��GetProcAddress�ĵ�ַ�����̺߳����Ĳ�������ȥ
��Ҫʹ���κεľ�̬�ַ���,�̺߳������д�ɾ�̬(Ҳ����ֱ�ӽ�ֹ��������)
************************************************************************/
#include <windows.h>
#include <Tlhelp32.h.>

#pragma check_stack (off)

static DWORD WINAPI ThreadProc (LPVOID lpParameter);
static void AfterThreadProc (void);

#pragma check_stack

typedef HINSTANCE (WINAPI *ProcLoadLibrary)(char*);
typedef FARPROC (WINAPI *ProcGetProcAddress)(HMODULE, LPCSTR);
typedef int (WINAPI *ProcMessageBox)(HWND,LPCTSTR,LPCTSTR,UINT);

typedef struct tagHYPINJECT {
	ProcLoadLibrary    fnLoad;
	ProcGetProcAddress fnGetProc;
	char MsgStr[MAX_PATH];
	char DLLName[MAX_PATH];
	char ProcName[MAX_PATH];
} HYPINJECT;


#pragma check_stack (off)

static DWORD WINAPI ThreadProc (LPVOID lpParameter)
{
	HYPINJECT* p = (HYPINJECT*)lpParameter;
	HMODULE hDLL = p->fnLoad (p->DLLName);
	ProcGetProcAddress GetProc = p->fnGetProc;
	ProcMessageBox MsgBox = (ProcMessageBox)GetProc(hDLL,p->ProcName);
	MsgBox(NULL,p->MsgStr,p->MsgStr,MB_OK);
	return 0;
}


static void AfterThreadProc () 
{
}

#pragma check_stack

HYPINJECT hypInject;
BOOL InjectFunc(DWORD PID)
{
       HMODULE hk = LoadLibrary ("kernel32.dll");
       hypInject.fnLoad = (ProcLoadLibrary)GetProcAddress (hk, "LoadLibraryA");
       hypInject.fnGetProc = (ProcGetProcAddress)GetProcAddress (hk, "GetProcAddress");
       lstrcpy(hypInject.MsgStr," hyp's Knowledge Base");
       lstrcpy (hypInject.DLLName, "user32.dll");
       lstrcpy (hypInject.ProcName, "MessageBoxA");
       PVOID pCode = NULL;
       PVOID pData = NULL;
       BOOL bc = FALSE;
       DWORD cbCodeSize = (BYTE*)AfterThreadProc - (BYTE*)ThreadProc;
       HANDLE hProc = OpenProcess(
              PROCESS_QUERY_INFORMATION |   
              PROCESS_CREATE_THREAD     |
              PROCESS_VM_OPERATION      |
              PROCESS_VM_WRITE,            
              FALSE, PID);
       if (hProc == NULL)
       {
              return FALSE;
       }
       pCode=VirtualAllocEx(hProc,NULL,cbCodeSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
       if(pCode == NULL)
       {
             return FALSE;
       }
       bc = WriteProcessMemory(hProc,pCode,(LPVOID)(DWORD) ThreadProc,cbCodeSize,NULL);
       if (!bc)
       {
            return FALSE;
       }
       pData = VirtualAllocEx (hProc,NULL, sizeof (hypInject), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
       if(pData == NULL)
       {
              return FALSE;
       }
       bc = WriteProcessMemory (hProc, pData, &hypInject, sizeof (hypInject), NULL);
       if (!bc)
       {
              return FALSE;
       }

       HANDLE ht=CreateRemoteThread(hProc,NULL,NULL,(LPTHREAD_START_ROUTINE)pCode,pData,0,NULL);
       if(ht == NULL)
       {
              return FALSE;
       }
       CloseHandle(hProc);
       return TRUE;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//return PID from a process name
ULONG     GetProcessID(char *szProcessName)
{
	PROCESSENTRY32      ProcessEntry32;
	HANDLE                        hSnap;
	int                                  ret;

	ProcessEntry32.dwSize = sizeof (PROCESSENTRY32);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap == 0)
		return 0;

	ret = Process32First(hSnap,&ProcessEntry32);
	while (ret)
	{
		if (_strcmpi(ProcessEntry32.szExeFile,szProcessName)==0 )
		{
			return ProcessEntry32.th32ProcessID;
		}
		ret = Process32Next(hSnap,&ProcessEntry32);
	}

	CloseHandle(hSnap);
	return 0;
}
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


int main()
{
      InjectFunc(GetProcessID("NOTEPAD.EXE"));
      return 0;
}
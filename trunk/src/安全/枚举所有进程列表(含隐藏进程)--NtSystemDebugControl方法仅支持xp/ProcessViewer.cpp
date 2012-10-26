
#pragma once

#include "stdafx.h"
#include "ProcessViewer.h"
#include "windows.h"
#include <WinDef.h>
#include <WinNT.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <shlwapi.h>

#pragma comment(lib,"shlwapi.lib")

HANDLE g_hMPM = NULL;
HANDLE g_pMapPhysicalMemory;


DWORD CProcessViewer::GetOsType()
{
	DWORD dwType = OSTYPE_unkown;

	OSVERSIONINFO os = {sizeof(os)};
	GetVersionEx(&os);

	if ( os.dwBuildNumber==2195 ){
		dwType = OSTYPE_win2k;
	}else if ( os.dwBuildNumber==2600 ){
		dwType = OSTYPE_winxp;
	}else if ( os.dwBuildNumber==3790 ){
		dwType = OSTYPE_win2003;
	}else if ( os.dwBuildNumber==7600 ){
		//再判断是否是64位
		dwType = OSTYPE_win732;
	}

	return dwType;
}


DWORD CProcessViewer::ReadDwordData(DWORD lngAddr)
{
	MEMORY_CHUNKS objQueryBuff;
	NTSTATUS ntStatus = 0;
	DWORD dwData = 0;
	DWORD dwReturnLen = 0;

	if ( m_nOsType==OSTYPE_winxp ){
		objQueryBuff.Address = lngAddr;
		objQueryBuff.Data = &dwData;
		objQueryBuff.Length = sizeof(dwData);
		ntStatus = m_NtSystemDebugControl(SysDbgCopyMemoryChunks_0, &objQueryBuff, /*sizeof(objQueryBuff)*/0x0C, 0, 0, &dwReturnLen);
	}else if ( m_nOsType==OSTYPE_win2k ){
		ULONG phys=(ULONG)LinearToPhys((PULONG)g_pMapPhysicalMemory,(PVOID)lngAddr);
		PULONG tmp=(PULONG)MapViewOfFile(g_hMPM, FILE_MAP_READ, 0, phys & 0xfffff000, 0x1000);
		if ( tmp ){
			dwData=tmp[(phys & 0xFFF)>>2];
			UnmapViewOfFile(tmp);
		}
	}

	return dwData;
}

//获取进程名
CString CProcessViewer::ReadStringData(DWORD lngAddr,BOOL bUnicode/*=FALSE*/)
{
	CString str;
	CHAR bytBuff[MAX_PATH*sizeof(WCHAR)] = {0};	//最坏的情况就是最大文件长度，且是unicode类型
	MEMORY_CHUNKS objQueryBuff;
	NTSTATUS ntStatus;
	ULONG lngReturn = 0;

	if ( m_nOsType==OSTYPE_winxp ){
		objQueryBuff.Address = lngAddr;
		objQueryBuff.Data = bytBuff;
		objQueryBuff.Length = sizeof(bytBuff);
		ntStatus = m_NtSystemDebugControl(SysDbgCopyMemoryChunks_0, &objQueryBuff, sizeof(objQueryBuff), 0, 0, &lngReturn);
		if ( SUCCEEDED(ntStatus) ){
			if ( bUnicode==FALSE ){
				str = bytBuff;
			}else{
				CStringW strTemp = (WCHAR *)bytBuff;
				str = strTemp;
			}
		}
	}else if ( m_nOsType==OSTYPE_win2k ){
		if ( bUnicode==FALSE ){
			ULONG phys=(ULONG)LinearToPhys((PULONG)g_pMapPhysicalMemory,(PVOID)lngAddr);
			PULONG tmp=(PULONG)MapViewOfFile(g_hMPM, FILE_MAP_READ, 0, phys & 0xfffff000, 0x1000);
			if ( tmp ){
				RtlMoveMemory(bytBuff,(const void *)&tmp[(phys & 0xFFF)>>2],16);	//ImageFileName    : [16]  "test.exe"
				str = bytBuff;
				UnmapViewOfFile(tmp);
			}
		}else{
			//先读取Unicode字符串的长度，再读取字符串
			WORD cCh = (WORD)ReadDwordData(lngAddr);
			lngAddr = ReadDwordData(lngAddr+sizeof(WORD)*2);	//Unicode偏移2个word后是字符串
			ULONG phys=(ULONG)LinearToPhys((PULONG)g_pMapPhysicalMemory,(PVOID)lngAddr);
			PULONG tmp=(PULONG)MapViewOfFile(g_hMPM, FILE_MAP_READ, 0, phys & 0xfffff000, 0x1000);
			if ( tmp ){
				RtlMoveMemory(bytBuff,(const void *)&tmp[(phys & 0xFFF)>>2],cCh*sizeof(WCHAR));
				CStringW strTemp = (WCHAR *)bytBuff;
				str = strTemp;
				UnmapViewOfFile(tmp);
			}
		}
	}

	return str;
}

//获取/Device/PhysicalMemory的可读写句柄
DWORD CProcessViewer::OpenPhysicalMemory()
{
    NTSTATUS        status;
    UNICODE_STRING        physmemString;
    OBJECT_ATTRIBUTES    attributes;
    
    m_RtlInitUnicodeString( &physmemString, L"\\Device\\PhysicalMemory" );
    
    attributes.Length            = sizeof(OBJECT_ATTRIBUTES);
    attributes.RootDirectory        = NULL;
    attributes.ObjectName            = &physmemString;
    attributes.Attributes            = 0;
    attributes.SecurityDescriptor        = NULL;
    attributes.SecurityQualityOfService    = NULL;
    
    status = m_NtOpenSection(&g_hMPM,SECTION_MAP_READ|SECTION_MAP_WRITE,&attributes); 
    
    if(status == STATUS_ACCESS_DENIED){ 
        status = m_NtOpenSection(&g_hMPM,READ_CONTROL|WRITE_DAC,&attributes); 
        SetPhyscialMemorySectionCanBeWrited(g_hMPM); 
        CloseHandle(g_hMPM);
        status =m_NtOpenSection(&g_hMPM,SECTION_MAP_READ|SECTION_MAP_WRITE,&attributes); 
    }

    if( FAILED( status )) 
    {
        return NULL;
    }
    
    g_pMapPhysicalMemory = MapViewOfFile(
        g_hMPM,
        4,
        0,
        0x30000,
        0x1000);
    if( g_pMapPhysicalMemory == NULL )
    {
        return NULL;
    }
    
    return (DWORD)g_hMPM;
}

//将物理内存影射到进程空间
PVOID CProcessViewer::LinearToPhys(PULONG BaseAddress,PVOID addr)
{
    ULONG VAddr=(ULONG)addr,PGDE,PTE,PAddr;
    PGDE=BaseAddress[VAddr>>22];
    if ((PGDE&1)!=0)
    {
        ULONG tmp=PGDE&0x00000080;
        if (tmp!=0)
        {
            PAddr=(PGDE&0xFFC00000)+(VAddr&0x003FFFFF);
        }
        else
        {
            PGDE=(ULONG)MapViewOfFile(g_hMPM, 4, 0, PGDE & 0xfffff000, 0x1000);
            PTE=((PULONG)PGDE)[(VAddr&0x003FF000)>>12];
            if ((PTE&1)!=0)
            {
                PAddr=(PTE&0xFFFFF000)+(VAddr&0x00000FFF);
                UnmapViewOfFile((PVOID)PGDE);
            }
            else return 0;
        }
    }
    else return 0;

    return (PVOID)PAddr;
}


//使物理内存可写
void CProcessViewer::SetPhyscialMemorySectionCanBeWrited(HANDLE hSection)
{
	EXPLICIT_ACCESS ea;
	PACL pDacl,pNewDacl;
	PSECURITY_DESCRIPTOR pSD;

	//获取PhysicalMemory的DACL
	GetSecurityInfo(hSection, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, &pDacl, 0, &pSD);
	
	//创建一个ACE，允许当前用户读写PhysicalMemory
	ea.grfAccessPermissions = SECTION_MAP_WRITE;
	ea.grfAccessMode = GRANT_ACCESS;
	ea.grfInheritance = NO_INHERITANCE;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
	ea.Trustee.ptstrName = _T("CURRENT_USER");

	//将新的ACE加入DACL
	SetEntriesInAcl(1, &ea, pDacl, &pNewDacl);

	//更新PhysicalMemory的DACL
	SetSecurityInfo(hSection, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, pNewDacl, 0);

	LocalFree(pSD);
	LocalFree(pNewDacl);
}


//获取ntoskrnl.exe/ntkrnlpa.exe的地址
DWORD CProcessViewer::FindModuleBase(const char *pName)
{
	DWORD dwReturn = 0;
	DWORD nSize = 1;
	LONG lngModuleCount = 0;
	SYSTEM_MODULE *pModules = NULL;
	PBYTE pBuff = NULL;

	DWORD ntStatus = m_NtQuerySystemInformation(SystemModuleInformation, NULL, 0, &nSize);
	if ( ntStatus==STATUS_INFO_LENGTH_MISMATCH ){
		pBuff = new BYTE[nSize];
		if ( pBuff ){

			ntStatus = m_NtQuerySystemInformation(SystemModuleInformation, pBuff, nSize, &nSize);
			if ( SUCCEEDED(ntStatus) ){
				lngModuleCount = *((PLONG)pBuff);
				pModules = (SYSTEM_MODULE *)(pBuff + sizeof(DWORD));

				for ( int i=0; i<lngModuleCount; ++i ){
					if ( StrStrIA(pModules[i].ImageName,pName)!=NULL ){
						dwReturn = pModules[i].Base;
						break;
					}
				}
			}
			
			delete []pBuff;
		}
	}

	return dwReturn;
}

BOOL WINAPI CProcessViewer::EnableDebugPrivilege(BOOL bEnable) 
{
	BOOL bOk = FALSE;
	HANDLE hToken;

	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, 
		&hToken)) 
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}
	return(bOk);
}


CString CProcessViewer::FindRootPath(DWORD dwSerialNumber)
{
	CString strRoot;

	for ( vector<VOLUMENAMEANDSERIAL>::iterator i = m_vtVolumes.begin(); i!=m_vtVolumes.end(); ++i ){
		if ( i->dwSerialNumber==dwSerialNumber ){
			strRoot = i->sVolumeName;
			break;
		}
	}

	return strRoot;
}

//目前只测试xp通过
CString CProcessViewer::GetFullPathBySection(DWORD dwSectionObject)
{
	CString strFilePath;
	CString strRoot;
	DWORD dwFileObject = 0;

	DWORD dwTemp = ReadDwordData(dwSectionObject+SEGMENT_OFFSET);						//PSEGMENT_OBJECT Segment;
	dwTemp = ReadDwordData(dwTemp+CTLAREA_OFFSET);										//PCONTROL_AREA ControlArea;
	dwFileObject = ReadDwordData(dwTemp+FILE_OBJ_OFFSET);								//PFILE_OBJECT FilePointer

	dwTemp = ReadDwordData(dwFileObject+FileNameOffsetOfFILE_OBJECT+sizeof(WORD)*2);	//UNICODE_STRING FileName::Buffer
	strFilePath = ReadStringData(dwTemp,TRUE);

	//读取根盘符
	dwTemp = ReadDwordData(dwFileObject+VpbOffsetOfFILE_OBJECT);
	strRoot = FindRootPath(ReadDwordData(dwTemp+SerialNumberOffsetOfVPB));

	return strRoot+strFilePath;
}


CString DosDevicePath2LogicalPath(LPCTSTR lpszDosPath)
{
	CString strResult;

	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';

	if ( lpszDosPath==NULL || !GetLogicalDriveStrings(_countof(szTemp)-1, szTemp) ){
		return strResult;
	}


	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;

	do{
		// Copy the drive letter to the template string
		*szDrive = *p;

		// Look up each device name
		if ( QueryDosDevice(szDrive, szName, _countof(szName)) ){
			UINT uNameLen = (UINT)_tcslen(szName);

			if (uNameLen < MAX_PATH) 
			{
				bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

				if ( bFound ){
					// Reconstruct pszFilename using szTemp
					// Replace device path with DOS path
					TCHAR szTempFile[MAX_PATH];
					_stprintf_s(szTempFile,	TEXT("%s%s"), szDrive, lpszDosPath+uNameLen);
					strResult = szTempFile;
				}
			}
		}

		// Go to the next NULL character.
		while (*p++);
	} while (!bFound && *p); // end of string

	return strResult;
}


//
CString CProcessViewer::GetFullPathByCreationInfo(DWORD dwEProcess)
{
	CString strFilePath;
	DWORD dwTemp = ReadDwordData(dwEProcess+m_dwOffsetCreationInfo);
	dwTemp = ReadDwordData(dwTemp+sizeof(WORD)*2);					//UNICODE_STRING FileName::Buffer
	strFilePath = ReadStringData(dwTemp,TRUE);

	return DosDevicePath2LogicalPath(strFilePath);
}

//win2000不是全路径，缺少盘符："\WINNT\system32\notepad.exe"   "\test.exe"
CString CProcessViewer::GetFullPathWin2K(DWORD dwEProcess)
{
	CString strFilePath;
	DWORD dwTemp = ReadDwordData(dwEProcess+0x284);
	if ( dwTemp ){
		strFilePath = ReadStringData(dwTemp,TRUE);					//UNICODE_STRING FileName::Buffer
	}

	return strFilePath;
}

CProcessViewer::CProcessViewer()
{
	m_bInited = FALSE;
	m_nOsType = OSTYPE_unkown;
	m_dwFirstEProcess = 0;

	m_dwOffsetUniqueProcessId = 0;
	m_dwOffsetImageFileName = 0;
	m_dwOffsetActiveProcessLinks = 0;
	m_dwOffsetSessionProcessLinks = 0;
	m_dwOffsetSectionObject = 0;
	m_dwOffsetCreationInfo = 0; 
}

CProcessViewer::~CProcessViewer()
{

}

int CProcessViewer::Init()
{
	int nErrorCode = 0;
	if ( m_bInited ){
		return nErrorCode;
	}
	BOOL bNeedFree = FALSE;
	EnableDebugPrivilege(TRUE);

	//////////////////////////////////////////////////////////////////////////
	HMODULE hNtdll = GetModuleHandle(_T("ntdll.dll"));
	if ( hNtdll==NULL ){
		hNtdll = LoadLibrary(_T("ntdll.dll"));
	}
	if ( hNtdll!=NULL ){
		m_NtSystemDebugControl = (NTSYSTEMDEBUGCONTROL)GetProcAddress(hNtdll,"NtSystemDebugControl"); 
		m_NtOpenSection = (NTOPENSECTION)GetProcAddress(hNtdll,"NtOpenSection"); 
		m_RtlInitUnicodeString = (TRtlInitUnicodeString)GetProcAddress(hNtdll,"RtlInitUnicodeString"); 
		m_NtQuerySystemInformation = (TNtQuerySystemInformation)GetProcAddress(hNtdll,"NtQuerySystemInformation");

		//这里不能释放，后面要用这些函数
		//FreeLibrary(hNtdll);
		if ( m_NtSystemDebugControl==NULL || m_NtOpenSection==NULL || m_RtlInitUnicodeString==NULL || m_NtQuerySystemInformation==NULL ){
			return 6;
		}
	}else{
		return 7;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	m_nOsType = GetOsType();

	if ( m_nOsType==OSTYPE_win2k ){
		m_dwOffsetUniqueProcessId = 0x9C;
		m_dwOffsetImageFileName = 0x1FC;
		m_dwOffsetSessionProcessLinks = 244;
		m_dwOffsetActiveProcessLinks = 0xa0;
		m_dwOffsetSectionObject = 0x1ac;		//这个值恒为4，不知道怎么处理了
		m_dwOffsetCreationInfo = 0x1e4; 

		//打开物理内存如果失败就退出程序
		if ( OpenPhysicalMemory()==0 ){
			return 1;
		}
	}else if ( m_nOsType==OSTYPE_winxp || m_nOsType==OSTYPE_win2003 ){
		m_dwOffsetUniqueProcessId = 0x084;		//UniqueProcessId: Ptr32 Void
		m_dwOffsetImageFileName = 0x174;		//ImageFileName: [16] UChar
		m_dwOffsetSessionProcessLinks = 0x0b4;	//SessionProcessLinks: _LIST_ENTRY
		m_dwOffsetActiveProcessLinks = 0x088;	//ActiveProcessLinks : _LIST_ENTRY
		m_dwOffsetSectionObject = 0x138;
		m_dwOffsetCreationInfo = 0x1f4; 
	}else if ( m_nOsType==OSTYPE_win732 ){
		m_dwOffsetUniqueProcessId = 0x0b4;	
		m_dwOffsetImageFileName = 0x16c;	
		m_dwOffsetActiveProcessLinks = 0x0b8;
		m_dwOffsetSectionObject = 0x128;
	}else{
		m_dwOffsetUniqueProcessId = 132;
		m_dwOffsetImageFileName = 0x154;
		m_dwOffsetSessionProcessLinks = 180;
		m_dwOffsetActiveProcessLinks = 136;
		return 2;
	}
	//////////////////////////////////////////////////////////////////////////
	//在用户态加载一份ntoskrnl.exe/ntkrnlpa.exe

	DWORD dwStartProcAddr = 0;
	std::string sNtoskrnlName;
	sNtoskrnlName = "ntkrnlpa.exe";
	DWORD dwNtoskrnlModule = FindModuleBase(sNtoskrnlName.c_str());
	if ( dwNtoskrnlModule==0 ){
		sNtoskrnlName = "ntoskrnl.exe";
		dwNtoskrnlModule = FindModuleBase(sNtoskrnlName.c_str());
	}

	if ( dwNtoskrnlModule==0 ){
		return 3;
	}

	bNeedFree = FALSE;
	HMODULE hMyNtoskrnlBase = GetModuleHandleA(sNtoskrnlName.c_str());
	if ( hMyNtoskrnlBase==NULL ){
		hMyNtoskrnlBase = LoadLibraryA(sNtoskrnlName.c_str());
		if ( hMyNtoskrnlBase!=NULL ){
			bNeedFree = TRUE;
		}
	}

	if ( hMyNtoskrnlBase!=NULL ){
		//获取System进程内核例程/变量在用户态的相对位置
		dwStartProcAddr = (DWORD)GetProcAddress(hMyNtoskrnlBase, "PsInitialSystemProcess");//IoGetCurrentProcess PsGetCurrentProcess.

		if ( bNeedFree ){
			//释放ntoskrnl.exe/ntkrnlpa.exe
			FreeLibrary(hMyNtoskrnlBase);
		}

		if ( dwStartProcAddr==0 ){
			return 4;
		}

		//获取系统核心模块ntoskrnl.exe的基址
		//System进程内核例程/变量的实际地址
		//printf("%08X %08X %08X \n",dwNtoskrnlModule,dwStartProcAddr,(DWORD)hMyNtoskrnlBase);
		dwStartProcAddr = dwNtoskrnlModule + (dwStartProcAddr - (DWORD)hMyNtoskrnlBase);
	}else{
		return 5;
	}

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CString strRoot;
	DWORD dwSerialNumber = 0;
	VOLUMENAMEANDSERIAL stVolume;
	m_vtVolumes.clear();

	for ( int i=0; i<26; ++i ){
		strRoot.Format(_T("%c:"),'C'+i);
		dwSerialNumber = 0;
		GetVolumeInformation(strRoot+_T("\\"),NULL,0,&dwSerialNumber,NULL,NULL,NULL,0);
		if ( dwSerialNumber!=0 ){
			stVolume.sVolumeName = strRoot;
			stVolume.dwSerialNumber = dwSerialNumber;
			m_vtVolumes.push_back(stVolume);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	m_dwFirstEProcess = ReadDwordData(dwStartProcAddr);
	//printf("dwStartProcAddr %08x m_dwFirstEProcess %08x\n",dwStartProcAddr,m_dwFirstEProcess);
	
	m_bInited = (nErrorCode == 0);
	return nErrorCode;
}

int CProcessViewer::GetProcessList(vector<PROCESSINFO>&vtProcesses)
{
	vtProcesses.clear();
	int nErrorCode = Init();
	if ( nErrorCode!=0 ){
		//printf("err:%d\n",nErrorCode);
		return 0;
	}

	DWORD pEProcess = m_dwFirstEProcess;
	DWORD dwFLink = 0;
	DWORD dwFirstFLink = 0;
	DWORD dwProcessId = 0;
	PROCESSINFO stProcess;
	DWORD dwOsType = GetOsType();

	dwFirstFLink = pEProcess + m_dwOffsetActiveProcessLinks;

	while ( TRUE ){

		dwProcessId = ReadDwordData(pEProcess+m_dwOffsetUniqueProcessId);
		if ( (int)dwProcessId<=0 ){
			break;
		}

		stProcess.dwProcessId = dwProcessId;
		stProcess.strImageFileName = ReadStringData(pEProcess+m_dwOffsetImageFileName);
		if ( dwOsType==OSTYPE_win2k ){
			stProcess.strImageFilePath = GetFullPathWin2K(pEProcess);
		}else{
			stProcess.strImageFilePath = GetFullPathBySection(ReadDwordData(pEProcess+m_dwOffsetSectionObject));
			//stProcess.strImageFilePath = GetFullPathByCreationInfo(pEProcess);
		}
		vtProcesses.push_back(stProcess);

		dwFLink = ReadDwordData(pEProcess + m_dwOffsetActiveProcessLinks);
		if ( dwFLink==0 || dwFLink==dwFirstFLink ){
			break;
		}

		pEProcess = dwFLink - m_dwOffsetActiveProcessLinks;
	}

	return (int)vtProcesses.size();
}
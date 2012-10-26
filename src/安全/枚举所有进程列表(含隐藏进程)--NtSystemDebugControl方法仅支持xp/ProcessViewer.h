
#pragma once
#include <afx.h>
#include <winternl.h>
#include <vector>
using namespace std;


typedef enum _SYSDBG_COMMAND 
{ 
	SysDbgQueryModuleInformation, 
	SysDbgQueryTraceInformation, 
	SysDbgSetTracepoint, 
	SysDbgSetSpecialCall, 
	SysDbgClearSpecialCalls, 
	SysDbgQuerySpecialCalls, 
	SysDbgCopyMemoryChunks_0 = 8 
} SYSDBG_COMMAND, *PSYSDBG_COMMAND; 

typedef struct _MEMORY_CHUNKS 
{ 
	ULONG Address; 
	PVOID Data; 
	ULONG Length; 
}MEMORY_CHUNKS, *PMEMORY_CHUNKS; 

typedef struct _SYSTEM_MODULE
{
    ULONG   Reserved[2];
    ULONG   Base;
    ULONG   Size;
    ULONG   Flags;
    USHORT  Index;
    USHORT  Unknown;
    USHORT  LoadCount;
    USHORT  ModuleNameOffset;
    CHAR    ImageName[256];
} SYSTEM_MODULE, *PSYSTEM_MODULE;


typedef NTSTATUS (__stdcall *NTSYSTEMDEBUGCONTROL)( 
	IN SYSDBG_COMMAND Command, 
	IN PVOID InputBuffer, 
	IN ULONG InputBufferLength, 
	OUT PVOID OutputBuffer, 
	IN ULONG OutputBufferLength, 
	OUT PULONG ReturnLength 
	); 


typedef NTSTATUS (__stdcall *NTOPENSECTION)(
	OUT PHANDLE             SectionHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes 
	);

typedef VOID (__stdcall *TRtlInitUnicodeString)(
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	);

typedef NTSTATUS (NTAPI * TNtQuerySystemInformation)(
	ULONG SystemInformationClass ,
	PVOID SystemInformation ,
	ULONG SystemInformationLength ,
	PULONG ReturnLength
	);

/*
typedef struct _CONTROL_AREA
{
	PVOID Segment          ;//Ptr32 _SEGMENT
	LIST_ENTRY DereferenceList  ;// _LIST_ENTRY
	ULONG NumberOfSectionReferences ;// Uint4B
	ULONG NumberOfPfnReferences ;// Uint4B
	ULONG NumberOfMappedViews ;// Uint4B
	ULONG NumberOfSystemCacheViews ;// Uint4B
	ULONG NumberOfUserReferences ;// Uint4B
	ULONG u                ;// __unnamed
	PFILE_OBJECT FilePointer      ;// Ptr32 _FILE_OBJECT
	PVOID WaitingForDeletion ;// Ptr32 _EVENT_COUNTER
	USHORT ModifiedWriteCount ;// Uint2B
	USHORT FlushInProgressCount ;// Uint2B
	ULONG WritableUserReferences ;// Uint4B
	ULONG QuadwordPad      ;// Uint4B
}CONTROL_AREA,*PCONTROL_AREA;

typedef struct _SEGMENT
{
	PCONTROL_AREA ControlArea      ;// Ptr32 _CONTROL_AREA
	ULONG TotalNumberOfPtes ;// Uint4B
	ULONG NonExtendedPtes  ;// Uint4B
	ULONG Spare0           ;// Uint4B
	ULONG64 SizeOfSegment    ;//  Uint8B
	ULONG64 SegmentPteTemplate ;//  _MMPTE
	ULONG NumberOfCommittedPages ;// Uint4B
	PVOID ExtendInfo       ;//  Ptr32 _MMEXTEND_INFO
	ULONG SegmentFlags     ;// _SEGMENT_FLAGS
	PVOID BasedAddress     ;// Ptr32 Void
	ULONG u1               ;// __unnamed
	ULONG u2               ;// __unnamed
	PVOID PrototypePte     ;// Ptr32 _MMPTE
	PVOID ThePtes          ;// [1] _MMPTE
}SEGMENT,*PSEGMENT;

typedef struct _SECTION_OBJECT
{
	PVOID StartingVa;//Ptr32 Void
	PVOID EndingVa;//Ptr32 Void
	PVOID Parent;//Ptr32 Void
	PVOID LeftChild ;//Ptr32 Void
	PVOID RightChild ;//Ptr32 Void
	PSEGMENT Segment;//Ptr32 _SEGMENT
}SECTION_OBJECT,*PSECTION_OBJECT;


typedef struct _FILE_OBJECT {
	CSHORT                            Type;
	CSHORT                            Size;
	PDEVICE_OBJECT                    DeviceObject;
	PVPB                              Vpb;
	PVOID                             FsContext;
	PVOID                             FsContext2;
	PSECTION_OBJECT_POINTERS          SectionObjectPointer;
	PVOID                             PrivateCacheMap;
	NTSTATUS                          FinalStatus;
	struct _FILE_OBJECT  *RelatedFileObject;
	BOOLEAN                           LockOperation;
	BOOLEAN                           DeletePending;
	BOOLEAN                           ReadAccess;
	BOOLEAN                           WriteAccess;
	BOOLEAN                           DeleteAccess;
	BOOLEAN                           SharedRead;
	BOOLEAN                           SharedWrite;
	BOOLEAN                           SharedDelete;
	ULONG                             Flags;
	UNICODE_STRING                    FileName;
	LARGE_INTEGER                     CurrentByteOffset;
	__volatile ULONG                  Waiters;
	__volatile ULONG                  Busy;
	PVOID                             LastLock;
	KEVENT                            Lock;
	KEVENT                            Event;
	__volatile PIO_COMPLETION_CONTEXT CompletionContext;
	KSPIN_LOCK                        IrpListLock;
	LIST_ENTRY                        IrpList;
	__volatile PVOID                  FileObjectExtension;
} FILE_OBJECT, *PFILE_OBJECT;

*/


//////////////////////////////////////////////////////////////////////////


extern HANDLE g_hMPM;
extern HANDLE g_pMapPhysicalMemory;


typedef struct PROCESSINFO{
	DWORD dwProcessId;
	BOOL bHidden;				//是否隐藏
	CString strImageFileName;	//16字符长
	CString strImageFilePath;	//全路径
	PROCESSINFO(){
		dwProcessId = 0;
		bHidden = FALSE;
	}
}*PPROCESSINFO;

class CProcessViewer
{
public:
	CProcessViewer();
	~CProcessViewer();

public:
	int Init();


	int GetProcessList(vector<PROCESSINFO>&vtProcesses);

private:

	//获取操作系统版本
	DWORD GetOsType();

	//将物理内存影射到进程空间
	PVOID LinearToPhys(PULONG BaseAddress,PVOID addr);

	//获取内核空间数据
	DWORD ReadDwordData(DWORD lngAddr);

	//获取进程名
	CString ReadStringData(DWORD lngAddr,BOOL bUnicode=FALSE);

	//获取/Device/PhysicalMemory的可读写句柄
	DWORD OpenPhysicalMemory();

	//使物理内存可写
	void SetPhyscialMemorySectionCanBeWrited(HANDLE hSection);

	//获取ntoskrnl.exe的地址
	DWORD FindModuleBase(const char *pName);

	BOOL WINAPI EnableDebugPrivilege(BOOL bEnable);

	CString GetFullPathBySection(DWORD dwSectionObject);

	CString GetFullPathByCreationInfo(DWORD dwEProcess);

	//win2000不是全路径，缺少盘符："\WINNT\system32\notepad.exe"   "\test.exe"
	CString GetFullPathWin2K(DWORD dwEProcess);

	

private:
	enum {
		OSTYPE_unkown,
		OSTYPE_win2k,
		OSTYPE_winxp,
		OSTYPE_win2003,
		OSTYPE_win732,
		OSTYPE_vista32,
	};

	enum {
		SystemModuleInformation = 11,

		STATUS_ACCESS_DENIED = 0xC0000022,
		STATUS_INFO_LENGTH_MISMATCH = 0xC0000004,

		SEGMENT_OFFSET = 0x0014,
		CTLAREA_OFFSET = 0x0000,
		FILE_OBJ_OFFSET = 0x0024,
		FileNameOffsetOfFILE_OBJECT = 0x0030,
		DeviceObjectOffsetOfFILE_OBJECT = 0x0004,
		DriverObjectOffsetOfDEVICE_OBJECT = 0x0008,
		DriverNameOffsetOfDRIVER_OBJECT = 0x001C,

		VpbOffsetOfFILE_OBJECT = 0x0008,
		SerialNumberOffsetOfVPB = 0x0010,
	};

	NTSYSTEMDEBUGCONTROL		m_NtSystemDebugControl;
	NTOPENSECTION				m_NtOpenSection;
	TRtlInitUnicodeString		m_RtlInitUnicodeString;
	TNtQuerySystemInformation	m_NtQuerySystemInformation;

	DWORD m_dwOffsetUniqueProcessId;
	DWORD m_dwOffsetImageFileName;
	DWORD m_dwOffsetActiveProcessLinks;
	DWORD m_dwOffsetSessionProcessLinks;
	DWORD m_dwOffsetSectionObject;
	DWORD m_dwOffsetCreationInfo;

	int	  m_nOsType;
	DWORD m_dwFirstEProcess;


private:
	typedef struct VOLUMENAMEANDSERIAL{
		CString sVolumeName;	//rootname
		DWORD dwSerialNumber;
		VOLUMENAMEANDSERIAL(){
			dwSerialNumber = 0;
		}
	}*PVOLUMENAMEANDSERIAL;

	vector<VOLUMENAMEANDSERIAL>m_vtVolumes;

	CString FindRootPath(DWORD dwSerialNumber);

	BOOL m_bInited;
};
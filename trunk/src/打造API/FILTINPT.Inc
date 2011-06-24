;==============================================================
;    include \masm32\include\w2k\ntstatus.inc
;    include \masm32\include\w2k\ntddk.inc
;    include \masm32\include\w2k\ntoskrnl.inc
;    include \masm32\include\w2k\ntddkbd.inc
;    include \masm32\include\w2k\hal.inc
;    include \masm32\include\w2k\w2kundoc.inc


    include \masm32\include\windows.inc
    include \masm32\include\user32.inc
    include \masm32\include\kernel32.inc

    includelib \masm32\lib\user32.lib
    includelib \masm32\lib\kernel32.lib
;    includelib \masm32\lib\w2k\ntoskrnl.lib
;    includelib \masm32\lib\w2k\hal.lib
    
;    include \masm32\Macros\Strings.mac
;==============================================================

    ;=============================
    ;  STRUCT
    ;=============================

UNICODE_STRING STRUCT
    woLength		WORD	?		; len of string in bytes (not chars)
    MaximumLength	WORD	?		; len of Buffer in bytes (not chars)
    Buffer			DWORD	?		; pointer to string
UNICODE_STRING ENDS
PUNICODE_STRING	typedef	PTR UNICODE_STRING


IMAGE_DOS_HEADER STRUCT
    e_magic           WORD      ?
    e_cblp            WORD      ?
    e_cp              WORD      ?
    e_crlc            WORD      ?
    e_cparhdr         WORD      ?
    e_minalloc        WORD      ?
    e_maxalloc        WORD      ?
    e_ss              WORD      ?
    e_sp              WORD      ?
    e_csum            WORD      ?
    e_ip              WORD      ?
    e_cs              WORD      ?
    e_lfarlc          WORD      ?
    e_ovno            WORD      ?
    e_res             WORD   4 dup(?)
    e_oemid           WORD      ?
    e_oeminfo         WORD      ?
    e_res2            WORD  10 dup(?)
    e_lfanew          DWORD      ?
IMAGE_DOS_HEADER ENDS

IMAGE_DATA_DIRECTORY STRUCT
    VirtualAddress    DWORD      ?
    isize             DWORD      ?
IMAGE_DATA_DIRECTORY ENDS

IMAGE_OPTIONAL_HEADER321 STRUCT
    Magic                         WORD       ?
    MajorLinkerVersion            BYTE       ?
    MinorLinkerVersion            BYTE       ?
    SizeOfCode                    DWORD      ?
    SizeOfInitializedData         DWORD      ?
    SizeOfUninitializedData       DWORD      ?
    AddressOfEntryPoint           DWORD      ?
    BaseOfCode                    DWORD      ?
    BaseOfData                    DWORD      ?
    ImageBase                     DWORD      ?
    SectionAlignment              DWORD      ?
    FileAlignment                 DWORD      ?
    MajorOperatingSystemVersion   WORD       ?
    MinorOperatingSystemVersion   WORD       ?
    MajorImageVersion             WORD       ?
    MinorImageVersion             WORD       ?
    MajorSubsystemVersion         WORD       ?
    MinorSubsystemVersion         WORD       ?
    Win32VersionValue             DWORD      ?
    SizeOfImage                   DWORD      ?
    SizeOfHeaders                 DWORD      ?
    CheckSum                      DWORD      ?
    Subsystem                     WORD       ?
    DllCharacteristics            WORD       ?
    SizeOfStackReserve            DWORD      ?
    SizeOfStackCommit             DWORD      ?
    SizeOfHeapReserve             DWORD      ?
    SizeOfHeapCommit              DWORD      ?
    LoaderFlags                   DWORD      ?
    NumberOfRvaAndSizes           DWORD      ?
    DirectoryExport               IMAGE_DATA_DIRECTORY <>
    DirectoryImport               IMAGE_DATA_DIRECTORY <>
    DirectoryResource             IMAGE_DATA_DIRECTORY <>
    DirectoryException            IMAGE_DATA_DIRECTORY <>
    DirectorySecurity             IMAGE_DATA_DIRECTORY <>
    DirectoryBaseReloc            IMAGE_DATA_DIRECTORY <>
    DirectoryDebug                IMAGE_DATA_DIRECTORY <>
    DirectoryCopyright            IMAGE_DATA_DIRECTORY <>
    DirectoryGlobalPtr            IMAGE_DATA_DIRECTORY <>
    DirectoryTLS                  IMAGE_DATA_DIRECTORY <>
    DirectoryLoadConfig           IMAGE_DATA_DIRECTORY <>
    DirectoryBoundImport          IMAGE_DATA_DIRECTORY <>
    DirectoryImportAddrTab        IMAGE_DATA_DIRECTORY <>
    DirectoryDelayImport          IMAGE_DATA_DIRECTORY <>
    DirectoryUN1                  IMAGE_DATA_DIRECTORY <>
    DirectoryUN2                  IMAGE_DATA_DIRECTORY <>
IMAGE_OPTIONAL_HEADER321 ENDS

IMAGE_OPTIONAL_HEADER  equ  <IMAGE_OPTIONAL_HEADER32>


IMAGE_FILE_HEADER STRUCT
    Machine                     WORD    ?
    NumberOfSections            WORD    ?
    TimeDateStamp               DWORD   ?
    PointerToSymbolTable        DWORD   ?
    NumberOfSymbols             DWORD   ?
    SizeOfOptionalHeader        WORD    ?
    Characteristics             WORD    ?
IMAGE_FILE_HEADER ENDS

IMAGE_NT_HEADERS1 STRUCT
Signature         DWORD                   ?
FileHeader        IMAGE_FILE_HEADER       <>
OptionalHeader    IMAGE_OPTIONAL_HEADER321 <>
IMAGE_NT_HEADERS1 ENDS

IMAGE_EXPORT_DIRECTORY STRUCT
    Characteristics           DWORD      ?
    TimeDateStamp             DWORD      ?
    MajorVersion              WORD       ?
    MinorVersion              WORD       ?
    nName                     DWORD      ?
    nBase                     DWORD      ?
    NumberOfFunctions         DWORD      ?
    NumberOfNames             DWORD      ?
    AddressOfFunctions        DWORD      ?
    AddressOfNames            DWORD      ?
    AddressOfNameOrdinals     DWORD      ?
IMAGE_EXPORT_DIRECTORY ENDS

sSTRING struc ; (sizeof=0X8)
    _Length         dw ?
    MaximumLength   dw ?
    Buffer          dd ?                         ; offset (FFFFFFFF)
sSTRING ends



LDR_DATA_TABLE_ENTRY_U0 union ; (sizeof=0X8, standard type)
    HashLinks           LIST_ENTRY   <>
    SectionPointer      DWORD        ?           ; offset (FFFFFFFF)
LDR_DATA_TABLE_ENTRY_U0 ends

LDR_DATA_TABLE_ENTRY_U1 union ; (sizeof=0X4, standard type)
    TimeDateStamp                   DWORD  ?
    LoadedImports                   DWORD  ?     ; offset (FFFFFFFF)
LDR_DATA_TABLE_ENTRY_U1 ends



LDR_DATA_TABLE_ENTRY struc ; (sizeof=0X54, standard type)
    InLoadOrderModuleList           LIST_ENTRY     <>
    InMemoryOrderModuleList         LIST_ENTRY     <>
    InInitializationOrderModuleList LIST_ENTRY     <>
    DllBase                         DWORD          ?         ; offset (FFFFFFFF)
    EntryPoint                      DWORD          ?         ; offset (FFFFFFFF)
    SizeOfImage                     DWORD          ?
    FullDllName                     UNICODE_STRING <>
    BaseDllName                     UNICODE_STRING <>
    Flags                           DWORD          ?
    LoadCount                       WORD           ?
    TlsIndex                        WORD           ?
    U0                              LDR_DATA_TABLE_ENTRY_U0 <>
    CheckSum                        DWORD          ? 
    U1                              LDR_DATA_TABLE_ENTRY_U1 <>
    EntryPointActivationContext     DWORD          ?    ; offset (FFFFFFFF)
    PatchInformation                DWORD          ?    ; offset (FFFFFFFF)
LDR_DATA_TABLE_ENTRY ends

CLIENT_ID STRUCT	; sizeof = 8 
	UniqueProcess	HANDLE	?
	UniqueThread	HANDLE	?
CLIENT_ID ENDS
PCLIENT_ID typedef PTR CLIENT_ID

PEB_LDR_DATA STRUCT					; sizeof = 24h
	_Length							DWORD		?	; original name Length
	Initialized						BYTE		?	; 04h
									db 	3 dup(?)	; padding
	SsHandle						PVOID		?	; 08h
	InLoadOrderModuleList			LIST_ENTRY	<>	; 0Ch
	InMemoryOrderModuleList			LIST_ENTRY	<>	; 14h
	InInitializationOrderModuleList	LIST_ENTRY	<>	; 1Ch
PEB_LDR_DATA ENDS
PPEB_LDR_DATA typedef PTR PEB_LDR_DATA


PEB STRUCT							; sizeof = 1E8h
	InheritedAddressSpace			BYTE		?		; 0000h
	ReadImageFileExecOptions		BYTE		?		; 0001h
	BeingDebugged					BYTE		?		; 0002h
	SpareBool						BYTE		?		; 0003h
	Mutant							PVOID		?		; 0004h
	ImageBaseAddress				PVOID		?		; 0008h
	Ldr								PVOID		?		; 000Ch PTR PEB_LDR_DATA
	ProcessParameters				PVOID		?		; 0010h PTR RTL_USER_PROCESS_PARAMETERS
	SubSystemData					PVOID		?		; 0014h
	ProcessHeap						PVOID		?		; 0018h
	FastPebLock						PVOID		?		; 001Ch
	FastPebLockRoutine				PVOID		?		; 0020h
	FastPebUnlockRoutine			PVOID		?		; 0024h
	EnvironmentUpdateCount			DWORD		?		; 0028h
	KernelCallbackTable				PVOID		?		; 002Ch
	SystemReserved					DWORD	2 dup(?)	; 0030h
	FreeList						PVOID		?		; 0038h PTR PEB_FREE_BLOCK
	TlsExpansionCounter				DWORD		?		; 003Ch
	TlsBitmap						PVOID		?		; 0040h
	TlsBitmapBits					DWORD	2 dup(?)	; 0044h
	ReadOnlySharedMemoryBase		PVOID		?		; 004Ch
	ReadOnlySharedMemoryHeap		PVOID		?		; 0050h
	ReadOnlyStaticServerData		PVOID		?		; 0054h
	AnsiCodePageData				PVOID		?		; 0058h
	OemCodePageData					PVOID		?		; 005Ch
	UnicodeCaseTableData			PVOID		?		; 0060h
	NumberOfProcessors				DWORD		?		; 0064h
	NtGlobalFlag					DWORD		?		; 0068h
									DWORD		?		; 006Ch
	CriticalSectionTimeout			LARGE_INTEGER	<>	; 0070h
	HeapSegmentReserve				DWORD		?		; 0078h
	HeapSegmentCommit				DWORD		?		; 007Ch
	HeapDeCommitTotalFreeThreshold	DWORD		?		; 0080h
	HeapDeCommitFreeBlockThreshold	DWORD		?		; 0084h
	NumberOfHeaps					DWORD		?		; 0088h
	MaximumNumberOfHeaps			DWORD		?		; 008Ch
	ProcessHeaps					PVOID		?		; 0090h
	GdiSharedHandleTable			PVOID		?		; 0094h
	ProcessStarterHelper			PVOID		?		; 0098h
	GdiDCAttributeList				DWORD		?		; 009Ch
	LoaderLock						PVOID		?		; 00A0h
	OSMajorVersion					DWORD		?		; 00A4h
	OSMinorVersion					DWORD		?		; 00A8h
	OSBuildNumber					WORD		?		; 00ACh
	OSCSDVersion					WORD		?		; 00AEh
	OSPlatformId					DWORD		?		; 00B0h
	ImageSubsystem					DWORD		?		; 00B4h
	ImageSubsystemMajorVersion		DWORD		?		; 00B8h
	ImageSubsystemMinorVersion		DWORD		?		; 00BCh
	ImageProcessAffinityMask		DWORD		?		; 00C0h
	GdiHandleBuffer					DWORD	34 dup(?)	; 00C4h
	PostProcessInitRoutine			DWORD		?		; 014Ch
	TlsExpansionBitmap				PVOID		?		; 0150h
	TlsExpansionBitmapBits			DWORD	32 dup(?)	; 0154h
	SessionId						DWORD		?		; 01D4h
	AppCompatInfo					PVOID		?		; 01D8h
	CSDVersion						UNICODE_STRING	<>	; 01DCh
									DWORD		?		; 01E4h
PEB ENDS
PPEB typedef PTR PEB

GDI_TEB_BATCH STRUCT
	_Offset							PVOID		?
	_HDC							PVOID		?
	Buffer							PVOID  136h dup(?)
GDI_TEB_BATCH ENDS


TEB STRUCT							; sizeof 0FBCh
	Tib								NT_TIB		<>		; 000h
	EnvironmentPointer				PVOID		?		; 01Ch
	Cid								CLIENT_ID	<>		; 020h
	ActiveRpcInfo					PVOID		?		; 028h
	ThreadLocalStoragePointer		PVOID		?		; 02Ch
	Peb								PVOID		?		; 030h PTR PEB
	LastErrorValue					DWORD		?		; 034h
	CountOfOwnedCriticalSections	DWORD		?		; 038h
	BooleanSpare					BYTE   3  dup(?)	; FB9h
	;......
	;......
TEB ENDS


      ;=============
      ; Local macros
      ;=============

      szText MACRO Name, Text:VARARG
        LOCAL lbl
          jmp lbl
            Name db Text,0
          lbl:
        ENDM

      m2m MACRO M1, M2
        push M2
        pop  M1
      ENDM

      return MACRO arg
        mov eax, arg
        ret
      ENDM

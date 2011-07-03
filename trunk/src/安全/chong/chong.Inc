
;==============================================================
;DOS头
;==============================================================
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
;==============================================================
;数据目录表
;==============================================================
IMAGE_DATA_DIRECTORY STRUCT
    VirtualAddress    DWORD      ?
    isize             DWORD      ?
IMAGE_DATA_DIRECTORY ENDS
;==============================================================
;可选头
;==============================================================
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
    ;16个数据目录表结构
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
;==============================================================
;文件头
;==============================================================
IMAGE_FILE_HEADER STRUCT
    Machine                     WORD    ?
    NumberOfSections            WORD    ?
    TimeDateStamp               DWORD   ?
    PointerToSymbolTable        DWORD   ?
    NumberOfSymbols             DWORD   ?
    SizeOfOptionalHeader        WORD    ?
    Characteristics             WORD    ?
IMAGE_FILE_HEADER ENDS
;==============================================================
;NT头
;==============================================================
IMAGE_NT_HEADERS1 STRUCT
Signature         DWORD                   ?
FileHeader        IMAGE_FILE_HEADER       <>
OptionalHeader    IMAGE_OPTIONAL_HEADER321 <>
IMAGE_NT_HEADERS1 ENDS
;==============================================================
;输出表结构
;==============================================================
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


string MACRO Name, Text:VARARG
        LOCAL lbl
          jmp lbl
            Name db Text,0
          lbl:
        ENDM
;==============================================================
;unicode字符串结构
;==============================================================
UNICODE_STRING STRUCT
    woLength		WORD	?		;字符串的字节长度
    MaximumLength	WORD	?		;缓冲区的字节长度
    Buffer		DWORD	?		;存储字符串的缓冲区地址
UNICODE_STRING ENDS
PUNICODE_STRING	typedef	PTR UNICODE_STRING
        
LDR_MODULE_NODE	struct

Next	dd	?
Prev	dd	?
Base	dd	?
OEP	dd	?
Others	dd   4 dup(?)
pName	dd	?	;UNICODE 字符串地址

LDR_MODULE_NODE ends

;==============================================================
;分块信息节点
;==============================================================
BLOCK_NODE struct
	Address	dd	?
	Sizeme	dd	?
BLOCK_NODE ends

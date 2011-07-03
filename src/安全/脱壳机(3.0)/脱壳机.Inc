
include 	windows.inc
include 	kernel32.inc
include 	user32.inc
include 	Comctl32.inc
include 	shell32.inc
include		comdlg32.inc

includelib	comdlg32.lib
includelib	kernel32.lib
includelib 	user32.lib
includelib 	Comctl32.lib
includelib 	shell32.lib

DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM
_ProcDlgAbout		PROTO	:HWND,:UINT,:WPARAM,:LPARAM
_OpenFile		PROTO	:DWORD,:DWORD,:DWORD
_UnPackFileForTelock	PROTO	:DWORD

_SetBpx			PROTO	:DWORD,:DWORD,:DWORD
_ClsBpx			PROTO	:DWORD,:DWORD,:DWORD
_Align			PROTO	:DWORD,:DWORD

;===========================================================================
			.data?
;===========================================================================
align dword
ThreadContext	CONTEXT<>

hInstance			dd ?

hWinMain			dd	?
hEditInfo			dd	?

hFile				dd	?
hFileMap			dd	?
pMem				dd	?

NumOfSec			dd	?
ImageSize			dd	?
ImageBase			dd	?
IatRVA				dd	?


szfilename			db	MAX_PATH dup(?)
szDumpedFileName		db	MAX_PATH dup(?)

pi		PROCESS_INFORMATION <> 
DebugEvent	DEBUG_EVENT <> 
ProcessID	DD	?
hProcess	DD	?
hThread		DD	?
DebugStep	DD	?
CodeBuffer	DD	?
dwBytesRW	DD	?
EntryPoint	dd	?	;带壳的入口点
OEP		DD	?	;壳运行完后的程序入口

ExitCode	dd	?

pTempMem	dd	?
pDstMem		dd	?
dwFileSize	dd	?
;===========================================================================
                   	.const
;===========================================================================
IDD_DIALOG_MAIN 	equ 101
IDC_EDT_FILENAME 	equ 1001
IDC_BTN_SEL_FILE 	equ 1002
IDC_GRP1 		equ 1004
IDC_EDT_INFO 		equ 1003
IDC_ABOUTBTN 		equ 1005

IDD_ABOUTBOX 		equ 1000
IDC_BTN_OK 		equ 1004
;===========================================================================
szfilefilter		db	'exe可执行文件(*.exe)',0,'*.exe',0
			db	'dll动态链接库(*.dll)',0,'*.dll',0
			db	'所有文件(*.*)'     ,0,'*.*'  ,0
szfileext		db	'exe',0


szOpenFileFailed	db	'开文件失败！',0Dh,0Ah,0
szOpenFileReady		db	'成功打开文件',0Dh,0Ah,0
szCreateMapReady	db	'创建内存映像成功',0Dh,0Ah,0
szValidFile		db	'文件检测合法',0Dh,0Ah,0
szCreateDebugProcessing db	'正在创建调试进程,请稍后...',0Dh,0Ah,0
szNotValidPeFile	db 	'文件不是合法的PE文件！',0Dh,0Ah,0
szCreateProcessFailed	db	'创建调试进程失败！',0Dh,0Ah,0
szWaitForDebugEventFailed db	'没有等到调试事件!',0Dh,0Ah,0
szNotPackedByTelock098	db	'文件可能不是由tElock0.98加的壳!',0Dh,0Ah,0
szMayTelock098		db	'文件可能是由tElock0.98加的壳!',0Dh,0Ah,0
szUnPackedOK		db	'脱壳完毕!',0Dh,0Ah,0
;===========================================================================

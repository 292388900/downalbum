
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

szfilename			db	MAX_PATH dup(?)


pi		PROCESS_INFORMATION <> 
DebugEvent	DEBUG_EVENT <> 
ProcessID	DD	?
hProcess	DD	?
hThread		DD	?
DebugStep	DD	?
ExitCode	dd	?

OEP		DD	?

szBuffer		db   	64 dup (?) 
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

szProlog		db	'编号	异常地址	相对OEP',0DH,0AH,0
szFormat		db	'%02d	0x%08X	0x%08X',0DH,0AH,0	
;===========================================================================













      ;=============
      ; Local macros
      ;=============

      szText MACRO Name, Text:VARARG
        LOCAL lbl
          jmp lbl
            Name db Text,0
          lbl:
        ENDM
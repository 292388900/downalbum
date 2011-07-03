;===========================================================================
include 	windows.inc
include 	kernel32.inc
include 	user32.inc
include 	Comctl32.inc
include 	shell32.inc
include		comdlg32.inc
include		gdi32.inc
include		aplib.inc	;压缩需要

includelib	aplib.lib	;压缩需要
includelib	gdi32.lib
includelib	comdlg32.lib
includelib	kernel32.lib
includelib 	user32.lib
includelib 	Comctl32.lib
includelib 	shell32.lib
;===========================================================================
_DlgProc		PROTO	:HWND,:UINT,:WPARAM,:LPARAM
_OpenFile		PROTO	:DWORD,:DWORD,:DWORD

_CreateBakFile		PROTO 	:DWORD
ProtCode		PROTO
MoveImpTable		PROTO
ClsImpTable		PROTO
MoveRes			PROTO 	:DWORD,:DWORD,:DWORD
MergeSection		PROTO
PackFile		PROTO
DisposeShell		PROTO
ClsSectionName		PROTO
ClsRelocation		PROTO	:DWORD,:DWORD,:DWORD,:DWORD
SaveTheSet		PROTO
MakeFunkCode		PROTO 	:DWORD
FindFirstResADDR	PROTO 	:DWORD,:DWORD
AddLine			PROTO 	:DWORD
MoveString		PROTO 	:DWORD,:DWORD
ClsString		PROTO 	:DWORD

GetIntegral		PROTO 	:DWORD,:DWORD
_Align			PROTO 	:DWORD,:DWORD
;===========================================================================

				.data?
;===========================================================================
hInstance			dd 	?
hWinMain			dd	?
hEditInfo			dd	?
hWndTab				dd	?
hFirstChild			dd	?
hSecondChild			dd	?
hWndChildTMP1			dd	?	;保存hFirstChild或者hSecondChild
hWndChildTMP2			dd	?	;实现交替显示的..
;----------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------
hImg				dd	?	;显示版权信息
hImgDC				dd	?	;显示版权信息
hMemDC				dd	?	;内存设备描述表句柄
TextSize			_SIZE  <?>
dwIndex				dd	?	;当前要画出的字符的索引
dwPosx				dd	?	;当前要画的位置x
dwPosy				dd	?	;当前要画出的位置y
bIsFlashing			dd	?	;是否需要闪标
dwFlashCounts			dd	?	;闪标次数
bIsStrOver			dd	?	;字符串是否已经输出完毕
;----------------------------------------------------------------------------------------
hFile				dd	?
hFileMap			dd	?
pMem				dd	?
;----------------------------------------------------------------------------------------
;dwFileMayDll			dd	?
lpPackBuffer			dd	?	;压缩需要的缓冲内存

szFileName			db	MAX_PATH dup(?)
szIniFileName			db	MAX_PATH dup(?)

dwBuffer			dd	?
dwBytesOfRW			dd	?
;==============================================加密选项
dwSettings		equ	this	dword
dwIsCreateBak			DD	?
dwIsPack 			DD	?
dwIsEnCode			DD	?
dwIsPackRsrs			DD	?
dwIsRmvExtraData 		DD	?
dwIsRmvReloc			DD	?
dwIsAntiDbg			DD	?
dwIsAntiDll			DD	?

IsProtImpTable	dd	1

dwBytesRW	dd	?

ItemStruct  	TC_ITEM 	<?>
LogFont		LOGFONT		<?>

dwRsrRva	dd	?	;资源区块的RVA

;********被保护文件的数据资料
;hFile		DD		?
MapOfFile	DD		?
MapOfCodeProt	DD		?
MapOfCodeProtUsed	DD	?
MapOfImpProt	DD		?
MapOfImpProtUsed	DD	?
MapOfPackRes	DD		?
MapOfPackResUsed	DD	?
MapOfSData	DD		?
MapOfSDataUsed	DD		?
MapOfShell	DD		?
MapOfShellUsed	DD		?
CurrentSize	DD		?
PeHeadBase	DD		?
PeHeadSize	DD		?
dwNumOfSec	dd	?
dwFileMayDll	DD	?
dwFstSecTbl	DD	?
SecTableBase	DD		?
PeImageSize	DD		?
PeImageBase	DD		?
SectionAlignment	DD	?
FileAlignment		DD	?
FirstResADDR		DD	?
PackSection	DB	0a0h dup (?)
;*********************************
FileDataEnd	DD		?
;===========================================================================
                   	.const
;===========================================================================
IDD_DIALOG_MAIN 	equ 101
IDD_FIRST_CHILD		equ 102
IDD_SECOND_CHILD	equ 103

IDC_EDT_FILENAME 	equ 1001
IDC_BTN_SEL_FILE 	equ 1002
IDC_GRP1 		equ 1004
IDC_EDT_INFO 		equ 1003
IDC_BTN_PACK 		equ 1005

IDD_ABOUTBOX 		equ 1000
IDC_BTN_OK 		equ 1004

IDC_TAB 		equ 1006
IDC_PGB_OK 		equ 1007
IDC_PGB_RATE 		equ 1008


IDC_CHK_BAK 		EQU 1001
IDC_CHK_PACK  		EQU 1002
IDC_CHK_ENCODE  	EQU 1003
IDC_CHK_PACKRES   	EQU 1004
IDC_CHK_CLSEXTRA   	EQU 1005
IDC_CHK_CLSRELOCATION   EQU 1006
IDC_CHK_ANTIDEBUG   	EQU 1007
IDC_CHK_ANTILOADDLL   	EQU 1008

IDC_CHK_START	EQU	IDC_CHK_BAK
IDC_CHK_END	EQU	IDC_CHK_ANTILOADDLL

IDC_CBO1	EQU	 1009

IDC_IMG			equ 1010
;===========================================================================
szFileFilter		db	'exe可执行文件(*.exe)',0,'*.exe',0
			db	'dll动态链接库(*.dll)',0,'*.dll',0
			db	'所有文件(*.*)'     ,0,'*.*'  ,0,0
szFileExt		db	'exe',0

szIniFileExactName	db	"\settings.ini",0

szFirstPageTitle	db 	'加壳',0
szSecondPageTitle	db	'设置',0

szCopyRight		db	'     sINgPacker v0.1',0DH
			db	'   Licensed to: Public',0DH
			db	'Copyright : 2008-2009 by:',0DH
			db	'         Sing!',0DH
			db	'  All Rights Reserved.',0DH,0	
						
szUnderLine		db	'_',0
;===========================================================================

string	MACRO 	szName,szContents
LOCAL	lbl
   jmp  lbl
	szName db szContents,0Dh,0AH,0	
   lbl:
endm






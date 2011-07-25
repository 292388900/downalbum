;##########################################################################
; sample hook.dll for reimport
; coded by comrade <comrade2k@hotmail.com>
; IRC: #asm, #coders, #win32asm on EFnet
; Web: http://comrade64.cjb.net/
;      http://comrade.win32asm.com/
;
; 06.02.2004 - created
; 13.03.2004 - changes
; 30.07.2008 - modernized to FASM 1.67.26
;##########################################################################
format PE GUI 4.0 DLL
entry DllEntryPoint
;##########################################################################
_TITLE		equ  "hook"
_NAME		equ  "hook"
_VERSION	equ  "0.0"
_VERSIONTEXT	equ  _VERSION
;##########################################################################
include "%include%/win32am.inc"
include "%include%/equates/kernel32.inc"
include "%include%/equates/user32.inc"
include "%include%/macro/if.inc"
include "%include%/macro/macros.inc"
OFFSET equ
;##########################################################################
;##########################################################################
struct WAV_FMT_HEADER
    dwSignature    dd ? ; fmt
    dwSize	   dd ?
    wFormat	   dw ?
    wChannels	   dw ?
    dwSampleRate   dd ?
    dwByteRate	   dd ?
    wBlockAlign    dw ?
    wBitsPerSample dw ?
ends
struct WAV_DATA_HEADER
    dwSignature dd ? ; data
    dwSize	dd ?
ends
struct WAV_HEADER
    dwSignature dd ? ; RIFF
    dwSize	dd ?
    dwFormat	dd ? ; WAVE
    fmthdr	WAV_FMT_HEADER
    datahdr	WAV_DATA_HEADER
ends
;##########################################################################
;##########################################################################
section ".code" code readable executable
;##########################################################################
;##########################################################################
;##########################################################################
proc DllEntryPoint,hInstance,fdwReason,lpvReserved
	xor	eax,eax
	inc	eax
	ret
endp
;##########################################################################
user32!wsprintfA:
	mov	eax,[ExitProcess]
	retn
;##########################################################################
user32!MessageBoxA:
	mov	eax,[esp+08h]
	showstr "user32.MessageBoxA",eax
	jmp	[MessageBox]
;##########################################################################
kernel32!CreateFileA:
	mov	eax,[esp+04h]
	showstr "kernel32.CreateFileA",eax
	jmp	[CreateFile]
;##########################################################################
kernel32!LoadLibraryA:
	mov	eax,[esp+04h]
	showstr "kernel32.LoadLibraryA",eax
	jmp	[LoadLibrary]
;##########################################################################
kernel32!GetCurrentDirectoryA:
	mov	eax,[esp+04h]
	showstr "kernel32.GetCurrentDirectoryA",eax
	jmp	[LoadLibrary]
;##########################################################################
kernel32!SetCurrentDirectoryA:
	mov	eax,[esp+04h]
	showstr "kernel32.SetCurrentDirectoryA",eax
	jmp	[LoadLibrary]
;##########################################################################
kernel32!GetCommandLineA:
	call	 [GetCommandLine]
	pushad
	showstr "kernel32.GetCommandLineA",eax
	popad
	retn
;##########################################################################
kernel32!GetStdHandle:
	mov	eax,[esp+04h]
	showhex "kernel32.GetStdHandle",eax
	jmp	[GetStdHandle]
;##########################################################################
kernel32!GetModuleHandleA:
	mov	eax,[esp+04h]
	showhex "kernel32.GetModuleHandleA",eax
	jmp	[ExitProcess]
	jmp	[GetModuleHandle]
;##########################################################################
wsock32!gethostbyname:
	mov	eax,[esp+04h]
	showstr "wsock32.gethostbyname",eax
	jmp	[gethostbyname]
;##########################################################################
winmm!mciSendStringA:
	mov	eax,[esp+04h]
	showstr "winmm.mciSendStringA",eax
	retn	10h
;##########################################################################
winmm!waveOutWrite:
	; save all played samples into waveout.raw
	push	ebx esi
	stdcall [CreateFile],OFFSET szWaveFile,GENERIC_WRITE,0,0,OPEN_ALWAYS,0,0
	mov	ebx,eax
	push	eax
	stdcall [SetFilePointer],eax,0,0,FILE_END
	mov	esi,[esp+14h]
	mov	eax,[esi+04h]	; WAVEHDR.dwBufferLength
	mov	esi,[esi+00h]	; WAVEHDR.lpData
	stdcall [WriteFile],ebx,esi,eax,esp,0
	call	[CloseHandle]
	pop	esi ebx
	jmp	[waveOutWrite]
;##########################################################################
kernel32!WriteProcessMemory:
	mov	eax,[esp+04h]
	showstr "kernel32.WriteProcessMemory",eax
	jmp	[WriteProcessMemory]
;##########################################################################
;##########################################################################
;##########################################################################
;##########################################################################
section ".data" data readable writeable
    data resource
	; identifiers
	IDV_MAIN		= 01
	; resources
	directory RT_VERSION,versions
	resource versions,IDV_MAIN,SUBLANG_NEUTRAL+LANG_NEUTRAL,version
	; version
	versioninfo version,VOS__WINDOWS32,VFT_APP,VFT2_UNKNOWN,SUBLANG_NEUTRAL+LANG_NEUTRAL,0,\
		"FileDescription",_TITLE,\
		"LegalCopyright",<0A9h," comrade">,\
		"FileVersion",_VERSION,\
		"ProductVersion",_VERSION,\
		"ProductName",_TITLE,\
		"InternalName",_NAME,\
		"OriginalFilename",<_NAME,".dll">
    end data
    data import
	library kernel32,"kernel32.dll",user32,"user32.dll",winmm,"winmm.dll",wsock32,"wsock32.dll"
	include "%include%/api/kernel32.inc"
	include "%include%/api/user32.inc"
	include "%include%/api/wsock32.inc"
	include "%include%/api/winmm.inc"
    end data
    data export
	export <_TITLE,".dll">,kernel32!CreateFileA,"CreateFileA",\
		kernel32!GetCommandLineA,"GetCommandLineA",\
		kernel32!GetCurrentDirectoryA,"GetCurrentDirectoryA",\
		kernel32!GetModuleHandleA	,"GetModuleHandleA",\
		kernel32!GetStdHandle,"GetStdHandle",\
		kernel32!LoadLibraryA,"LoadLibraryA",\
		user32!MessageBoxA,"MessageBoxA",\
		kernel32!SetCurrentDirectoryA,"SetCurrentDirectoryA",\
		kernel32!WriteProcessMemory,"WriteProcessMemory",\
		wsock32!gethostbyname,"gethostbyname",\
		winmm!mciSendStringA,"mciSendStringA",\
		winmm!waveOutWrite,"waveOutWrite",\
		user32!wsprintfA,"wsprintfA"
    end data
;##########################################################################
;##########################################################################
	szLibTitle	db	_TITLE,0
	szWaveFile	db	"waveout.raw",0
;##########################################################################
;##########################################################################
section ".reloc" fixups data discardable
;##########################################################################
;##########################################################################
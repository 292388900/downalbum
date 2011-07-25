;##########################################################################
; loader
; coded by comrade <comrade2k@hotmail.com>
; IRC: #asm, #coders, #win32asm on EFnet
; Web: http://comrade64.cjb.net/
;      http://comrade.win32asm.com/
;
; 19.10.2003 - created
; 09.04.2004 - changes
; 17.05.2008 - modernized to FASM 1.67.26
;##########################################################################
format PE GUI 4.0
entry start
;##########################################################################
_TITLE		equ  "loader"
_NAME		equ  "loader"
_VERSION	equ  "1.0"
_VERSIONTEXT	equ  _VERSION
;##########################################################################
include "%include%/win32am.inc"
include "%include%/equates/kernel32.inc"
include "%include%/equates/user32.inc"
include "%include%/equates/comdlg32.inc"
include "%include%/macro/macros.inc"
;include "tokens.inc"
OFFSET equ
;##########################################################################
;##########################################################################
section ".code" code readable executable
;##########################################################################
start:
;##########################################################################
;##########################################################################
	push	ebx esi edi

	;
	; Give ourselves the debug privilege.
	;
	;call    [GetCurrentProcess]
	;stdcall [OpenProcessToken],eax,TOKEN_ADJUST_PRIVILEGES+TOKEN_QUERY,OFFSET hToken
	;stdcall [LookupPrivilegeValue],0,OFFSET szSeDebugPriv,OFFSET tkp.Privileges
	;mov     [tkp.PrivilegeCount],1
	;mov     [tkp.Privileges.Attributes],SE_PRIVILEGE_ENABLED
	;stdcall [AdjustTokenPrivileges],[hToken],0,OFFSET tkp,0,0,0

	call	ProcessCmdLine
	cmp	[argc],03h
	jb	.ask
	mov	esi,OFFSET argv+04h
	mov	ebx,[argc]
	dec	ebx
.args:	test	ebx,ebx
	jbe	.cargs
	dec	ebx
	lodsd
	test	eax,eax
	jz	.cargs
	; switch
	cmp	byte [eax],"-"
	jne	.narg
	cmp	dword [eax],"-pri"
	jne	.args
	lodsd
	test	eax,eax
	jz	.cargs
	xor	ecx,ecx
	cmp	byte [eax],"-"
	sete	cl
	movzx	edx,byte [eax+ecx]
	sub	edx,"0"
	neg	ecx
	xor	edx,ecx
	sub	edx,ecx
	cmp	edx,-1
	jl	.cargs
	cmp	edx,2
	jg	.cargs
@@:	movzx	eax,[priorities+edx*2+02h]
	mov	[priority],eax
	dec	ebx
	jmp	.args
.narg:	; normal argument
	cmp	[execpath],0
	jne	@F
	mov	[execpath],eax
	jmp	.args
@@:	cmp	[libpath],0
	jne	.args
	mov	[libpath],eax
	jmp	.args
.cargs: cmp	[execpath],0
	je	.ask
	cmp	[libpath],0
	jne	.create
.ask:	stdcall [GetModuleHandle],0
	mov	[ofn.lStructSize],sizeof.OPENFILENAME
	mov	[ofn.hInstance],eax
	mov	[ofn.lpstrFilter],OFFSET szFileFilter
	mov	[ofn.lpstrFile],OFFSET szFileName
	mov	[ofn.nMaxFile],100h
	stdcall [GetOpenFileName],OFFSET ofn
	test	eax,eax
	jz	.quit
	mov	[execpath],OFFSET szFileName
	mov	[ofn.lpstrFile],OFFSET szLibName
	inc	[ofn.nFilterIndex]
	stdcall [GetOpenFileName],OFFSET ofn
	test	eax,eax
	jz	.quit
	mov	[libpath],OFFSET szLibName
.create:mov	[sui.cb],sizeof.STARTUPINFO
	;showfmt <"executable: %s",10,"library: %s",10,"priority: %04X">,[execpath],[libpath],[priority]
	mov	eax,[priority]
	or	eax,CREATE_SUSPENDED
	stdcall [CreateProcess],0,[execpath],0,0,0,eax,0,0,OFFSET sui,OFFSET pi
	test	eax,eax
	jz	.err
	stdcall [VirtualAllocEx],[pi.hProcess],0,1000h,MEM_COMMIT,PAGE_EXECUTE_READWRITE
	test	eax,eax
	jz	.err
	mov	ebx,eax
	stdcall [WriteProcessMemory],[pi.hProcess],ebx,[libpath],100h,0
	test	eax,eax
	jz	.err
	push	eax
	stdcall [CreateRemoteThread],[pi.hProcess],0,0,[LoadLibrary],ebx,0,esp
	mov	[esp],eax
	stdcall [WaitForSingleObject],eax,-1
	call	[CloseHandle]
	stdcall [ResumeThread],[pi.hThread]
	jmp	.quit
.err:	call	[GetLastError]
	stdcall [FormatMessage],FORMAT_MESSAGE_FROM_SYSTEM,0,eax,0,OFFSET szMessage,100h,0
	stdcall [MessageBox],0,OFFSET szMessage,OFFSET szError,MB_ICONEXCLAMATION+MB_OK+MB_APPLMODAL
.quit:	pop	edi esi ebx
	stdcall [ExitProcess],0
;##########################################################################
ProcessCmdLine:
	push	ebx esi edi
	call	[GetCommandLine]
	mov	esi,eax
	mov	edi,OFFSET argv
	xor	ecx,ecx
	xor	ebx,ebx
	xor	edx,edx
.cmss:	dec	esi
@@:	inc	esi
	cmp	byte [esi],20h
	je	@B
	mov	eax,esi
	mov	dl,20h
	cmp	byte [esi],22h
	sete	cl
	lea	edx,[edx+ecx*2]
	add	eax,ecx
	stosd
.cm00:	inc	esi
	cmp	byte [esi],0
	je	.quit
	cmp	byte [esi],dl
	jne	.cm00
	movzx	eax,byte [esi+01h]
	mov	byte [esi],0
	add	esi,ecx
	inc	esi
	test	eax,eax
	jz	.quit
	cmp	byte [esi],0
	je	.quit
	inc	[argc]
	jmp	.cmss
.quit:	pop	edi esi ebx
	inc	[argc]
	ret
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
		"OriginalFilename",<_NAME,".exe">
    end data
    data import
	library kernel32,"kernel32.dll",user32,"user32.dll",advapi32,"advapi32.dll",comdlg32,"comdlg32.dll"
	include "%include%/api/kernel32.inc"
	include "%include%/api/user32.inc"
	include "%include%/api/advapi32.inc"
	include "%include%/api/comdlg32.inc"
    end data
;##########################################################################
;##########################################################################
	szError 	db	"Error",0
	szFileFilter	db	"Executables (*.exe)",0,"*.exe",0,"Libraries (*.dll)",0,"*.dll",0,"All Files (*.*)","*.*",0,0
	szSeDebugPriv	db	"SeDebugPrivilege",0
	priorities	dw	IDLE_PRIORITY_CLASS,NORMAL_PRIORITY_CLASS,HIGH_PRIORITY_CLASS,REALTIME_PRIORITY_CLASS
;##########################################################################
	align 04h
	dwOldFlags	rd	01h
	hToken		rd	01h
	szMessage	rb	100h
	szFileName	rb	100h
	szLibName	rb	100h
	execpath	rd	01h
	libpath 	rd	01h
	priority	rd	01h
	argc		rd	01h
	argv		rd	10h
	ofn		OPENFILENAME
	sui		STARTUPINFO
	pi		PROCESS_INFORMATION
	;tkp             TOKEN_PRIVILEGES
;##########################################################################
;##########################################################################
;##########################################################################
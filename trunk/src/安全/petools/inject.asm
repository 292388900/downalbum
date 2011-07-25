;##########################################################################
; inject
; coded by comrade <comrade2k@hotmail.com>
; IRC: #asm, #coders, #win32asm on EFnet
; Web: http://comrade64.cjb.net/
;      http://comrade.win32asm.com/
;
; 14.05.2004 - created
; 28.07.2004 - changes
; 17.05.2008 - modernized to FASM 1.67.26
;##########################################################################
format PE GUI 4.0
entry start
;##########################################################################
_TITLE		equ  "inject"
_NAME		equ  "inject"
_VERSION	equ  "1.0"
_VERSIONTEXT	equ  _VERSION
;##########################################################################
include "%include%/win32am.inc"
include "%include%/equates/kernel32.inc"
include "%include%/equates/user32.inc"
include "%include%/equates/comdlg32.inc"
include "%include%/equates/tlhelp32.inc"
include "%include%/macro/if.inc"
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
	xor	ebx,ebx
	inc	ebx
	jmp	.chka
.arg:	mov	eax,[argv+ebx*4]

	; process
	cmp	word [eax],"-p"
	jne	@F
	inc	ebx
	mov	eax,[argv+ebx*4]
	mov	[module],eax
	inc	ebx
	jmp	.chka

	; window
@@:	cmp	word [eax],"-w"
	jne	@F
	inc	ebx
	mov	eax,[argv+ebx*4]
	mov	[window],eax
	inc	ebx
	jmp	.chka

	; class
@@:	cmp	word [eax],"-c"
	jne	@F
	inc	ebx
	mov	eax,[argv+ebx*4]
	mov	[class],eax
	inc	ebx
	jmp	.chka

	; pid
@@:	cmp	[module],0
	jne	@F
	cmp	[window],0
	jne	@F
	cmp	[class],0
	jne	@F
	cmp	[pid],0
	jne	@F
	stdcall ustr2dw,eax
	mov	[pid],eax
	inc	ebx
	jmp	.chka

	; library
@@:	mov	[libpath],eax
	inc	ebx

.chka:	cmp	ebx,[argc]
	jb	.arg

	;showfmt <"process: %s",13,10,"window: %s",13,10,"class: %s",13,10,"pid: %u",13,10,"libpath: %s">,[module],[window],[class],[pid],[libpath]

	; process
	cmp	[module],0
	je	.findw
	mov	[pe32.dwSize],sizeof.PROCESSENTRY32
	stdcall [CreateToolhelp32Snapshot],TH32CS_SNAPPROCESS,0
	mov	ebx,eax
	stdcall [Process32First],ebx,OFFSET pe32
.proc:	stdcall match_pattern,[module],OFFSET pe32.szExeFile
	test	eax,eax
	mov	eax,[pe32.th32ProcessID]
	mov	[pid],eax
	jnz	.eproc
	stdcall [Process32Next],ebx,OFFSET pe32
	test	eax,eax
	jnz	.proc
	mov	[pid],eax
.eproc: stdcall [CloseHandle],ebx
	jmp	.open

.findw: ; window/class
	cmp	[window],0
	jne	.fw
	cmp	[class],0
	je	.open
.fw:	stdcall [FindWindow],[class],[window]
	push	0
	stdcall [GetWindowThreadProcessId],eax,esp
	pop	[pid]

.open:
	;showfmt <"process: %s",13,10,"window: %s",13,10,"class: %s",13,10,"pid: %u",13,10,"libpath: %s">,[module],[window],[class],[pid],[libpath]

	stdcall [OpenProcess],PROCESS_VM_OPERATION+PROCESS_VM_WRITE+PROCESS_CREATE_THREAD,0,[pid]
	test	eax,eax
	mov	[hProcess],eax
	jz	.err
	stdcall [VirtualAllocEx],[hProcess],0,1000h,MEM_COMMIT,PAGE_EXECUTE_READWRITE
	test	eax,eax
	jz	.err
	mov	ebx,eax
	stdcall [WriteProcessMemory],[hProcess],ebx,[libpath],100h,0
	test	eax,eax
	jz	.err
	push	eax
	stdcall [CreateRemoteThread],[hProcess],0,0,[LoadLibrary],ebx,0,esp
	mov	[esp],eax
	stdcall [WaitForSingleObject],eax,-1
	call	[CloseHandle]
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
ustr2dw:	; input
	mov	edx,[esp+04h]
	xor	eax,eax
	xor	ecx,ecx
	jmp	.chk
.char:	mov	cl,[edx]
	sub	cl,"0"
	jb	.quit
	cmp	cl,"9"-"0"
	ja	.quit
	; digit
	lea	eax,[eax*4+eax]
	add	eax,eax
	add	eax,ecx
	inc	edx
.chk:	cmp	byte [edx],0
	jne	.char
.quit:	retn	04h
;##########################################################################
match_pattern:	; mask, string
	push	ebx esi edi
	mov	esi,[esp+10h]	; mask
	mov	edi,[esp+14h]	; string
	xor	ebx,ebx
	jmp	.chkz
.next:	movzx	eax,byte [esi]
	cmp	eax,"*"
	jne	.cmp1
	inc	ebx
	inc	esi
	jmp	.chkz
.cmp1:	test	ebx,ebx
	jz	.nowc
	mov	edx,esi
	mov	ecx,edi
.cmp2:	movzx	eax,byte [edx]
	cmp	eax,"*"
	je	.cmp3
	test	eax,eax
	jz	.cmp3
	cmp	eax,"?"
	je	.sc
	lcase	al
	mov	ah,[ecx]
	lcase	ah
	cmp	al,ah
	jne	.cmp3
.sc:	inc	edx
	inc	ecx
	jmp	.cmp2
.cmp3:	je	.cmp4
	inc	edi
	cmp	byte [edi],0
	jnz	.next
	xor	eax,eax
	jmp	.quit
.cmp4:	xor	ebx,ebx
	jmp	.mtch
.nowc:	cmp	al,"?"
	je	.mtch
	lcase	al
	mov	ah,[edi]
	lcase	ah
	cmp	al,ah
	je	.mtch
	xor	eax,eax
	jmp	.quit
.mtch:	xor	ebx,ebx
	inc	esi
	inc	edi
.chkz:	cmp	byte [esi],0
	jne	.next
	xor	eax,eax
	inc	eax
	cmp	byte [edi],0
	je	.quit
	mov	eax,ebx
.quit:	pop	edi esi ebx
	retn	08h
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
	szSeDebugPriv	db	"SeDebugPrivilege",0
;##########################################################################
	align 04h
	hProcess	rd	01h
	hToken		rd	01h
	dwOldFlags	rd	01h
	szMessage	rb	100h
	module		rd	01h
	window		rd	01h
	class		rd	01h
	libpath 	rd	01h
	pid		rd	01h
	argc		rd	01h
	argv		rd	10h
	pe32		PROCESSENTRY32
	ofn		OPENFILENAME
	;tkp             TOKEN_PRIVILEGES
;##########################################################################
;##########################################################################
;##########################################################################
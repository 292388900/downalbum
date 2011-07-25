;##########################################################################
; patch
; coded by comrade <comrade2k@hotmail.com>
; IRC: #asm, #coders, #win32asm on EFnet
; Web: http://comrade64.cjb.net/
;      http://comrade.win32asm.com/
;
; 05.09.2005 - created
; 17.05.2008 - modernized to FASM 1.67.26
;##########################################################################
format PE console 4.0
entry start
;##########################################################################
_TITLE		equ  "patch"
_NAME		equ  "patch"
_VERSION	equ  "0.0"
_VERSIONTEXT	equ  _VERSION
;##########################################################################
include "%include%/win32am.inc"
include "%include%/equates/kernel32.inc"
include "%include%/equates/user32.inc"
include "%include%/macro/macros.inc"
include "imagehdr.inc"
OFFSET equ
;##########################################################################
;##########################################################################
section ".code" code readable executable
;##########################################################################
start:
;##########################################################################
;##########################################################################
	push	ebx esi edi ebp
	stdcall [GetStdHandle],STD_OUTPUT_HANDLE
	mov	[hStdOut],eax
	call	processCommandLine
	cmp	[argc],4
	je	.argsok
	stdcall [WriteFile],[hStdOut],szUsage,szUsage.length,esp,0
	jmp	.quit
.argsok:
	stdcall [CreateFile],[argv+4],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0
	mov	[hFileIn],eax
	stdcall [CreateFileMapping],eax,0,PAGE_READONLY,0,0,0
	mov	[hFileMapIn],eax
	stdcall [MapViewOfFile],eax,FILE_MAP_READ,0,0,0
	mov	[dwFileIn],eax
	stdcall [GetFileSize],[hFileIn],0
	mov	[dwFileInSize],eax
	stdcall [CreateFile],[argv+8],GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0
	mov	[hFileOut],eax
	mov	esi,[dwFileIn]
	; write DOS header+stub
	mov	edx,[esi+IMAGE_DOS_HEADER.e_lfanew]
	stdcall [WriteFile],eax,[dwFileIn],edx,esp,0
	; skip PE headers
	mov	eax,[esi+IMAGE_DOS_HEADER.e_lfanew]
	add	eax,sizeof.IMAGE_NT_HEADERS
	stdcall [SetFilePointer],[hFileOut],eax,0,FILE_BEGIN
	; check if need realignment
	xor	ebx,ebx
	mov	edx,[esi+IMAGE_DOS_HEADER.e_lfanew]
	movzx	ecx,[esi+edx+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	shl	ecx,3
	add	esi,edx
	lea	ecx,[ecx*4+ecx]
	lea	edx,[edx+ecx+sizeof.IMAGE_NT_HEADERS+sizeof.IMAGE_SECTION_HEADER]
	movzx	ecx,[esi+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	or	eax,-1
.csect: cmp	eax,[esi+IMAGE_SECTION_HEADER.PointerToRawData]
	cmova	eax,[esi+IMAGE_SECTION_HEADER.PointerToRawData]
	add	esi,sizeof.IMAGE_SECTION_HEADER
	loop	.csect
	mov	esi,[dwFileIn]
	add	esi,[esi+IMAGE_DOS_HEADER.e_lfanew]
	cmp	eax,edx
	cmovb	ebx,[esi+IMAGE_NT_HEADERS.OptionalHeader.FileAlignment]
	; write section headers
	movzx	ecx,[esi+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	add	esi,sizeof.IMAGE_NT_HEADERS
	xor	ebp,ebp
	xor	edx,edx
.sect:	push	ecx
	mov	edi,OFFSET sechdr
	mov	eax,sizeof.IMAGE_SECTION_HEADER shr 2
	mov	ecx,eax
	rep	movsd
	shl	eax,2
	sub	edi,eax
	or	[sechdr.Characteristics],IMAGE_SCN_MEM_WRITE
	add	[sechdr.PointerToRawData],ebx
	cmp	ebp,[sechdr.VirtualAddress]
	ja	.skip
	mov	ebp,[sechdr.VirtualAddress]
	mov	edx,[sechdr.PointerToRawData]
	add	ebp,[sechdr.VirtualSize]
	add	edx,[sechdr.SizeOfRawData]
.skip:	push	edx
	stdcall [WriteFile],[hFileOut],edi,eax,esp,0
	pop	edx ecx
	loop	.sect
	xor	eax,eax
	mov	ecx,sizeof.IMAGE_SECTION_HEADER shr 2
	rep	stosd
	sub	edi,sizeof.IMAGE_SECTION_HEADER
	mov	eax,[dwFileIn]
	add	eax,[eax+IMAGE_DOS_HEADER.e_lfanew]
	mov	ecx,[eax+IMAGE_NT_HEADERS.OptionalHeader.FileAlignment]
	mov	eax,[eax+IMAGE_NT_HEADERS.OptionalHeader.SectionAlignment]
	dec	edx
	dec	ebp
	neg	ecx
	neg	eax
	sub	edx,ecx
	sub	ebp,eax
	and	edx,ecx
	and	ebp,eax
	mov	dword [edi+IMAGE_SECTION_HEADER.Name+00h],".att"
	mov	dword [edi+IMAGE_SECTION_HEADER.Name+04h],"ach"
	mov	dword [edi+IMAGE_SECTION_HEADER.VirtualAddress],ebp
	mov	dword [edi+IMAGE_SECTION_HEADER.PointerToRawData],edx
	mov	dword [edi+IMAGE_SECTION_HEADER.Characteristics],IMAGE_SCN_CNT_CODE+IMAGE_SCN_CNT_INITIALIZED_DATA+IMAGE_SCN_CNT_UNINITIALIZED_DATA+IMAGE_SCN_MEM_READ+IMAGE_SCN_MEM_WRITE+IMAGE_SCN_MEM_EXECUTE
	; align
	stdcall [SetFilePointer],[hFileOut],ebx,0,FILE_CURRENT
	; write section data
	mov	ecx,[dwFileInSize]
	add	ecx,[dwFileIn]
	sub	ecx,esi
	stdcall [WriteFile],[hFileOut],esi,ecx,esp,0
	stdcall [SetFilePointer],[hFileOut],[sechdr.PointerToRawData],0,FILE_BEGIN
	; new section
	mov	esi,[dwFileIn]
	add	esi,[esi+IMAGE_DOS_HEADER.e_lfanew]
	mov	byte [attach.code.oep],$E9
	mov	eax,[esi+IMAGE_NT_HEADERS.OptionalHeader.AddressOfEntryPoint]
	sub	eax,attach.code.oep-attach+05h
	sub	eax,[sechdr.VirtualAddress]
	mov	dword [attach.code.oep+1],eax
	mov	esi,[argv+$C]
	mov	edi,OFFSET _szLibrary
@@:	lodsb
	cmp	al,0
	stosb
	jne	@B
	; relocate
	mov	esi,[dwFileIn]
	add	esi,[esi+IMAGE_DOS_HEADER.e_lfanew]
	mov	ebx,[esi+IMAGE_NT_HEADERS.OptionalHeader.ImageBase]
	mov	ecx,attach.size
	mov	esi,OFFSET attach
.reloc: mov	eax,[esi]
	cmp	eax,attach
	jb	.rskip
	cmp	eax,attach+attach.size
	ja	.rskip
	sub	eax,attach
	add	eax,[sechdr.VirtualAddress]
	add	eax,ebx
	mov	[esi],eax
.rskip: inc	esi
	loop	.reloc
	stdcall [WriteFile],[hFileOut],OFFSET attach,attach.size,esp,0
	mov	[sechdr.VirtualSize],attach.size
	mov	[sechdr.SizeOfRawData],attach.size
	; write PE headers
	mov	eax,[dwFileIn]
	stdcall [SetFilePointer],[hFileOut],[eax+IMAGE_DOS_HEADER.e_lfanew],0,FILE_BEGIN
	mov	esi,[dwFileIn]
	mov	edi,OFFSET nthdrs
	mov	ecx,sizeof.IMAGE_NT_HEADERS shr 2
	add	esi,[esi+IMAGE_DOS_HEADER.e_lfanew]
	rep	movsd
	sub	edi,sizeof.IMAGE_NT_HEADERS
	mov	edx,[sechdr.VirtualSize]
	movzx	ecx,[edi+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	mov	ebx,[edi+IMAGE_NT_HEADERS.OptionalHeader.SectionAlignment]
	mov	esi,[dwFileIn]
	add	esi,[esi+IMAGE_DOS_HEADER.e_lfanew]
	add	esi,sizeof.IMAGE_NT_HEADERS
	neg	ebx
	or	edx,-1
	pushd	0
.addsec:mov	eax,[esi+IMAGE_SECTION_HEADER.VirtualAddress]
	cmp	edx,eax
	cmova	edx,eax
	mov	eax,[esi+IMAGE_SECTION_HEADER.VirtualSize]
	dec	eax
	sub	eax,ebx
	and	eax,ebx
	add	[esp],eax
	add	esi,sizeof.IMAGE_SECTION_HEADER
	loop	.addsec
	pop	eax
	mov	ecx,[sechdr.VirtualAddress]
	add	eax,[sechdr.VirtualSize]
	add	eax,edx
	inc	[edi+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	mov	[edi+IMAGE_NT_HEADERS.OptionalHeader.SizeOfImage],eax
	mov	[edi+IMAGE_NT_HEADERS.OptionalHeader.AddressOfEntryPoint],ecx
	stdcall [WriteFile],[hFileOut],edi,sizeof.IMAGE_NT_HEADERS,esp,0
	movzx	eax,[edi+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	shl	eax,3
	lea	eax,[eax*4+eax-sizeof.IMAGE_SECTION_HEADER]
	stdcall [SetFilePointer],[hFileOut],eax,0,FILE_CURRENT
	stdcall [WriteFile],[hFileOut],OFFSET sechdr,sizeof.IMAGE_SECTION_HEADER,esp,0

	stdcall [CloseHandle],[hFileOut]
	stdcall [UnmapViewOfFile],[dwFileIn]
	stdcall [CloseHandle],[hFileMapIn]
	stdcall [CloseHandle],[hFileIn]
.quit:	pop	ebp edi esi ebx
	stdcall [ExitProcess],0
;##########################################################################
rva2raw:
	xor	eax,eax
	mov	edx,[esp+04h]
	add	edx,[edx+IMAGE_DOS_HEADER.e_lfanew]
	movzx	ecx,[edx+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	add	edx,sizeof.IMAGE_NT_HEADERS
	jecxz	.quit
.next:	mov	eax,[edx+IMAGE_SECTION_HEADER.VirtualAddress]
	cmp	[esp+08h],eax
	jb	.skip
	add	eax,[edx+IMAGE_SECTION_HEADER.VirtualSize]
	cmp	[esp+08h],eax
	jae	.skip
	mov	eax,[edx+IMAGE_SECTION_HEADER.PointerToRawData]
	sub	eax,[edx+IMAGE_SECTION_HEADER.VirtualAddress]
	add	eax,[esp+08h]
	jmp	.quit
.skip:	add	edx,sizeof.IMAGE_SECTION_HEADER
	loop	.next
	xor	eax,eax
.quit:	retn	08h
;##########################################################################
processCommandLine:
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
section ".data" data readable writeable
    data import
	library kernel32,"kernel32.dll",user32,"user32.dll"
	include "%include%/api/kernel32.inc"
	include "%include%/api/user32.inc"
    end data
;##########################################################################
	szUsage 	db	"Usage: patch old.exe new.exe lib.dll",13,10
	 .length	=	($ - szUsage)
;##########################################################################
	include "attach.inc"
;##########################################################################
	hStdOut 	rd	01h
	hFileIn 	rd	01h
	hFileMapIn	rd	01h
	dwFileIn	rd	01h
	dwFileInSize	rd	01h
	hFileOut	rd	01h
	argc		rd	01h
	argv		rd	80h
	nthdrs		IMAGE_NT_HEADERS
	sechdr		IMAGE_SECTION_HEADER
;##########################################################################
;##########################################################################
;##########################################################################
;##########################################################################
; reimport
; coded by comrade <comrade2k@hotmail.com>
; IRC: #asm, #coders, #win32asm on EFnet
; Web: http://comrade64.cjb.net/
;      http://comrade.win32asm.com/
;
; 01.02.2004 - created
; 13.03.2004 - changes
; 14.07.2008 - modernized to FASM 1.67.26
;##########################################################################
format PE console 4.0
entry start
;##########################################################################
_TITLE		equ  "reimport"
_NAME		equ  "reimport"
_VERSION	equ  "0.1"
_VERSIONTEXT	equ  _VERSION
;##########################################################################
include "%include%/win32am.inc"
include "%include%/equates/kernel32.inc"
include "%include%/equates/user32.inc"
include "%include%/macro/if.inc"
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
	call	ProcessCmdLine
	cmp	[argc],5
	jae	@F
	stdcall [WriteFile],[hStdOut],OFFSET szMsgAbout,szMsgAbout.length,esp,0
	jmp	.quit
@@:	stdcall [CreateFile],[argv+04h],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0
	mov	[hFileIn],eax
	stdcall [CreateFileMapping],eax,0,PAGE_READONLY,0,0,0
	mov	[hFileMapIn],eax
	stdcall [MapViewOfFile],eax,FILE_MAP_READ,0,0,0
	mov	[dwFileIn],eax
	stdcall [GetFileSize],[hFileIn],0
	mov	[dwFileInSize],eax
	stdcall [CreateFile],[argv+08h],GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0
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
	mov	dword [edi+IMAGE_SECTION_HEADER.Name+00h],".imp"
	mov	dword [edi+IMAGE_SECTION_HEADER.Name+04h],"orts"
	mov	dword [edi+IMAGE_SECTION_HEADER.VirtualAddress],ebp
	mov	dword [edi+IMAGE_SECTION_HEADER.PointerToRawData],edx
	mov	dword [edi+IMAGE_SECTION_HEADER.Characteristics],IMAGE_SCN_CNT_INITIALIZED_DATA + IMAGE_SCN_CNT_UNINITIALIZED_DATA + IMAGE_SCN_MEM_READ + IMAGE_SCN_MEM_WRITE
	; align
	stdcall [SetFilePointer],[hFileOut],ebx,0,FILE_CURRENT
	; write section data
	mov	ecx,[dwFileInSize]
	add	ecx,[dwFileIn]
	sub	ecx,esi
	stdcall [WriteFile],[hFileOut],esi,ecx,esp,0
	stdcall [SetFilePointer],[hFileOut],[sechdr.PointerToRawData],0,FILE_BEGIN

	mov	ebx,[sechdr.VirtualAddress]
	stdcall strlen,[argv+0Ch]
	inc	eax
	add	ebx,eax
	stdcall [WriteFile],[hFileOut],[argv+0Ch],eax,esp,0
	; copy import table
	mov	edx,[dwFileIn]
	mov	eax,[edx+IMAGE_DOS_HEADER.e_lfanew]
	stdcall rva2raw,[dwFileIn],dword [eax+edx+IMAGE_NT_HEADERS.OptionalHeader.DataDirectory+(1*sizeof.IMAGE_DATA_DIRECTORY)+IMAGE_DATA_DIRECTORY.VirtualAddress]
	add	eax,[dwFileIn]
	mov	esi,eax
	jmp	.chk
.next:	mov	edi,OFFSET impd
	mov	ecx,sizeof.IMAGE_IMPORT_DESCRIPTOR shr 2
	rep	movsd
	add	ecx,sizeof.IMAGE_IMPORT_DESCRIPTOR
	sub	edi,ecx
	add	ebx,ecx
	stdcall [WriteFile],[hFileOut],edi,ecx,esp,0
.chk:	cmp	dword [esi+IMAGE_IMPORT_DESCRIPTOR.FirstThunk],0
	jne	.next
	sub	[argc],4
	mov	esi,OFFSET argv+10h
.arg:	lodsd
	mov	[lib],eax
	push	esi
.split: inc	eax
	cmp	byte [eax],0
	je	.skipa
	cmp	word [eax],"::"
	jne	.split
	mov	byte [eax],0
	add	eax,2
	mov	[procedure],eax
	call	rebuild
.skipa: pop	esi
	dec	[argc]
	ja	.arg

	sub	ebx,[sechdr.VirtualAddress]
	mov	[sechdr.VirtualSize],ebx
	mov	[sechdr.SizeOfRawData],ebx

	; write PE headers
	mov	eax,[dwFileIn]
	stdcall [SetFilePointer],[hFileOut],[eax+IMAGE_DOS_HEADER.e_lfanew],0,FILE_BEGIN
	mov	esi,[dwFileIn]
	mov	edi,OFFSET nthdrs
	mov	ecx,sizeof.IMAGE_NT_HEADERS shr 2
	add	esi,[esi+IMAGE_DOS_HEADER.e_lfanew]
	rep	movsd
	sub	edi,sizeof.IMAGE_NT_HEADERS
	stdcall strlen,[argv+0Ch]
	inc	eax
	mov	edx,[sechdr.VirtualSize]
	add	eax,[sechdr.VirtualAddress]
	mov	dword [edi+IMAGE_NT_HEADERS.OptionalHeader.DataDirectory+(1*sizeof.IMAGE_DATA_DIRECTORY)+IMAGE_DATA_DIRECTORY.VirtualAddress],eax
	mov	dword [edi+IMAGE_NT_HEADERS.OptionalHeader.DataDirectory+(1*sizeof.IMAGE_DATA_DIRECTORY)+IMAGE_DATA_DIRECTORY.Size],edx
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
	add	eax,[sechdr.VirtualSize]
	add	eax,edx
	inc	[edi+IMAGE_NT_HEADERS.FileHeader.NumberOfSections]
	mov	[edi+IMAGE_NT_HEADERS.OptionalHeader.SizeOfImage],eax
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
rebuild:
	; rebuild import table
	mov	edx,[dwFileIn]
	mov	eax,[edx+IMAGE_DOS_HEADER.e_lfanew]
	stdcall rva2raw,[dwFileIn],dword [eax+edx+IMAGE_NT_HEADERS.OptionalHeader.DataDirectory+(1*sizeof.IMAGE_DATA_DIRECTORY)+IMAGE_DATA_DIRECTORY.VirtualAddress]
	add	eax,[dwFileIn]
	mov	esi,eax
	jmp	.chk
.next:	push	esi
	stdcall rva2raw,[dwFileIn],[esi+IMAGE_IMPORT_DESCRIPTOR.Name]
	add	eax,[dwFileIn]
	xor	ecx,ecx
	mov	esi,eax
	mov	edi,[lib]
	xor	eax,eax
@@:	lodsb
	mov	cl,[edi]
	lcase	eax
	lcase	ecx
	cmp	eax,ecx
	jne	.skip
	inc	edi
	test	eax,eax
	jnz	@B
	; correct library, check for needed import
	mov	edx,[esp]
	mov	eax,[edx+IMAGE_IMPORT_DESCRIPTOR.OriginalFirstThunk]
	test	eax,eax
	cmovz	eax,[edx+IMAGE_IMPORT_DESCRIPTOR.FirstThunk]
	test	eax,eax
	jz	.skip
	stdcall rva2raw,[dwFileIn],eax
	add	eax,[dwFileIn]
	mov	esi,eax
	xor	ebp,ebp
	jmp	.chkd
.thunk: push	esi
	stdcall rva2raw,[dwFileIn],eax
	add	eax,[dwFileIn]
	lea	esi,[eax+2]
	mov	edi,[procedure]
	xor	eax,eax
	xor	ecx,ecx
@@:	lodsb
	mov	cl,[edi]
	lcase	eax
	lcase	ecx
	cmp	eax,ecx
	jne	.skipd
	inc	edi
	test	eax,eax
	jnz	@B
	mov	esi,[esp+04h]
	mov	edi,OFFSET impd
	mov	ecx,sizeof.IMAGE_IMPORT_DESCRIPTOR shr 2
	rep	movsd
	sub	edi,sizeof.IMAGE_IMPORT_DESCRIPTOR
	mov	edx,[sechdr.VirtualAddress]
	add	[edi+IMAGE_IMPORT_DESCRIPTOR.FirstThunk],ebp
	mov	[edi+IMAGE_IMPORT_DESCRIPTOR.Name],edx

	mov	eax,[esp]
	mov	eax,[eax-04h]
	lea	ecx,[ebx+IMAGE_IMPORT_DESCRIPTOR.TimeDateStamp]
	mov	[edi+IMAGE_IMPORT_DESCRIPTOR.TimeDateStamp],eax
	and	[edi+IMAGE_IMPORT_DESCRIPTOR.ForwarderChain],0
	mov	[edi+IMAGE_IMPORT_DESCRIPTOR.OriginalFirstThunk],ecx

	stdcall [WriteFile],[hFileOut],edi,sizeof.IMAGE_IMPORT_DESCRIPTOR,esp,0
	add	ebx,sizeof.IMAGE_IMPORT_DESCRIPTOR
.skipd: pop	esi
	add	ebp,4
.chkd:	lodsd
	test	eax,eax
	jnz	.thunk
.skip:	pop	esi
	add	esi,sizeof.IMAGE_IMPORT_DESCRIPTOR
.chk:	cmp	dword [esi+IMAGE_IMPORT_DESCRIPTOR.FirstThunk],0
	jne	.next
	retn
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
strlen:
	xor	eax,eax
	mov	edx,[esp+04h]
	test	edx,edx
	jz	.quit
	dec	eax
	dec	edx
.char:	inc	edx
	inc	eax
	cmp	byte [edx],0
	jne	.char
.quit:	retn
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
	library kernel32,"kernel32.dll",user32,"user32.dll"
	include "%include%/api/kernel32.inc"
	include "%include%/api/user32.inc"
    end data
;##########################################################################
;##########################################################################
	szMsgAbout	db	_TITLE," v",_VERSIONTEXT," (","01.03.2003 - ",DATE," ",TIME,")",13,10
			db	"coded by comrade <comrade2k@hotmail.com>",13,10
			db	"IRC:",9,"#asm, #coders, #win32asm on EFnet",13,10
			db	"Web:",9,"http://comrade64.cjb.net/",13,10,13,10
			db	"Usage: reimport.exe <input.exe> <output.exe> <hook.dll> <library::procedure> ...",13,10
	 .length	=	$ - szMsgAbout
	szTab		db	9
	szNewLine	db	13,10
;##########################################################################
	hStdOut 	rd	01h
	hFileIn 	rd	01h
	hFileMapIn	rd	01h
	dwFileIn	rd	01h
	dwFileInSize	rd	01h
	hFileOut	rd	01h
	argc		rd	01h
	argv		rd	80h
	lib		rd	01h
	procedure	rd	01h
	nthdrs		IMAGE_NT_HEADERS
	sechdr		IMAGE_SECTION_HEADER
	impd		IMAGE_IMPORT_DESCRIPTOR
;##########################################################################
;##########################################################################
;##########################################################################
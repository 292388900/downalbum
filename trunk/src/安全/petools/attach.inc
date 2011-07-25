;##########################################################################
; attached code for patch.asm
; coded by comrade <comrade2k@hotmail.com>
; IRC: #asm, #coders, #win32asm on EFnet
; Web: http://comrade64.cjb.net/
;      http://comrade.win32asm.com/
;##########################################################################
macro _import [name,alias] {
    if used name
	db alias,0
	name rd 01h
    end if
}
;##########################################################################
attach:
;##########################################################################
attach.code:
;##########################################################################
	pushad
	mov	eax,[esp+20h]	; eax = ~kernel32
	and	eax,0FFFF0000h
@@:	cmp	dword [eax],00905A4Dh
	je	@F
	sub	eax,10000h
	jmp	@B
@@:	mov	ebx,eax 	; ebx = kernel32
	add	eax,[eax+IMAGE_DOS_HEADER.e_lfanew]
	mov	edi,dword [eax+IMAGE_NT_HEADERS.OptionalHeader.DataDirectory]
	add	edi,ebx
	mov	esi,[edi+IMAGE_EXPORT_DIRECTORY.AddressOfNames]
	add	esi,ebx

	xor	edx,edx
.name:	mov	eax,[esi]
	add	eax,ebx
	; check for GetProcAddress()
	cmp	dword [eax+00h],"GetP"
	jne	@F
	cmp	dword [eax+04h],"rocA"
	jne	@F
	cmp	dword [eax+08h],"ddre"
	jne	@F
	cmp	word [eax+0Ch],"ss"
	jne	@F
	mov	eax,[edi+IMAGE_EXPORT_DIRECTORY.AddressOfNameOrdinals]
	add	eax,ebx
	movzx	esi,word [edx*2+eax]
	mov	eax,[edi+IMAGE_EXPORT_DIRECTORY.AddressOfFunctions]
	add	eax,ebx
	mov	esi,[esi*4+eax]
	add	esi,ebx
	jmp	.fnd
@@:	add	esi,4
	inc	edx
	cmp	edx,[edi+IMAGE_EXPORT_DIRECTORY.NumberOfNames]
	jne	.name
.fnd:	; esi contains address of GetProcAddress()
	stdcall esi,ebx,OFFSET _szProcLoadLibrary

	;
	; Load the library.
	; If we can't load it, do an int3.
	;
	stdcall eax,OFFSET _szLibrary
	test	eax,eax
	jnz	@F
	int3

@@:
	popad
.oep:	rb	05h
;##########################################################################
attach.codesize = ($ - attach.code)
;##########################################################################
attach.data:
;##########################################################################
	_szProcLoadLibrary	db	"LoadLibraryA",0
	_szLibrary		rb	200h
;##########################################################################
attach.datasize = ($ - attach.data)
attach.size	= ($ - attach)
;##########################################################################
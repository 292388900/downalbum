ShellStart0:
	call	next0
;********************
ImportTableBegin:
;自建输入表形式
ImportTable	DD	AddressFirst-ImportTable  
		DD	0,0 
AppImpRVA1	DD	DllName-ImportTable   
AppImpRVA2	DD	AddressFirst-ImportTable 
		DD	0,0,0,0,0
AddressFirst	DD	FirstFunc-ImportTable   
AddressSecond	DD	SecondFunc-ImportTable    
AddressThird	DD	ThirdFunc-ImportTable
		DD	0
DllName		DB	'KERNEL32.dll'
		DW	0
FirstFunc	DW	0	
		DB	'GetProcAddress',0
SecondFunc	DW	0
		DB	'GetModuleHandleA',0
ThirdFunc	DW	0
		DB	'LoadLibraryA',0
ImportTableEnd:
ShellBase	DD	0
ShellPackSize	DD	0
Virtualalloc	DB	'VirtualAlloc',0
VirtualallocADDR	DD	0
TlsTable	DB	18h dup (?)
next0:
	pop	ebp
	sub	ebp,(ImportTable-ShellStart0)
	lea	esi,[ebp+(DllName-ShellStart0)]
	push	esi
	call	dword ptr [ebp+(AddressSecond-ShellStart0)]
	lea	esi,[ebp+(Virtualalloc-ShellStart0)]
	push	esi
	push	eax
	call	dword ptr [ebp+(AddressFirst-ShellStart0)]
	mov	dword ptr [ebp+(VirtualallocADDR-ShellStart0)],eax
	push	PAGE_READWRITE
	push	MEM_COMMIT
	push	dword ptr [ebp+(ShellPackSize-ShellStart0)]
	push	0
	call	dword ptr [ebp+(VirtualallocADDR-ShellStart0)]
	push	eax
	mov	ebx,dword ptr [ebp+(ShellBase-ShellStart0)]
	add	ebx,ebp
	push	eax
	push	ebx
	call	_aP_depack_asm
	pop	edx
	push	ebp
	jmp	edx
;============================解压缩源码(引用)============================	
_aP_depack_asm:
    pushad
    mov    esi, [esp + 36]    ; C calling convention
    mov    edi, [esp + 40]
    cld
    mov    dl, 80h
    xor    ebx, ebx
literal:
    movsb
    mov    bl, 2
nexttag:
    call   getbit
    jnc    literal

    xor    ecx, ecx
    call   getbit
    jnc    codepair
    xor    eax, eax
    call   getbit
    jnc    shortmatch
    mov    bl, 2
    inc    ecx
    mov    al, 10h
getmorebits:
    call   getbit
    adc    al, al
    jnc    getmorebits
    jnz    domatch
    stosb
    jmp    short nexttag
codepair:
    call   getgamma_no_ecx
    sub    ecx, ebx
    jnz    normalcodepair
    call   getgamma
    jmp    short domatch_lastpos
shortmatch:
    lodsb
    shr    eax, 1
    jz     donedepacking
    adc    ecx, ecx
    jmp    short domatch_with_2inc
normalcodepair:
    xchg   eax, ecx
    dec    eax
    shl    eax, 8
    lodsb
    call   getgamma
    cmp    eax, 32000
    jae    domatch_with_2inc
    cmp    ah, 5
    jae    domatch_with_inc
    cmp    eax, 7fh
    ja     domatch_new_lastpos
domatch_with_2inc:
    inc    ecx
domatch_with_inc:
    inc    ecx
domatch_new_lastpos:
    xchg   eax, ebp
domatch_lastpos:
    mov    eax, ebp
    mov    bl, 1
domatch:
    push   esi
    mov    esi, edi
    sub    esi, eax
    rep    movsb
    pop    esi
    jmp    short nexttag
getbit:
    add     dl, dl
    jnz     stillbitsleft
    mov     dl, byte ptr[esi]
    inc     esi
    adc     dl, dl
stillbitsleft:
    ret
getgamma:
    xor    ecx, ecx
getgamma_no_ecx:
    inc    ecx
getgammaloop:
    call   getbit
    adc    ecx, ecx
    call   getbit
    jc     getgammaloop
    ret
donedepacking:
    sub    edi, [esp + 40]
    mov    [esp + 28], edi    ; return unpacked length in eax
    popad
    ret	8h
;========================================================================
ShellEnd0:
;*****************
ShellStart:
	call	$+5
	pop	edx
	sub	edx,5h
	pop	ebp
	mov	ecx,3h
	lea	esi,[ebp+(AddressFirst-ShellStart0)]
	lea	edi,[edx+(GetprocaddressADDR-ShellStart)]
    MoveThreeFuncAddr:
	mov	eax,dword ptr [esi]
	mov	dword ptr [edi],eax
	add	esi,4h
	add	edi,4h
	loop	MoveThreeFuncAddr
	lea	eax,[ebp+(_aP_depack_asm-ShellStart0)]
	mov	dword ptr [edx+(aP_depackAddr-ShellStart)],eax
	mov	eax,dword ptr [ebp+(VirtualallocADDR-ShellStart0)]
	mov	dword ptr [edx+(S_VirtualallocADDR-ShellStart)],eax
	mov	ebp,edx
	push	0
	call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
	mov	dword ptr [ebp+(FileHandle-ShellStart)],eax	;取得当前文件句柄
	;*******取一些函数入口
	lea	esi,dword ptr [ebp+(Ker32DllName-ShellStart)]
	push	esi
	call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
	.if	eax==0
		push	esi
		call	dword ptr [ebp+(LoadlibraryADDR-ShellStart)]
	.endif
	mov	esi,eax
	lea	ebx,dword ptr [ebp+(S_Virtualfree-ShellStart)]
	push	ebx
	push	esi
	call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
	mov	dword ptr [ebp+(S_VirtualfreeADDR-ShellStart)],eax
	;*******解压缩各段********
	mov	ebx,S_PackSection-ShellStart
    DePackNextSection:
	cmp	dword ptr [ebp+ebx],0h
	jz	AllSectionDePacked
	push	ebx
	push	PAGE_READWRITE
	push	MEM_COMMIT
	push	dword ptr [ebp+ebx]
	push	0
	call	dword ptr [ebp+(S_VirtualallocADDR-ShellStart)]	;申请内存进行读写
	pop	ebx
	mov	esi,eax
	mov	eax,ebx
	add	eax,ebp
	mov	edi,dword ptr [eax+4h]
	add	edi,dword ptr [ebp+(FileHandle-ShellStart)]
	push	esi
	push	edi
	call	dword ptr [ebp+(aP_depackAddr-ShellStart)]
	mov	ecx,dword ptr [ebp+ebx]
	push	esi
	rep	movsb
	pop	esi
	push	ebx
	push	MEM_RELEASE
	push	0
	push	esi
	call	dword ptr [ebp+(S_VirtualfreeADDR-ShellStart)]	;释放内存
	pop	ebx
	add	ebx,0ch
	jmp	DePackNextSection
    AllSectionDePacked:
	;*******恢复原输入表******
	mov	eax,dword ptr [ebp+(S_IsProtImpTable-ShellStart)]
	.if	eax == 0
		mov	edi,dword ptr [ebp+(ImpTableAddr-ShellStart)]
		add	edi,dword ptr [ebp+(FileHandle-ShellStart)]
	    GetNextDllFuncAddr:
		mov	esi,dword ptr [edi+0ch]
		.if	esi == 0
			jmp	AllDllFuncAddrGeted
		.endif
		add	esi,dword ptr [ebp+(FileHandle-ShellStart)]
		push	esi
		call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
		.if	eax==0
			push	esi
			call	dword ptr [ebp+(LoadlibraryADDR-ShellStart)]
		.endif
		mov	esi,eax
		mov	edx,dword ptr [edi]
		.if	edx == 0
			mov	edx,dword ptr [edi+10h]
		.endif
		add	edx,dword ptr [ebp+(FileHandle-ShellStart)]
		mov	ebx,dword ptr [edi+10h]
		add	ebx,dword ptr [ebp+(FileHandle-ShellStart)]
	    GetNextFuncAddr:
		mov	eax,dword ptr [edx]
		.if	eax == 0
			jmp	AllFuncAddrGeted
		.endif
		push	ebx
		push	edx
		cdq
		.if	edx == 0	
			add	eax,2h
			add	eax,dword ptr [ebp+(FileHandle-ShellStart)]
		.else
			and	eax,7fffffffh
		.endif
		push	eax
		push	esi
		call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
		mov	dword ptr [ebx],eax
		pop	edx
		pop	ebx
		add	edx,4h
		add	ebx,4h
		jmp	GetNextFuncAddr
	    AllFuncAddrGeted:
		add	edi,14h
		jmp	GetNextDllFuncAddr
	    AllDllFuncAddrGeted:
	.else
		mov	edx,dword ptr [ebp+(ImpTableAddr-ShellStart)]
		add	edx,ebp
	    GetNextDllFuncAddr2:
		mov	edi,dword ptr [edx]
		.if	edi == 0
			jmp	AllDllFuncAddrGeted2
		.endif
		add	edi,dword ptr [ebp+(FileHandle-ShellStart)]
		add	edx,5h
		mov	esi,edx
		push	esi
		call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
		.if	eax==0
			push	esi
			call	dword ptr [ebp+(LoadlibraryADDR-ShellStart)]
		.endif
		movzx	ecx,byte ptr [esi-1]
		add	esi,ecx
		mov	edx,esi
		mov	esi,eax
		inc	edx
		mov	ecx,dword ptr [edx]
		add	edx,4h
	    GetNextFuncAddr2:
		push	ecx
		movzx	eax,byte ptr [edx]
		.if	eax == 0
			inc	edx
			push	edx
			mov	eax,dword ptr [edx]
			push	eax
			push	esi
			call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
			mov	dword ptr [edi],eax
			pop	edx
			add	edx,4h
		.else
			inc	edx
			push	edx
			push	edx
			push	esi
			call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
			mov	dword ptr [edi],eax
			pop	edx
			movzx	eax,byte ptr [edx-1]
			add	edx,eax
		.endif
		inc	edx
		add	edi,4h
		pop	ecx
		loop	GetNextFuncAddr2
		jmp	GetNextDllFuncAddr2
	    AllDllFuncAddrGeted2:
	.endif
	;*******修正特殊代码加密的代码
	mov	eax,dword ptr [ebp+(S_IsCodeProt-ShellStart)]
	.if	eax == 1
		mov	edi,dword ptr [ebp+(CodeProtAddr-ShellStart)]
		add	edi,ebp
		lea	esi,[ebp+(CodeProtFunc-ShellStart)]
	    FixNextCodeProcCode:
		mov	eax,dword ptr [edi]
		.if	eax == 0
			jmp	FixAllCodeProcCode
		.endif
		and	eax,7fffffffh
		mov	ebx,esi
		sub	ebx,eax
		mov	dword ptr [eax-4],ebx
		add	edi,8h
		jmp	FixNextCodeProcCode
	    FixAllCodeProcCode:
	.endif
	;*******anti  dump*****************
	assume 	fs:nothing
	push	fs:[30h]
	pop	eax
	test	eax,eax
	js	fuapfdw_is9x  
fuapfdw_isNT:
	mov	eax, [eax+0ch]
	mov	eax, [eax+0ch]
	mov	dword ptr [eax+20h], 1000h 
	jmp	fuapfdw_finished
fuapfdw_is9x:
	push	0
	call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
	test	edx, edx
	jns	fuapfdw_finished 
	cmp	dword ptr [edx+8h], -1
	jne	fuapfdw_finished  
	mov	edx, dword ptr [edx+4]  
	mov	dword ptr [edx+50h], 1000h 
fuapfdw_finished:
	;*************准备返回OEP***************
	mov	dword ptr [ebp+(ShellImageBase-ShellStart)],ebp
	mov	eax,dword ptr [ebp+(OEP-ShellStart)]
	add	eax,dword ptr [ebp+(FileHandle-ShellStart)]
	jmp	eax
	;*******特殊代码加密的恢复函数******
	;***通过此函数,找到正确的输入表项***
    CodeProtFunc:
	push	eax
	mov	eax,esp
	pushad
	mov	ebx,eax
	call	$+9
    ShellImageBase:
	DD	0
	pop	ebp
	mov	ebp,dword ptr [ebp]
	mov	edi,dword ptr [ebx+4]
	mov	esi,dword ptr [ebp+(CodeProtAddr-ShellStart)]
	add	esi,ebp
    FindCodeFuncReturnAddr:
	mov	eax,dword ptr [esi]
	xor	edx,edx
	mov	ecx,2h
	mul	ecx
	shr	eax,1
	.if	edi == eax
		.if	dl == 0
		jmp	FoundCodeFuncReturnAddr
		.else
		jmp	FoundCodeFuncReturnAddr2
		.endif
	.endif
	add	esi,8h
	jmp	FindCodeFuncReturnAddr
    FoundCodeFuncReturnAddr:
	mov	eax,dword ptr [esi+4]
	mov	dword ptr [ebx],eax
	popad
	pop	eax
	mov	eax,dword ptr [eax]
	jmp	eax
    FoundCodeFuncReturnAddr2:
	mov	eax,dword ptr [esi+4]
	mov	dword ptr [ebx],eax
	popad
	pop	eax
	add	esp,4h
	mov	eax,dword ptr [eax]
	jmp	eax
	;************************
GetprocaddressADDR	DD	0
GetmulehandleADDR	DD	0
LoadlibraryADDR		DD	0
S_VirtualallocADDR	DD	0
FileHandle		DD	0
S_IsProtImpTable	DD	0
S_IsCodeProt		DD	0
ImpTableAddr		DD	0
CodeProtAddr		DD	0
OEP			DD	0
aP_depackAddr		DD	0
Ker32DllName		DB	'KERNEL32.dll',0
S_Virtualfree		DB	'VirtualFree',0
S_VirtualfreeADDR	DD	0
S_PackSection	DB	0a0h dup (?)
ShellEnd:

;******************************************************;
;在外壳前由程序在加壳时加入随机花指令,产生方法是有8组花;
;指令由程序;随机抽取,每部分之中又有可随机插入的部分,一 ;
;般看上去就较难发现其规律了.********************;
Junk_Code_1_Start:		;17字节
	call	$+0ah
	db	0fh
	db	01h
	jmp	$+7h
	db	0e8h
	jmp	$-3h
	db	0h
	db	0h
	add	esp,4h
Junk_Code_1_End:
	;*****
Junk_Code_2_Start:		;22字节
	jmp	$+3h
	db	0fh
	db	31h
	db	0f0h
	jmp	$+0eh
	xor	ecx,eax		;2字节
	jmp	$+5h
	jmp	$+0bh
	db	0fh
	pop	ecx
	jz	$+7h
	jnz	$-6h
	push	ecx
	jmp	$-0dh
Junk_Code_2_End:
	;*****
Junk_Code_3_Start:		;42字节
	mov	ecx,4h
	call	$+24h
	jmp	$-4h
	call	$+1bh
	db	0e9h
	jmp	$-6
	db	0h,0h
	pop	eax
	jmp	$+0bh
	db	0fh
	db	25h
	call	$-9h
	db	0fh
	db	0b9h
	dec	ecx
	jnz	$-0dh
	jmp	$+7h
	jmp	$-5h
	jmp	$-0eh
	db	0d6h
Junk_Code_3_End:
	;*****
Junk_Code_4_Start:		;26字节
	call	$+0ch
	db	0C7h
	db	083h
	add	eax,13h
	jmp	$+0dh	
	pop	eax
	jmp	$+4h
	db	0CDh
	db	020h
	add	eax,2	;3字节
	jmp	$+3h
	db	0E9h
	push    eax
	ret
	db	0e8h
Junk_Code_4_End:
	;*****
Junk_Code_5_Start:		;19字节
	call	$+8h
	db	0c7h
	db	084h
	db	0h
	pop	eax
	jmp	$+3h
	db	0e9h
	add	eax,7h
	push	eax
	ret
	db	0ffh
	db	35h
Junk_Code_5_End:
	;*****
Junk_Code_6_Start:
	call	$+0dh
	db	0fh
	db	01h
	add	eax,0fh
	push	eax
	ret
	db	0ffh
	pop	eax
	add	eax,2h
	jmp	eax
	db	0fh
	db	01h
	db	0ch
Junk_Code_6_End:
	;*****
Junk_Code_7_Start:
	call	$+1bh
	mov	ebx,dword ptr [esp+0ch]
	mov	esp,dword ptr [ebx+0c4h]
	pop	dword ptr fs:[0]
	add	esp,4h
	jmp	$+16h
	push	dword ptr fs:[0]
	mov	fs:[0],esp
	xor	ecx,ecx
	cdq
	div	ecx
	db	0e9h
Junk_Code_7_End:
	;*****
Junk_Code_8_Start:
	xor	esi,esi
	call	$+15h
	mov	esp,dword ptr [esp+08h]
	pop	dword ptr fs:[0]
	pop	eax
	jmp	$+15h
	db	0c7h
	db	83h
	push	dword ptr fs:[0]
	mov	fs:[0],esp
	lodsd
	db	0cdh
	db	20h
Junk_Code_8_End:
	;*****
lpFuckCode	  equ  this	dword
FuckCode1_Addr		dd	Junk_Code_1_Start
FuckCode1_Size		dd	Junk_Code_1_End - Junk_Code_1_Start
FuckCode2_Addr		dd	Junk_Code_2_Start
FuckCode2_Size		dd	Junk_Code_2_End - Junk_Code_2_Start
FuckCode3_Addr		dd	Junk_Code_3_Start
FuckCode3_Size		dd	Junk_Code_3_End - Junk_Code_3_Start
FuckCode4_Addr		dd	Junk_Code_4_Start
FuckCode4_Size		dd	Junk_Code_4_End - Junk_Code_4_Start
FuckCode5_Addr		dd	Junk_Code_5_Start
FuckCode5_Size		dd	Junk_Code_5_End - Junk_Code_5_Start
FuckCode6_Addr		dd	Junk_Code_6_Start
FuckCode6_Size		dd	Junk_Code_6_End - Junk_Code_6_Start
FuckCode7_Addr		dd	Junk_Code_7_Start
FuckCode7_Size		dd	Junk_Code_7_End - Junk_Code_7_Start
FuckCode8_Addr		dd	Junk_Code_8_Start
FuckCode8_Size		dd	Junk_Code_8_End - Junk_Code_8_Start


	
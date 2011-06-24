

	format PE GUI 4.0
	include 'win32ax.inc'
	entry	__Entry
	
	
section '.text' code readable executable writeable

 __Entry:
 
 	push	dword [esp]
 	call	GetKrnlBase
 	
 	call	GetKrnlBase2
 	
 	call	GetKrnlBase3
 	
 	ret


 ;++
 ;
 ; int
 ;   GetKrnlBase(
 ;      int KrnlApiAddress
 ;   )
 ;
 ; Routine Description:
 ;
 ;    获得kernel32基地址
 ;
 ; Arguments:
 ;
 ;    (esp)          - return address
 ;
 ;    Data   (esp+4) - KrnlApiAddress, ptr ApiAddress
 ;
 ;
 ; Return Value:
 ;
 ;    eax =  krnl32 base
 ;
 ;--

 GetKrnlBase:
 	pop	eax
 	pop	edx
 	push	eax
 	
 .Next:
 	cmp	word [edx], 'MZ' 
 	jz	.IsPe
	dec	edx
	xor	dx, dx
 	jmp	.Next
 
 .IsPe:	
 	mov	eax, [edx+3ch]
 	cmp	word [eax+edx], 'PE'
 	jnz	.Next
 	xchg	eax, edx
 	ret


 ;++
 ;
 ; int
 ;   GetKrnlBase2(
 ;    void
 ;   )
 ;
 ; Routine Description:
 ;
 ;    获得kernel32基地址
 ;
 ; Arguments:
 ;
 ;    (esp)          - return address
 ;
 ;
 ; Return Value:
 ;
 ;    eax =  krnl32 base
 ;
 ;--
 
 GetKrnlBase2:
 	mov	edx, [fs:0]
 .Next:
	inc	dword [edx]
	jz	.Krnl
	dec	dword [edx]
	mov	edx,  [edx]
	jmp	.Next
 .Krnl:
	dec	dword [edx]
	mov	edx, [edx+4]
 .Loop:
	cmp	word [edx], 'MZ' 
 	jz	.IsPe
	dec	edx
	xor	dx, dx
	jmp	.Loop

 .IsPe:	
 	mov	eax, [edx+3ch]
 	cmp	word [eax+edx], 'PE'
 	jnz	.Next
 	xchg	eax, edx
 	ret
 	
 ;++
 ;
 ; int
 ;   GetKrnlBase3(
 ;    void
 ;   )
 ;
 ; Routine Description:
 ;
 ;    获得kernel32基地址
 ;
 ; Arguments:
 ;
 ;    (esp)          - return address
 ;
 ;
 ; Return Value:
 ;
 ;    eax =  krnl32 base
 ;
 ;--
 
 GetKrnlBase3:
 	mov	eax, [fs:30h]
 	mov	eax, [eax+0ch]
 	mov	eax, [eax+1ch]
 	mov	eax, [eax]
	mov	eax, [eax+8h]
	ret 	
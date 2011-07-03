
;-------------------------------------------------
;��PE�ļ�,����:hWinMainΪ��������
;lpfilebuffΪ�ļ���������
;lpext�ļ�Ĭ����չ��
;lpfilter������
_OpenFile	proc lpfilebuff,lpfilter,lpext
local   @stOF:OPENFILENAME

invoke  RtlZeroMemory,addr @stOF,sizeof @stOF
mov     @stOF.lStructSize,sizeof @stOF
push    hWinMain
pop     @stOF.hwndOwner
push	lpfilter
pop     @stOF.lpstrFilter
push	lpfilebuff
pop     @stOF.lpstrFile
mov     @stOF.nMaxFile,MAX_PATH
mov     @stOF.Flags,OFN_FILEMUSTEXIST or OFN_PATHMUSTEXIST
push	lpext
pop     @stOF.lpstrDefExt
invoke	GetOpenFileName,addr @stOF
ret
_OpenFile	endp
;-------------------------------------------------

;����Ϣ����׷����Ϣ,����lpMsg����Ϣ����ַ,ʹ����һ��ȫ�־��:hEditInfo
_AddLine	proc	lpMsg
	pushad
	invoke	SendMessage,hEditInfo,EM_SETSEL,-1,0
	invoke	SendMessage,hEditInfo,EM_REPLACESEL,0,lpMsg
	popad
	ret

_AddLine endp

;����Ϣ����׷����Ϣ,����lpMsg����Ϣ����ַ,ʹ����һ��ȫ�־��:hEditInfo
AddLine	proc	lpMsg
	pushad
	invoke	SendMessage,hEditInfo,EM_SETSEL,-1,0
	invoke	SendMessage,hEditInfo,EM_REPLACESEL,0,lpMsg
	popad
	ret

AddLine endp
;-------------------------------------------------
;=====================================================================
;ʵ�ֶ���ĺ���,��dwOffset��dwAlignment����,����ֵ��EAX��
_Align	proc	dwOffset,dwAlignment
LOCAL	@return
	
	pushad
	xor	edx,edx
	mov	eax,dwOffset
	mov	ecx,dwAlignment
	div	ecx
	inc	eax
	xor	edx,edx
	mul	ecx
	mov	@return,eax
	popad
	mov	eax,@return
	ret

_Align endp
;-------------------------------------------------
;********************ȡ��*************************************************
GetIntegral	PROC	SourceValue:DWORD,AlignmentValue:DWORD
;��SourceValue���ݶ���ֵAlignmentValue����ȡ��,�����ڶ����鳤�Ȳ���
;���ص�ֵEAX��Ϊȡ�����ֵ.
	push	edx
	push	ecx
	mov	eax,SourceValue
	mov	ecx,AlignmentValue
	xor	edx,edx
	div	ecx
	.if   edx!=0
		inc eax	
	.endif
	xor	edx,edx
	mul	AlignmentValue
	pop	ecx
	pop	edx
	ret
GetIntegral	endp

;*****************���������ļ�*******************************
_CreateBakFile	PROC	lpSrcFileName:DWORD
;��SourceFileNameADDRָ����ַ������ļ�����һ���Ӻ�׺'BAK'�ı����ļ�
;����eax��ֵ��ʾ�����ɹ����.
LOCAL	szBakFileName[MAX_PATH]:BYTE
	pushad
	
	invoke	lstrcpy,addr szBakFileName,lpSrcFileName
	mov	edi,eax
	invoke	lstrlen,lpSrcFileName
	add	edi,eax
	mov	dword ptr [edi],'KAB.'	;��ԭ�ļ����Ӻ�׺.bak
	and	byte ptr[edi+4],0	;��֤����� NULL����
	invoke	CopyFile,lpSrcFileName , addr szBakFileName ,FALSE
	
	popad
	ret
_CreateBakFile	endp

;***************����ض�λ����*********************
;����˵��:
;lpImgBase->DOSͷ����˵ӳ���ַ
;lpPeHead->PEͷ
;lpFstSecTbl->��һ�������ĵ�ַVA
;dwSecCnts->������Ŀ
ClsRelocation	proc	lpImgBase,lpPeHead,lpFstSecTbl,dwSecCnts
	pushad
	
	mov	esi,lpPeHead
	assume	esi:ptr IMAGE_NT_HEADERS
	mov	eax,[esi].OptionalHeader.DataDirectory[5*8].VirtualAddress
	.if	eax == 0
		string	szNoRelocation,"�ļ���û���ض�λ����."
		invoke	AddLine,addr szNoRelocation
		popad
		ret
	.endif
	mov	ecx,dwSecCnts
	mov	esi,lpFstSecTbl
	assume 	esi:ptr IMAGE_SECTION_HEADER
    @@:
	cmp	eax,dword ptr [esi].VirtualAddress
	jz	foundRelSec
	add	esi,sizeof IMAGE_SECTION_HEADER
	loop	@B
	
	invoke	AddLine,addr szNoRelocation
	popad
	ret
    foundRelSec:
	mov	edi,[esi].VirtualAddress
	add	edi,lpImgBase
	mov	ecx,[esi].Misc.VirtualSize
	.if	ecx==0
		mov	ecx,[esi].SizeOfRawData
	.endif
	xor	eax,eax
	rep	stosb
	and	[esi].SizeOfRawData,0
	
	mov	esi,lpPeHead
	assume	esi:ptr IMAGE_NT_HEADERS
	and 	[esi].OptionalHeader.DataDirectory[5*8].VirtualAddress,0
	and	[esi].OptionalHeader.DataDirectory[5*8].isize,0h
	string	szRelocCleared,"�ض�λ����������."
	invoke	AddLine,addr szRelocCleared
	popad
	ret
ClsRelocation	endp


;***************���������ܲ���*****************************
ProtCode	PROC
;������ԭ������CALL [????]��JMP [????]ͳһ��ΪCALL ????,
;????ָ���������ĵ�ַ�ռ��һ�δ���,����δ������CALL�ķ���
;��ַ��ԭ��ַ��¼��ȽϺ��ҳ�������ʹ�õ��������,��ָ��������
;���ܺ�����ݽṹΪ"  ���ص�ַ:DWORD,ԭ��������ַ:DWORD  "
;Ϊ������ԭ����CALL [????]��JMP [????],��ԭ��CALL [????]�ķ���
;��ַ�����λ��1��Ϊ���.
  LOCAL	CodeSectionBase:DWORD
  LOCAL	CodeSectionSize:DWORD
  LOCAL	CodeProtSize:DWORD
  LOCAL	@return
  
  	pushad
  	
	and	CodeProtSize,0
	and	CodeSectionSize,0
	and	CodeSectionBase,0
	;*********ȡ����β���**
	mov	esi,SecTableBase
	assume	esi:ptr	IMAGE_SECTION_HEADER
	mov	eax,dword ptr [esi].Misc.VirtualSize
	mov	CodeSectionSize,eax
	mov	eax,dword ptr [esi].VirtualAddress
	add	eax,MapOfFile
	mov	CodeSectionBase,eax
	
	mov	esi,PeHeadBase
	assume	esi : ptr IMAGE_NT_HEADERS
	;*********ȡ������ַ***
	mov	esi,dword ptr [esi].OptionalHeader.DataDirectory[8].VirtualAddress
	add	esi,MapOfFile
	assume	esi:ptr IMAGE_IMPORT_DESCRIPTOR
	;************************
    ProtNextDllCode:
	mov	eax,dword ptr [esi].Name1
	test	eax,eax	;�����һ������ȫΪ���IMAGE_IMPORT_DESCRIPTOR�ṹ����
	jz	AllDllCodeProt
	
	mov	edi,dword ptr [esi].FirstThunk
   ProtNextFuncCode:
	mov	edx,edi
	add	edx,PeImageBase		;call [edx] or Jmp [edx]	edxӦ�þ���ԭ�ļ���ʵ��IAT��(OriginalFirstThunk)
	;***********
	mov	eax,edi
	add	eax,MapOfFile		;������������Ϊ�İ��ļ����ص�һ���ڴ���,ӳ���ַ��Ҫ��Ĳ�ͬ(eax=FirstThunk)
	mov	eax,dword ptr [eax]	;��һ��ȫΪ���IMAGE_THUNK_DATA�ṹ����,�Դ��жϴӴ�dll�ļ��е���ĺ�������
	;**********
	test	eax,eax
	jz	AllFuncCodeIsProt
	
	push	edi		
	mov	edi,CodeSectionBase	;�������	esi->����ָ���ַ
	mov	ecx,CodeSectionSize	
	sub	ecx,3h			;���ҷ�Χ
SearchNextCall:
	;�Ӵ�����в��� ��ַΪedx��ָ�� ���ǵ�Ŀ��ָ����ʽ��:call [edx] or Jmp [edx]
	mov	eax,dword ptr [edi]
	.if	eax == edx
		push	esi
		mov	esi,MapOfCodeProt	;���ڱ�����ܺ��Call�ķ��ص�ַ
		add	esi,CodeProtSize
		lea	eax,[edi+4h]	;��һ��ָ���ַ
		sub	eax,MapOfFile
		add	eax,PeImageBase	;ת��Ϊԭ�ļ��ڼ��ص�ӳ���ַ���(��ȷ)��ַ ;���ܺ�CALL�ķ��ص�ַ
						
		;���ǰ�����ֽ��Ƿ��� CALL���� JMP
		.if	word ptr [edi-2] == 15ffh	;call
			mov	dword ptr [esi],eax	;������ܺ��Call�ķ��ص�ַ
			mov	dword ptr [esi+4h],edx	;XXXX��ַ
			mov	word ptr [edi-2h],0e890h ;0E8��JMPָ����,90H��NOPָ����
			mov	dword ptr [edi],0h	;��ԭ����CALL [XXXX]��
			add	CodeProtSize,8h		;JMP [XXXX]ͳһ��Ϊ:
		.elseif	word ptr [edi-2] == 25ffh	;jmp		;NOP
							;CALL XXXX
			or	eax,80000000h		;ԭ��jmp [xxxx]��һ���
			mov	dword ptr [esi],eax
			mov	dword ptr [esi+4h],edx
			mov	word ptr [edi-2h],0e890h
			mov	dword ptr [edi],0h
			add	CodeProtSize,8h
		.endif
		pop	esi
	.endif
	inc	edi
	loop	SearchNextCall
	pop	edi
		
	add	edi,4h
	jmp	ProtNextFuncCode
	
  AllFuncCodeIsProt:	
	add	esi,sizeof IMAGE_IMPORT_DESCRIPTOR
	jmp	ProtNextDllCode
	
  AllDllCodeProt:
	mov	eax,CodeProtSize
	mov	@return,eax
	popad
	mov	eax,@return
	ret
ProtCode	endp
;***************�������ܲ���************************
MoveImpTable	PROC
;��һ����������������л��ֽṹ��ʽ�洢,�Դﵽ���ܵ�Ŀ��.
;����eax��ֵΪ�����������ݵĴ�С.
LOCAL	@return

	pushad
	
	mov	edi,MapOfImpProt	;���ܵ������洢��MapOfImpProt��ַ�����ڴ���
	
	mov	ebx,PeHeadBase
	assume	ebx : ptr IMAGE_NT_HEADERS
	mov	ebx,dword ptr [ebx].OptionalHeader.DataDirectory[8].VirtualAddress
	add	ebx,MapOfFile		;������ַ
	assume	ebx:ptr	IMAGE_IMPORT_DESCRIPTOR
	
	mov	eax,[ebx].FirstThunk
	.while	eax!=0
		stosd
		inc	edi			;��һ���ֽ�,�ź���dllname�ַ����ĳ���
		mov	esi,[ebx].Name1
		add	esi,MapOfFile
		invoke	MoveString,esi,edi
		mov	byte ptr [edi-1],al	;ldllname�ַ����ĳ���
		
		add	edi,eax
		inc	edi		;��һ�ֽ�,��Ϊ�ַ����������
		mov	ecx,edi		;[ecx]��ź�������������
		add	edi,4h	
		
		mov	esi,[ebx].OriginalFirstThunk
		.if	esi == 0
			mov	esi,[ebx].FirstThunk
		.endif
		add	esi,MapOfFile
		assume	esi:ptr IMAGE_THUNK_DATA
		lodsd
		.while	eax
			cdq
			.if	edx == 0
				add	eax,MapOfFile	;eax->IMAGE_IMPORT_BY_NAME
				add	eax,2h		;eax->Name1	
				inc	edi	;�ź��溯�������ĳ���
				invoke	MoveString,eax,edi	;[eax].Name1
				mov	byte ptr [edi-1],al
				add	edi,eax
				inc	edi		;��һ�ֽ�,��Ϊ�ַ����������
			.else
				inc	edi
				and	eax,7fffffffh
				stosd
				inc	edi
			.endif
			inc	dword ptr [ecx]		;����ĺ������� ++
			lodsd
		.endw
		add	ebx,sizeof IMAGE_IMPORT_DESCRIPTOR	;��һ���ṹ
		mov	eax,[ebx].FirstThunk
	.endw
	sub	edi,MapOfImpProt
	add	edi,4h
	mov	@return,edi
	
	popad
	mov	eax,@return
	ret
MoveImpTable	endp

;***************���ԭ�����************************
ClsImpTable	PROC
	pushad
	mov	edi,PeHeadBase
	assume	edi : ptr IMAGE_NT_HEADERS
	mov	edi,dword ptr [edi].OptionalHeader.DataDirectory[SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress
	add	edi,MapOfFile		;������ַ
	assume	edi:ptr IMAGE_IMPORT_DESCRIPTOR
	mov	eax,[edi].Name1
	.while	eax!=0
		add	eax,MapOfFile
		invoke	ClsString,eax
		mov	esi,[edi].OriginalFirstThunk
		.if	esi == 0
			mov	esi,[edi].FirstThunk
		.endif
		add	esi,MapOfFile
		assume	esi:ptr IMAGE_THUNK_DATA
		xor	ecx,ecx
		lodsd
		.while	eax !=0
			cdq
			.if	edx == 0	
				add	eax,MapOfFile		;eax->IMAGE_IMPORT_BY_NAME
				mov	word ptr [eax],0h	;set Hint=0
				add	eax,2h			;clear Name1
				invoke	ClsString,eax
			.endif
			inc	ecx
			and	dword ptr [esi-4],0h		;IMAGE_THUNK_DATAֵ���
			lodsd
		.endw
		
		xor	eax,eax
		push	edi
		mov	edi,[edi].FirstThunk	;edi->IAT
		add	edi,MapOfFile
		rep	stosd
		pop	edi
		
		mov	ecx,sizeof IMAGE_IMPORT_DESCRIPTOR
		rep	stosb
		
		add	edi,sizeof IMAGE_IMPORT_DESCRIPTOR	;�����ӵ�,Ӧ��������
		mov	eax,[edi].Name1
	.endw
	popad
	ret
ClsImpTable	endp

;***************�Ҹ�����Դ�����ַ��ǰ��һ��,�Ժ󲿷ֿ���ѹ��
FindFirstResADDR	PROC	lpMapOfFile,lpNt
  LOCAL	@return:DWORD
  LOCAL	@ResRva
  
	pushad
	
	mov	@return,7fffffffh
	mov	esi,lpNt
	assume	esi : ptr IMAGE_NT_HEADERS
	mov	esi,[esi].OptionalHeader.DataDirectory[2*8].VirtualAddress
	mov	@ResRva,esi			;��Դ���ƫ��
	add	esi,lpMapOfFile
	assume	esi:ptr IMAGE_RESOURCE_DIRECTORY;��Դ����ַ
	xor	ecx,ecx
	mov	cx,word ptr [esi].NumberOfNamedEntries
	add	cx,word ptr [esi].NumberOfIdEntries;�м�����Դ
	add	esi,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	esi:ptr	IMAGE_RESOURCE_DIRECTORY_ENTRY
    SearchNextResType:
	push	ecx

	mov	edi,[esi].OffsetToData
	and	edi,7fffffffh			;������Ŀ¼�������Դ����ƫ��
	add	edi,@ResRva
	add	edi,lpMapOfFile			;������Ŀ¼��ַ
	assume	edi:ptr IMAGE_RESOURCE_DIRECTORY 
	xor	ecx,ecx
	mov	cx,word ptr [edi].NumberOfNamedEntries
	add	cx,word ptr [edi].NumberOfIdEntries;���������м�����Ŀ
	add	edi,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	edi:ptr	IMAGE_RESOURCE_DIRECTORY_ENTRY
	    SearchNextResObject:	
		push	edi
		mov	edi,[edi].OffsetToData
		and	edi,7fffffffh		;����ĿĿ¼�������Դ����ƫ��
		add	edi,@ResRva
		add	edi,lpMapOfFile		;����ĿĿ¼��ַ
		add	edi,sizeof IMAGE_RESOURCE_DIRECTORY
		mov	edi,[edi].OffsetToData	;����Ŀ�������Դ����ƫ��
		add	edi,@ResRva
		add	edi,lpMapOfFile		;����Ŀ��ַ
		mov	eax,dword ptr [edi]	;����Ŀ���ƫ��
		.if	eax > @ResRva
			.if	eax < @return
				mov	@return,eax
			.endif
		.endif
		pop	edi
		add	edi,sizeof IMAGE_RESOURCE_DIRECTORY_ENTRY
		loop	SearchNextResObject

	pop	ecx
	add	esi,sizeof IMAGE_RESOURCE_DIRECTORY_ENTRY
	loop	SearchNextResType
	
	popad
	mov	eax,@return
	ret
FindFirstResADDR	endp

;****************ѹ����Դ****************************
MoveRes		PROC	ResType:DWORD,MoveADDR:DWORD,MoveResSize:DWORD
  LOCAL	ResBase:DWORD
	pushad
	mov	edx,PeHeadBase
	assume	edx : ptr IMAGE_NT_HEADERS
	mov	edx,dword ptr [edx].OptionalHeader.DataDirectory[2*8].VirtualAddress
	add	edx,MapOfFile		;��Դ��ַ
	assume	edx:ptr IMAGE_RESOURCE_DIRECTORY
	mov	ResBase,edx
	xor	ecx,ecx
	mov	cx,word ptr [edx].NumberOfNamedEntries
	add	cx,word ptr [edx].NumberOfIdEntries
	add	edx,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	edx:ptr IMAGE_RESOURCE_DIRECTORY_ENTRY
    FindResDir:
	mov	eax,[edx].Name1	;��ԴID(���������ַ���)
	cmp	eax,ResType	;Ҫ���ҵ���ԴID
	jz	FoundResDir
	add	edx,sizeof IMAGE_RESOURCE_DIRECTORY_ENTRY
	loop	FindResDir
	
	jmp	NotFoundResDir
	
    FoundResDir:
	mov	edx,[edx].OffsetToData
	and	edx,7fffffffh
	add	edx,ResBase
	assume	edx:ptr IMAGE_RESOURCE_DIRECTORY
	xor	ecx,ecx
	mov	cx,[edx].NumberOfNamedEntries
	add	cx,[edx].NumberOfIdEntries
	add	edx,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	edx:ptr IMAGE_RESOURCE_DIRECTORY_ENTRY
    MoveResItem:
	mov	ebx,[edx].OffsetToData
	and	ebx,7fffffffh
	add	ebx,ResBase
	add	ebx,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	ebx:ptr IMAGE_RESOURCE_DIRECTORY_ENTRY
	mov	ebx,[ebx].OffsetToData
	add	ebx,ResBase
	assume	ebx:ptr IMAGE_RESOURCE_DATA_ENTRY ;��Դ�������
	push	ecx
	mov	ecx,[ebx].Size1	;��Դ���ݳ���
	mov	esi,[ebx].OffsetToData;��Դ���ݵ�RVA
	add	esi,MapOfFile
	mov	eax,PeImageSize
	add	eax,ShellEnd0-ShellStart0
	add	eax,MoveResSize
	mov	[ebx].OffsetToData,eax;�޸���Դ���ݵ�RVAΪ�ƶ����λ��
	mov	edi,MoveADDR		;�Ȱ���Դ�ƶ�����ʱ�ڴ���,���д�ļ�ʱд��:ShellStart0����	
	add	edi,MoveResSize
	add	MoveResSize,ecx
	push	esi
	push	ecx
	rep	movsb	;�ƶ���Դ����
	pop	ecx
	pop	edi
	xor	eax,eax
	rep	stosb
	pop	ecx
	add	edx,sizeof IMAGE_RESOURCE_DIRECTORY_ENTRY
	loop	MoveResItem
    NotFoundResDir:
	popad
	mov	eax,MoveResSize
	ret
MoveRes		endp


;***************�ƶ��ַ���***************************
MoveString	PROC	uses  esi edi ecx lpStrSrc:DWORD,lpStrDst:DWORD
;��Դ��ַָ�����'00'��β���ַ������Ƶ�Ŀ���ַָ��Ŀռ�,������
;��eax�����мĴ���������,���eax��ֵΪ���Ƶ��ֽ���

	mov	edi,lpStrDst
	mov	esi,lpStrSrc
	xor	ecx,ecx
    MoveNextByte:
	lodsb
	test 	al,al
	jz	@Over
	stosb
	inc	ecx
	jmp	MoveNextByte
    @Over:
    	stosb	;�洢���һ��0
    	inc	ecx
	mov	eax,ecx

	ret
MoveString	endp
;********************���һ���ַ���********************************
ClsString	PROC	lpStr
;��һ����'00'�������ַ�������Ϊ'00'
	pushad
	mov	esi,lpStr
	mov	edi,lpStr
    ClsNextByte:
	lodsb
	test	al,al
	jz	@Over
	xor	al,al
	stosb
	jmp	ClsNextByte
    @Over:
	popad
	ret
ClsString	endp

;***************��������ָ��***********************
MakeFunkCode	PROC	FuncCodeBase:DWORD
  LOCAL	@FuckCodeSize:DWORD
	pushad
	and	@FuckCodeSize,0h
	mov	ecx,100h
	mov	edi,FuncCodeBase
    @@:
	push	ecx
	rdtsc
	and	eax,7h

	lea	esi,lpFuckCode
	shl	eax,3
	add	esi,eax
	mov	ecx,dword ptr[esi+4]
	add	@FuckCodeSize,ecx
	mov	esi,dword ptr[esi]
	rep	movsb
	
	pop	ecx
	loop	@B
	
	popad
	mov	eax,@FuckCodeSize
	ret
MakeFunkCode	endp

;***************��ն���**************************
ClsSectionName	PROC
	pushad
	mov	ecx,dwNumOfSec
	mov	edi,SecTableBase	;������
	xor	eax,eax
     @@:
	stosd
	stosd
	add	edi,20h	;sizeof IMAGE_SECTION_HEADER - 8(Name)
	loop	@B
	popad
	ret
ClsSectionName	endp

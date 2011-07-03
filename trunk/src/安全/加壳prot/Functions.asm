
;-------------------------------------------------
;打开PE文件,参数:hWinMain为主窗体句柄
;lpfilebuff为文件名缓冲区
;lpext文件默认扩展名
;lpfilter过滤器
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

;向信息框里追加信息,参数lpMsg是信息串地址,使用了一个全局句柄:hEditInfo
_AddLine	proc	lpMsg
	pushad
	invoke	SendMessage,hEditInfo,EM_SETSEL,-1,0
	invoke	SendMessage,hEditInfo,EM_REPLACESEL,0,lpMsg
	popad
	ret

_AddLine endp

;向信息框里追加信息,参数lpMsg是信息串地址,使用了一个全局句柄:hEditInfo
AddLine	proc	lpMsg
	pushad
	invoke	SendMessage,hEditInfo,EM_SETSEL,-1,0
	invoke	SendMessage,hEditInfo,EM_REPLACESEL,0,lpMsg
	popad
	ret

AddLine endp
;-------------------------------------------------
;=====================================================================
;实现对齐的函数,将dwOffset按dwAlignment对齐,返回值在EAX里
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
;********************取整*************************************************
GetIntegral	PROC	SourceValue:DWORD,AlignmentValue:DWORD
;将SourceValue根据对齐值AlignmentValue进行取整,常用于对区块长度操作
;返回的值EAX即为取整后的值.
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

;*****************创建备份文件*******************************
_CreateBakFile	PROC	lpSrcFileName:DWORD
;将SourceFileNameADDR指向的字符串的文件创建一个加后缀'BAK'的备份文件
;返回eax的值表示创建成功与否.
LOCAL	szBakFileName[MAX_PATH]:BYTE
	pushad
	
	invoke	lstrcpy,addr szBakFileName,lpSrcFileName
	mov	edi,eax
	invoke	lstrlen,lpSrcFileName
	add	edi,eax
	mov	dword ptr [edi],'KAB.'	;于原文件名加后缀.bak
	and	byte ptr[edi+4],0	;保证最后以 NULL结束
	invoke	CopyFile,lpSrcFileName , addr szBakFileName ,FALSE
	
	popad
	ret
_CreateBakFile	endp

;***************清除重定位数据*********************
;参数说明:
;lpImgBase->DOS头或者说映像基址
;lpPeHead->PE头
;lpFstSecTbl->第一个区块表的地址VA
;dwSecCnts->区块数目
ClsRelocation	proc	lpImgBase,lpPeHead,lpFstSecTbl,dwSecCnts
	pushad
	
	mov	esi,lpPeHead
	assume	esi:ptr IMAGE_NT_HEADERS
	mov	eax,[esi].OptionalHeader.DataDirectory[5*8].VirtualAddress
	.if	eax == 0
		string	szNoRelocation,"文件中没有重定位数据."
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
	string	szRelocCleared,"重定位数据清除完毕."
	invoke	AddLine,addr szRelocCleared
	popad
	ret
ClsRelocation	endp


;***************特殊代码加密部分*****************************
ProtCode	PROC
;将调用原输入表的CALL [????]和JMP [????]统一改为CALL ????,
;????指向外壳申请的地址空间的一段代码,由这段代码根据CALL的返回
;地址与原地址记录相比较后找出真正该使用的输入表项,再指向它运行
;加密后的数据结构为"  返回地址:DWORD,原输入表项地址:DWORD  "
;为了区别原来的CALL [????]和JMP [????],将原来CALL [????]的返回
;地址的最高位置1作为标记.
  LOCAL	CodeSectionBase:DWORD
  LOCAL	CodeSectionSize:DWORD
  LOCAL	CodeProtSize:DWORD
  LOCAL	@return
  
  	pushad
  	
	and	CodeProtSize,0
	and	CodeSectionSize,0
	and	CodeSectionBase,0
	;*********取代码段参数**
	mov	esi,SecTableBase
	assume	esi:ptr	IMAGE_SECTION_HEADER
	mov	eax,dword ptr [esi].Misc.VirtualSize
	mov	CodeSectionSize,eax
	mov	eax,dword ptr [esi].VirtualAddress
	add	eax,MapOfFile
	mov	CodeSectionBase,eax
	
	mov	esi,PeHeadBase
	assume	esi : ptr IMAGE_NT_HEADERS
	;*********取输入表地址***
	mov	esi,dword ptr [esi].OptionalHeader.DataDirectory[8].VirtualAddress
	add	esi,MapOfFile
	assume	esi:ptr IMAGE_IMPORT_DESCRIPTOR
	;************************
    ProtNextDllCode:
	mov	eax,dword ptr [esi].Name1
	test	eax,eax	;最后以一个内容全为零的IMAGE_IMPORT_DESCRIPTOR结构结束
	jz	AllDllCodeProt
	
	mov	edi,dword ptr [esi].FirstThunk
   ProtNextFuncCode:
	mov	edx,edi
	add	edx,PeImageBase		;call [edx] or Jmp [edx]	edx应该就是原文件真实的IAT表(OriginalFirstThunk)
	;***********
	mov	eax,edi
	add	eax,MapOfFile		;现在是我们人为的把文件加载到一个内存中,映像基址和要求的不同(eax=FirstThunk)
	mov	eax,dword ptr [eax]	;以一个全为零的IMAGE_THUNK_DATA结构结束,以此判断从此dll文件中导入的函数结束
	;**********
	test	eax,eax
	jz	AllFuncCodeIsProt
	
	push	edi		
	mov	edi,CodeSectionBase	;查找起点	esi->代码指令地址
	mov	ecx,CodeSectionSize	
	sub	ecx,3h			;查找范围
SearchNextCall:
	;从代码段中查找 地址为edx的指令 我们的目标指令形式是:call [edx] or Jmp [edx]
	mov	eax,dword ptr [edi]
	.if	eax == edx
		push	esi
		mov	esi,MapOfCodeProt	;用于保存加密后的Call的返回地址
		add	esi,CodeProtSize
		lea	eax,[edi+4h]	;下一条指令地址
		sub	eax,MapOfFile
		add	eax,PeImageBase	;转换为原文件在加载到映像基址后的(正确)地址 ;加密后CALL的返回地址
						
		;检测前两个字节是否是 CALL或者 JMP
		.if	word ptr [edi-2] == 15ffh	;call
			mov	dword ptr [esi],eax	;保存加密后的Call的返回地址
			mov	dword ptr [esi+4h],edx	;XXXX地址
			mov	word ptr [edi-2h],0e890h ;0E8是JMP指令码,90H是NOP指令码
			mov	dword ptr [edi],0h	;将原来的CALL [XXXX]和
			add	CodeProtSize,8h		;JMP [XXXX]统一改为:
		.elseif	word ptr [edi-2] == 25ffh	;jmp		;NOP
							;CALL XXXX
			or	eax,80000000h		;原来jmp [xxxx]做一标记
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
;***************输入表加密部分************************
MoveImpTable	PROC
;将一个程序的输入表的所有换种结构方式存储,以达到加密的目的.
;返回eax的值为加密生成数据的大小.
LOCAL	@return

	pushad
	
	mov	edi,MapOfImpProt	;加密的输入表存储在MapOfImpProt地址处的内存里
	
	mov	ebx,PeHeadBase
	assume	ebx : ptr IMAGE_NT_HEADERS
	mov	ebx,dword ptr [ebx].OptionalHeader.DataDirectory[8].VirtualAddress
	add	ebx,MapOfFile		;输入表地址
	assume	ebx:ptr	IMAGE_IMPORT_DESCRIPTOR
	
	mov	eax,[ebx].FirstThunk
	.while	eax!=0
		stosd
		inc	edi			;空一个字节,放后面dllname字符串的长度
		mov	esi,[ebx].Name1
		add	esi,MapOfFile
		invoke	MoveString,esi,edi
		mov	byte ptr [edi-1],al	;ldllname字符串的长度
		
		add	edi,eax
		inc	edi		;空一字节,作为字符串结束标记
		mov	ecx,edi		;[ecx]存放后续函数的数量
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
				inc	edi	;放后面函数名串的长度
				invoke	MoveString,eax,edi	;[eax].Name1
				mov	byte ptr [edi-1],al
				add	edi,eax
				inc	edi		;空一字节,作为字符串结束标记
			.else
				inc	edi
				and	eax,7fffffffh
				stosd
				inc	edi
			.endif
			inc	dword ptr [ecx]		;导入的函数个数 ++
			lodsd
		.endw
		add	ebx,sizeof IMAGE_IMPORT_DESCRIPTOR	;下一个结构
		mov	eax,[ebx].FirstThunk
	.endw
	sub	edi,MapOfImpProt
	add	edi,4h
	mov	@return,edi
	
	popad
	mov	eax,@return
	ret
MoveImpTable	endp

;***************清除原输入表************************
ClsImpTable	PROC
	pushad
	mov	edi,PeHeadBase
	assume	edi : ptr IMAGE_NT_HEADERS
	mov	edi,dword ptr [edi].OptionalHeader.DataDirectory[SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress
	add	edi,MapOfFile		;输入表地址
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
			and	dword ptr [esi-4],0h		;IMAGE_THUNK_DATA值清空
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
		
		add	edi,sizeof IMAGE_IMPORT_DESCRIPTOR	;后来加的,应该是这样
		mov	eax,[edi].Name1
	.endw
	popad
	ret
ClsImpTable	endp

;***************找各个资源保存地址最前的一个,以后部分可以压缩
FindFirstResADDR	PROC	lpMapOfFile,lpNt
  LOCAL	@return:DWORD
  LOCAL	@ResRva
  
	pushad
	
	mov	@return,7fffffffh
	mov	esi,lpNt
	assume	esi : ptr IMAGE_NT_HEADERS
	mov	esi,[esi].OptionalHeader.DataDirectory[2*8].VirtualAddress
	mov	@ResRva,esi			;资源起点偏移
	add	esi,lpMapOfFile
	assume	esi:ptr IMAGE_RESOURCE_DIRECTORY;资源起点地址
	xor	ecx,ecx
	mov	cx,word ptr [esi].NumberOfNamedEntries
	add	cx,word ptr [esi].NumberOfIdEntries;有几类资源
	add	esi,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	esi:ptr	IMAGE_RESOURCE_DIRECTORY_ENTRY
    SearchNextResType:
	push	ecx

	mov	edi,[esi].OffsetToData
	and	edi,7fffffffh			;该类型目录相对于资源起点的偏移
	add	edi,@ResRva
	add	edi,lpMapOfFile			;该类型目录地址
	assume	edi:ptr IMAGE_RESOURCE_DIRECTORY 
	xor	ecx,ecx
	mov	cx,word ptr [edi].NumberOfNamedEntries
	add	cx,word ptr [edi].NumberOfIdEntries;该类型中有几个项目
	add	edi,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	edi:ptr	IMAGE_RESOURCE_DIRECTORY_ENTRY
	    SearchNextResObject:	
		push	edi
		mov	edi,[edi].OffsetToData
		and	edi,7fffffffh		;该项目目录相对于资源起点的偏移
		add	edi,@ResRva
		add	edi,lpMapOfFile		;该项目目录地址
		add	edi,sizeof IMAGE_RESOURCE_DIRECTORY
		mov	edi,[edi].OffsetToData	;该项目相对于资源起点的偏移
		add	edi,@ResRva
		add	edi,lpMapOfFile		;该项目地址
		mov	eax,dword ptr [edi]	;该项目存放偏移
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

;****************压缩资源****************************
MoveRes		PROC	ResType:DWORD,MoveADDR:DWORD,MoveResSize:DWORD
  LOCAL	ResBase:DWORD
	pushad
	mov	edx,PeHeadBase
	assume	edx : ptr IMAGE_NT_HEADERS
	mov	edx,dword ptr [edx].OptionalHeader.DataDirectory[2*8].VirtualAddress
	add	edx,MapOfFile		;资源地址
	assume	edx:ptr IMAGE_RESOURCE_DIRECTORY
	mov	ResBase,edx
	xor	ecx,ecx
	mov	cx,word ptr [edx].NumberOfNamedEntries
	add	cx,word ptr [edx].NumberOfIdEntries
	add	edx,sizeof IMAGE_RESOURCE_DIRECTORY
	assume	edx:ptr IMAGE_RESOURCE_DIRECTORY_ENTRY
    FindResDir:
	mov	eax,[edx].Name1	;资源ID(或者名称字符串)
	cmp	eax,ResType	;要查找的资源ID
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
	assume	ebx:ptr IMAGE_RESOURCE_DATA_ENTRY ;资源数据入口
	push	ecx
	mov	ecx,[ebx].Size1	;资源数据长度
	mov	esi,[ebx].OffsetToData;资源数据的RVA
	add	esi,MapOfFile
	mov	eax,PeImageSize
	add	eax,ShellEnd0-ShellStart0
	add	eax,MoveResSize
	mov	[ebx].OffsetToData,eax;修改资源数据的RVA为移动后的位置
	mov	edi,MoveADDR		;先把资源移动到临时内存里,最后写文件时写到:ShellStart0后面	
	add	edi,MoveResSize
	add	MoveResSize,ecx
	push	esi
	push	ecx
	rep	movsb	;移动资源数据
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


;***************移动字符串***************************
MoveString	PROC	uses  esi edi ecx lpStrSrc:DWORD,lpStrDst:DWORD
;将源地址指向的以'00'结尾的字符串复制到目标地址指向的空间,并加密
;除eax外所有寄存器都不变,输出eax的值为复制的字节数

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
    	stosb	;存储最后一个0
    	inc	ecx
	mov	eax,ecx

	ret
MoveString	endp
;********************清除一个字符串********************************
ClsString	PROC	lpStr
;将一个以'00'结束的字符串都清为'00'
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

;***************产生垃圾指令***********************
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

;***************清空段名**************************
ClsSectionName	PROC
	pushad
	mov	ecx,dwNumOfSec
	mov	edi,SecTableBase	;块表起点
	xor	eax,eax
     @@:
	stosd
	stosd
	add	edi,20h	;sizeof IMAGE_SECTION_HEADER - 8(Name)
	loop	@B
	popad
	ret
ClsSectionName	endp

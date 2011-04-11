;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 水波特效演示程序
; by 罗云彬，http://asm.yeah.net，luoyunbin@sina.com
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 所有实现特效的代码单独封装在 WaveObject.asm 中
; 具体使用方法见 WaveObject.asm 中的说明
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
include		Gdi32.inc
includelib	Gdi32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?

hInstance	dd	?
hWinMain	dd	?
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.const

szClassName	db	'Ripper',0
szTitle		db	'水波效果 - http://asm.yeah.net',0
szTip		db	'水波特效演示 by 罗云彬',0dh,0ah
		db	'使用Win32汇编语言编写，源代码见http://asm.yeah.net',0dh,0ah,0dh,0ah
		db	'鼠标左键：在点击处激发水波',0dh,0ah
		db	'鼠标右键：循环切换特效（下雨、快艇、波浪等）',0
szError		db	'初始化水波对象错误!',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		WaveObject.asm

		.data?
stWaveObj	WAVE_OBJECT	<?>

		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Quit		proc

		invoke	_WaveFree,addr stWaveObj
		invoke	DestroyWindow,hWinMain
		invoke	PostQuitMessage,NULL
		ret

_Quit		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Init		proc
		local	@stBmp:BITMAP

;********************************************************************
; 装入资源中的位图、初始化水波对象
;********************************************************************
		invoke	LoadBitmap,hInstance,1
		push	eax
		invoke	_WaveInit,addr stWaveObj,hWinMain,eax,30,0
		.if	eax
			invoke	MessageBox,hWinMain,addr szError,addr szTitle,MB_OK or MB_ICONSTOP
			invoke	_Quit
		.else
			invoke	MessageBox,hWinMain,addr szTip,addr szTitle,MB_OK
		.endif
		pop	eax
		invoke	DeleteObject,eax
;********************************************************************
; 将窗口大小修正到位图的大小
;********************************************************************
		mov	eax,stWaveObj.dwBmpWidth
		mov	ecx,stWaveObj.dwBmpHeight
		add	eax,6
		add	ecx,25
		invoke	SetWindowPos,hWinMain,HWND_TOPMOST,0,0,eax,ecx,\
			SWP_NOMOVE

		invoke	_WaveEffect,addr stWaveObj,1,5,4,250
		ret

_Init		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcWinMain	proc	uses ebx edi esi hWnd,uMsg,wParam,lParam
		local	@stPs:PAINTSTRUCT,@hDc,@stRect:RECT

		mov	eax,uMsg
;********************************************************************
		.if	eax ==	WM_CREATE
			push	hWnd
			pop	hWinMain
			call	_Init
		.elseif	eax ==	WM_CLOSE
			call	_Quit
		.elseif	eax ==	WM_PAINT
			invoke	BeginPaint,hWnd,addr @stPs
			mov	@hDc,eax
			invoke	_WaveUpdateFrame,addr stWaveObj,eax,TRUE
			invoke	EndPaint,hWnd,addr @stPs
			xor	eax,eax
			ret
;********************************************************************
; 鼠标左键激起水波
;********************************************************************
		.elseif	eax ==	WM_LBUTTONDOWN
			mov	eax,lParam
			movzx	ecx,ax		; x
			shr	eax,16		; y

			invoke	_WaveDropStone,addr stWaveObj,ecx,eax,2,256
;********************************************************************
; 鼠标右键切换特效
;********************************************************************
		.elseif	eax ==	WM_RBUTTONDOWN
			mov	eax,stWaveObj.dwEffectType
			.if	eax ==	0
				invoke	_WaveEffect,addr stWaveObj,1,5,4,250
			.elseif	eax ==	1
				invoke	_WaveEffect,addr stWaveObj,3,250,4,8
			.elseif	eax ==	3
				invoke	_WaveEffect,addr stWaveObj,2,4,2,180
			.else
				invoke	_WaveEffect,addr stWaveObj,0,0,0,0
			.endif
;********************************************************************
		.else
			invoke	DefWindowProc,hWnd,uMsg,wParam,lParam
			ret
		.endif
;********************************************************************
		xor	eax,eax
		ret

_ProcWinMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_WinMain	proc
		local	@stWndClass:WNDCLASSEX
		local	@stMsg:MSG

		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
;********************************************************************
; 注册窗口类
;********************************************************************
		invoke	RtlZeroMemory,addr @stWndClass,sizeof @stWndClass
		invoke	LoadCursor,0,IDC_ARROW
		mov	@stWndClass.hCursor,eax
		push	hInstance
		pop	@stWndClass.hInstance
		mov	@stWndClass.cbSize,sizeof WNDCLASSEX
		mov	@stWndClass.style,CS_HREDRAW or CS_VREDRAW
		mov	@stWndClass.lpfnWndProc,offset _ProcWinMain
		mov	@stWndClass.hbrBackground,COLOR_WINDOW + 1
		mov	@stWndClass.lpszClassName,offset szClassName
		invoke	RegisterClassEx,addr @stWndClass
;********************************************************************
; 建立并显示窗口
;********************************************************************
		invoke	CreateWindowEx,NULL,\
			offset szClassName,offset szTitle,\
			WS_OVERLAPPED or WS_SYSMENU,\
			300,300,500,500,\
			NULL,NULL,hInstance,NULL
		mov	hWinMain,eax
		invoke	ShowWindow,hWinMain,SW_SHOWNORMAL
		invoke	UpdateWindow,hWinMain
;********************************************************************
; 消息循环
;********************************************************************
		.while	TRUE
			invoke	GetMessage,addr @stMsg,NULL,0,0
			.break	.if eax	== 0
			invoke	TranslateMessage,addr @stMsg
			invoke	DispatchMessage,addr @stMsg
		.endw
		ret

_WinMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		call	_WinMain
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

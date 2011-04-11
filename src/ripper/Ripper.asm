;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ˮ����Ч��ʾ����
; by ���Ʊ�http://asm.yeah.net��luoyunbin@sina.com
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ����ʵ����Ч�Ĵ��뵥����װ�� WaveObject.asm ��
; ����ʹ�÷����� WaveObject.asm �е�˵��
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include �ļ�����
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
include		Gdi32.inc
includelib	Gdi32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ���ݶ�
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?

hInstance	dd	?
hWinMain	dd	?
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ���ݶ�
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.const

szClassName	db	'Ripper',0
szTitle		db	'ˮ��Ч�� - http://asm.yeah.net',0
szTip		db	'ˮ����Ч��ʾ by ���Ʊ�',0dh,0ah
		db	'ʹ��Win32������Ա�д��Դ�����http://asm.yeah.net',0dh,0ah,0dh,0ah
		db	'���������ڵ��������ˮ��',0dh,0ah
		db	'����Ҽ���ѭ���л���Ч�����ꡢ��ͧ�����˵ȣ�',0
szError		db	'��ʼ��ˮ���������!',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; �����
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
; װ����Դ�е�λͼ����ʼ��ˮ������
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
; �����ڴ�С������λͼ�Ĵ�С
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
; ����������ˮ��
;********************************************************************
		.elseif	eax ==	WM_LBUTTONDOWN
			mov	eax,lParam
			movzx	ecx,ax		; x
			shr	eax,16		; y

			invoke	_WaveDropStone,addr stWaveObj,ecx,eax,2,256
;********************************************************************
; ����Ҽ��л���Ч
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
; ע�ᴰ����
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
; ��������ʾ����
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
; ��Ϣѭ��
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

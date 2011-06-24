; #########################################################################

      .386
      .model flat, stdcall
      option casemap :none   ; case sensitive

; #########################################################################

      include FILTINPT.inc 

; #########################################################################


        ;=================
        ;  My API prototypes
        ;=================
        MyCallWindowProc   PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
        MyCreateWindowEx   PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
        MyDefWindowProc    PROTO :DWORD,:DWORD,:DWORD,:DWORD
        MyDispatchMessage  PROTO :DWORD
        MyGetMessage       PROTO :DWORD,:DWORD,:DWORD,:DWORD
        MyGetSystemMetrics PROTO :DWORD
        MyLoadCursor       PROTO :DWORD,:DWORD
        MyLoadIcon         PROTO :DWORD,:DWORD
        MyLoadMenu         PROTO :DWORD,:DWORD
        MyMessageBox       PROTO :DWORD,:DWORD,:DWORD,:DWORD
        MyPostQuitMessage  PROTO :DWORD
        MyRegisterClassEx  PROTO :DWORD
        MySendMessage      PROTO :DWORD,:DWORD,:DWORD,:DWORD
        MySetMenu          PROTO :DWORD,:DWORD
        MySetWindowLong    PROTO :DWORD,:DWORD,:DWORD
        MyShowWindow       PROTO :DWORD,:DWORD
        MyTranslateMessage PROTO :DWORD
        MyUpdateWindow     PROTO :DWORD
        MyExitProcess      PROTO :DWORD
        MyGetCommandLine   PROTO
        MyGetModuleHandle  PROTO :DWORD
        ;=================
        ; Local prototypes
        ;=================
        GetFunctionAddress PROTO :DWORD,:DWORD
        GetProcBaseAddress PROTO :DWORD
        GetDllBase         PROTO :DWORD
        
        WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
        WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
        TopXY   PROTO :DWORD,:DWORD
        EditSl  PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
        Static  PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
        Ed1Proc PROTO :DWORD,:DWORD,:DWORD,:DWORD
        Ed2Proc PROTO :DWORD,:DWORD,:DWORD,:DWORD
        Ed3Proc PROTO :DWORD,:DWORD,:DWORD,:DWORD

    .data
        szDisplayName          db " Filtered Input DEMO",0
        CommandLine            dd 0
        hWnd                   dd 0
        hInstance              dd 0
        hEdit1                 dd 0
        hEdit2                 dd 0
        hEdit3                 dd 0
        lpfnEd1Proc            dd 0
        lpfnEd2Proc            dd 0
        lpfnEd3Proc            dd 0
        MyImageBase            dd 0
        
        szKernel32             db 'kernel32.dll',0
        szNTDLL                db 'NTDLL.dll',0
        
        ;API 函数名列表 
        szCallWindowProcA      db 'CallWindowProcA',0
        szCreateWindowExA      db 'CreateWindowExA',0
        szDefWindowProcA       db 'DefWindowProcA',0
        szDispatchMessageA     db 'DispatchMessageA',0 
        szGetMessageA          db 'GetMessageA',0
        szGetSystemMetrics     db 'GetSystemMetrics',0
        szLoadCursorA          db 'LoadCursorA',0
        szLoadIconA            db 'LoadIconA',0
        szLoadMenuA            db 'LoadMenuA',0
        szMessageBoxA          db 'MessageBoxA',0
        szPostQuitMessage      db 'PostQuitMessage',0
        szRegisterClassExA     db 'RegisterClassExA',0
        szSendMessageA         db 'SendMessageA',0
        szSetMenu              db 'SetMenu',0
        szSetWindowLongA       db 'SetWindowLongA',0
        szShowWindow           db 'ShowWindow',0
        szTranslateMessage     db 'TranslateMessage',0
        szUpdateWindow         db 'UpdateWindow',0
        szExitProcess          db 'ExitProcess',0
        szRtlZeroMemory        db 'RtlZeroMemory',0
        szGetCommandLineA      db 'GetCommandLineA',0
        szGetModuleHandleA     db 'GetModuleHandleA',0
                               dd  0
        EVEN                       
        ;API 函数名引用表
        OffCallWindowProcA     dd  offset szCallWindowProcA
        OffCreateWindowExA     dd  offset szCreateWindowExA
        OffDefWindowProcA      dd  offset szDefWindowProcA
        OffDispatchMessageA    dd  offset szDispatchMessageA
        OffGetMessageA         dd  offset szGetMessageA
        OffGetSystemMetrics    dd  offset szGetSystemMetrics
        OffLoadCursorA         dd  offset szLoadCursorA
        OffLoadIconA           dd  offset szLoadIconA
        OffLoadMenuA           dd  offset szLoadMenuA
        OffMessageBoxA         dd  offset szMessageBoxA
        OffPostQuitMessage     dd  offset szPostQuitMessage
        OffRegisterClassExA    dd  offset szRegisterClassExA
        OffSendMessageA        dd  offset szSendMessageA
        OffSetMenu             dd  offset szSetMenu
        OffSetWindowLongA      dd  offset szSetWindowLongA
        OffShowWindow          dd  offset szShowWindow
        OffTranslateMessage    dd  offset szTranslateMessage
        OffUpdateWindow        dd  offset szUpdateWindow
                               dd  0
        OffExitProcess         dd  offset szExitProcess
        OffRtlZeroMemory       dd  offset szRtlZeroMemory                       
        OffGetCommandLineA     dd  offset szGetCommandLineA
        OffGetModuleHandleA    dd  offset szGetModuleHandleA
                               dd  0
        ;自建输入表
        ; Imports from user32
        _CallWindowProcA       dd  ?
        _CreateWindowExA       dd  ?
        _DefWindowProcA        dd  ?
        _DispatchMessageA      dd  ?
        _GetMessageA           dd  ?
        _GetSystemMetrics      dd  ?
        _LoadCursorA           dd  ?
        _LoadIconA             dd  ?
        _LoadMenuA             dd  ?
        _MessageBoxA           dd  ?
        _PostQuitMessage       dd  ?
        _RegisterClassExA      dd  ?
        _SendMessageA          dd  ?
        _SetMenu               dd  ?
        _SetWindowLongA        dd  ?
        _ShowWindow            dd  ?
        _TranslateMessage      dd  ?
        _UpdateWindow          dd  ?
                               dd  0
        ; Imports from kernel32
        _ExitProcess           dd  ?
        _RtlZeroMemory         dd  ?
        _GetCommandLineA       dd  ?
        _GetModuleHandleA      dd  ?
                               dd  0
        ProcName               sSTRING <,,Offset PNS>
        PNS                    db  100h dup(?)

    .code
    option prologue:none
; 自建输入表引用表
; __stdcall CallWindowProcA(x,x,x,x,x)
MyCallWindowProc    proc lpPrevWndFunc:DWORD,hWndl:DWORD,Msg:DWORD,wParam:DWORD,lParam:DWORD
                jmp     _CallWindowProcA    ; CallWindowProcA(x,x,x,x,x)
MyCallWindowProc    endp
; __stdcall CreateWindowExA(x,x,x,x,x,x,x,x,x,x,x,x)
MyCreateWindowEx    proc dwExStyle:DWORD,lpClassName:DWORD,lpWindowName:DWORD,dwStyle:DWORD,x:DWORD,y:DWORD,nWidth:DWORD,nHeight:DWORD,hWndParent:DWORD,hMenu:DWORD,hInstance1:DWORD,lpParam:DWORD
                jmp     _CreateWindowExA    ; CreateWindowExA(x,x,x,x,x,x,x,x,x,x,x,x)
MyCreateWindowEx    endp
; __stdcall DefWindowProcA(x,x,x,x)
MyDefWindowProc     proc hWnd1:DWORD,Msg:DWORD,wParam:DWORD,lParam:DWORD
                jmp     _DefWindowProcA    ; DefWindowProcA(x,x,x,x)
MyDefWindowProc     endp
; __stdcall DispatchMessageA(x)
MyDispatchMessage   proc lpmsg:DWORD
                jmp     _DispatchMessageA   ; DispatchMessageA(x)
MyDispatchMessage   endp
; __stdcall GetMessageA(x,x,x,x)
MyGetMessage        proc lpMsg:DWORD,hWnd1:DWORD,wMsgFilterMin:DWORD,wMsgFilterMax:DWORD
                jmp     _GetMessageA    ; GetMessageA(x,x,x,x)
MyGetMessage        endp
; __stdcall GetSystemMetrics(x)
MyGetSystemMetrics  proc nIndex:DWORD
                jmp     _GetSystemMetrics   ; GetSystemMetrics(x)
MyGetSystemMetrics  endp
; __stdcall LoadCursorA(x,x)
MyLoadCursor        proc hInstance1:DWORD,lpCursorName:DWORD
                jmp     _LoadCursorA   ; LoadCursorA(x,x)
MyLoadCursor        endp
; __stdcall LoadIconA(x,x)
MyLoadIcon          proc hInstance1:DWORD,lpIconName:DWORD
                jmp     _LoadIconA   ; LoadIconA(x,x)
MyLoadIcon          endp
; __stdcall LoadMenuA(x,x)
MyLoadMenu          proc hInstance1:DWORD,lpMenuName:DWORD
                jmp     _LoadMenuA   ; Loads the specified menu resource
MyLoadMenu          endp                    ; from the current executable (.EXE) file
; __stdcall MessageBoxA(x,x,x,x)
MyMessageBox        proc hWnd1:DWORD,lpText:DWORD,lpCaption:DWORD,uType:DWORD
                jmp     _MessageBoxA    ; MessageBoxA(x,x,x,x)
MyMessageBox        endp
; __stdcall PostQuitMessage(x)
MyPostQuitMessage   proc nExitCode:DWORD
                jmp     _PostQuitMessage   ; PostQuitMessage(x)
MyPostQuitMessage   endp
; __stdcall RegisterClassExA(x)
MyRegisterClassEx   proc lpwcx:DWORD
                jmp     _RegisterClassExA   ; RegisterClassExA(x)
MyRegisterClassEx   endp
; __stdcall SendMessageA(x,x,x,x)
MySendMessage       proc hWnd1:DWORD,Msg:DWORD,wParam:DWORD,lParam:DWORD
                jmp     _SendMessageA    ; SendMessageA(x,x,x,x)
MySendMessage       endp
; __stdcall SetMenu(x,x)
MySetMenu           proc hWnd1:DWORD,hMenu:DWORD
                jmp     _SetMenu   ; Assign a new menu to the specified window
MySetMenu           endp
; __stdcall SetWindowLongA(x,x,x)
MySetWindowLong     proc hWnd1:DWORD,nIndex:DWORD,dwNewLong:DWORD
                jmp     _SetWindowLongA    ; SetWindowLongA(x,x,x)
MySetWindowLong     endp
; __stdcall ShowWindow(x,x)
MyShowWindow        proc hWnd1:DWORD,nCmdShow:DWORD
                jmp     _ShowWindow   ; ShowWindow(x,x)
MyShowWindow        endp
; __stdcall TranslateMessage(x)
MyTranslateMessage   proc lpMsg:DWORD
                jmp     _TranslateMessage   ; TranslateMessage(x)
MyTranslateMessage   endp
; __stdcall UpdateWindow(x)
MyUpdateWindow      proc hWnd1:DWORD
                jmp     _UpdateWindow   ; UpdateWindow(x)
MyUpdateWindow      endp
; __stdcall ExitProcess(x)

MyExitProcess       proc uExitCode:DWORD
                jmp     _ExitProcess   ; ExitProcess(x)
MyExitProcess       endp

MyRtlZeroMemory proc
                jmp _RtlZeroMemory
MyRtlZeroMemory endp
; __stdcall GetCommandLineA()
MyGetCommandLine    proc
                jmp     _GetCommandLineA   ; GetCommandLineA()
MyGetCommandLine    endp
; __stdcall GetModuleHandleA(x)
MyGetModuleHandle   proc lpModuleName:DWORD
                jmp     _GetModuleHandleA   ; GetModuleHandleA(x)
MyGetModuleHandle   endp

    option prologue:PrologueDef
    
ALIGN
ComparAnsiStrUnicodeStr proc uses ecx edx esi edi lpAnsiStr:DWORD, lpUnicodeStr:DWORD
    mov edi, lpAnsiStr
    mov esi, lpUnicodeStr
    movzx ecx, [esi].UNICODE_STRING.woLength
    shr ecx, 1
    sub ecx, 3
    mov esi, [esi].UNICODE_STRING.Buffer
    .repeat
        movzx dx, byte ptr [edi]
        inc edi
        dec ecx
        lodsw
        .if al > 40h
            and al, 5fh
        .endif
        .if dl > 40h
            and dl, 5fh            
        .endif
        .if (al == '.') && (dl == '.')
            mov eax, 1
            ret
        .endif
    .until (ecx == 0) || (ax != dx)
    mov eax, 0
    ret
ComparAnsiStrUnicodeStr endp
ALIGN

CheckIsStr proc uses esi lpStr:DWORD
    mov esi, lpStr
    .while byte ptr [esi] != 0
        lodsb
        .if ((al < '.' ) || (al > 'z'))
            xor eax, eax
            ret
        .endif
    .endw
    mov eax, lpStr 
    ret
CheckIsStr endp
ALIGN

GetDllBase proc uses ecx edx esi lpProcName:DWORD

    ;mov eax, large fs:18h
    xor eax, eax
    ; masm32 在编译普通exe文件时，不支持 mov eax, fs:[18] 指令
    ; 因此写成如下格式，编译后等同 mov eax, fs:[18]
    db 64h
    mov eax, ds:18h             ; TEB.Self
    
    mov eax, [eax].TEB.Peb      ; 取 PEB 表的地址
    mov eax, [eax].PEB.Ldr      ; 取 PEB_LDR_DATA 结构的地址
    ; PEB_LDR_DATA.InLoadOrderModuleList.Flink 指向 LDR_DATA_TABLE_ENTRY
    add eax, PEB_LDR_DATA.InLoadOrderModuleList.Flink
    ; 这个结构形成一个链表
    mov ecx, [eax].LDR_DATA_TABLE_ENTRY.InLoadOrderModuleList.Flink
    
Loop_SeachModule:
    cmp ecx, eax
    jz  Exit
    mov edx, ecx
    ; 获取下一个 LDR_DATA_TABLE_ENTRY 结构的地址
    mov ecx, [ecx].LDR_DATA_TABLE_ENTRY.InLoadOrderModuleList.Flink
    ; 比较该内存地址是否为空，如果是，则出错
    cmp [edx].LDR_DATA_TABLE_ENTRY.InMemoryOrderModuleList.Flink, 0
    jz  Loop_SeachModule
    ; 获取模块名的地址
    lea esi, [edx].LDR_DATA_TABLE_ENTRY.BaseDllName.UNICODE_STRING.woLength
    push eax
    ; 验证是否是我们需要的模块
    invoke ComparAnsiStrUnicodeStr, lpProcName, esi
    cmp eax, 1
    pop eax
    jnz Loop_SeachModule
    ; 如果是，取该模块的基地址
    mov eax, [edx].LDR_DATA_TABLE_ENTRY.DllBase
    jmp Exit1

Exit:
    xor eax, eax

Exit1: 
    ret
GetDllBase endp
ALIGN

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
    ; 如果返回的函数调用地址在 ExportAddr 之间，则这个模块在外部模块
    .if ((ecx > ExportAddr) && (ecx <= edx))
        ; 验证是否为有效的字符串
        invoke CheckIsStr, ecx
        .if eax 
            ; 如果是，根据模块前部的指引获得该模块的基地址
            invoke GetDllBase, eax
            ; 调用自己，获得函数调用地址
            invoke GetFunctionAddress, eax, lpProcName
            mov ProcAddr, eax
        .endif
    .endif	
	
Exit:
    mov eax, ProcAddr
	ret
GetFunctionAddress endp
ALIGN

GetProcbaseAddress proc uses ebx esi FunAddr:DWORD
    .if !FunAddr
        ;如果 FunAddr = 0 随便 Call 一下，pop eax 就是当前的地址
        call $ + 5
        pop eax
    .else
        ;如果是某个函数的调用地址，函数调用地址一般有下面的格式:
        ; __stdcall MessageBoxA(x,x,x,x)
        ; MessageBoxA proc near
        ;FF 25 08 04 40 00     jmp     ds:__imp__MessageBoxA
        ; MessageBoxA endp
        ;其中 FF 25 是 jmp 的操作码，后面的 00400408 是函数调用地址
        ;这样就容易理解下面的代码了。
        mov	eax, FunAddr
        mov	eax, [eax+2]
        mov	eax, [eax]
	.endif   
	and	eax, 0FFFF0000h
	.repeat 
		.if word ptr [eax] != 'ZM'
            xor esi, esi
			sub	eax, 1000h
			
        .else
    		mov	esi, [eax+IMAGE_DOS_HEADER.e_lfanew]
		.endif		
	    add	esi, eax
	.until dword ptr [esi] == 'EP'

	ret
GetProcbaseAddress	endp
ALIGN

; #########################################################################
WinMain proc hInst     :DWORD,
             hPrevInst :DWORD,
             CmdLine   :DWORD,
             CmdShow   :DWORD

        ;====================
        ; Put LOCALs on stack
        ;====================

        LOCAL wc   :WNDCLASSEX
        LOCAL msg  :MSG

        LOCAL Wwd  :DWORD
        LOCAL Wht  :DWORD
        LOCAL Wtx  :DWORD
        LOCAL Wty  :DWORD

        ;==================================================
        ; Fill WNDCLASSEX structure with required variables
        ;==================================================

        mov wc.cbSize,         sizeof WNDCLASSEX
        mov wc.style,          CS_HREDRAW or CS_VREDRAW \
                               or CS_BYTEALIGNWINDOW
        mov wc.lpfnWndProc,    offset WndProc
        mov wc.cbClsExtra,     NULL
        mov wc.cbWndExtra,     NULL
        m2m wc.hInstance,      hInst   ;<< NOTE: macro not mnemonic
        mov wc.hbrBackground,  COLOR_BTNFACE+1
        mov wc.lpszMenuName,   NULL
        mov wc.lpszClassName,  offset szClassName
          invoke  MyLoadIcon,hInst,500    ; icon ID
        mov wc.hIcon,          eax
          invoke  MyLoadCursor,NULL,IDC_ARROW
        mov wc.hCursor,        eax
        mov wc.hIconSm,        0

        invoke  MyRegisterClassEx, ADDR wc

        ;================================
        ; Centre window at following size
        ;================================

        mov Wwd, 340
        mov Wht, 250

        invoke  MyGetSystemMetrics,SM_CXSCREEN
        invoke  TopXY,Wwd,eax
        mov Wtx, eax

        invoke  MyGetSystemMetrics,SM_CYSCREEN
        invoke  TopXY,Wht,eax
        mov Wty, eax

        szText szClassName,"Template_Class"

        invoke  MyCreateWindowEx,WS_EX_OVERLAPPEDWINDOW,
                                 ADDR szClassName,
                                 ADDR szDisplayName,
                                 WS_OVERLAPPEDWINDOW,
                                 Wtx,Wty,Wwd,Wht,
                                 NULL,NULL,
                                 hInst,NULL
        mov   hWnd,eax

        invoke  MyLoadMenu,hInst,600  ; menu ID
        invoke  MySetMenu,hWnd,eax

        invoke  MyShowWindow,hWnd,SW_SHOWNORMAL
        invoke  MyUpdateWindow,hWnd

      ;===================================
      ; Loop until PostQuitMessage is sent
      ;===================================

    StartLoop:
      invoke  MyGetMessage,ADDR msg,NULL,0,0
      cmp eax, 0
      je ExitLoop
      invoke  MyTranslateMessage, ADDR msg
      invoke  MyDispatchMessage,  ADDR msg
      jmp StartLoop
    ExitLoop:

      return msg.wParam

WinMain endp
ALIGN

; #########################################################################

WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    .if uMsg == WM_COMMAND
    ;======== menu commands ========
        .if wParam == 1000
            invoke  MySendMessage,hWin,WM_SYSCOMMAND,SC_CLOSE,NULL
        .elseif wParam == 1900
            szText TheMsg,"Assembler, Pure & Simple"
            invoke  MyMessageBox,hWin,ADDR TheMsg,ADDR szDisplayName,MB_OK
        .endif
    ;====== end menu commands ======


    .elseif uMsg == WM_CREATE

        jmp wpLbl
          txt1    db " Numeric",0
          txt2    db " Lcase",0
          txt3    db " Ucase",0
          nulbyte db 0
        wpLbl:

        invoke  Static,ADDR txt1,hWin,10,10,100,22,500
        invoke  Static,ADDR txt2,hWin,10,40,100,22,501
        invoke  Static,ADDR txt3,hWin,10,70,100,22,502

        invoke  EditSl,ADDR nulbyte,120,10,200,23,hWin,200
        mov hEdit1, eax
        invoke  EditSl,ADDR nulbyte,120,40,200,23,hWin,201
        mov hEdit2, eax
        invoke  EditSl,ADDR nulbyte,120,70,200,23,hWin,202
        mov hEdit3, eax

        invoke  MySetWindowLong,hEdit1,GWL_WNDPROC,Ed1Proc
        mov lpfnEd1Proc, eax

        invoke  MySetWindowLong,hEdit2,GWL_WNDPROC,Ed2Proc
        mov lpfnEd2Proc, eax

        invoke  MySetWindowLong,hEdit3,GWL_WNDPROC,Ed3Proc
        mov lpfnEd3Proc, eax

    .elseif uMsg == WM_CLOSE
        szText TheText,"Please Confirm Exit"
        invoke  MyMessageBox,hWin,ADDR TheText,ADDR szDisplayName,MB_YESNO
          .if eax == IDNO
            return 0
          .endif
    .elseif uMsg == WM_DESTROY
        invoke  MyPostQuitMessage,NULL
        return 0 
    .endif

    invoke  MyDefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp

ALIGN
; ########################################################################
TopXY proc wDim:DWORD, sDim:DWORD

    shr sDim, 1      ; divide screen dimension by 2
    shr wDim, 1      ; divide window dimension by 2
    mov eax, wDim    ; copy window dimension into eax
    sub sDim, eax    ; sub half win dimension from half screen dimension

    return sDim

TopXY endp
ALIGN

; ########################################################################
EditSl proc szMsg:DWORD,a:DWORD,b:DWORD,
               wd:DWORD,ht:DWORD,hParent:DWORD,ID:DWORD

; EditSl PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
; invoke  MyEditSl,adrTxt:DWORD,200,10,150,250,hWnd,700

    szText slEdit,"EDIT"

    invoke  MyCreateWindowEx,WS_EX_CLIENTEDGE,ADDR slEdit,szMsg,
                WS_VISIBLE or WS_CHILDWINDOW or \
                ES_AUTOHSCROLL or ES_NOHIDESEL,
              a,b,wd,ht,hParent,ID,hInstance,NULL

    ret

EditSl endp
ALIGN

; ########################################################################
Static proc lpText:DWORD,hParent:DWORD,
                 a:DWORD,b:DWORD,wd:DWORD,ht:DWORD,ID:DWORD

    szText statClass,"STATIC"

    invoke  MyCreateWindowEx,WS_EX_STATICEDGE,
            ADDR statClass,lpText,
            WS_CHILD or WS_VISIBLE or SS_LEFT,
            a,b,wd,ht,hParent,ID,
            hInstance,NULL

    ret

Static endp
ALIGN

; ########################################################################
Ed1Proc proc hCtl   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    LOCAL Buffer[32]:BYTE

    ; -----------------------------
    ; Process control messages here
    ; -----------------------------

    .if uMsg == WM_CHAR
        .if wParam == 8             ; backspace
            jmp accept
        .endif

        .if wParam == "."           ; only allow one decimal point

            invoke  MySendMessage,hCtl,WM_GETTEXT,sizeof Buffer,ADDR Buffer

            mov ecx, sizeof Buffer  ; byte count in ecx
            lea esi, Buffer         ; address in esi
          @xxx:
            lodsb                   ; load byte into al

            cmp al, "."             ; if decimal point already in Buffer
            jne @xx1
              return 0              ; throw it away
            @xx1:

            dec ecx
            cmp ecx, 0
            jne @xxx

            jmp accept
        .endif

        .if wParam < "0"
            return 0
        .endif

        .if wParam > "9"
            return 0
        .endif

    .endif

    accept:

    invoke  MyCallWindowProc,lpfnEd1Proc,hCtl,uMsg,wParam,lParam

    ret

Ed1Proc endp
ALIGN

; #########################################################################
Ed2Proc proc hCtl   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    ; -----------------------------
    ; Process control messages here
    ; -----------------------------

    .if uMsg == WM_CHAR
        .if wParam >= "A"
          .if wParam <= "Z"
            add wParam, 32
          .endif
        .endif
    .endif

    invoke  MyCallWindowProc,lpfnEd2Proc,hCtl,uMsg,wParam,lParam

    ret

Ed2Proc endp

; #########################################################################

Ed3Proc proc hCtl   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    ; -----------------------------
    ; Process control messages here
    ; -----------------------------

    .if uMsg == WM_CHAR
        .if wParam >= "a"
          .if wParam <= "z"
            sub wParam, 32
          .endif
        .endif
    .endif

    invoke  MyCallWindowProc,lpfnEd3Proc,hCtl,uMsg,wParam,lParam

    ret

Ed3Proc endp
ALIGN

; #########################################################################

start:
    jmp S1
    ;这里加了一句永远也不会执行的 API 调用，目的是为了获得 user32.dll 的基地址
    invoke  MessageBox,0,ADDR TheText,ADDR szDisplayName,MB_OK
S1:
    mov eax, offset ExitProcess        ;取ExitProcess函数地址
    invoke GetProcbaseAddress, eax     ;获取kernel32.dll基地址
;    invoke GetDllBase, addr szKernel32  ;测试
    mov MyImageBase, eax               ;保存
    mov esi, offset OffExitProcess     ;取kernel32.dll API 函数名引用表的首地址
    mov edi, offset _ExitProcess       ;取kernel32.dll 自建输入表首地址
    .while (dword ptr [esi] != 0)
        invoke GetFunctionAddress, MyImageBase, [esi]
        stosd                          ;建立kernel32.dll的输入表
        lodsd
    .endw

    mov eax, offset MessageBox         ;取MessageBox函数的地址
    invoke GetProcbaseAddress, eax     ;获取user32.dll基地址
    mov MyImageBase, eax               ;保存
    mov esi, offset OffCallWindowProcA ;取user32.dll API 函数名引用表首地址
    mov edi, offset _CallWindowProcA   ;取user32.dll 自建输入表首地址
    .while (dword ptr [esi] != 0)
        invoke GetFunctionAddress, MyImageBase, [esi]
        stosd                          ;建立user32.dll的输入表
        lodsd
    .endw
    
    ;下面是原程序的代码，所有 API 函数都家里 My 前缀
    invoke  MyGetModuleHandle, NULL
    mov hInstance, eax

    invoke  MyGetCommandLine
    mov CommandLine, eax
    invoke  WinMain,hInstance,NULL,CommandLine,SW_SHOWDEFAULT
    ;这里保留是为了取 Kernel32.dll 的基地址
    invoke  ExitProcess,eax
    ret
end start

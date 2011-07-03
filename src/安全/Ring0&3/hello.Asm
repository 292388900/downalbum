.386
.model flat, stdcall
option casemap:none

include hello.inc
;include windows.inc
;include w2k\ntstatus.inc
include w2k\ntddk.inc

include w2k\ntoskrnl.inc
;include kernel32.inc
include user32.inc


includelib user32.lib
includelib kernel32.lib
includelib D:\RadASM\masm32\lib\w2k\ntoskrnl.lib
include Strings.mac

GetModuleHandleA PROTO :DWORD
GetModuleHandle equ <GetModuleHandleA>



_PROCVAR1		typedef proto :dword
PROCVAR1		typedef ptr _PROCVAR1

_PROCVAR2		typedef proto :dword,:dword
PROCVAR2		typedef ptr _PROCVAR2

_PROCVAR4		typedef proto :dword,:dword,:dword,:dword
PROCVAR4		typedef ptr _PROCVAR4

			.data?
hDllInstance        	  dd              	?
lpDbgPrint		PROCVAR1		?
lpLoadLibrary		PROCVAR1		?
lpGetProcAddress	PROCVAR2		?
lpMessageBox		PROCVAR4		?

lpKernelBase		  dd			?
lpUser32Base		  dd			?

			.data
szntoskrnlexe		  db	'ntoskrnl.exe',0
szDbgPrint		  db	'DbgPrint',0

.code

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

    mov eax, [ebx].IMAGE_NT_HEADERS.OptionalHeader.DataDirectory.isize
    mov ExportSize, eax
    
	mov ebx, [ebx].IMAGE_NT_HEADERS.OptionalHeader.DataDirectory.VirtualAddress
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
	
Exit:
    mov eax, ProcAddr
	ret
GetFunctionAddress endp

DriverEntry proc pDriverObject:PDRIVER_OBJECT,pusRegistryPath:PUNICODE_STRING
;-----------------------------------------------------------------------------
;get ret of DriverEntry, then get the base of kernel32 or ntoskrnl
	lea 	eax,[ebp][4]     
	mov	eax,dword ptr [eax]
	and	eax,0FFFFF000h
   	.while	TRUE
   		.if word ptr [eax]=='ZM'
   			.break;
   		.endif
   		sub	eax,1000h
   	.endw
   	mov 	lpKernelBase,eax
;-----------------------------------------------------------------------------
;get the image base of me
	call	bellow
   bellow:	
   	pop	eax
;-------------------------	
   	and	eax,0FFFFF000h
   	.while	TRUE
   		.if word ptr [eax]=='ZM'
   			.break;
   		.endif
   		sub	eax,1000h
   	.endw
;-----------------------------------------------------------------------------
;test the subsystem
	assume 	eax:ptr IMAGE_DOS_HEADER
	add 	eax,[eax].e_lfanew
	assume 	eax:ptr IMAGE_NT_HEADERS
	.if 	[eax].OptionalHeader.Subsystem==1	;in ring0 as driver
		invoke 	GetFunctionAddress,lpKernelBase,$CTA0("DbgPrint")
		mov	lpDbgPrint,eax
		.if 	eax!=0 
			invoke 	lpDbgPrint,$CTA0("I am in ring 0") 
		.endif  
    		ret 
	.endif	
;-----------------------------------------------------------------------------
;in ring3 
	invoke 	GetFunctionAddress,lpKernelBase,$CTA0("LoadLibraryA")
	mov	lpLoadLibrary,eax
	invoke 	GetFunctionAddress,lpKernelBase,$CTA0("GetProcAddress")	
	mov	lpGetProcAddress,eax
	
	invoke	lpLoadLibrary,$CTA0("user32.dll")
	mov	lpUser32Base,eax
	invoke	lpGetProcAddress,lpUser32Base,$CTA0("MessageBoxA")
	mov	lpMessageBox,eax
	invoke	lpMessageBox,0,$CTA0("I am in ring 3"),0,0	
	leave
	retn 

DriverEntry endp

end DriverEntry

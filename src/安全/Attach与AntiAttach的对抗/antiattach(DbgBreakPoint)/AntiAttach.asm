; =======================================================================
;当调试进程创建时，DbgBreakPoint会被调用，只要Hook掉入口代码即可，
;尽管这个函数只有2个指令：int3 and ret.
; =======================================================================

.386
.model flat,stdcall
option casemap:none

include windows.inc
include kernel32.inc
include user32.inc
 
includelib kernel32.lib
includelib user32.lib
 
extrn _imp__MessageBoxA@16:near   
extrn _imp__ExitProcess@4:near  

.data?
    ddOldProtect 	 	dd ?
    ptrNtContinue	 	dd ?

.data  
    szNTDLL              	db 'ntdll.dll',NULL
    szDbgBreakPoint 	 	db 'DbgBreakPoint',NULL
    szAntiCaption        	db 'AntiAttach',NULL
    szAntiWarning   	 	db 'Gotcha! You are trying to attach debugger...',NULL
    szAntiTitleInfo      	db 'Now... try to attach debugger to AntiAttach process.',NULL

.code

;---------------------------------------
option prologue:none
HookCodeBegin:
;push	MB_OK or MB_ICONEXCLAMATION
;push	0
;push  	0
;push 	0
;call	dword ptr _imp__MessageBoxA@16	;防止产生jmp跳转表
push	0
call 	dword ptr _imp__ExitProcess@4	;防止产生jmp跳转表
HookCodeEnd:
option prologue:prologuedef
;---------------------------------------

Start: 
  
invoke 	GetModuleHandle,offset szNTDLL
invoke 	GetProcAddress,eax,offset szDbgBreakPoint
push	eax
     
invoke  VirtualProtect,eax,1,PAGE_EXECUTE_READWRITE,offset ddOldProtect

pop	edi
mov	esi,offset HookCodeBegin
mov	ecx,HookCodeEnd - HookCodeBegin
cld
rep	movsb

invoke 	MessageBox,0,offset szAntiTitleInfo,offset szAntiCaption,MB_OK
ret
    
end Start

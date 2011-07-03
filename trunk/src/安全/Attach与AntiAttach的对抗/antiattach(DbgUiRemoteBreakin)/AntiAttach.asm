; 
; KaKeeware is proud to present a small piece of code that
; demonstrates how to block usermode debuggers from attaching 
; to your process.      
;
; Author: Adam Blaszczyk (c) 2005
; WWW:    http://www.kakeeware.com     
; e-mail: adam[]kakeeware[]com
;
; Feel free to use this source code in your applications, but remember
; that credits are always welcomed :-)                            
;
; ============================================================

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
    ddOldProtect 		dd ?
    ptrDbgUiRemoteBreakin 	dd ?

.data  
    szNTDLL              db 'ntdll.dll',NULL
    szDbgUiRemoteBreakin db 'DbgUiRemoteBreakin',NULL
    szAntiCaption        db 'AntiAttach',NULL
    szAntiWarning   	 db 'Gotcha! You are trying to attach debugger...',NULL
    szAntiTitleInfo      db 'Now... try to attach debugger to AntiAttach process.',NULL

.code

;---------------------------------------
option prologue:none
HookCodeBegin:
push	MB_OK or MB_ICONEXCLAMATION
push	offset szAntiCaption
push	offset szAntiWarning
push	0
call	dword ptr _imp__MessageBoxA@16	;防止产生jmp跳转表
push	0
call 	dword ptr _imp__ExitProcess@4	;防止产生jmp跳转表
HookCodeEnd:
option prologue:prologuedef
;---------------------------------------

Start: 
  
invoke 	GetModuleHandle,offset szNTDLL
invoke 	GetProcAddress,eax,offset szDbgUiRemoteBreakin
mov 	ptrDbgUiRemoteBreakin,eax 
     
invoke  VirtualProtect,eax,1,PAGE_EXECUTE_READWRITE,offset ddOldProtect

mov 	edi,ptrDbgUiRemoteBreakin
mov	esi,offset HookCodeBegin
mov	ecx,HookCodeEnd - HookCodeBegin
cld
rep	movsb

invoke 	MessageBox,0,offset szAntiTitleInfo,offset szAntiCaption,MB_OK
ret
    
end Start

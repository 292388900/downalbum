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
 
extrn _imp__ExitProcess@4:near  

.data?
    dwhModule		 dd ?
    ddOldProtect 	 dd ?
    lpNtContinue	 dd ?
    lpDbgUiRemoteBreakin dd ?
    lpDbgBreakPoint	 dd ?

.data  
    szNTDLL              db 'ntdll.dll',NULL
    szNtContinue 	 db 'NtContinue',NULL
    szDbgUiRemoteBreakin db 'DbgUiRemoteBreakin',NULL
    szDbgBreakPoint 	 db 'DbgBreakPoint',NULL
    szAntiCaption        db 'AntiAttach',NULL
    szAntiTitleInfo      db 'Attach Me!',NULL

.code

;---------------------------------------
option prologue:none
HookCodeBegin:
push	0
call 	dword ptr _imp__ExitProcess@4	;防止产生jmp跳转表
HookCodeEnd:
option prologue:prologuedef
;---------------------------------------

Start: 
  
invoke 	GetModuleHandle,offset szNTDLL
mov	dwhModule,eax
invoke 	GetProcAddress,dwhModule,offset szNtContinue
mov 	lpNtContinue,eax 
invoke 	GetProcAddress,dwhModule,offset szDbgUiRemoteBreakin
mov 	lpDbgUiRemoteBreakin,eax 
invoke 	GetProcAddress,dwhModule,offset szDbgBreakPoint
mov 	lpDbgBreakPoint,eax 
     
invoke  VirtualProtect,lpNtContinue,1,PAGE_EXECUTE_READWRITE,offset ddOldProtect
invoke  VirtualProtect,lpDbgUiRemoteBreakin,1,PAGE_EXECUTE_READWRITE,offset ddOldProtect
invoke  VirtualProtect,lpDbgBreakPoint,1,PAGE_EXECUTE_READWRITE,offset ddOldProtect

mov 	edi,lpNtContinue
mov	esi,offset HookCodeBegin
mov	ecx,HookCodeEnd - HookCodeBegin
cld
rep	movsb

mov 	edi,lpDbgUiRemoteBreakin
mov	esi,offset HookCodeBegin
mov	ecx,HookCodeEnd - HookCodeBegin
cld
rep	movsb

mov 	edi,lpDbgBreakPoint
mov	esi,offset HookCodeBegin
mov	ecx,HookCodeEnd - HookCodeBegin
cld
rep	movsb

invoke 	MessageBox,0,offset szAntiTitleInfo,offset szAntiCaption,MB_OK
ret
    
end Start

; This is just a small snippet that demonstrates how 
; to use Structured and Vectored Exception Handlers
; together (requires at lest Windows XP). 
; Vectored Exception Handler works as a man-in-the-middle 
; that can intercept all "exceptional" traffic handled 
; by SEH, thus, providing an interesting mechanism for 
; tracing bugs or developing new copy protection schemes.
; ============================================================

.386
.model flat,stdcall
option casemap:none

include windows.inc
include kernel32.inc
include user32.inc

includelib kernel32.lib
includelib user32.lib

_PROCVAR2		typedef proto :dword,:dword
PROCVAR2		typedef ptr _PROCVAR2

.data  
lpAddVectoredExceptionHandler PROCVAR2 ?
lpKernel32                    dd ?
szTitleNoVEH                  db 'You need at least Windows XP. Can''t'
                              db 'find function AddVectoredExceptionHandler in kernel32!',NULL
szAddVectoredExceptionHandler db 'AddVectoredExceptionHandler',NULL
szKernel32                    db 'kernel32',NULL
szCaption                     db 'VectoredExceptionHandler',NULL
szTitleSEH                    db 'Hello from SEH handler.',NULL
szTitleVEH                    db 'Hello from VEH handler',NULL

.code            
  assume fs:nothing
  
; --- SEH Handler  
SEHHandler proc C uses ebx ExceptionRecord,EstablisherFrame,ContextRecord,DispatcherContext:DWORD
	mov  	eax,ExceptionRecord        
	assume	eax:ptr EXCEPTION_RECORD
	.if	[eax].ExceptionFlags==0 && [eax].ExceptionCode==STATUS_INTEGER_DIVIDE_BY_ZERO
		mov	eax,ContextRecord
		assume	eax:ptr CONTEXT
		inc	[eax].regEcx	;ecx++，下次不再会有除零异常了
		invoke 	MessageBoxA,0,offset szTitleSEH,offset szCaption,MB_OK	
		mov	eax,EXCEPTION_CONTINUE_EXECUTION
		ret	
	.endif
	
  SEHHandler_contSearch:
	mov	eax,EXCEPTION_CONTINUE_SEARCH
	ret     
SEHHandler endp  

; --- VEH Handler  
VEHHandler proc uses ebx ExceptionInfo:ptr EXCEPTION_POINTERS
	mov  	eax,ExceptionInfo
	assume 	eax:ptr EXCEPTION_POINTERS
	mov	ebx,[eax].pExceptionRecord	
	assume	ebx:ptr EXCEPTION_RECORD
	.if	[ebx].ExceptionFlags==0 && [ebx].ExceptionCode==STATUS_INTEGER_DIVIDE_BY_ZERO
		;mov	eax,[eax].ContextRecord
		;assume	eax:ptr CONTEXT
		;inc	[eax].regEcx	;ecx++，下次不再会有除零异常了
		invoke 	MessageBoxA,0,offset szTitleVEH,offset szCaption,MB_OK	
		;mov	eax,EXCEPTION_CONTINUE_EXECUTION
		;ret	
	.endif
  ;以便让其他异常处理程序处理
  VEHHandler_contSearch:
	mov	eax,EXCEPTION_CONTINUE_SEARCH
    	ret     
VEHHandler endp

; --- START
Start:   
	invoke GetModuleHandle,offset szKernel32
	mov lpKernel32,eax
	
	invoke GetProcAddress,lpKernel32,offset szAddVectoredExceptionHandler
	mov lpAddVectoredExceptionHandler,eax
	.if eax!=0
		; install VEH Handler
		invoke lpAddVectoredExceptionHandler,1,offset VEHHandler
		
		; install SEH Handler
		push  offset SEHHandler
		push  dword ptr fs:[0]
		mov   fs:[0],esp
		
		; generate DIVIDE BY ZERO exception
		xor   ecx,ecx
		; when this instruction is executed, VEHHandler will be 
		; called and then SEHHandler will be called next;
		div   ecx
		
		; remove SEH Handler
		pop   dword ptr fs:[0]
		add   esp,4
	.else
		invoke MessageBoxA,0,offset szTitleNoVEH,offset szCaption,MB_OK
	.endif
	
	ret
end Start

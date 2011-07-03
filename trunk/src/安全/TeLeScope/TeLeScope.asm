; This is the simplest example that shows how to use
; TLS (Thread Local Storage) callbacks to execute
; code before the system transfers controls to program's
; main entry point.
; There's nothing new here. Elicz did that in 2000 already,
; but as I played with TLS recently, so... here it is.
; ============================================================

COMMENT $

.386
.model flat,stdcall
option casemap:none
assume fs:nothing
assume gs:nothing

include windows.inc

include kernel32.inc
include user32.inc

includelib kernel32.lib
includelib user32.lib

.code 
szCaption 		db 'TLS callback',NULL
szTextProcessAttach	db 'DLL_PROCESS_ATTACH',NULL
szTextProcessDetach	db 'DLL_PROCESS_DETACH',NULL
szTextThreadAttach	db 'DLL_THREAD_ATTACH',NULL
szTextThreadDetach	db 'DLL_THREAD_DETACH',NULL

public _tls_used
_tls_used:
TLS_Start:
	dd offset TLS_Start     	; DWORD   StartAddressOfRawData;
      	dd offset TLS_End 		; DWORD   EndAddressOfRawData;
      	dd offset dwTLS_Index    	; DWORD   AddressOfIndex;             // PDWORD
      	dd offset TLS_CallBackStart	; DWORD   AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
      	dd 0                 		; DWORD   SizeOfZeroFill;
      	dd 0                 		; DWORD   Characteristics;
TLS_End: 
     
dwTLS_Index		dd ?  
TLS_CallBackStart	dd offset TLSCallback
			dd NULL 


TLSCallback proc DllH,Reas,Resv:DWORD
    	LOCAL sz:DWORD
	.if Reas == DLL_PROCESS_ATTACH
         	lea	eax,szTextProcessAttach
        .elseif Reas == DLL_PROCESS_DETACH
        	lea	eax,szTextProcessDetach
        .elseif Reas == DLL_THREAD_ATTACH
        	lea	eax,szTextThreadAttach
        .elseif Reas == DLL_THREAD_DETACH
        	lea	eax,szTextThreadDetach
     	.endif
     	invoke	MessageBox,0,eax,offset szCaption,MB_OK
     	xor eax,eax
     	inc eax
     	ret
TLSCallback endp

Start: 
	ret 	
end Start

$


.386
.model flat,stdcall
option casemap:none
assume fs:nothing
assume gs:nothing

include windows.inc

include kernel32.inc
include user32.inc

includelib kernel32.lib
includelib user32.lib


;这里的OPTION DOTNAME的意思为让ML可以使用以“.”开头的变量，结构，联合，标号等
;开启这个选项也可以在ML中使用/Zm选项来启动。
;OPTION    DOTNAME
;定义一个TLS节
;SEGMENT标志在ML里是用于定义一个新节。
;.tls  SEGMENT                        
;	TLS_Start LABEL  DWORD
;	 	dd ?
;	TLS_End   LABEL  DWORD
;.tls   ENDS
;OPTION    NODOTNAME

.data

dwTLS_Index dd  ?

szCaption 		db 'TLS callback',NULL
szTextProcessAttach	db 'DLL_PROCESS_ATTACH',NULL
szTextProcessDetach	db 'DLL_PROCESS_DETACH',NULL
szTextThreadAttach	db 'DLL_THREAD_ATTACH',NULL
szTextThreadDetach	db 'DLL_THREAD_DETACH',NULL

TLS_CallBackStart   	dd TLSCallback
TLS_CallBackEnd     	dd NULL

public _tls_used	    ;TLS_Start	;TLS_End
_tls_used IMAGE_TLS_DIRECTORY <0, 0, dwTLS_Index, TLS_CallBackStart, 0, 0>
;dwTLS_Index和TLS_CallBackStart不能为零


.code 
TLSCallback proc DllH,Reas,Resv:DWORD
    	LOCAL sz:DWORD
	.if Reas == DLL_PROCESS_ATTACH
         	lea	eax,szTextProcessAttach
        .elseif Reas == DLL_PROCESS_DETACH
        	lea	eax,szTextProcessDetach
        .elseif Reas == DLL_THREAD_ATTACH
        	lea	eax,szTextThreadAttach
        .elseif Reas == DLL_THREAD_DETACH
        	lea	eax,szTextThreadDetach
     	.endif
     	invoke	MessageBox,0,eax,offset szCaption,MB_OK
     	xor eax,eax
     	inc eax
     	ret
TLSCallback endp

Start: 
	ret 	
end Start


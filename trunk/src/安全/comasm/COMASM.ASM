
 ; ============================================================
 ;  COMASM v1.0 (Win32Asm/COM - usage of ITaskbarList interface)
 ;
 ;  This little program shows how to use COM technology in
 ;  Win32Asm. When it's run it lets the user freely decide
 ;  about the presence of the program with the WS_EX_TOOLWINDOW
 ;  style on the taskbar. The main goal is to show the easiest
 ;  possible example instead of killing people with the details
 ;  about virtual tables, polymorphism, inheritance and using
 ;  nested macros etc., etc.
 ;
 ;  Requirements:
 ;   - Windows 98 (or Windows 95 + IE 4.0+)
 ;   - Windows 2000 (or Windows NT 4.0 + IE 4.0+)
 ;
 ;  Error handling is reduced to minimum :).
 ;
 ;  This program may be freely distributed as long as it's content
 ;  stays unchanged. 
 ; ============================================================

.386
.model flat,stdcall
option casemap:none


ID_DLG          =   100
;ID_GROUP        =   1001
ID_CHANGE       =   1002

include windows.inc

include kernel32.inc 
include user32.inc
include ole32.inc

includelib kernel32.lib
includelib user32.lib 
includelib ole32.lib

 ; ============================================================
 ;  Declaration of VTable that includes the pointers to the
 ;  methods. Note that it always includes 3 methods of IUnknown
 ;  In fact, all the COM interfaces are built like this.
 ;  Warning: Always refer to Windows Platform SDK header files
 ;           to check for the sequence of the methods
 ;           the sequence is crucial !!! and don't rely on SDK
 ;           help file, but always on header files!!!
 ; ============================================================
 ITaskBarListVtbl STRUC
    ; These are pointers to IUnknown interface
   QueryInterface     	dd ?
   AddRef             	dd ?
   Release          	dd ?
    ; and these ones are for ITaskbarlist
   HrInit             	dd ?
   AddTab              	dd ?
   DeleteTab           	dd ?
   ActivateTab         	dd ?
   SetActiveAlt        	dd ?
 ITaskBarListVtbl ENDS
 

.data
 ; We need two GUIDs to communicate with the DLL in which the object
 ; is implemented - GUIDs may be found in header files(SHLGUID.h)
 ; and ... in the Registry (look for HKCR/CLSID/...)
 ; CLSID_TaskbarList '56FDF344-FD6D-11D0-958A-006097C9A090'
   CLSID_TaskbarList GUID <56FDF344h,0FD6Dh,11D0h,<95h,8Ah,0,60h,97h,0C9h,0A0h,90h>>
 ; IID_ITaskbarList  '56FDF342-FD6D-11D0-958A-006097C9A090'
   IID_ITaskbarList  GUID <56fdf342h,0FD6Dh,11D0h,<95h,8Ah,0,60h,97h,0C9h,0A0h,90h>>

 dwVisible  	dd FALSE
 szTextShow   	db 'Show',NULL 
 szTextHide   	db 'Hide',NULL 
 dwTextOffset 	dd offset szTextShow,offset szTextHide
 
 hInstance 	dd ?
 pobj		dd ?;define the interface
		
.code

 _ShellMethod proc C pthis,method,args:VARARG
 	mov	eax,pthis	;◊¢£∫mov eax,[pthis]’‚æ‰÷∏¡Ó°∂=°∑mov eax,pthis
 	mov	eax,[eax]	;get poiter to VTBL
 	add	eax,method	;No.method in VTBL
 	push	args
 	push	pthis
 	call	dword ptr[eax]
 	ret
 _ShellMethod endp
 
 ; ============================================================
 ;  Dialog's procedure
 ; ============================================================
 DialogProc proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD,lParam:DWORD
	pushad
   	.if uMsg==WM_INITDIALOG
       		invoke 	SetDlgItemText,hDlg,ID_CHANGE,offset szTextShow
		invoke 	CoInitialize,NULL
		invoke 	CoCreateInstance,offset CLSID_TaskbarList,NULL,\
	     			CLSCTX_INPROC_SERVER,offset IID_ITaskbarList,addr pobj
		.if eax==S_OK
	  		invoke	_ShellMethod,pobj,ITaskBarListVtbl.HrInit
	  	.endif 
	  	
   	.elseif  uMsg==WM_CLOSE
		invoke 	CoUninitialize
	       	invoke 	EndDialog,hDlg,0

   	.elseif  uMsg==WM_COMMAND ; BN_CLICKED=0

      		.if wParam==IDOK
          		invoke 	SendMessage,hDlg,WM_CLOSE,0,0

      		.elseif wParam==ID_CHANGE

			inc   	dwVisible        ; \ Visible/inVisible
			and   	dwVisible,1      ; /
			
			mov   	edx,dwVisible    ; eax=0 or 1
			mov   	eax,[dwTextOffset][edx*4]
			invoke	SetDlgItemText,hDlg,ID_CHANGE,eax
			
			.if dwVisible==TRUE
				invoke	_ShellMethod,pobj,ITaskBarListVtbl.AddTab,hDlg
				invoke	_ShellMethod,pobj,ITaskBarListVtbl.ActivateTab,hDlg
			.else
				invoke	_ShellMethod,pobj,ITaskBarListVtbl.DeleteTab,hDlg
			.endif
			
		.endif
   	.endif
   	popad
   	xor eax,eax
   	ret
 DialogProc endp
 ; ============================================================
 Start:
   	invoke DialogBoxParam,hInstance,ID_DLG,0,offset DialogProc,0
   	invoke ExitProcess,0
end	Start

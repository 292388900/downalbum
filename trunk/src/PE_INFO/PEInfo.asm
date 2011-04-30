                    .386
                    .model flat, stdcall
                    option casemap :none
;-------------------------------------------------
; Include 文件定义
;-------------------------------------------------
include         windows.inc
include         user32.inc
includelib      user32.lib
include         kernel32.inc
includelib      kernel32.lib
;include         Comctl32.inc
;includelib      Comctl32.lib
include		comdlg32.inc
includelib	comdlg32.lib
;include	Gdi32.inc
;includelib	Gdi32.lib
;include	shell32.inc
;includelib	shell32.lib
include		imagehlp.inc
includelib	imagehlp.lib
;-------------------------------------------------
; Equ 等值定义
;-------------------------------------------------
include		resource.h
;-------------------------------------------------
;-------------------------------------------------
; 数据段
;-------------------------------------------------
                    .data?
hInstance	dd	?
hWinMain	dd	?

hFile		dd	?
hMapping	dd	?
ImgBase		dd	?

szfilename	db	MAX_PATH dup(?)
;-------------------------------------------------
                    .const
szfilefilter		db	'exe可执行文件(*.exe)',0,'*.exe',0
			db	'dll动态链接库(*.dll)',0,'*.dll',0
			db	'所有文件(*.*)'     ,0,'*.*'  ,0
szfileext		db	'exe',0

szFormat_4		db	'%04x',0
szFormat_8		db	'%08x',0
szOrdFormat_8		db	'Ord:%08x',0
szMemAddrFormat_8	db	'MemAddr:%08x',0

szstrName		db	'Name',0
szstrAddr		db	'VAddress',0
szstrVsize		db	'VSize',0
szstrROffset		db	'ROffset',0
szstrRSize		db	'RSize',0
szstrFlags		db	'Flags',0

szstrOrdinal		db	'Ordinal',0
szstrRVA		db	'RVA',0
szstrFunction_Name	db	'Function Name',0	

szstrDLLName		db	'DLL Name',0
szstrOriginalFirstThunk db	'OriginalFirstThunk',0
szstrTimeDateStamp	db	'TimeDateStamp',0
szstrForwarderChin	db	'ForwarderChin',0
szstrName2		db	'Name',0
szstrFirstThunk		db	'FirstThunk',0

szstrThunkRVA		db	'ThunkRVA',0
szstrThunkValue		db	'ThunkValue',0	
szstrHint		db	'Hint',0
szstrFunctionName	db	'Function Name',0

szerrcreatefile		db	'打开文件失败!',0
szerrmapview	        equ     this byte
szerrcreatemap		db	'创建文件映像失败!',0
szerrGetFileHdr		db	'获取文件头失败!',0
szErrGetOpHdr		db	'获取可选头失败!',0
szErrNotPE		db	'文件似乎不是合法的PE文件!',0


szFuncNoName	db	'--',0
;-------------------------------------------------
; 代码段
                    .code
;-------------------------------------------------
include		FunctionUnit.asm
;-------------------------------------------------
;------------------------------------------------------------
;------------------------------------------------------------
_InitSectionListCtrl	proc	hDlg
local	hList
local	lvColumn:LVCOLUMN	

invoke	RtlZeroMemory,addr lvColumn,sizeof lvColumn
invoke	GetDlgItem,hDlg,IDC_SECTIONLIST
mov	hList,eax
invoke	SendMessage,hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT

lea	ebx,lvColumn
mov	dword ptr[ebx],LVCF_TEXT
or	dword ptr[ebx],LVCF_WIDTH
;<=>mov	lvColumn.mask,LVCF_TEXT | LVCF_WIDTH
add	ebx,2*4
mov	dword ptr[ebx],80
;<=>mov	lvColumn.cx,80

mov	lvColumn.pszText,offset szstrName
invoke	SendMessage,hList, LVM_INSERTCOLUMN, 0, addr lvColumn

mov	lvColumn.pszText,offset szstrAddr
invoke	SendMessage,hList, LVM_INSERTCOLUMN, 1, addr lvColumn
	
mov	lvColumn.pszText,offset szstrVsize
invoke	SendMessage,hList, LVM_INSERTCOLUMN, 2, addr lvColumn
	
mov	lvColumn.pszText,offset szstrROffset 
invoke	SendMessage,hList, LVM_INSERTCOLUMN, 3, addr lvColumn

mov	lvColumn.pszText,offset szstrRSize 
invoke	SendMessage,hList, LVM_INSERTCOLUMN, 4, addr lvColumn
	
mov	lvColumn.pszText,offset szstrFlags 
invoke	SendMessage,hList, LVM_INSERTCOLUMN, 5, addr lvColumn
ret
_InitSectionListCtrl	endp
;------------------------------------------------------------
_IsPEFile	proc	lpbase
.if	!lpbase
	xor	eax,eax
	ret
.endif
mov	eax,lpbase
assume	eax:ptr IMAGE_DOS_HEADER
.if	[eax].e_magic!=IMAGE_DOS_SIGNATURE
	xor	eax,eax
	ret
.endif
add	eax,[eax].e_lfanew
assume	eax:ptr IMAGE_NT_HEADERS
.if	[eax].Signature != IMAGE_NT_SIGNATURE
	xor	eax,eax
	ret
.endif
assume	eax:nothing
ret
_IsPEFile	endp
;------------------------------------------------------------
;------------------------------------------------------------
_GetFileHeader	proc	lpbase
invoke	_IsPEFile,lpbase
.if	eax
	add	eax,IMAGE_NT_HEADERS.FileHeader
.endif
ret
_GetFileHeader	endp
;------------------------------------------------------
_GetOptionalHeader	proc	lpbase
invoke	_IsPEFile,lpbase
.if	eax
	add	eax,IMAGE_NT_HEADERS.OptionalHeader
.endif
ret
_GetOptionalHeader	endp
;------------------------------------------------------------
;------------------------------------------------------------
_GetFirstSectionHeader	proc	lpimgbase
invoke	_IsPEFile,lpimgbase
.if	eax==0
	ret
.endif
;mov	ebx,eax
;assume	ebx:ptr IMAGE_NT_HEADERS
;add	eax,IMAGE_NT_HEADERS.OptionalHeader
;add	ax,[ebx].FileHeader.SizeOfOptionalHeader
;assume	ebx:nothing
add	eax,sizeof  IMAGE_NT_HEADERS
ret
_GetFirstSectionHeader	endp
;------------------------------------------------------------
_ShowSectionHeaderInfo	proc	hDlg
local	hList
local	SecNum
local	icount
local	cBuff[10]:byte
local	cName[10]:byte
local	lvItem:LVITEM 

invoke	GetDlgItem,hDlg,IDC_SECTIONLIST
mov	hList,eax
invoke	_GetFileHeader,ImgBase
.if	eax==0
	ret
.endif
assume	eax:ptr IMAGE_FILE_HEADER 
xor	ecx,ecx	
mov	cx,[eax].NumberOfSections
mov	SecNum,ecx

assume	eax:nothing
invoke	_GetFirstSectionHeader,ImgBase
.if	eax==0
	ret
.endif
mov	edi,eax
assume	edi:ptr IMAGE_SECTION_HEADER
invoke	RtlZeroMemory,addr lvItem,sizeof lvItem
lea	ebx,lvItem
mov	dword ptr[ebx],LVIF_TEXT
;<=>mov	lvItem.mask,LVIF_TEXT
xor	ecx,ecx
.while	TRUE
	push	ecx
	mov	lvItem.iItem,ecx	;第几个..项目
	mov	lvItem.iSubItem,0	;在msdn里介绍在插入项目时,lvItem.iSubItem必须为0,本来我没有写,这样在循环中,lvItem.iSubItem变为5当然不对了,
	invoke	RtlZeroMemory,addr cName,sizeof cName
	invoke	RtlMoveMemory,addr cName,edi,8	;[ebx].Name1	拷贝节名
	lea	edx,cName
	mov	lvItem.pszText,edx
	invoke	SendMessage,hList,LVM_INSERTITEM,0,addr lvItem	;添加项目
  
	lea	edx,cBuff
	mov	lvItem.pszText,edx
	invoke	wsprintf,addr cBuff, offset szFormat_8, [edi].VirtualAddress 
	inc	lvItem.iSubItem	;mov	lvItem.iSubItem,1 
	invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem 

	invoke	wsprintf,addr cBuff, offset szFormat_8, [edi].Misc.VirtualSize 
	inc	lvItem.iSubItem	;mov	lvItem.iSubItem,2
	invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem 

	invoke	wsprintf,addr cBuff, offset szFormat_8, [edi].PointerToRawData 
	inc	lvItem.iSubItem	;mov	lvItem.iSubItem,3
	invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem

	invoke	wsprintf,addr cBuff, offset szFormat_8, [edi].SizeOfRawData 
	inc	lvItem.iSubItem	;mov	lvItem.iSubItem,4
	invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem
	
	invoke	wsprintf,addr cBuff, offset szFormat_8, [edi].Characteristics 
	inc	lvItem.iSubItem	;mov	lvItem.iSubItem,5
	invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem

	add	edi,sizeof IMAGE_SECTION_HEADER
	pop	ecx
	inc	ecx
	.if	ecx==SecNum
		.break
	.endif
.endw
assume	edi:nothing
ret
_ShowSectionHeaderInfo	endp
;------------------------------------------------------------
;------------------------------------------------------------

_ProcDlgSHT	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	invoke EndDialog, hWnd,NULL
.elseif	eax == WM_INITDIALOG
	invoke 	_InitSectionListCtrl,hWnd
	invoke	_ShowSectionHeaderInfo,hWnd
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
ret
_ProcDlgSHT	endp
;-------------------------------------------------

;-------------------------------------------------
_ProcDlgAbout	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	invoke EndDialog, hWnd,NULL
.elseif eax == WM_COMMAND
	mov      eax,wParam
	.if ax==IDOK
		invoke EndDialog, hWnd,NULL
	.endif
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
ret
_ProcDlgAbout	endp
;-------------------------------------------------
_GetExportDirectory	proc	lpbase
local	lpNTheader

invoke	_IsPEFile,lpbase
.if	eax==0
	ret
.endif
mov	lpNTheader,eax
assume	eax:ptr IMAGE_NT_HEADERS
mov	eax,[eax].OptionalHeader.DataDirectory[0].VirtualAddress
assume	eax:nothing
.if	eax==0
	ret
.endif
invoke	ImageRvaToVa,lpNTheader,lpbase,eax,NULL
ret
_GetExportDirectory	endp
;-------------------------------------------------
_GetFirstImportDesc	proc	lpbase
local	lpNTheader

invoke	_IsPEFile,lpbase
.if	eax==0
	ret
.endif
mov	lpNTheader,eax
assume	eax:ptr IMAGE_NT_HEADERS
mov	eax,[eax].OptionalHeader.DataDirectory[8].VirtualAddress
assume	eax:nothing
.if	eax==0
	ret
.endif
invoke	ImageRvaToVa,lpNTheader,lpbase,eax,NULL
ret
_GetFirstImportDesc	endp
;-------------------------------------------------
_ShowDataDirInfo	proc	hDlg
local	iRva,iSize,ifor
local	cBuff[9]:byte
invoke	_GetOptionalHeader,ImgBase
.if	eax==0
	ret
.endif
pushad
mov	ebx,eax
assume	ebx:ptr IMAGE_OPTIONAL_HEADER32
mov	iRva ,ID_RVA_START
mov	iSize,ID_SIZE_START
mov	ifor,0
.while	TRUE	
	mov	ecx,ifor
	invoke	wsprintf,addr cBuff,offset szFormat_8,[ebx].DataDirectory[ecx*8].VirtualAddress
	invoke	SetDlgItemText,hDlg,iRva,addr cBuff

	mov	ecx,ifor
	invoke	wsprintf,addr cBuff,offset szFormat_8,[ebx].DataDirectory[ecx*8].isize;DataDirectory[ecx*8].Size
	invoke	SetDlgItemText,hDlg,iSize,addr cBuff

	inc	iRva
	inc	iSize
	inc	ifor
	.if	ifor==16
		jmp @F
	.endif
.endw
@@:
assume	ebx:nothing
popad
ret
_ShowDataDirInfo	endp
;-------------------------------------------------
_ShowExportDirInfo	proc	hDlg
local	lpNTheader
local	cBuff[10]:byte

invoke	_IsPEFile,ImgBase
.if	eax==0
	ret
.endif
mov	lpNTheader,eax
invoke	_GetExportDirectory,ImgBase
.if	eax==0
	ret
.endif
mov	ebx,eax
assume	ebx:ptr IMAGE_EXPORT_DIRECTORY

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].Characteristics
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_CHARACTERISTICS,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].TimeDateStamp
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_TDS,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].nBase
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_BASE,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].nName	;dizhi
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_NAME,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].NumberOfFunctions
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_NUMOFFUNCS,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].NumberOfNames
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_NUMOFNAMES,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].AddressOfFunctions
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_ADDROFFUNCS,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].AddressOfNames
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_ADDROFNAMES,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].AddressOfNameOrdinals
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_ADDROFNAMEORD,addr cBuff
	
	invoke	ImageRvaToVa,lpNTheader,ImgBase,[ebx].nName,NULL	;quchuzifuchuan
	invoke	SetDlgItemText,hDlg,IDC_EDIT_EXPDIR_DLLNAME,eax

assume	ebx:nothing
ret
_ShowExportDirInfo	endp
;-------------------------------------------------
_InitExportListCtrl	proc	hDlg
local	lvColumn:LVCOLUMN     

invoke	RtlZeroMemory,addr lvColumn,sizeof lvColumn
lea	ebx,lvColumn
mov	dword ptr[ebx],LVCF_TEXT
or	dword ptr[ebx],LVCF_WIDTH
;<=>mov	lvColumn.mask,LVCF_TEXT | LVCF_WIDTH
add	ebx,2*4
mov	dword ptr[ebx],100
;<=>mov	lvColumn.cx,200
mov	lvColumn.pszText,offset szstrOrdinal
invoke	SendDlgItemMessage,hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 0, addr lvColumn
mov	lvColumn.pszText,offset szstrRVA  
invoke	SendDlgItemMessage,hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 1, addr lvColumn
mov	dword ptr[ebx],280
mov	lvColumn.pszText,offset szstrFunction_Name
invoke	SendDlgItemMessage,hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 2, addr lvColumn
ret
_InitExportListCtrl	endp
;-------------------------------------------------
_ShowExportFuncsInfo	proc	hDlg
local	hList,lpNTheader
local	lpFuncName,iNumOfName,pdwRvas, pdwNames,pwOrds,bIsByName
local	ifor,jfor,kfor
local	lvItem:LVITEM
local	cBuff[10]:byte

invoke	_IsPEFile,ImgBase
.if	eax==0
	ret
.endif
mov	lpNTheader,eax

xor	eax,eax
mov	ifor,eax
mov	jfor,eax
mov	kfor,eax

invoke	RtlZeroMemory,addr lvItem,sizeof lvItem
lea	ebx,lvItem
mov	dword ptr[ebx],LVIF_TEXT


invoke	_GetExportDirectory,ImgBase
.if	eax==0
	ret
.endif
mov	ebx,eax
assume	ebx:ptr IMAGE_EXPORT_DIRECTORY

invoke	ImageRvaToVa,lpNTheader,ImgBase,[ebx].AddressOfNameOrdinals,NULL
mov	pwOrds,eax	;AddressOfNameOrdinals地址,注意是字
invoke	ImageRvaToVa,lpNTheader,ImgBase,[ebx].AddressOfFunctions,NULL
.if	eax==0	;没有获取函数地址表
	ret
.endif
mov	pdwRvas,eax
invoke	ImageRvaToVa,lpNTheader,ImgBase,[ebx].AddressOfNames,NULL
mov	pdwNames,eax

invoke	GetDlgItem,hDlg,IDC_EXPORT_LIST
mov	hList,eax
invoke	SendMessage,hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT

push	[ebx].NumberOfNames
pop	iNumOfName

pushad
.while	TRUE	;i++
	.if	pdwRvas
		.while	TRUE	;j++
			mov	eax,ifor
			mov	edi,jfor
			shl	edi,1		;*2,每个元素占2字节
			mov	edx,pwOrds	;地址
			add	edx,edi
			.if	ax==word ptr [edx]	
				mov	bIsByName,TRUE	
				shl	edi,1	;接着上次的shl 现在是:edi=jfor*4
				mov	edx,pdwNames
				add	edx,edi
				invoke	ImageRvaToVa,lpNTheader,ImgBase,dword ptr [edx],NULL
				mov	lpFuncName,eax
				.break
			.endif
			mov	bIsByName,FALSE	
			inc	jfor
			mov	eax,jfor
			.if	eax==iNumOfName
				.break
			.endif
		.endw
		push	kfor
		pop	lvItem.iItem 
		mov	lvItem.iSubItem,0
		lea	edx, cBuff
		mov	lvItem.pszText,edx
		mov	eax,ifor
		add	eax,[ebx].nBase
		invoke	wsprintf,addr cBuff, offset szFormat_4,eax
		invoke	SendMessage,hList,LVM_INSERTITEM,0,addr lvItem 

		mov	eax,pdwRvas
		invoke	wsprintf,addr cBuff, offset szFormat_8,dword ptr[eax]
		inc	lvItem.iSubItem		;lvItem.iSubItem = 1;
		invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem 

		.if	bIsByName
			push	lpFuncName
			pop	lvItem.pszText 
		.else
			mov	lvItem.pszText,offset szFuncNoName	;'-'
		.endif

		inc	lvItem.iSubItem		;lvItem.iSubItem = 2;
		invoke	SendMessage,hList,LVM_SETITEM, 0, addr lvItem 

		inc	kfor
	.endif
	
	add pdwRvas,4

	inc	ifor
	mov	eax,ifor
	.if eax==[ebx].NumberOfFunctions
		.break
	.endif
.endw
assume	ebx:nothing
popad
ret
_ShowExportFuncsInfo	endp
;-------------------------------------------------
;输出表对话框回调过程...
_ProcDlgExportTable	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	invoke EndDialog, hWnd,NULL
.elseif	eax == WM_COMMAND
	mov      eax,wParam
	.if	ax==IDC_OK
		invoke EndDialog, hWnd,NULL
	.endif
.elseif	eax == WM_INITDIALOG
	invoke	_InitExportListCtrl,hWnd
	invoke	_ShowExportDirInfo,hWnd
	invoke	_ShowExportFuncsInfo,hWnd
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
ret
_ProcDlgExportTable	endp
;-------------------------------------------------
_InitImportListCtrl	proc	hDlg 
local	lvColumn:LVCOLUMN

invoke	RtlZeroMemory,addr lvColumn,sizeof lvColumn
lea	ebx,lvColumn
mov	dword ptr[ebx],LVCF_TEXT
or	dword ptr[ebx],LVCF_WIDTH
;<=>mov	lvColumn.mask,LVCF_TEXT | LVCF_WIDTH
add	ebx,2*4
mov	dword ptr[ebx],98
;<=>mov	lvColumn.cx,100

mov	lvColumn.pszText,offset szstrDLLName
invoke	SendDlgItemMessage,hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 0, addr lvColumn

mov	lvColumn.pszText,offset szstrOriginalFirstThunk
invoke	SendDlgItemMessage,hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 1, addr lvColumn

mov	lvColumn.pszText,offset szstrTimeDateStamp
invoke	SendDlgItemMessage,hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 2, addr lvColumn

mov	lvColumn.pszText,offset szstrForwarderChin
invoke	SendDlgItemMessage,hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 3, addr lvColumn

mov	lvColumn.pszText,offset szstrName2
invoke	SendDlgItemMessage,hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 4, addr lvColumn

mov	lvColumn.pszText,offset szstrFirstThunk
invoke	SendDlgItemMessage,hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 5, addr lvColumn

ret
_InitImportListCtrl	endp

;-------------------------------------------------
_InitImportFunctionsListCtrl	proc	hDlg
local	lvColumn:LVCOLUMN

invoke	RtlZeroMemory,addr lvColumn,sizeof lvColumn
lea	ebx,lvColumn
mov	dword ptr[ebx],LVCF_TEXT
or	dword ptr[ebx],LVCF_WIDTH
;<=>mov	lvColumn.mask,LVCF_TEXT | LVCF_WIDTH
add	ebx,2*4
mov	dword ptr[ebx],98
;<=>mov	lvColumn.cx,98

mov	lvColumn.pszText,offset szstrThunkRVA
invoke	SendDlgItemMessage,hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 0,addr lvColumn
	
mov	lvColumn.pszText,offset szstrThunkValue
invoke	SendDlgItemMessage,hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 1,addr lvColumn
	
mov	lvColumn.pszText,offset szstrHint
invoke	SendDlgItemMessage,hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 2,addr lvColumn
	
mov	dword ptr[ebx],300
mov	lvColumn.pszText,offset szstrFunctionName
invoke	SendDlgItemMessage,hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 3,addr lvColumn

ret
_InitImportFunctionsListCtrl	endp
;-------------------------------------------------
_ShowImportDescInfo	proc	hDlg 
local	hList,lpDllName,lpNTheader
local	lvItem:LVITEM 
local	cBuff[10]:byte

invoke	RtlZeroMemory,addr lvItem,sizeof lvItem
lea	ebx,lvItem
mov	dword ptr[ebx],LVIF_TEXT
	
invoke	GetDlgItem,hDlg,IDC_IMPORT_LIST 
mov	hList,eax
invoke	SendMessage,hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT 

invoke	_IsPEFile,ImgBase
mov	lpNTheader,eax

invoke	_GetFirstImportDesc,ImgBase
.if 	eax==0
	ret
.endif
mov	ebx,eax
assume	ebx:ptr IMAGE_IMPORT_DESCRIPTOR
push	edi
xor	edi,edi
.while	[ebx].FirstThunk
	mov	lvItem.iItem,edi
	mov	lvItem.iSubItem,0
	invoke	ImageRvaToVa,lpNTheader,ImgBase,[ebx].Name1,NULL
	mov	lpDllName,eax
	mov	lvItem.pszText,eax
	invoke	SendMessage,hList,LVM_INSERTITEM,0,addr lvItem
	
	lea	edx,cBuff
	mov	lvItem.pszText,edx
	invoke	wsprintf,addr cBuff,offset szFormat_8, [ebx].OriginalFirstThunk 
	inc	lvItem.iSubItem		;=1
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	invoke	wsprintf,addr cBuff,offset szFormat_8, [ebx].TimeDateStamp
	inc	lvItem.iSubItem		;=2
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	invoke	wsprintf,addr cBuff,offset szFormat_8, [ebx].ForwarderChain
	inc	lvItem.iSubItem		;=3
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	invoke	wsprintf,addr cBuff,offset szFormat_8, [ebx].Name1
	inc	lvItem.iSubItem		;=4
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	invoke	wsprintf,addr cBuff,offset szFormat_8, [ebx].FirstThunk 
	inc	lvItem.iSubItem		;=5
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	inc	edi
	add	ebx,sizeof IMAGE_IMPORT_DESCRIPTOR
.endw
pop	edi
assume	ebx:nothing
ret
_ShowImportDescInfo	endp
;-------------------------------------------------
_ShowImportFuncsByDllIndex	proc	hDlg,index
local	hList,lpFuncName,pdwRVA,pdwThunk,lpNTheader
local	lvItem:LVITEM       
local	cBuff[30]:byte,cOrd[30]:byte,cMemAddr[30]:byte 
;初始化lvItem变量,lvItem.mask=LVIF_TEXT
invoke	RtlZeroMemory,addr lvItem,sizeof lvItem
lea	ebx,lvItem
mov	dword ptr[ebx],LVIF_TEXT
;获取列表句柄,并发送消息:行全选风格,清除所有项目
invoke	GetDlgItem,hDlg,IDC_IMPORTFUNCTIONS_LIST
mov	hList,eax
invoke	SendMessage,hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT 
invoke	SendMessage,hList,LVM_DELETEALLITEMS ,0,0
;获取NT头...
invoke	_IsPEFile,ImgBase
mov	lpNTheader,eax
;获取输入表地址...和ebx关联
invoke	_GetFirstImportDesc,ImgBase
.if 	eax==0
	ret
.endif
mov	ebx,eax
assume	ebx:ptr IMAGE_IMPORT_DESCRIPTOR
;输入表项与选择的行号有关,ebx:=ebx+index*(sizeof IMAGE_IMPORT_DESCRIPTOR)
mov	eax,sizeof IMAGE_IMPORT_DESCRIPTOR
mov	ecx,index
mul	ecx
add	ebx,eax
;获取IMAGE_THUNK_DATA表地址
push	[ebx].OriginalFirstThunk
pop	pdwRVA
invoke	ImageRvaToVa,lpNTheader,ImgBase,pdwRVA,NULL
.if 	eax==0
	ret
.endif
mov	pdwThunk,eax
mov	eax,[eax]
xor	edi,edi
.while	eax
	mov	lvItem.iItem,edi	;edi作为项目数
	mov	lvItem.iSubItem,0
	lea	edx,cBuff
	mov	lvItem.pszText,edx
	invoke	wsprintf,addr cBuff,offset szFormat_8,pdwRVA	;IMAGE_THUNK_DATA表地址 THUNK_RVA
	invoke	SendMessage,hList,LVM_INSERTITEM,0,addr lvItem
	
	inc	lvItem.iSubItem	;=1					
	mov	edx,pdwThunk
	invoke	wsprintf,addr cBuff,offset szFormat_8,dword ptr [edx]	;THUNK_VALUE
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem
	
	mov	edx,pdwThunk
	mov	eax,dword ptr [edx]	;THUNK_VALUE的值含义...
	test	eax,80000000h
	.if	!ZERO?		;按序号导入...
		and	eax,0FFFFH
		invoke	wsprintf,addr cOrd,offset szOrdFormat_8,eax	;低16位为序号...
		lea	eax,cOrd
		mov	lpFuncName,eax
		invoke	lstrcpy,addr cBuff,offset szFuncNoName	;名字无...
	.else				;地址...
		mov	edx,pdwThunk
		invoke	ImageRvaToVa,lpNTheader,ImgBase,dword ptr [edx],NULL
		.if	eax!=0
			mov	esi,eax
			assume	esi:ptr IMAGE_IMPORT_BY_NAME
			movzx	eax,[esi].Hint 
			invoke	wsprintf,addr cBuff,offset szFormat_4,eax	;出错原因,参数[esi].Hint是2字节,必须是4字节的
			lea	eax,[esi].Name1
			mov	lpFuncName,eax
			assume	esi:nothing
		.else
			mov	edx,pdwThunk
			invoke	lstrcpy,addr cBuff,offset szFuncNoName
			invoke	wsprintf,addr cMemAddr,offset szMemAddrFormat_8,dword ptr [edx]
			lea	eax,cMemAddr
			mov	lpFuncName,eax
		.endif

	.endif	
	lea	eax,cBuff 
	mov	lvItem.pszText,eax
	inc	lvItem.iSubItem	;=2
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	push	lpFuncName
	pop	lvItem.pszText
	inc	lvItem.iSubItem	;=3
	invoke	SendMessage,hList,LVM_SETITEM,0,addr lvItem

	inc	edi
	add	pdwRVA,4
	add	pdwThunk,4
	mov	eax,pdwThunk
	mov	eax,[eax]
.endw

assume	ebx:nothing
ret
_ShowImportFuncsByDllIndex	endp
;-------------------------------------------------
;输入表对话框回调过程...
_ProcDlgDImportTable	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	invoke EndDialog, hWnd,NULL
.elseif	eax == WM_INITDIALOG
	invoke	_InitImportListCtrl, hWnd
	invoke	_InitImportFunctionsListCtrl, hWnd
	invoke	_ShowImportDescInfo, hWnd
	
.elseif	eax==WM_NOTIFY
	mov	eax,wParam
	mov	ebx,lParam
	assume	ebx:ptr NMHDR
	.if	ax==IDC_IMPORT_LIST
		;invoke	MessageBox,NULL,NULL,NULL,NULL
		.if	[ebx].code==LVN_ITEMCHANGED
			mov	ebx,lParam
			assume	ebx:ptr NMLISTVIEW
				;invoke	MessageBox,NULL,NULL,NULL,NULL
				invoke	_ShowImportFuncsByDllIndex, hWnd,[ebx].iItem
			assume	ebx:nothing
		.endif
	.endif	
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
ret
_ProcDlgDImportTable	endp
;-------------------------------------------------
;-------------------------------------------------
;获取目录表信息的对话框...回调过程
_ProcDlgDATADIR	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	invoke EndDialog, hWnd,NULL
.elseif	eax == WM_COMMAND
	mov      eax,wParam
	.if	ax==IDC_OK
		invoke EndDialog, hWnd,NULL
	.elseif	ax==IDC_SHOWEXPORT_BTN
		invoke     DialogBoxParam,hInstance,IDD_EXPORT_DLG,hWnd,offset _ProcDlgExportTable,NULL
	.elseif	ax==IDC_SHOWIMPORT_BTN
		invoke     DialogBoxParam,hInstance,IDD_IMPORT_DLG,hWnd,offset _ProcDlgDImportTable,NULL
	.endif
	
.elseif	eax == WM_INITDIALOG
	invoke	_ShowDataDirInfo,hWnd
.else
	mov      eax,FALSE
	ret
.endif                 
mov	eax,TRUE
ret
_ProcDlgDATADIR	endp
;-------------------------------------------------
;打开PE文件,参数:hWinMain为主窗体句柄
;lpfilebuff为文件名缓冲区
;lpext文件默认扩展名
;lpfilter过滤器
_OpenFile	proc lpfilebuff,lpfilter,lpext
local   @stOF:OPENFILENAME

invoke  RtlZeroMemory,addr @stOF,sizeof @stOF
mov     @stOF.lStructSize,sizeof @stOF
push    hWinMain
pop     @stOF.hwndOwner
push	lpfilter
pop     @stOF.lpstrFilter
push	lpfilebuff
pop     @stOF.lpstrFile
mov     @stOF.nMaxFile,MAX_PATH
mov     @stOF.Flags,OFN_FILEMUSTEXIST or OFN_PATHMUSTEXIST
push	lpext
pop     @stOF.lpstrDefExt
invoke	GetOpenFileName,addr @stOF
ret
_OpenFile	endp
;------------------------------------------------------------
;------------------------------------------------------------
_UnLoadFile	proc
.if	ImgBase
	invoke	UnmapViewOfFile,ImgBase
.endif
.if	hMapping
	invoke	CloseHandle,hMapping
.endif
.if	hFile
	invoke	CloseHandle,hFile
.endif
ret
_UnLoadFile	endp
;------------------------------------------------------------
_LoadPeFile	proc	lppefile
invoke	CreateFile,lppefile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
.if	eax==0
	invoke	MessageBox,NULL,offset szerrcreatefile,NULL,0	;CTEXT("打开文件失败!")
	ret
.endif
mov	hFile,eax
invoke	CreateFileMapping,eax,NULL,PAGE_READONLY,0,0,NULL
.if	eax==0
	invoke	CloseHandle,hFile
	invoke	MessageBox,NULL,offset szerrcreatemap,NULL,0		;CTEXT("创建文件映像失败!")
	ret
.endif
mov	hMapping,eax
invoke	MapViewOfFile,eax,FILE_MAP_READ,0,0,0
.if	eax==0
	invoke	CloseHandle,hMapping
	invoke	CloseHandle,hFile
	invoke	MessageBox,NULL,offset szerrmapview,NULL,0	;CTEXT("创建文件映像失败!")
	ret
.endif
mov	ImgBase,eax
ret
_LoadPeFile	endp

;------------------------------------------------------------
_GetNtHeader	proc	lpbase
invoke	_IsPEFile,lpbase
ret
_GetNtHeader	endp
;------------------------------------------------------------
_ShowFileHeaderInfo	proc hWnd
local		cBuff[10]:byte

invoke	_GetFileHeader,ImgBase
.if	eax==0
	invoke	MessageBox,NULL,offset szerrGetFileHdr,NULL,0	;CTEXT("获取文件头失败!")
.elseif
	mov	ebx,eax
	assume	ebx:ptr IMAGE_FILE_HEADER

	invoke	wsprintf,addr cBuff, offset szFormat_4, [ebx].Machine
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_MACHINE,addr cBuff
	 
	invoke	wsprintf,addr cBuff, offset szFormat_4, [ebx].NumberOfSections
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_NUMOFSECTIONS,addr cBuff
	 
	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].TimeDateStamp
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_TDS,addr cBuff
	 
	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].PointerToSymbolTable
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_PTSYMBOL,addr cBuff
	 
	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].NumberOfSymbols
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_NUMOFSYM,addr cBuff
	
	invoke	wsprintf,addr cBuff, offset szFormat_4, [ebx].SizeOfOptionalHeader
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_SIZEOFOH,addr cBuff
	 
	invoke	wsprintf,addr cBuff, offset szFormat_4, [ebx].Characteristics
	invoke	SetDlgItemText,hWnd,IDC_EDIT_FH_CHARACTERISTICS,addr cBuff
	 
	assume	ebx:nothing
.endif
ret
_ShowFileHeaderInfo	endp
;------------------------------------------------------------
_ShowOptionHeaderInfo	proc	hWnd
local		cBuff[10]:byte

invoke _GetOptionalHeader,ImgBase
.if	eax==0
	invoke	MessageBox,NULL,offset szErrGetOpHdr,NULL,0	; CTEXT("??è??é??í·ê§°ü!")
.elseif
	mov	ebx,eax
	assume	ebx:ptr IMAGE_OPTIONAL_HEADER

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].AddressOfEntryPoint
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_EP,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].ImageBase
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_IMAGEBASE,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].BaseOfCode
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_CODEBASE,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].BaseOfData
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_DATABASE,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].SizeOfImage
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_IMAGESIZE,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].SizeOfHeaders
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_HEADERSIZE,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].SectionAlignment
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_SECTIONALIGN,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].FileAlignment
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_FILEALIGN,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_8, [ebx].Subsystem
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_SUBSYSTEM,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_4, [ebx].CheckSum
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_CHECKSUM,addr cBuff

	invoke	wsprintf,addr cBuff, offset szFormat_4, [ebx].DllCharacteristics
	invoke	SetDlgItemText,hWnd,IDC_EDIT_OH_DLLFLAGS,addr cBuff

	assume	ebx:nothing
.endif
ret
_ShowOptionHeaderInfo	endp
;-------------------------------------------------
_ProcDlgMain	proc       uses ebx edi esi hWnd,wMsg,wParam,lParam
mov	eax,wMsg
.if	eax == WM_CLOSE
	;invoke	_Exit
	invoke EndDialog, hWnd,NULL
.elseif	eax == WM_INITDIALOG
	push	hWnd
	pop	hWinMain
	xor	eax,eax
	mov	hFile,eax
	mov	hMapping,eax
	mov	ImgBase	,eax
.elseif eax == WM_COMMAND  
	mov      eax,wParam
	.if	ax==IDC_ABOUTBTN	;关于
		invoke     DialogBoxParam,hInstance,IDD_ABOUTBOX,hWnd,offset _ProcDlgAbout,NULL
		;invoke	MessageBox,NULL,NULL,NULL,0
	.elseif ax==IDC_EXIT
		invoke EndDialog, hWnd,NULL
	.elseif ax==IDC_OPEN_BTN	;打开并加载PE文件
		invoke	_OpenFile,addr szfilename,offset szfilefilter,offset szfileext
		invoke	SendDlgItemMessage,hWnd,IDC_FILEPATH_EDIT,WM_SETTEXT,MAX_PATH,addr szfilename
		invoke	_UnLoadFile
		invoke	_LoadPeFile,addr szfilename
		invoke	_IsPEFile,ImgBase
		.if	eax==0
			invoke	MessageBox,NULL,offset szErrNotPE,NULL,0	;CTEXT("文件似乎不是合法的PE文件!")
		.else 
			invoke	_ShowFileHeaderInfo,hWnd
			invoke	_ShowOptionHeaderInfo,hWnd
		.endif
	.elseif	ax==IDC_SHOWSHT		;节表dlg
		invoke     DialogBoxParam,hInstance,IDD_SHT_DLG,hWnd,offset _ProcDlgSHT,NULL
	.elseif ax==IDC_SHOWDATADIR	;数据目录表dlg
		invoke     DialogBoxParam,hInstance,IDD_DATADIR_DLG,hWnd,offset _ProcDlgDATADIR,NULL
	.endif  
.else     
	mov      eax,FALSE           
	ret    
.endif        
mov	eax,TRUE
ret
_ProcDlgMain  endp
;-------------------------------------------------
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
	invoke     GetModuleHandle,NULL
	mov        hInstance,eax
	invoke     DialogBoxParam,hInstance,IDD_MAIN_DLG,\
			NULL,offset _ProcDlgMain,NULL
	invoke     ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
end     start


// ApiTracing.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ApiTracing.h"
#include "PEFuncs.h"
#include <algorithm>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void start();

CString GetStartPath(HMODULE h)
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(h,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	_tcsrchr(szTemp,'\\')[1]=0;
	return szTemp;
}

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CApiTracingApp

BEGIN_MESSAGE_MAP(CApiTracingApp, CWinApp)
END_MESSAGE_MAP()


// CApiTracingApp 构造

CApiTracingApp::CApiTracingApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

/************************************************************************/
/* 全局变量																*/
/************************************************************************/

CApiTracingApp theApp;	// 唯一的一个 CApiTracingApp 对象

HWND hwmain;			//保存OD主窗口句柄	
CString strAppPath;
CString strCfgFile;
int nStartAddr;
int nEndAddr;
ulong nLastIp = 0;
/************************************************************************/
/* 全局变量                                                             */
/************************************************************************/

/*
一些不能hook的函数表
user32:gapfnScSendMessage
kenel32:SetLastError OutputDebugStringA(这个我们用来打印输出的)
*/

//完全模式：对dll导出的所有函数hook，导出转向的函数不处理
void SetExportApiHook(HMODULE hModule)
{
	if ( hModule==NULL ){
		return;
	}

	static int nCount = 0;
	PBYTE lpImageBase=(PBYTE)hModule;

	//查找到模块基址后，解析其导出表获取指定函数地址
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpImageBase;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (PBYTE)pDosHeader + pDosHeader->e_lfanew );
	DWORD dwDataStartRVA=pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
	DWORD dwDataEndRVA=dwDataStartRVA+pNtHeader->OptionalHeader.DataDirectory[0].Size;
	if ( dwDataStartRVA==0 ){
		return;
	}

	PIMAGE_EXPORT_DIRECTORY pExportDir=(PIMAGE_EXPORT_DIRECTORY)((LPBYTE)lpImageBase+dwDataStartRVA);

	PULONG pNameTableBase = (PULONG)(lpImageBase + pExportDir->AddressOfNames);
	PUSHORT pNameOrdinalTableBase = (PUSHORT)(lpImageBase + pExportDir->AddressOfNameOrdinals);
	PULONG pAddressTableBase = (PULONG)(lpImageBase + pExportDir->AddressOfFunctions);


	for ( DWORD i=0; i<pExportDir->NumberOfNames; ++i ){

		char *pFuncName = (char*)(lpImageBase+pNameTableBase[i]);
		DWORD dwOridinalName = pNameOrdinalTableBase[i];
		if( dwOridinalName > pExportDir->NumberOfFunctions ){
			continue;	//获取的函数序号错误
		}
		DWORD dwFuncAddrRva = pAddressTableBase[dwOridinalName];

		//判断这个地址是不是在导出表所在的节中，如果是说明是转向函数
		if ( dwFuncAddrRva>=dwDataStartRVA && dwFuncAddrRva<dwDataEndRVA ){
			dwFuncAddrRva = 0;
		}else{
			nCount++;

			//下断
			Addtolist(0,1,"函数：%s, 地址：%p",pFuncName,lpImageBase+dwFuncAddrRva);
			Setbreakpoint((DWORD)(lpImageBase+dwFuncAddrRva),TY_ACTIVE|TY_SET,0);
		}
	}//end for

	Addtolist(0,1,"导出函数个数：%d",nCount);
}

//完全模式：对dll导出的所有函数hook，导出转向的函数不处理
void SetExportApiHook(LPCTSTR lpszModuleName)
{
	//如果代码中没有GetModuleHandleA函数，则会crash
	HMODULE hModule = GetModuleHandleA( lpszModuleName );
	if ( hModule==NULL ){
		hModule = LoadLibraryA( lpszModuleName );
	}
	SetExportApiHook(hModule);
}

// CApiTracingApp 初始化
BOOL CApiTracingApp::InitInstance()
{
	CWinApp::InitInstance();

	strAppPath = GetStartPath(m_hInstance);
	strCfgFile = strAppPath + "ApiTracing.ini";

	return TRUE;
}


//OD插件的必要元素,用来显示在OD的插件菜单里面,表示插件名称（32字节长）
extc int _export cdecl ODBG_Plugindata(char *shortname) 
{
	strcpy_s(shortname,32,"ApiTracing");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) 
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	hwmain=hw;


	//在log窗口显示插件名称和版本，以及版权信息
	Addtolist(0,0,"ApiTracing plugin v0.1");
	Addtolist(0,-1," Copyright (C) 2009 sing");
	return 0;
}

//插件的子菜单的显示
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin)//调用该回调函数的窗口编号 
	{ 
	case PM_MAIN:
		strcpy_s(data,4096,"0 &ApiTracing");
		return 1;
	case PM_DISASM: //子菜单显示在主窗口的插件菜单中,而PM_DISASM表示子菜单显示在反汇编窗口中
		strcpy_s(data,4096,"0 &ApiTracing");
		return 1;
	default: 
		return 0;
	}
}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	if ( origin!=PM_DISASM && origin!=PM_MAIN ) //是否在反汇编窗口或者主窗口中 
		return;

	t_dump *pDump = (t_dump *)item;
	if (pDump==NULL || pDump->size==0) //Window empty, don't add
		return;

	//没有选择任何文本
	if ( pDump->sel1 <= pDump->sel0 )
		return;

	switch( action )
	{
	case 0:	//第1个菜单
		{
			int nRet = ::MessageBox(hwmain,
				"YES:跟踪选择代码中的API调用\n\n"
				"NO:从文件中读取结束位置\n\n"
				"CANCEL:暂不开始"
				,"请选择开始方式",MB_YESNOCANCEL);
			if ( nRet==IDYES ){
				nStartAddr = pDump->sel0;	//选择的文本起始地址
				nEndAddr = pDump->sel1;		//选择的文本结束地址
				start();
			}else if ( nRet==IDNO ){
				TCHAR szTemp[MAX_PATH];
				GetPrivateProfileString("main","addr1","",szTemp,MAX_PATH,strCfgFile);
				nStartAddr = strtol(szTemp,NULL,16);
				GetPrivateProfileString("main","addr2","",szTemp,MAX_PATH,strCfgFile);
				nEndAddr = strtol(szTemp,NULL,16);
				start();
			}
		}
		break;
	case 1:
	case 2:
	case 3:
		break;
	case 4:
		AfxMessageBox("啥玩意儿没有!");
		break;
	default:
	    break;
	}
}

extc int ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item)
{
	if (origin==PM_DISASM)
	{
		if (ctrl==1&&key==90)
		{
			ODBG_Pluginaction(PM_DISASM,0,item);
// 			CString str;
// 			str.Format("%d",key);
// 			MessageBox(NULL,str,NULL,NULL);
			return 1;
		}
	}
	return 0;
}

extc void _export cdecl ODBG_Pluginreset(void) 
{
	nLastIp = 0;
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void) 
{
}

#if 0
void start()
{
	nLastIp = 1;

	//先获取当前代码所在的模块
	t_module* pModule = Findmodule(nStartAddr);
	if ( pModule==NULL ){
		::MessageBox(0,"未能找到当前代码所在模块!",NULL,0);
		return;
	}

	//基址
	PBYTE pBaseAddr = (PBYTE)NULL + pModule->base;
	IMAGE_DOS_HEADER stDosHdr;
	Readmemory(&stDosHdr,pModule->base,sizeof(stDosHdr),0);

	IMAGE_NT_HEADERS stNtHdr;
	Readmemory(&stNtHdr,pModule->base+stDosHdr.e_lfanew,sizeof(stNtHdr),0);

	PIMAGE_IMPORT_DESCRIPTOR pIAT = (PIMAGE_IMPORT_DESCRIPTOR)(pBaseAddr + stNtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	DWORD dwIATSize = stNtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;


	//把输入表整个块读进来分析
	PBYTE pBuff = new BYTE[dwIATSize];
	if ( pBuff==NULL ){
		::MessageBox(0,"分配内存失败!",NULL,0);
		return;
	}

	Readmemory(pBuff,(DWORD)pIAT,dwIATSize,0);

	//分析
	for ( DWORD i=0; i<dwIATSize; i+=4 ){
		DWORD dwFuncAddr = *((PDWORD)(pBuff+i));
		if ( dwFuncAddr==0 ){
			continue;
		}
		//下断
		Setbreakpoint(dwFuncAddr,TY_ACTIVE|TY_SET,0);
	}
	//分析结束

	//释放内存
	if ( pBuff ){
		delete []pBuff;
	}

	Addtolist(0,1,"ApiTracing 执行开始");
	Go(0,nEndAddr,STEP_RUN,TRUE,0);
}

#else

#define GETTHUNK(pImportDesc) ((DWORD)                          \
	((PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->OriginalFirstThunk ?                      \
	(PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->OriginalFirstThunk:(PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->FirstThunk \
	))

void start()
{
	nLastIp = 1;

	SetExportApiHook("kernel32.dll");
	SetExportApiHook("user32.dll");
	SetExportApiHook("shell32.dll");
	SetExportApiHook("shell32.dll");
	SetExportApiHook("shlwapi.dll");
	SetExportApiHook("gdi32.dll");
	SetExportApiHook("advapi32.dll");
	SetExportApiHook("urlmon.dll");
	SetExportApiHook("wininet.dll");
	SetExportApiHook("ws2_32.dll");

	Addtolist(0,1,"ApiTracing 执行开始");
	Go(0,nEndAddr,STEP_RUN,TRUE,0);
}
#endif

//int Analyze(ulong ip)
//{
//	t_disasm stDisasm={0};	//保存反汇编后的信息
//	uchar *pDecode=NULL;	//保存指令码的起始地址
//	ulong nOpcodeLen;		//保存一条指令的长度，按字节计算
//	BYTE buff[MAXCMDSIZE]={0};	//一段缓冲区
//	DWORD RetLen=0;			//
//	int nAddr=0;
//	int nRunFlag = STEP_OVER;
//
//	char szComment[TEXTLEN];
//	Findname(ip,NM_COMMENT,szComment);
//	Addtolist(0,1,szComment);
//	return STEP_RUN;
//
//	//从指定地址开始读取解码数据，成功返回解码数据，psize＝数据大小；未成功则返回NULL,psize＝0
//	Readmemory(buff,ip,MAXCMDSIZE,MM_RESILENT);
//
//	pDecode = Finddecode(ip,&RetLen);
//
//	//汇编指令到stDisasm结构
//	nOpcodeLen = Disasm(buff,100,ip,pDecode,&stDisasm,DISASM_RTRACE,0);
//
//	if ( (stDisasm.cmdtype&C_TYPEMASK)==C_CAL ){
//		if ( stDisasm.jmpaddr ){
//			nAddr=stDisasm.jmpaddr;
//		}else if ( stDisasm.jmpconst ){
//			nAddr=stDisasm.jmpconst;
//		}else if ( stDisasm.adrconst ){
//			nAddr=stDisasm.adrconst;
//		}
//
//		if ( nAddr>(ulong)0x70000000 ){
//			Addtolist(0,1,"%08X call %08X：%s",ip, nAddr,stDisasm.comment);
//			nRunFlag = STEP_OVER;
//		}else{
//			nRunFlag = STEP_IN;
//
//		}
//
//	}
//
//	return nRunFlag;
//}

extc int _export cdecl ODBG_Paused(int reason, t_reg *reg) 
{
	int nRunFlag = STEP_IN;

	if ( reg!=NULL && nLastIp ){
		if ( reg->ip!=nEndAddr ){
			//nRunFlag = Analyze(reg->ip);
			nLastIp = reg->ip;
			Setbreakpoint(reg->ip,TY_DISABLED,0);
			Go(0,nEndAddr,STEP_RUN,TRUE,0);
		}else{
			nLastIp = 0;
			Addtolist(0,1,"ApiTracing 执行完毕");
			return 0;
		}
	}

	return 1;
}


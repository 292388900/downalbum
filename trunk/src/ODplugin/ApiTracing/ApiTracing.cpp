// ApiTracing.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ApiTracing.h"
#include <algorithm>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	strcpy(shortname,"ApiTracing");
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
		strcpy(data,"0 &ApiTracing");
		return 1;
	case PM_DISASM: //子菜单显示在主窗口的插件菜单中,而PM_DISASM表示子菜单显示在反汇编窗口中
		strcpy(data,"0 &ApiTracing");
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
		if ( ::MessageBox(hwmain,"请设置好起始地址和终止地址后继续","准备开始吗",MB_YESNO)==IDYES ){
			TCHAR szTemp[MAX_PATH];
			GetPrivateProfileString("main","addr1","",szTemp,MAX_PATH,strCfgFile);
			nStartAddr = strtol(szTemp,NULL,16);
			GetPrivateProfileString("main","addr2","",szTemp,MAX_PATH,strCfgFile);
			nEndAddr = strtol(szTemp,NULL,16);
			nLastIp = 1;
			Go(0,nEndAddr,STEP_IN,TRUE,0);
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

int Analyze(ulong ip)
{
	t_disasm stDisasm={0};	//保存反汇编后的信息
	uchar *pDecode=NULL;	//保存指令码的起始地址
	ulong nOpcodeLen;		//保存一条指令的长度，按字节计算
	BYTE buff[MAXCMDSIZE]={0};	//一段缓冲区
	DWORD RetLen=0;			//
	int nAddr=0;
	int nRunFlag = STEP_IN;


	//从指定地址开始读取解码数据，成功返回解码数据，psize＝数据大小；未成功则返回NULL,psize＝0
	Readmemory(buff,ip,MAXCMDSIZE,MM_RESILENT);

	pDecode = Finddecode(ip,&RetLen);

	//汇编指令到stDisasm结构
	nOpcodeLen = Disasm(buff,100,ip,pDecode,&stDisasm,DISASM_RTRACE,0);

	if ( (stDisasm.cmdtype&C_TYPEMASK)==C_CAL ){
		if ( stDisasm.jmpaddr ){
			nAddr=stDisasm.jmpaddr;
		}else if ( stDisasm.jmpconst ){
			nAddr=stDisasm.jmpconst;
		}else if ( stDisasm.adrconst ){
			nAddr=stDisasm.adrconst;
		}

		if ( nAddr>(ulong)0x70000000 ){
			Addtolist(0,1,"%08X call %08X：%s",ip, nAddr,stDisasm.comment);
			nRunFlag = STEP_OVER;
		}else{
			nRunFlag = STEP_IN;
		}

	}

	return nRunFlag;
}

extc int _export cdecl ODBG_Paused(int reason, t_reg *reg) 
{
	int nRunFlag = STEP_IN;

	if ( reg!=NULL && nLastIp ){
		if ( reg->ip!=nEndAddr ){
			nRunFlag = Analyze(reg->ip);
			nLastIp = reg->ip;
			Go(0,nEndAddr,nRunFlag,TRUE,0);
		}else{
			nLastIp = 0;
			Addtolist(0,1,"ApiTracing 执行完毕");
			return 0;
		}
	}

	return 1;
}


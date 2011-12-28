// ApiTracing.cpp : ���� DLL �ĳ�ʼ�����̡�
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
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CApiTracingApp

BEGIN_MESSAGE_MAP(CApiTracingApp, CWinApp)
END_MESSAGE_MAP()


// CApiTracingApp ����

CApiTracingApp::CApiTracingApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

/************************************************************************/
/* ȫ�ֱ���																*/
/************************************************************************/

CApiTracingApp theApp;	// Ψһ��һ�� CApiTracingApp ����

HWND hwmain;			//����OD�����ھ��	
CString strAppPath;
CString strCfgFile;
int nStartAddr;
int nEndAddr;
ulong nLastIp = 0;
/************************************************************************/
/* ȫ�ֱ���                                                             */
/************************************************************************/

/*
һЩ����hook�ĺ�����
user32:gapfnScSendMessage
kenel32:SetLastError OutputDebugStringA(�������������ӡ�����)
*/

//��ȫģʽ����dll���������к���hook������ת��ĺ���������
void SetExportApiHook(HMODULE hModule)
{
	if ( hModule==NULL ){
		return;
	}

	static int nCount = 0;
	PBYTE lpImageBase=(PBYTE)hModule;

	//���ҵ�ģ���ַ�󣬽����䵼�����ȡָ��������ַ
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
			continue;	//��ȡ�ĺ�����Ŵ���
		}
		DWORD dwFuncAddrRva = pAddressTableBase[dwOridinalName];

		//�ж������ַ�ǲ����ڵ��������ڵĽ��У������˵����ת����
		if ( dwFuncAddrRva>=dwDataStartRVA && dwFuncAddrRva<dwDataEndRVA ){
			dwFuncAddrRva = 0;
		}else{
			nCount++;

			//�¶�
			Addtolist(0,1,"������%s, ��ַ��%p",pFuncName,lpImageBase+dwFuncAddrRva);
			Setbreakpoint((DWORD)(lpImageBase+dwFuncAddrRva),TY_ACTIVE|TY_SET,0);
		}
	}//end for

	Addtolist(0,1,"��������������%d",nCount);
}

//��ȫģʽ����dll���������к���hook������ת��ĺ���������
void SetExportApiHook(LPCTSTR lpszModuleName)
{
	//���������û��GetModuleHandleA���������crash
	HMODULE hModule = GetModuleHandleA( lpszModuleName );
	if ( hModule==NULL ){
		hModule = LoadLibraryA( lpszModuleName );
	}
	SetExportApiHook(hModule);
}

// CApiTracingApp ��ʼ��
BOOL CApiTracingApp::InitInstance()
{
	CWinApp::InitInstance();

	strAppPath = GetStartPath(m_hInstance);
	strCfgFile = strAppPath + "ApiTracing.ini";

	return TRUE;
}


//OD����ı�ҪԪ��,������ʾ��OD�Ĳ���˵�����,��ʾ������ƣ�32�ֽڳ���
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


	//��log������ʾ������ƺͰ汾���Լ���Ȩ��Ϣ
	Addtolist(0,0,"ApiTracing plugin v0.1");
	Addtolist(0,-1," Copyright (C) 2009 sing");
	return 0;
}

//������Ӳ˵�����ʾ
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin)//���øûص������Ĵ��ڱ�� 
	{ 
	case PM_MAIN:
		strcpy_s(data,4096,"0 &ApiTracing");
		return 1;
	case PM_DISASM: //�Ӳ˵���ʾ�������ڵĲ���˵���,��PM_DISASM��ʾ�Ӳ˵���ʾ�ڷ���ര����
		strcpy_s(data,4096,"0 &ApiTracing");
		return 1;
	default: 
		return 0;
	}
}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	if ( origin!=PM_DISASM && origin!=PM_MAIN ) //�Ƿ��ڷ���ര�ڻ����������� 
		return;

	t_dump *pDump = (t_dump *)item;
	if (pDump==NULL || pDump->size==0) //Window empty, don't add
		return;

	//û��ѡ���κ��ı�
	if ( pDump->sel1 <= pDump->sel0 )
		return;

	switch( action )
	{
	case 0:	//��1���˵�
		{
			int nRet = ::MessageBox(hwmain,
				"YES:����ѡ������е�API����\n\n"
				"NO:���ļ��ж�ȡ����λ��\n\n"
				"CANCEL:�ݲ���ʼ"
				,"��ѡ��ʼ��ʽ",MB_YESNOCANCEL);
			if ( nRet==IDYES ){
				nStartAddr = pDump->sel0;	//ѡ����ı���ʼ��ַ
				nEndAddr = pDump->sel1;		//ѡ����ı�������ַ
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
		AfxMessageBox("ɶ�����û��!");
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

	//�Ȼ�ȡ��ǰ�������ڵ�ģ��
	t_module* pModule = Findmodule(nStartAddr);
	if ( pModule==NULL ){
		::MessageBox(0,"δ���ҵ���ǰ��������ģ��!",NULL,0);
		return;
	}

	//��ַ
	PBYTE pBaseAddr = (PBYTE)NULL + pModule->base;
	IMAGE_DOS_HEADER stDosHdr;
	Readmemory(&stDosHdr,pModule->base,sizeof(stDosHdr),0);

	IMAGE_NT_HEADERS stNtHdr;
	Readmemory(&stNtHdr,pModule->base+stDosHdr.e_lfanew,sizeof(stNtHdr),0);

	PIMAGE_IMPORT_DESCRIPTOR pIAT = (PIMAGE_IMPORT_DESCRIPTOR)(pBaseAddr + stNtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	DWORD dwIATSize = stNtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;


	//����������������������
	PBYTE pBuff = new BYTE[dwIATSize];
	if ( pBuff==NULL ){
		::MessageBox(0,"�����ڴ�ʧ��!",NULL,0);
		return;
	}

	Readmemory(pBuff,(DWORD)pIAT,dwIATSize,0);

	//����
	for ( DWORD i=0; i<dwIATSize; i+=4 ){
		DWORD dwFuncAddr = *((PDWORD)(pBuff+i));
		if ( dwFuncAddr==0 ){
			continue;
		}
		//�¶�
		Setbreakpoint(dwFuncAddr,TY_ACTIVE|TY_SET,0);
	}
	//��������

	//�ͷ��ڴ�
	if ( pBuff ){
		delete []pBuff;
	}

	Addtolist(0,1,"ApiTracing ִ�п�ʼ");
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

	Addtolist(0,1,"ApiTracing ִ�п�ʼ");
	Go(0,nEndAddr,STEP_RUN,TRUE,0);
}
#endif

//int Analyze(ulong ip)
//{
//	t_disasm stDisasm={0};	//���淴�������Ϣ
//	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
//	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
//	BYTE buff[MAXCMDSIZE]={0};	//һ�λ�����
//	DWORD RetLen=0;			//
//	int nAddr=0;
//	int nRunFlag = STEP_OVER;
//
//	char szComment[TEXTLEN];
//	Findname(ip,NM_COMMENT,szComment);
//	Addtolist(0,1,szComment);
//	return STEP_RUN;
//
//	//��ָ����ַ��ʼ��ȡ�������ݣ��ɹ����ؽ������ݣ�psize�����ݴ�С��δ�ɹ��򷵻�NULL,psize��0
//	Readmemory(buff,ip,MAXCMDSIZE,MM_RESILENT);
//
//	pDecode = Finddecode(ip,&RetLen);
//
//	//���ָ�stDisasm�ṹ
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
//			Addtolist(0,1,"%08X call %08X��%s",ip, nAddr,stDisasm.comment);
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
			Addtolist(0,1,"ApiTracing ִ�����");
			return 0;
		}
	}

	return 1;
}


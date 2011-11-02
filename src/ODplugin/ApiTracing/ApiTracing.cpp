// ApiTracing.cpp : ���� DLL �ĳ�ʼ�����̡�
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
	strcpy(shortname,"ApiTracing");
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
		strcpy(data,"0 &ApiTracing");
		return 1;
	case PM_DISASM: //�Ӳ˵���ʾ�������ڵĲ���˵���,��PM_DISASM��ʾ�Ӳ˵���ʾ�ڷ���ര����
		strcpy(data,"0 &ApiTracing");
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
		if ( ::MessageBox(hwmain,"�����ú���ʼ��ַ����ֹ��ַ�����","׼����ʼ��",MB_YESNO)==IDYES ){
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

int Analyze(ulong ip)
{
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[MAXCMDSIZE]={0};	//һ�λ�����
	DWORD RetLen=0;			//
	int nAddr=0;
	int nRunFlag = STEP_IN;


	//��ָ����ַ��ʼ��ȡ�������ݣ��ɹ����ؽ������ݣ�psize�����ݴ�С��δ�ɹ��򷵻�NULL,psize��0
	Readmemory(buff,ip,MAXCMDSIZE,MM_RESILENT);

	pDecode = Finddecode(ip,&RetLen);

	//���ָ�stDisasm�ṹ
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
			Addtolist(0,1,"%08X call %08X��%s",ip, nAddr,stDisasm.comment);
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
			Addtolist(0,1,"ApiTracing ִ�����");
			return 0;
		}
	}

	return 1;
}


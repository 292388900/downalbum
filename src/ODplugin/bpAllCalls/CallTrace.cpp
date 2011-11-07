// CallTrace.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CallTrace.h"
#include <algorithm>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CCallTraceApp

BEGIN_MESSAGE_MAP(CCallTraceApp, CWinApp)
END_MESSAGE_MAP()


// CCallTraceApp ����

CCallTraceApp::CCallTraceApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

/************************************************************************/
/* ȫ�ֱ���																*/
/************************************************************************/

CCallTraceApp theApp;	// Ψһ��һ�� CCallTraceApp ����

HWND hwmain;			//����OD�����ھ��	
int nRunFlag = 0;
/************************************************************************/
/* ȫ�ֱ���                                                             */
/************************************************************************/


// CCallTraceApp ��ʼ��
BOOL CCallTraceApp::InitInstance()
{
	CWinApp::InitInstance();


	return TRUE;
}


//OD����ı�ҪԪ��,������ʾ��OD�Ĳ���˵�����,��ʾ������ƣ�32�ֽڳ���
extc int _export cdecl ODBG_Plugindata(char *shortname) 
{
	strcpy(shortname,"CallTrace");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) 
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	hwmain=hw;


	//��log������ʾ������ƺͰ汾���Լ���Ȩ��Ϣ
	Addtolist(0,0,"CallTrace plugin v0.1");
	Addtolist(0,-1," Copyright (C) 2009 sing");
	return 0;
}

//������Ӳ˵�����ʾ
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin)//���øûص������Ĵ��ڱ�� 
	{ 
	case PM_MAIN:
		strcpy(data,"0 &��������call����|1 ��ʼִ��,2 ִֹͣ��,3 C Source|4 &Options");
		return 1;
	case PM_DISASM: //�Ӳ˵���ʾ�������ڵĲ���˵���,��PM_DISASM��ʾ�Ӳ˵���ʾ�ڷ���ര����
		strcpy(data,"#CallTrace{0 &��������call����|1 ��ʼִ��,2 ִֹͣ��,3 C Source|4 &Options}");
		return 1;
	default: 
		return 0;
	}
}
//
//CString GetMyTempPath()
//{
//	TCHAR szTemp[MAX_PATH*2];
//	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
//	lstrcpy( StrRChr(szTemp,NULL,'\\')+1, _T("tmp\\") );
//	if ( GetFileAttributes(szTemp)==-1 ){
//		CreateDirectory(szTemp,NULL);
//	}
//	return szTemp;
//}

void BpAllCalls(int origin,int action,void *item)
{
	t_dump *pDump=(t_dump *)item;//pDump = item
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[MAXCMDSIZE]={0};	//һ�λ�����
	DWORD RetLen=0;			//

	vector<int>vtAddrs;
	int nAddr=0;
	CString strText;
	CString strTemp;

	/************************************************************************/
	/* ���´���ѡ����һ���ı�����������ʽ��������Ƶ���������           */
	/************************************************************************/

	DWORD dwSize=pDump->sel1 - pDump->sel0;	//ѡ����ı���С
	DWORD dwAddr=pDump->sel0;				//ѡ����ı���ʼ��ַ

	while (dwSize)
	{	
		//��ָ����ַ��ʼ��ȡ�������ݣ��ɹ����ؽ������ݣ�psize�����ݴ�С��δ�ɹ��򷵻�NULL,psize��0
		Readmemory(buff,dwAddr,MAXCMDSIZE,MM_RESILENT);

		pDecode = Finddecode(dwAddr,&RetLen);

		//���ָ�stDisasm�ṹ
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);

		if ( (stDisasm.cmdtype&C_TYPEMASK)==C_CAL ){
			if ( stDisasm.jmpaddr ){
				nAddr=stDisasm.jmpaddr;
			}else{
				nAddr=stDisasm.ip;
			}

			vector<int>::iterator pos=find(vtAddrs.begin(),vtAddrs.end(),nAddr);
			if  ( pos==vtAddrs.end() ){
				vtAddrs.push_back(nAddr);
			}

		}



		//���������
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;
	}//end while

	for ( vector<int>::iterator iter=vtAddrs.begin(); iter!=vtAddrs.end(); ++iter ){
		strTemp.Format("bp %08X \n",*iter);
		strText+=strTemp;
	}

	CString strFileName="c:\\1.txt";
	CStdioFile file;
	file.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite);
	file.WriteString(strText);
	file.Close();

	ShellExecute(NULL,"open","NOTEPAD.EXE",strFileName,NULL,SW_SHOWNORMAL);
	//DeleteFile(strFileName);
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
		BpAllCalls(origin,action,item);
		break;
	case 1:
		nRunFlag = 1;
		Go(0,0,STEP_RUN,TRUE,0);
		break;
	case 2:
		nRunFlag = 0;
		break;
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
	nRunFlag = 0;
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void) 
{
}

extc int _export cdecl ODBG_Paused(int reason, t_reg *reg) 
{
	if ( reg!=NULL && nRunFlag ){
		//Setbreakpoint(reg->ip,TY_DISABLED,0);
		Manualbreakpoint(reg->ip,VK_F2,0,0,0);
		Go(0,0,STEP_RUN,TRUE,0);
	}

	return 1;
}

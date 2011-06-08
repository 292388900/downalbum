// ColorCopy.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ColorCopy.h"

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


// CColorCopyApp

BEGIN_MESSAGE_MAP(CColorCopyApp, CWinApp)
END_MESSAGE_MAP()


// CColorCopyApp ����

CColorCopyApp::CColorCopyApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

/************************************************************************/
/* ȫ�ֱ���																*/
/************************************************************************/

CColorCopyApp theApp;	// Ψһ��һ�� CColorCopyApp ����

HWND hwmain;			//����OD�����ھ��		
CRichEditCtrl edtTemp;	//���ص�CRichEditCtrl�ؼ������������ı���ʽ�ģ���Ϊ�н�
CHARFORMAT2 m_cfDefault;//�Զ����ַ���ʽ 
/************************************************************************/
/* ȫ�ֱ���                                                             */
/************************************************************************/


// CColorCopyApp ��ʼ��
BOOL CColorCopyApp::InitInstance()
{
	CWinApp::InitInstance();

	AfxInitRichEdit();	//ʹ��richedit�������

	return TRUE;
}


//OD����ı�ҪԪ��,������ʾ��OD�Ĳ���˵�����,��ʾ������ƣ�32�ֽڳ���
extc int _export cdecl ODBG_Plugindata(char *shortname) 
{
	strcpy(shortname,"ColorCopy");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) 
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	hwmain=hw;
	edtTemp.Create(WS_CHILD|/*WS_VISIBLE|*/ES_MULTILINE|ES_WANTRETURN,
		CRect(-100,-100,10,10),AfxGetMainWnd(),0);

	memset(&m_cfDefault,0,sizeof(m_cfDefault));   
	m_cfDefault.cbSize=sizeof(CHARFORMAT2);   
	m_cfDefault.dwMask=CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_SIZE|CFM_STRIKEOUT|CFM_UNDERLINE; 
	m_cfDefault.dwEffects&=~CFE_BOLD;
	_tcscpy(m_cfDefault.szFaceName,_T("����"));//����    
	m_cfDefault.yHeight=180;  


	Addtolist(0,0,"ColorCopy plugin v0.1");
	Addtolist(0,-1," Copyright (C) 2009 sing");
	return 0;
}

//������Ӳ˵�����ʾ
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin)//���øûص������Ĵ��ڱ�� 
	{ 
	case PM_DISASM: //�Ӳ˵���ʾ�������ڵĲ���˵���,��PM_DISASM��ʾ�Ӳ˵���ʾ�ڷ���ര����
		strcpy(data,"0 &ColorCopy(����ɫ��ʽ����)");
		return 1;
	default: 
		return 0;
	}
}

/************************************************************************/
/*
��������AppendMsg
������ edtMsg��strMsg
����ֵ��void
˵�������ı���edtMsg��׷����Ϣ�����軻�����strMsg�ӣ�"\r\n"
������:sing
*/
/************************************************************************/
void AppendMsg(CRichEditCtrl*edtMsg,CString strMsg)
{
	LONG nLen=(LONG)edtMsg->SendMessage(WM_GETTEXTLENGTH);
	edtMsg->SetSel(nLen,-1);
	edtMsg->ReplaceSel(strMsg);
}

/************************************************************************/
/*
��������ChangeTextColor
������ color,nOffset
����ֵ��void
˵�����ı�CRichEditCtrl�ؼ����ı�ǰ��ɫ��nOffsetΪ������һ���ַ���ƫ�ơ�
������:sing
*/
/************************************************************************/
void ChangeTextColor(COLORREF color,LONG nOffset=0)
{
	m_cfDefault.crTextColor=color;
	m_cfDefault.dwEffects&=~CFE_BOLD;
	m_cfDefault.dwMask=(CFM_COLOR|CFM_FACE|CFM_SIZE)&~CFM_BOLD; 
	LONG nLen=(LONG)edtTemp.SendMessage(WM_GETTEXTLENGTH);

	edtTemp.SetSel(nLen+nOffset,-1);

	edtTemp.SetSelectionCharFormat(m_cfDefault);
}

/************************************************************************/
/*
��������ChangeBackColor
������ color,nOffset
����ֵ��void
˵�����ı�CRichEditCtrl�ؼ����ı�����ɫ��nOffsetΪ������һ���ַ���ƫ�ơ�
������:sing
*/
/************************************************************************/
void ChangeBackColor(COLORREF color,LONG nOffset=0)
{
	m_cfDefault.crBackColor=color;
	m_cfDefault.dwEffects&=~CFE_BOLD;
	m_cfDefault.dwMask=(CFM_BACKCOLOR|CFM_FACE|CFM_SIZE)&~CFM_BOLD; 
	LONG nLen=(LONG)edtTemp.SendMessage(WM_GETTEXTLENGTH);

	edtTemp.SetSel(nLen+nOffset,-1);   

	edtTemp.SetSelectionCharFormat(m_cfDefault);
}

void CheckEbpEsp()
{

}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	CString strText;		//���淴������ı�
	CString strOprand;		//���淴����ı��Ĳ�����
	CString strOpcode;		//���淴����ı��Ĳ�����
	CString strOpcode2;
	t_dump *pDump;			//pDump = item
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[0x10]={0};	//һ�λ�����
	DWORD RetLen=0;			//

	if (origin != PM_DISASM)//�Ƿ��ڷ���ര���� 
		return;
	if(action != 0)			//�Ƿ��ǵ�1���˵���Ӧ
		return;

	pDump = (t_dump *)item;
	if (pDump==NULL || pDump->size==0) //Window empty, don't add
		return;

	//û��ѡ���κ��ı�
	if (pDump->sel1<=pDump->sel0)
		return;

	/************************************************************************/
	/* ���´���ѡ����һ���ı�����������ʽ��������Ƶ���������           */
	/************************************************************************/
	
	DWORD dwSize=pDump->sel1-pDump->sel0;	//ѡ����ı���С
	DWORD dwAddr=pDump->sel0;				//ѡ����ı���ʼ��ַ
	edtTemp.Clear();						//��ձ༭��
	while (dwSize)
	{	
		Readmemory(buff,dwAddr,0x10,MM_RESILENT);
		pDecode = Finddecode(dwAddr,&RetLen);
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);

		//�ָ�Ϊ��ɫ�����EIP��Ϣ
		ChangeTextColor(RGB(0,0,0));
		strText.Format("%08X| ",stDisasm.ip);
		AppendMsg(&edtTemp,strText);
		
		//������ı�
		strText.Format("%s",stDisasm.result);

		//�����תָ�jmp,jnz������call
		if (stDisasm.jmpconst||stDisasm.jmptable||stDisasm.jmpaddr)
		{
			strOprand=strText.Mid(0,8).TrimRight();
			strOpcode=strText.Mid(strOprand.GetLength());
			if ((char)strOprand[0]==(char)'j')
			{
				if((char)strOprand[1]!=(char)'m')
					ChangeTextColor(RGB(255,0,0));	//��ɫǰ��ɫ
					ChangeBackColor(RGB(255,255,0));//��ɫ����ɫ								
			}
			else
			{
				ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
			}
			AppendMsg(&edtTemp,strOprand);
			ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ

			ChangeTextColor(RGB(128,0,0));	
			AppendMsg(&edtTemp,strOpcode);
		}
		else if (*((DWORD*)&stDisasm.result[0])==(DWORD)'nter')
		{
			strOprand=strText.Mid(0,8).TrimRight();
			
			ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
			AppendMsg(&edtTemp,strOprand);
			ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ
			if (nOpcodeLen>4)
			{
				strOpcode=strText.Mid(strOprand.GetLength());
				AppendMsg(&edtTemp,strOpcode);
			}
			
		}
		else//���� ptr [ebp...]�� ptr [esp...]
		{
			int nPos=-1;
			int nLocation=-1;
			strOprand=strText.Mid(0,8);
			strOpcode=strText.Mid(strOprand.GetLength());

			if ((char)strText[0]==(char)'p')//push pop
			{
				ChangeTextColor(RGB(0,0,255));
				AppendMsg(&edtTemp,strOprand);

				if (stDisasm.immconst)//push pop�������������
				{
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(&edtTemp,strOpcode);
					goto _AppendCmt;
				}
				else//��Ҫ����Ƿ���ptr [ebp...]�� ptr [esp...]
				{
					goto _CheckEbpEsp;
				}
				ChangeTextColor(RGB(0,0,0));
				AppendMsg(&edtTemp,strOpcode);
				goto _AppendCmt;				
			}
			
			ChangeTextColor(RGB(0,0,0));
			AppendMsg(&edtTemp,strOprand);
_CheckEbpEsp:
			nPos=strText.Find("[ebp");
			nLocation=strText.Find(',',0);
			if (nPos==-1)
				nPos=strText.Find("[esp");
			if(nPos==-1)
				goto _AppendOpcode;

			
			if (nLocation!=-1)
			{
				if (nPos<nLocation)//ptr�����
				{
					strOpcode=strText.Mid(8,nLocation-8);
					strOpcode2=strText.Mid(nLocation);
					ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
					AppendMsg(&edtTemp,strOpcode);
					ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ
					if (stDisasm.immconst)
					{
						AppendMsg(&edtTemp,", ");
						ChangeTextColor(RGB(128,128,0));
						strOpcode2=strText.Mid(nLocation+2);
						AppendMsg(&edtTemp,strOpcode2);
					}
					AppendMsg(&edtTemp,strOpcode2);
				}
				else//ptr���Ҳ�
				{
					strOpcode=strText.Mid(8,nLocation+2-8);
					AppendMsg(&edtTemp,strOpcode);
					strOpcode2=strText.Mid(nLocation+2);
					ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
					AppendMsg(&edtTemp,strOpcode2);
					ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ
				}
				goto _AppendCmt;
			}
			else //push    dword ptr [ebp-18]	
			{
				ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
				ChangeTextColor(RGB(0,0,0));
				AppendMsg(&edtTemp,strOpcode);
				goto _AppendCmt;
			}
_AppendOpcode:
			if (stDisasm.immconst)
			{
				if(nLocation>0)
				{
					strOpcode=strText.Mid(8,nLocation-8+2);
					strOpcode2=strText.Mid(nLocation+2);
					AppendMsg(&edtTemp,strOpcode);
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(&edtTemp,strOpcode2);
				}
				else//db 1111
				{
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(&edtTemp,strOpcode);
				}
				
			}
			else
			{
				ChangeTextColor(RGB(0,0,0));
				AppendMsg(&edtTemp,strOpcode);
			}
			
		}

_AppendCmt:
		//���ע�ͣ�ʹ����ɫ
		int nLineLen=strText.GetLength()+10;
		if (nLineLen>60)
		{
			strText="����\r\n";
		}
		else
		{
			char szBuff[61];
			memset(szBuff,' ',60-nLineLen);
			szBuff[60-nLineLen]=0;
			strText.Format("%s|%s\r\n",szBuff,stDisasm.comment);
		}
		ChangeTextColor(RGB(0,128,0));						
		AppendMsg(&edtTemp,strText);

		//���������
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;
	}//end while
	
	//�����ı���������
	edtTemp.SetSel(0,-1);
	edtTemp.Copy();
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
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void) 
{
}


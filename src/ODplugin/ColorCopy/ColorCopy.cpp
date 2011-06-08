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
CRichEditCtrl g_edtTemp;	//���ص�CRichEditCtrl�ؼ������������ı���ʽ�ģ���Ϊ�н�
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
#ifdef _DEBUG
	g_edtTemp.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN,
		CRect(0,0,600,600),AfxGetMainWnd(),0);
#else
	g_edtTemp.Create(WS_CHILD|/*WS_VISIBLE|*/ES_MULTILINE|ES_WANTRETURN,
		CRect(-100,-100,10,10),AfxGetMainWnd(),0);
#endif


	memset(&m_cfDefault,0,sizeof(m_cfDefault));   
	m_cfDefault.cbSize=sizeof(CHARFORMAT2);   
	m_cfDefault.dwMask=CFM_COLOR|CFM_FACE|CFM_SIZE&(~CFM_BOLD); 
	m_cfDefault.dwEffects&=~CFE_BOLD;
	lstrcpy(m_cfDefault.szFaceName,_T("����"));//����    
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
	case PM_MAIN:
		strcpy(data,"0 &ColorCopy|1 Hex Value(with space split),2 Hex Value,3 C Source|4 &Options");
		return 1;
	case PM_DISASM: //�Ӳ˵���ʾ�������ڵĲ���˵���,��PM_DISASM��ʾ�Ӳ˵���ʾ�ڷ���ര����
		strcpy(data,"#ColorCopy{0 &ColorCopy|1 Hex Value(with space split),2 Hex Value,3 C Source|4 &Options}");
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
void AppendMsg(CString strMsg)
{
	LONG nLen=(LONG)g_edtTemp.SendMessage(WM_GETTEXTLENGTH);
	g_edtTemp.SetSel(nLen,-1);
	g_edtTemp.ReplaceSel(strMsg);
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
	LONG nLen=(LONG)g_edtTemp.SendMessage(WM_GETTEXTLENGTH);

	g_edtTemp.SetSel(nLen+nOffset,-1);

	g_edtTemp.SetSelectionCharFormat(m_cfDefault);
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
	LONG nLen=(LONG)g_edtTemp.SendMessage(WM_GETTEXTLENGTH);

	g_edtTemp.SetSel(nLen+nOffset,-1);   

	g_edtTemp.SetSelectionCharFormat(m_cfDefault);
}

void CheckEbpEsp()
{

}

/*����ʮ��������

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	CString strText;		//���淴������ı�
	CString strOpcode;		//���淴����ı��Ĳ�����
	CString strOprand1;		//���淴����ı��Ĳ�����
	CString strOprand2;
	t_dump *pDump;			//pDump = item
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[0x10]={0};	//һ�λ�����
	DWORD RetLen=0;			//

	if (origin != PM_DISASM)//�Ƿ��ڷ���ര���� 
		return;
	if(action != 4)			//�Ƿ��ǵ�1���˵���Ӧ
		return;

	pDump = (t_dump *)item;
	if (pDump==NULL || pDump->size==0) //Window empty, don't add
		return;

	//û��ѡ���κ��ı�
	if (pDump->sel1<=pDump->sel0)
		return;

	/************************************************************************
	// ���´���ѡ����һ���ı�����������ʽ��������Ƶ���������           
	/************************************************************************
	
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
		AppendMsg(strText);
		
		//������ı�
		strText.Format("%s",stDisasm.result);

		//�����תָ�jmp,jnz������call
		if (stDisasm.jmpconst||stDisasm.jmptable||stDisasm.jmpaddr)
		{
			strOpcode=strText.Mid(0,8).TrimRight();
			strOprand1=strText.Mid(strOpcode.GetLength());
			if ((char)strOpcode[0]==(char)'j')
			{
				if((char)strOpcode[1]!=(char)'m')
					ChangeTextColor(RGB(255,0,0));	//��ɫǰ��ɫ
					ChangeBackColor(RGB(255,255,0));//��ɫ����ɫ								
			}
			else
			{
				ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
			}
			AppendMsg(strOpcode);
			ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ

			ChangeTextColor(RGB(128,0,0));	
			AppendMsg(strOprand1);
		}
		else if (*((DWORD*)&stDisasm.result[0])==(DWORD)'nter')
		{
			strOpcode=strText.Mid(0,8).TrimRight();
			
			ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
			AppendMsg(strOpcode);
			ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ
			if (nOpcodeLen>4)
			{
				strOprand1=strText.Mid(strOpcode.GetLength());
				AppendMsg(strOprand1);
			}
			
		}
		else//���� ptr [ebp...]�� ptr [esp...]
		{
			int nPos=-1;
			int nLocation=-1;
			strOpcode=strText.Mid(0,8);
			strOprand1=strText.Mid(strOpcode.GetLength());

			if ((char)strText[0]==(char)'p')//push pop
			{
				ChangeTextColor(RGB(0,0,255));
				AppendMsg(strOpcode);

				if (stDisasm.immconst)//push pop�������������
				{
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(strOprand1);
					goto _AppendCmt;
				}
				else//��Ҫ����Ƿ���ptr [ebp...]�� ptr [esp...]
				{
					goto _CheckEbpEsp;
				}
				ChangeTextColor(RGB(0,0,0));
				AppendMsg(strOprand1);
				goto _AppendCmt;				
			}
			
			ChangeTextColor(RGB(0,0,0));
			AppendMsg(strOpcode);
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
					strOprand1=strText.Mid(8,nLocation-8);
					strOprand2=strText.Mid(nLocation);
					ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
					AppendMsg(strOprand1);
					ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ
					if (stDisasm.immconst)
					{
						AppendMsg(", ");
						ChangeTextColor(RGB(128,128,0));
						strOprand2=strText.Mid(nLocation+2);
						AppendMsg(strOprand2);
					}
					AppendMsg(strOprand2);
				}
				else//ptr���Ҳ�
				{
					strOprand1=strText.Mid(8,nLocation+2-8);
					AppendMsg(strOprand1);
					strOprand2=strText.Mid(nLocation+2);
					ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
					AppendMsg(strOprand2);
					ChangeBackColor(RGB(255,255,255));//�ָ���ɫ����ɫ
				}
				goto _AppendCmt;
			}
			else //push    dword ptr [ebp-18]	
			{
				ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
				ChangeTextColor(RGB(0,0,0));
				AppendMsg(strOprand1);
				goto _AppendCmt;
			}
_AppendOpcode:
			if (stDisasm.immconst)
			{
				if(nLocation>0)
				{
					strOprand1=strText.Mid(8,nLocation-8+2);
					strOprand2=strText.Mid(nLocation+2);
					AppendMsg(strOprand1);
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(strOprand2);
				}
				else//db 1111
				{
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(strOprand1);
				}
				
			}
			else
			{
				ChangeTextColor(RGB(0,0,0));
				AppendMsg(strOprand1);
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
		AppendMsg(strText);

		//���������
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;
	}//end while
	
	//�����ı���������
	edtTemp.SetSel(0,-1);
	edtTemp.Copy();
}
*/
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	CString strText;		//���淴������ı�
	CString strOpcode;		//���淴����ı��Ĳ�����
	CString strOprand1;		//���淴����ı��Ĳ�����1
	CString strOprand2;		//���淴����ı��Ĳ�����2,�������������Ȳ���
	t_dump *pDump;			//pDump = item
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[MAXCMDSIZE]={0};	//һ�λ�����
	DWORD RetLen=0;			//

	int nPos1=0;
	int nPos2=0;

	if ( origin!=PM_DISASM && origin!=PM_MAIN ) //�Ƿ��ڷ���ര�ڻ����������� 
		return;
	if( action!=0)			//�Ƿ��ǵ�1���˵���Ӧ
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

	DWORD dwSize=pDump->sel1 - pDump->sel0;	//ѡ����ı���С
	DWORD dwAddr=pDump->sel0;				//ѡ����ı���ʼ��ַ
	g_edtTemp.Clear();						//��ձ༭��
	while (dwSize)
	{	
		//��ָ����ַ��ʼ��ȡ�������ݣ��ɹ����ؽ������ݣ�psize�����ݴ�С��δ�ɹ��򷵻�NULL,psize��0
		Readmemory(buff,dwAddr,MAXCMDSIZE,MM_RESILENT);

		pDecode = Finddecode(dwAddr,&RetLen);

		//���ָ�stDisasm�ṹ
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);

		//1.�ָ�Ϊ��ɫ�����EIP��Ϣ
		ChangeTextColor(RGB(0,0,0));

		strText.Format("%08X| ",stDisasm.ip);
		AppendMsg(strText);

		/************************************************************************/
		//2.������ָ�����ݴ�
		const int CODEDATALEN=22;	 
		char szTemp[CODEDATALEN+1];
		int nCodeLen=strlen(stDisasm.dump);

		if(nCodeLen<=CODEDATALEN)
		{
			memcpy(szTemp,stDisasm.dump,nCodeLen);
			memset(&szTemp[nCodeLen],' ',CODEDATALEN-nCodeLen);
			szTemp[CODEDATALEN]=0;
		}
		else
		{
			szTemp[0]=0;
			MessageBox(NULL,"�����ϳ�ָ��","��ʾ",MB_OK|MB_ICONEXCLAMATION);
		}

		strText.Format("%s| ",szTemp);
		AppendMsg(strText);


		/************************************************************************/


		//3.������ı�
		strText=stDisasm.result;
		strOpcode.Empty();
		strOprand1.Empty();
		strOprand2.Empty();

		//���������Ͳ�����
		nPos1=strText.Find(' ');
		if ( nPos1==-1 ){		//0������
			strOpcode=strText;
		}else{
			strOpcode=strText.Left(nPos1);
			nPos2=strText.Find(',',nPos1+1);
			if ( nPos2==-1 ){	//1������
				strOprand1=strText.Mid(nPos1+1);
			}else{				//2������
				strOprand1=strText.Mid(nPos1+1,nPos2-nPos1-1);
				strOprand2=strText.Mid(nPos2+1);
			}
		}

		if ( stDisasm.cmdtype==C_JMP ){			//jmp �Ƶ׺���
			ChangeTextColor(RGB(0,0,0));									
			ChangeBackColor(RGB(255,255,0));
			AppendMsg(strOpcode);
			
		}else if ( stDisasm.cmdtype==C_JMC ){	//������ת �Ƶ׺���
			ChangeTextColor(RGB(255,0,0));									
			ChangeBackColor(RGB(255,255,0));
			AppendMsg(strOpcode);

		}else if ( stDisasm.cmdtype==C_REP ){	//rep
			ChangeTextColor(RGB(255,0,0));									
			ChangeBackColor(RGB(255,255,0));
			AppendMsg(strOpcode);

		}else if ( stDisasm.cmdtype==C_CAL ){	//call �����׺���
			ChangeTextColor(RGB(0,0,0));									
			ChangeBackColor(RGB(0,255,255));//����ɫ����ɫ
			AppendMsg(strOpcode);
		
		}else if ( stDisasm.cmdtype==C_RET ){	//ret �����׺���
			ChangeTextColor(RGB(0,0,0));									
			ChangeBackColor(RGB(0,255,255));
			AppendMsg(strOpcode);

		}else if ( stDisasm.cmdtype==C_PSH || stDisasm.cmdtype==C_POP ){	//PUSH POP �׵�����
			ChangeTextColor(RGB(0,0,255));
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(strOpcode);	
		
		}else if ( stDisasm.cmdtype==C_FLT ){
			ChangeTextColor(RGB(128,0,0));
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(strOpcode);

		}else{									//�׵׺���
			ChangeTextColor(RGB(0,0,0));
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(strOpcode);	
		}

		
		if ( strOprand1.IsEmpty()==FALSE ){	//������1��Ϊ��
			//���ð׵ף����һ���ո�
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(" ");

			//�����ֱ�ӵ�ַ����ʾΪ�׵��غ�����
			if ( stDisasm.jmpconst || stDisasm.jmptable || stDisasm.jmpaddr ){
				ChangeTextColor(RGB(128,0,0));	
				ChangeBackColor(RGB(255,255,255));
			}else if ( strOprand1.Find("[esp")!=-1 || strOprand1.Find("[ebp")!=-1 ){	//��������
				ChangeTextColor(RGB(0,0,128));	
				ChangeBackColor(RGB(0,255,255));	
			}else if ( stDisasm.memtype==DEC_BYTE || stDisasm.memtype==DEC_WORD || stDisasm.memtype==DEC_DWORD ){//�׵�ǳ����
				ChangeTextColor(RGB(0,0,128));	
				ChangeBackColor(RGB(255,255,255));
				
			}else if ( (stDisasm.adrconst || stDisasm.immconst) && strOprand2.IsEmpty()==TRUE ){	
				//�������ȼ���DEC_BYTE DEC_WORD DEC_DWORD�ͣ���������������ʱֱ�ӵ�ַ�������ڲ�����1��
				ChangeTextColor(RGB(128,0,0));	
				ChangeBackColor(RGB(255,255,255));
			}else{	//�׵׺���
				ChangeTextColor(RGB(0,0,0));	
				ChangeBackColor(RGB(255,255,255));	
			}
			AppendMsg(strOprand1);
		}

		if ( strOprand2.IsEmpty()==FALSE ){	//������2��Ϊ��
			//���ð׵ף����һ������
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			//ͨ�����ָ�������������������Ҳ�����2Ϊ��������ʾ�غ�ɫ
			if ( strOprand2.Find("[esp")!=-1 || strOprand2.Find("[ebp")!=-1 ){	//��������
				ChangeTextColor(RGB(0,0,128));	
				ChangeBackColor(RGB(0,255,255));	
			}else if ( stDisasm.memtype==DEC_BYTE || stDisasm.memtype==DEC_WORD || stDisasm.memtype==DEC_DWORD ){//�׵�ǳ����
				ChangeTextColor(RGB(0,0,128));	
				ChangeBackColor(RGB(255,255,255));
			}else if ( stDisasm.adrconst || stDisasm.immconst ){
				ChangeTextColor(RGB(128,0,0));	
				ChangeBackColor(RGB(255,255,255));
			}else{	//�׵׺���
				ChangeTextColor(RGB(0,0,0));	
				ChangeBackColor(RGB(255,255,255));	
			}
			AppendMsg(strOprand2);
		}

		TRACE("����ࣺ%s %s , \n",strOpcode,strOprand1,strOprand2);

_AppendCmt:
		//���ע�ͣ�ʹ����ɫ
		int nLineLen=strText.GetLength()+10;
		if (nLineLen>60){
			strText="����\r\n";
		}else{
			char szBuff[61];
			memset(szBuff,' ',60-nLineLen);
			szBuff[60-nLineLen]=0;
			strText.Format("%s|%s\r\n",szBuff,stDisasm.comment);
		}
		ChangeTextColor(RGB(0,128,0));						
		ChangeBackColor(RGB(255,255,255));						
		AppendMsg(strText);

		//���������
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;
	}//end while

	//�����ı���������
	g_edtTemp.SetSel(0,-1);
	g_edtTemp.Copy();
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


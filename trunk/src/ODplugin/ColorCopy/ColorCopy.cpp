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
	g_edtTemp.Create(WS_CHILD|/*WS_VISIBLE|*/ES_MULTILINE|ES_WANTRETURN,
		CRect(-100,-100,10,10),AfxGetMainWnd(),0);


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


void AlignOpcode(CString&strOpcode)
{
	//���ð׵ף�����8λ
	ChangeBackColor(RGB(255,255,255));
	int nLen=strOpcode.GetLength();
	if ( nLen>=8 ){	//ֱ�Ӽ�һ���ո�
		AppendMsg(" ");
	}else{
		char szBuff[MAX_PATH]={0};
		memset(szBuff,' ',8-nLen);
		AppendMsg(szBuff);
	}
}

void AppendOprand(int nOprandCnt,t_operand&op,CString&strOprand1)
{
	if ( nOprandCnt==1 && op.opsize==0 ){	//������ֻ��һ�������⴦��
		//Ӧ����jmp��call�ĳ���:�غ�ɫ
		ChangeTextColor(RGB(128,0,0));	
		ChangeBackColor(RGB(255,255,255));
		AppendMsg(strOprand1);
		return;
	}

	if ( op.seg==(char)SEG_UNDEF ){	
		if ( op.optype==0x40 ){							//��������ɫ
			ChangeTextColor(RGB(128,128,0));	
			ChangeBackColor(RGB(255,255,255));
		}else{											//�ڵ׺���
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
		}
	}else if (op.seg==SEG_SS ){							//ss:[ebp] s::[esp] ��������
		ChangeTextColor(RGB(0,0,128));	
		ChangeBackColor(RGB(0,255,255));	
	}else{												//��������ds:[edi] �׵�����
		ChangeTextColor(RGB(0,0,128));	
		ChangeBackColor(RGB(255,255,255));
	}
	AppendMsg(strOprand1);
}

void ColorCopy(int origin,int action,void *item)
{
	CString strText;		//���淴������ı�
	CString strOpcode;		//���淴����ı��Ĳ�����
	CString strOprand1;		//���淴����ı��Ĳ�����1
	CString strOprand2;		//���淴����ı��Ĳ�����2
	CString strOprand3;		//���淴����ı��Ĳ�����3

	t_dump *pDump=(t_dump *)item;//pDump = item
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[MAXCMDSIZE]={0};	//һ�λ�����
	DWORD RetLen=0;			//

	int nOPCnt=0;			//����������
	int nPos1=0;
	int nPos2=0;
	int nPos3=0;

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
		strOprand3.Empty();

		nPos1=strText.Find(' ');
		if ( nPos1==-1 ){			//0������
			nOPCnt=0;
			strOpcode=strText;
		}else{
			strOpcode=strText.Left(nPos1);
			nPos2=strText.Find(',',nPos1+1);
			if ( nPos2==-1 ){		//1������
				nOPCnt=1;
				strOprand1=strText.Mid(nPos1+1);
			}else{
				strOprand1=strText.Mid(nPos1+1,nPos2-nPos1-1);
				nPos3=strText.Find(',',nPos2+1);
				if ( nPos3==-1 ){	//2������
					nOPCnt=2;
					strOprand2=strText.Mid(nPos2+1);
				}else{				//3������
					nOPCnt=3;
					strOprand2=strText.Mid(nPos2+1,nPos3-nPos2-1);
					strOprand3=strText.Mid(nPos3+1);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////

		if ( nOPCnt==0 ){						//0������
			//ret pushad popad pushaf popaf...

			if ( (stDisasm.cmdtype&C_TYPEMASK)==C_RET ){		//ret �����׺���
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(0,255,255));
			}else{
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(255,255,255));
			}
			AppendMsg(strOpcode);
			AlignOpcode(strOpcode);

		}else if ( nOPCnt==1 ){					//1������
			//ret 4; ret eax
			//push eax; push 1; pop eax; push dword ptr ds:[eax]; push dword ptr ss:[ebp]
			//jmp 400000; jmp eax; jmp dword ptr ds:[eax]; jmp dword ptr ss:[ebp]
			//jnz 400000; jnz eax; jnz dword ptr ds:[eax]; jnz dword ptr ss:[ebp]
			//call 400000; call eax; call dword ptr ds:[eax]; call dword ptr ss:[ebp]

			if ( (stDisasm.cmdtype&C_TYPEMASK)==C_JMP ){			//jmp �Ƶ׺���
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(255,255,0));
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_JMC ){	//������ת �Ƶ׺���jpo:0x68
				ChangeTextColor(RGB(255,0,0));									
				ChangeBackColor(RGB(255,255,0));
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);


			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_REP ){	//rep
				ChangeTextColor(RGB(255,0,0));									
				ChangeBackColor(RGB(255,255,0));
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				ChangeTextColor(RGB(0,0,0));	
				ChangeBackColor(RGB(255,255,255));
				AppendMsg(strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_CAL ){	//call �����׺���
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(0,255,255));	//����ɫ����ɫ
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_RET ){	//ret �����׺���
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(0,255,255));
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_PSH || (stDisasm.cmdtype&C_TYPEMASK)==C_POP ){	//PUSH POP �׵�����
				ChangeTextColor(RGB(0,0,255));
				ChangeBackColor(RGB(255,255,255));
				AppendMsg(strOpcode);	

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else{

				if ( (stDisasm.cmdtype&C_TYPEMASK)==C_FLT ){
					ChangeTextColor(RGB(128,0,0));
					ChangeBackColor(RGB(255,255,255));
				}else{
					ChangeTextColor(RGB(0,0,0));
					ChangeBackColor(RGB(255,255,255));
				}
				AppendMsg(strOpcode);	

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}

		}else if ( nOPCnt==2 ){					//2������
			if ( (stDisasm.cmdtype&C_TYPEMASK)==C_FLT ){
				ChangeTextColor(RGB(128,0,0));
				ChangeBackColor(RGB(255,255,255));
			}else{
				ChangeTextColor(RGB(0,0,0));
				ChangeBackColor(RGB(255,255,255));
			}
			AppendMsg(strOpcode);	

			AlignOpcode(strOpcode);

			AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			//���ð׵׺��֣����һ������
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			AppendOprand(nOPCnt,stDisasm.op[1],strOprand2);

		}else if ( nOPCnt==3 ){					//3������
			if ( (stDisasm.cmdtype&C_TYPEMASK)==C_FLT ){
				ChangeTextColor(RGB(128,0,0));
				ChangeBackColor(RGB(255,255,255));
			}else{
				ChangeTextColor(RGB(0,0,0));
				ChangeBackColor(RGB(255,255,255));
			}
			AppendMsg(strOpcode);	

			AlignOpcode(strOpcode);

			AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			//���ð׵׺��֣����һ������
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			AppendOprand(nOPCnt,stDisasm.op[0],strOprand2);

			//���ð׵׺��֣����һ������
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			AppendOprand(nOPCnt,stDisasm.op[0],strOprand3);
		}

		//////////////////////////////////////////////////////////////////////////

		TRACE("����ࣺ%s %s, %s, %s\n",strOpcode,strOprand1,strOprand2,strOprand3);

_AppendCmt:
		//���ע�ͣ�ʹ����ɫ
		int nLineLen=0;
		if ( strOpcode.GetLength()>=8 ){
			nLineLen=strText.GetLength();	//һ���ո�����ɸ����Ŷ����ȥ��
		}else{
			nLineLen=strText.GetLength()+ (8-strOpcode.GetLength());	//�������ڵ���AlignOpcode����Ŀո񳤶�
			if ( strOprand1.IsEmpty()==FALSE ){	//ԭ�����м����һ���ո�
				nLineLen--;
			}
		}

		if (nLineLen>50){
			strText="����\r\n";
		}else{
			char szBuff[51];
			memset(szBuff,' ',50-nLineLen);
			szBuff[50-nLineLen]=0;
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

void HexCopy(int origin,int action,void *item)
{
	CString strText;		//���淴������ı�
	CString strTemp;		
	t_dump *pDump=(t_dump *)item;//pDump = item
	t_disasm stDisasm={0};	//���淴�������Ϣ
	uchar *pDecode=NULL;	//����ָ�������ʼ��ַ
	ulong nOpcodeLen;		//����һ��ָ��ĳ��ȣ����ֽڼ���
	BYTE buff[MAXCMDSIZE]={0};	//һ�λ�����
	DWORD RetLen=0;			//

	int nCount=0;
	/************************************************************************/
	/* ���´���ѡ����һ���ı�����������ʽ��������Ƶ���������           */
	/************************************************************************/

	DWORD dwSize=pDump->sel1 - pDump->sel0;	//ѡ����ı���С
	DWORD dwAddr=pDump->sel0;				//ѡ����ı���ʼ��ַ
	while ( dwSize ){	
		//��ָ����ַ��ʼ��ȡ�������ݣ��ɹ����ؽ������ݣ�psize�����ݴ�С��δ�ɹ��򷵻�NULL,psize��0
		Readmemory(buff,dwAddr,MAXCMDSIZE,MM_RESILENT);

		pDecode = Finddecode(dwAddr,&RetLen);

		//���ָ�stDisasm�ṹ
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);
		//���������
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;

		if ( action==1 ){
			for ( int i=0; i<nOpcodeLen; ++i ){
				strTemp.Format("%02X ",buff[i]);
				strText+=strTemp;
			}
		}else if ( action==2 ){
			for ( int i=0; i<nOpcodeLen; ++i ){
				strTemp.Format("%02X",buff[i]);
				strText+=strTemp;
			}
		}else if ( action==3 ){
			for ( int i=0; i<nOpcodeLen; ++i ){
				if ( nCount%0x10 == 0 ){
					strText+="\r\n\t";
				}
				if ( i==nOpcodeLen-1 && dwSize==0 ){	//���һ��
					strTemp.Format("0x%02X \r\n};",buff[i]);
					strText+=strTemp;
					nCount++;
					strTemp.Format("unsigned char data[%d] = {",nCount);
					strText=strTemp+strText;
				}else{
					strTemp.Format("0x%02X, ",buff[i]);
					strText+=strTemp;
					nCount++;
				}
			}//end for
		}

		//////////////////////////////////////////////////////////////////////////
		
	}//end while

	//��󶼼Ӹ�����
	strText+="\r\n";

	//���Ƶ�������
	if ( ::OpenClipboard(hwmain) ){
		::EmptyClipboard();

		HANDLE hData=::GlobalAlloc(GMEM_MOVEABLE,strText.GetLength()+1);
		LPSTR pData=(LPSTR)::GlobalLock(hData);
		lstrcpy(pData,(LPCTSTR)strText);
		::GlobalUnlock(hData);

		::SetClipboardData(CF_TEXT,hData);
		::CloseClipboard();
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
		ColorCopy(origin,action,item);
		break;
	case 1:
	case 2:
	case 3:
		HexCopy(origin,action,item);
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
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void) 
{
}


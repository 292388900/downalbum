// ColorCopy.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ColorCopy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CColorCopyApp

BEGIN_MESSAGE_MAP(CColorCopyApp, CWinApp)
END_MESSAGE_MAP()


// CColorCopyApp 构造

CColorCopyApp::CColorCopyApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

/************************************************************************/
/* 全局变量																*/
/************************************************************************/

CColorCopyApp theApp;	// 唯一的一个 CColorCopyApp 对象

HWND hwmain;			//保存OD主窗口句柄		
CRichEditCtrl g_edtTemp;	//隐藏的CRichEditCtrl控件，用于设置文本格式的，作为中介
CHARFORMAT2 m_cfDefault;//自定义字符格式 
/************************************************************************/
/* 全局变量                                                             */
/************************************************************************/


// CColorCopyApp 初始化
BOOL CColorCopyApp::InitInstance()
{
	CWinApp::InitInstance();

	AfxInitRichEdit();	//使用richedit必须调用

	return TRUE;
}


//OD插件的必要元素,用来显示在OD的插件菜单里面,表示插件名称（32字节长）
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
	lstrcpy(m_cfDefault.szFaceName,_T("宋体"));//字体    
	m_cfDefault.yHeight=180;  


	Addtolist(0,0,"ColorCopy plugin v0.1");
	Addtolist(0,-1," Copyright (C) 2009 sing");
	return 0;
}

//插件的子菜单的显示
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin)//调用该回调函数的窗口编号 
	{ 
	case PM_MAIN:
		strcpy(data,"0 &ColorCopy|1 Hex Value(with space split),2 Hex Value,3 C Source|4 &Options");
		return 1;
	case PM_DISASM: //子菜单显示在主窗口的插件菜单中,而PM_DISASM表示子菜单显示在反汇编窗口中
		strcpy(data,"#ColorCopy{0 &ColorCopy|1 Hex Value(with space split),2 Hex Value,3 C Source|4 &Options}");
		return 1;
	default: 
		return 0;
	}
}

/************************************************************************/
/*
函数名：AppendMsg
参数： edtMsg，strMsg
返回值：void
说明：向文本框edtMsg中追加信息，如需换行请给strMsg加："\r\n"
创建者:sing
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
函数名：ChangeTextColor
参数： color,nOffset
返回值：void
说明：改变CRichEditCtrl控件的文本前景色，nOffset为相对最后一个字符的偏移。
创建者:sing
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
函数名：ChangeBackColor
参数： color,nOffset
返回值：void
说明：改变CRichEditCtrl控件的文本背景色，nOffset为相对最后一个字符的偏移。
创建者:sing
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
	//设置白底，补齐8位
	ChangeBackColor(RGB(255,255,255));
	int nLen=strOpcode.GetLength();
	if ( nLen>=8 ){	//直接加一个空格
		AppendMsg(" ");
	}else{
		char szBuff[MAX_PATH]={0};
		memset(szBuff,' ',8-nLen);
		AppendMsg(szBuff);
	}
}

void AppendOprand(int nOprandCnt,t_operand&op,CString&strOprand1)
{
	if ( nOprandCnt==1 && op.opsize==0 ){	//操作数只有一个的特殊处理
		//应该是jmp和call的常数:棕红色
		ChangeTextColor(RGB(128,0,0));	
		ChangeBackColor(RGB(255,255,255));
		AppendMsg(strOprand1);
		return;
	}

	if ( op.seg==(char)SEG_UNDEF ){	
		if ( op.optype==0x40 ){							//常数：金色
			ChangeTextColor(RGB(128,128,0));	
			ChangeBackColor(RGB(255,255,255));
		}else{											//黑底黑字
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
		}
	}else if (op.seg==SEG_SS ){							//ss:[ebp] s::[esp] 蓝底蓝字
		ChangeTextColor(RGB(0,0,128));	
		ChangeBackColor(RGB(0,255,255));	
	}else{												//基本上是ds:[edi] 白底蓝字
		ChangeTextColor(RGB(0,0,128));	
		ChangeBackColor(RGB(255,255,255));
	}
	AppendMsg(strOprand1);
}

void ColorCopy(int origin,int action,void *item)
{
	CString strText;		//保存反汇编后的文本
	CString strOpcode;		//保存反汇编文本的操作码
	CString strOprand1;		//保存反汇编文本的操作数1
	CString strOprand2;		//保存反汇编文本的操作数2
	CString strOprand3;		//保存反汇编文本的操作数3

	t_dump *pDump=(t_dump *)item;//pDump = item
	t_disasm stDisasm={0};	//保存反汇编后的信息
	uchar *pDecode=NULL;	//保存指令码的起始地址
	ulong nOpcodeLen;		//保存一条指令的长度，按字节计算
	BYTE buff[MAXCMDSIZE]={0};	//一段缓冲区
	DWORD RetLen=0;			//

	int nOPCnt=0;			//操作数个数
	int nPos1=0;
	int nPos2=0;
	int nPos3=0;

	/************************************************************************/
	/* 以下处理选择了一段文本后的情况，格式化处理后复制到剪贴板上           */
	/************************************************************************/

	DWORD dwSize=pDump->sel1 - pDump->sel0;	//选择的文本大小
	DWORD dwAddr=pDump->sel0;				//选择的文本起始地址
	g_edtTemp.Clear();						//清空编辑框
	while (dwSize)
	{	
		//从指定地址开始读取解码数据，成功返回解码数据，psize＝数据大小；未成功则返回NULL,psize＝0
		Readmemory(buff,dwAddr,MAXCMDSIZE,MM_RESILENT);

		pDecode = Finddecode(dwAddr,&RetLen);

		//汇编指令到stDisasm结构
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);

		//1.恢复为黑色，添加EIP信息
		ChangeTextColor(RGB(0,0,0));

		strText.Format("%08X| ",stDisasm.ip);
		AppendMsg(strText);

		/************************************************************************/
		//2.二进制指令数据串
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
			MessageBox(NULL,"遇到较长指令","提示",MB_OK|MB_ICONEXCLAMATION);
		}

		strText.Format("%s| ",szTemp);
		AppendMsg(strText);


		/************************************************************************/


		//3.反汇编文本
		strText=stDisasm.result;
		strOpcode.Empty();
		strOprand1.Empty();
		strOprand2.Empty();
		strOprand3.Empty();

		nPos1=strText.Find(' ');
		if ( nPos1==-1 ){			//0操作数
			nOPCnt=0;
			strOpcode=strText;
		}else{
			strOpcode=strText.Left(nPos1);
			nPos2=strText.Find(',',nPos1+1);
			if ( nPos2==-1 ){		//1操作数
				nOPCnt=1;
				strOprand1=strText.Mid(nPos1+1);
			}else{
				strOprand1=strText.Mid(nPos1+1,nPos2-nPos1-1);
				nPos3=strText.Find(',',nPos2+1);
				if ( nPos3==-1 ){	//2操作数
					nOPCnt=2;
					strOprand2=strText.Mid(nPos2+1);
				}else{				//3操作数
					nOPCnt=3;
					strOprand2=strText.Mid(nPos2+1,nPos3-nPos2-1);
					strOprand3=strText.Mid(nPos3+1);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////

		if ( nOPCnt==0 ){						//0操作数
			//ret pushad popad pushaf popaf...

			if ( (stDisasm.cmdtype&C_TYPEMASK)==C_RET ){		//ret 海蓝底黑字
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(0,255,255));
			}else{
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(255,255,255));
			}
			AppendMsg(strOpcode);
			AlignOpcode(strOpcode);

		}else if ( nOPCnt==1 ){					//1操作数
			//ret 4; ret eax
			//push eax; push 1; pop eax; push dword ptr ds:[eax]; push dword ptr ss:[ebp]
			//jmp 400000; jmp eax; jmp dword ptr ds:[eax]; jmp dword ptr ss:[ebp]
			//jnz 400000; jnz eax; jnz dword ptr ds:[eax]; jnz dword ptr ss:[ebp]
			//call 400000; call eax; call dword ptr ds:[eax]; call dword ptr ss:[ebp]

			if ( (stDisasm.cmdtype&C_TYPEMASK)==C_JMP ){			//jmp 黄底黑字
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(255,255,0));
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_JMC ){	//条件跳转 黄底红字jpo:0x68
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

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_CAL ){	//call 海蓝底黑字
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(0,255,255));	//海蓝色背景色
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_RET ){	//ret 海蓝底黑字
				ChangeTextColor(RGB(0,0,0));									
				ChangeBackColor(RGB(0,255,255));
				AppendMsg(strOpcode);

				AlignOpcode(strOpcode);

				AppendOprand(nOPCnt,stDisasm.op[0],strOprand1);

			}else if ( (stDisasm.cmdtype&C_TYPEMASK)==C_PSH || (stDisasm.cmdtype&C_TYPEMASK)==C_POP ){	//PUSH POP 白底蓝字
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

		}else if ( nOPCnt==2 ){					//2操作数
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

			//设置白底黑字，添加一个逗号
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			AppendOprand(nOPCnt,stDisasm.op[1],strOprand2);

		}else if ( nOPCnt==3 ){					//3操作数
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

			//设置白底黑字，添加一个逗号
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			AppendOprand(nOPCnt,stDisasm.op[0],strOprand2);

			//设置白底黑字，添加一个逗号
			ChangeTextColor(RGB(0,0,0));	
			ChangeBackColor(RGB(255,255,255));
			AppendMsg(",");

			AppendOprand(nOPCnt,stDisasm.op[0],strOprand3);
		}

		//////////////////////////////////////////////////////////////////////////

		TRACE("反汇编：%s %s, %s, %s\n",strOpcode,strOprand1,strOprand2,strOprand3);

_AppendCmt:
		//添加注释，使用绿色
		int nLineLen=0;
		if ( strOpcode.GetLength()>=8 ){
			nLineLen=strText.GetLength();	//一个空格和若干个逗号都算进去了
		}else{
			nLineLen=strText.GetLength()+ (8-strOpcode.GetLength());	//加上由于调用AlignOpcode多出的空格长度
			if ( strOprand1.IsEmpty()==FALSE ){	//原本就有间隔的一个空格
				nLineLen--;
			}
		}

		if (nLineLen>50){
			strText="……\r\n";
		}else{
			char szBuff[51];
			memset(szBuff,' ',50-nLineLen);
			szBuff[50-nLineLen]=0;
			strText.Format("%s|%s\r\n",szBuff,stDisasm.comment);
		}
		ChangeTextColor(RGB(0,128,0));						
		ChangeBackColor(RGB(255,255,255));						
		AppendMsg(strText);

		//继续反汇编
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;
	}//end while

	//复制文本到剪贴板
	g_edtTemp.SetSel(0,-1);
	g_edtTemp.Copy();
}

void HexCopy(int origin,int action,void *item)
{
	CString strText;		//保存反汇编后的文本
	CString strTemp;		
	t_dump *pDump=(t_dump *)item;//pDump = item
	t_disasm stDisasm={0};	//保存反汇编后的信息
	uchar *pDecode=NULL;	//保存指令码的起始地址
	ulong nOpcodeLen;		//保存一条指令的长度，按字节计算
	BYTE buff[MAXCMDSIZE]={0};	//一段缓冲区
	DWORD RetLen=0;			//

	int nCount=0;
	/************************************************************************/
	/* 以下处理选择了一段文本后的情况，格式化处理后复制到剪贴板上           */
	/************************************************************************/

	DWORD dwSize=pDump->sel1 - pDump->sel0;	//选择的文本大小
	DWORD dwAddr=pDump->sel0;				//选择的文本起始地址
	while ( dwSize ){	
		//从指定地址开始读取解码数据，成功返回解码数据，psize＝数据大小；未成功则返回NULL,psize＝0
		Readmemory(buff,dwAddr,MAXCMDSIZE,MM_RESILENT);

		pDecode = Finddecode(dwAddr,&RetLen);

		//汇编指令到stDisasm结构
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);
		//继续反汇编
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
				if ( i==nOpcodeLen-1 && dwSize==0 ){	//最后一个
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

	//最后都加个换行
	strText+="\r\n";

	//复制到剪贴板
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
		ColorCopy(origin,action,item);
		break;
	case 1:
	case 2:
	case 3:
		HexCopy(origin,action,item);
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
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void) 
{
}


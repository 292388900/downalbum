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
CRichEditCtrl edtTemp;	//隐藏的CRichEditCtrl控件，用于设置文本格式的，作为中介
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
	edtTemp.Create(WS_CHILD|/*WS_VISIBLE|*/ES_MULTILINE|ES_WANTRETURN,
		CRect(-100,-100,10,10),AfxGetMainWnd(),0);

	memset(&m_cfDefault,0,sizeof(m_cfDefault));   
	m_cfDefault.cbSize=sizeof(CHARFORMAT2);   
	m_cfDefault.dwMask=CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_SIZE|CFM_STRIKEOUT|CFM_UNDERLINE; 
	m_cfDefault.dwEffects&=~CFE_BOLD;
	_tcscpy(m_cfDefault.szFaceName,_T("宋体"));//字体    
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
	case PM_DISASM: //子菜单显示在主窗口的插件菜单中,而PM_DISASM表示子菜单显示在反汇编窗口中
		strcpy(data,"0 &ColorCopy(带颜色格式复制)");
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
void AppendMsg(CRichEditCtrl*edtMsg,CString strMsg)
{
	LONG nLen=(LONG)edtMsg->SendMessage(WM_GETTEXTLENGTH);
	edtMsg->SetSel(nLen,-1);
	edtMsg->ReplaceSel(strMsg);
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
	LONG nLen=(LONG)edtTemp.SendMessage(WM_GETTEXTLENGTH);

	edtTemp.SetSel(nLen+nOffset,-1);

	edtTemp.SetSelectionCharFormat(m_cfDefault);
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
	LONG nLen=(LONG)edtTemp.SendMessage(WM_GETTEXTLENGTH);

	edtTemp.SetSel(nLen+nOffset,-1);   

	edtTemp.SetSelectionCharFormat(m_cfDefault);
}

void CheckEbpEsp()
{

}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	CString strText;		//保存反汇编后的文本
	CString strOprand;		//保存反汇编文本的操作码
	CString strOpcode;		//保存反汇编文本的操作数
	CString strOpcode2;
	t_dump *pDump;			//pDump = item
	t_disasm stDisasm={0};	//保存反汇编后的信息
	uchar *pDecode=NULL;	//保存指令码的起始地址
	ulong nOpcodeLen;		//保存一条指令的长度，按字节计算
	BYTE buff[0x10]={0};	//一段缓冲区
	DWORD RetLen=0;			//

	if (origin != PM_DISASM)//是否在反汇编窗口中 
		return;
	if(action != 0)			//是否是第1个菜单响应
		return;

	pDump = (t_dump *)item;
	if (pDump==NULL || pDump->size==0) //Window empty, don't add
		return;

	//没有选择任何文本
	if (pDump->sel1<=pDump->sel0)
		return;

	/************************************************************************/
	/* 以下处理选择了一段文本后的情况，格式化处理后复制到剪贴板上           */
	/************************************************************************/
	
	DWORD dwSize=pDump->sel1-pDump->sel0;	//选择的文本大小
	DWORD dwAddr=pDump->sel0;				//选择的文本起始地址
	edtTemp.Clear();						//清空编辑框
	while (dwSize)
	{	
		Readmemory(buff,dwAddr,0x10,MM_RESILENT);
		pDecode = Finddecode(dwAddr,&RetLen);
		nOpcodeLen = Disasm(buff,dwSize,dwAddr,pDecode,&stDisasm,DISASM_RTRACE,pDump->runtraceoffset);

		//恢复为黑色，添加EIP信息
		ChangeTextColor(RGB(0,0,0));
		strText.Format("%08X| ",stDisasm.ip);
		AppendMsg(&edtTemp,strText);
		
		//反汇编文本
		strText.Format("%s",stDisasm.result);

		//针对跳转指令：jmp,jnz……，call
		if (stDisasm.jmpconst||stDisasm.jmptable||stDisasm.jmpaddr)
		{
			strOprand=strText.Mid(0,8).TrimRight();
			strOpcode=strText.Mid(strOprand.GetLength());
			if ((char)strOprand[0]==(char)'j')
			{
				if((char)strOprand[1]!=(char)'m')
					ChangeTextColor(RGB(255,0,0));	//红色前景色
					ChangeBackColor(RGB(255,255,0));//黄色背景色								
			}
			else
			{
				ChangeBackColor(RGB(0,255,255));//海蓝色背景色
			}
			AppendMsg(&edtTemp,strOprand);
			ChangeBackColor(RGB(255,255,255));//恢复白色背景色

			ChangeTextColor(RGB(128,0,0));	
			AppendMsg(&edtTemp,strOpcode);
		}
		else if (*((DWORD*)&stDisasm.result[0])==(DWORD)'nter')
		{
			strOprand=strText.Mid(0,8).TrimRight();
			
			ChangeBackColor(RGB(0,255,255));//海蓝色背景色
			AppendMsg(&edtTemp,strOprand);
			ChangeBackColor(RGB(255,255,255));//恢复白色背景色
			if (nOpcodeLen>4)
			{
				strOpcode=strText.Mid(strOprand.GetLength());
				AppendMsg(&edtTemp,strOpcode);
			}
			
		}
		else//处理 ptr [ebp...]和 ptr [esp...]
		{
			int nPos=-1;
			int nLocation=-1;
			strOprand=strText.Mid(0,8);
			strOpcode=strText.Mid(strOprand.GetLength());

			if ((char)strText[0]==(char)'p')//push pop
			{
				ChangeTextColor(RGB(0,0,255));
				AppendMsg(&edtTemp,strOprand);

				if (stDisasm.immconst)//push pop后跟立即操作数
				{
					ChangeTextColor(RGB(128,128,0));
					AppendMsg(&edtTemp,strOpcode);
					goto _AppendCmt;
				}
				else//需要检测是否有ptr [ebp...]和 ptr [esp...]
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
				if (nPos<nLocation)//ptr在左侧
				{
					strOpcode=strText.Mid(8,nLocation-8);
					strOpcode2=strText.Mid(nLocation);
					ChangeBackColor(RGB(0,255,255));//海蓝色背景色
					AppendMsg(&edtTemp,strOpcode);
					ChangeBackColor(RGB(255,255,255));//恢复白色背景色
					if (stDisasm.immconst)
					{
						AppendMsg(&edtTemp,", ");
						ChangeTextColor(RGB(128,128,0));
						strOpcode2=strText.Mid(nLocation+2);
						AppendMsg(&edtTemp,strOpcode2);
					}
					AppendMsg(&edtTemp,strOpcode2);
				}
				else//ptr在右侧
				{
					strOpcode=strText.Mid(8,nLocation+2-8);
					AppendMsg(&edtTemp,strOpcode);
					strOpcode2=strText.Mid(nLocation+2);
					ChangeBackColor(RGB(0,255,255));//海蓝色背景色
					AppendMsg(&edtTemp,strOpcode2);
					ChangeBackColor(RGB(255,255,255));//恢复白色背景色
				}
				goto _AppendCmt;
			}
			else //push    dword ptr [ebp-18]	
			{
				ChangeBackColor(RGB(0,255,255));//海蓝色背景色
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
		//添加注释，使用绿色
		int nLineLen=strText.GetLength()+10;
		if (nLineLen>60)
		{
			strText="……\r\n";
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

		//继续反汇编
		dwSize-=nOpcodeLen;
		dwAddr+=nOpcodeLen;
	}//end while
	
	//复制文本到剪贴板
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


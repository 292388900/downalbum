// CppClassToCSharpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CppClassToCSharp.h"
#include "CppClassToCSharpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************************
函数名：
参  数：
返回值：
说  明：
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
static CString strKeyWord[]=
{
	" afx_msg ",
	" WINAPI "
	"const",
	"&"
};

/************************************************************************
函数名：
参  数：
返回值：
说  明：
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
static CString strLineHas[]=
{
	"enum ",
	"DECLARE_DYNAMIC"
};


// CCppClassToCSharpDlg 对话框
CCppClassToCSharpDlg::CCppClassToCSharpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCppClassToCSharpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCppClassToCSharpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edtCpp);
	DDX_Control(pDX, IDC_EDIT1, m_edtCSharp);
}

BEGIN_MESSAGE_MAP(CCppClassToCSharpDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNTRANSLATE, &CCppClassToCSharpDlg::OnBnClickedBtntranslate)
	ON_BN_DOUBLECLICKED(IDC_EDIT1, &CCppClassToCSharpDlg::OnBnClickedBtntranslate)
	ON_WM_LBUTTONDBLCLK()	
END_MESSAGE_MAP()


// CCppClassToCSharpDlg 消息处理程序
WNDPROC OldEditProc = NULL;
LRESULT NewCallWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{	
	if(Msg == WM_LBUTTONDBLCLK)
	{
		::SendMessage(hWnd,EM_SETSEL,LOWORD(0),HIWORD(-1));
		return 0;
	}
	return CallWindowProc(OldEditProc,hWnd,Msg,wParam,lParam);
}


BOOL CCppClassToCSharpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OldEditProc = (WNDPROC)SetWindowLong(m_edtCpp.m_hWnd, GWL_WNDPROC,(LONG)NewCallWindowProc);
	OldEditProc = (WNDPROC)SetWindowLong(m_edtCSharp.m_hWnd, GWL_WNDPROC,(LONG)NewCallWindowProc);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCppClassToCSharpDlg::OnPaint()
{
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCppClassToCSharpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/************************************************************************
函数名：AppendMsg
参  数：edtMsg，strMsg
返回值：void
说  明：向文本框edtMsg中追加信息，如需换行请给strMsg加："\r\n"
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
void AppendMsg(CEdit*edtMsg,CString strMsg)
{
	LONG nLen=(LONG)edtMsg->SendMessage(WM_GETTEXTLENGTH);
	edtMsg->SetSel(nLen,-1);
	edtMsg->ReplaceSel(strMsg);
}

/************************************************************************
函数名：
参  数：
返回值：
说  明：
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
BOOL DelOneLineIfHasKey(CString &strText)
{
	for (int i=0;i<2/*sizeof(strLineHas)*/;i++)
	{
		if(strText.Find(strLineHas[i])>=0)
		{
			strText="";
			return TRUE;
		}
	}
	return FALSE;
}

/************************************************************************
函数名：
参  数：
返回值：
说  明：删除在c#中不能识别的关键字
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
void DelCppKeyword(CString &strText)
{
	for (int i=0;i<3/*sizeof(strKeyWord)*/;i++)
	{
		strText.Replace(strKeyWord[i]," ");
	}
}

/************************************************************************
函数名：
参  数：
返回值：
说  明：删除void参数
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
void DelVoidArg(CString &strText)
{
	int nPos;
	nPos=strText.Find('(');
	if(nPos<0)
		return;
	nPos=strText.Find("void",nPos);
	if(nPos>0)
		strText.Delete(nPos,4);
}


/************************************************************************
函数名：
参  数：
返回值：
说  明：删除函数声明中的默认参数
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
void DelDefaultArg(CString &strText)
{
	int nPos,nLocation;
	
	nPos=strText.Find('(');
	if(nPos<0)
		return;

	nPos=strText.Find('=');
	if(nPos<0)
		return;
	nLocation=strText.Find(',',nPos);
	if(nLocation<0)
		nLocation=strText.Find(')',nPos);
	if(nLocation<0)
		return;

	strText.Delete(nPos,nLocation-nPos);
	DelDefaultArg(strText);
}

/************************************************************************
函数名：
参  数：
返回值：
说  明：
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
void TransAbsFunc(CString &strText)
{
	int nPos,nLocation;
	nPos=strText.ReverseFind('=');
	if (nPos>0)
	{
		nLocation=strText.Find(';',nPos);
		if (nLocation>0)
		{
			strText.Delete(nPos,nLocation-nPos);
		}
	}
}

/************************************************************************
函数名：
参  数：
返回值：
说  明：转换
日  期：2009-3-27
作  者：zhuxingxing
************************************************************************/
void CCppClassToCSharpDlg::OnBnClickedBtntranslate()
{
	int nLineCount=m_edtCpp.GetLineCount();
	if (nLineCount<1)//太少，		
		return;

	int nPos,nLocation;
	BOOL bVisitPropertyOn;
	CString strVisit;
	CString strText;
	CString strMsg;
	char szText[MAX_PATH*2];

	bVisitPropertyOn=FALSE;

	for (int i=0;i<nLineCount;i++)
	{
		int nRet=m_edtCpp.GetLine(i,szText,MAX_PATH*2);
		szText[nRet]=0;
		strText=szText;
		strText.TrimLeft();

		if (szText[0]=='/')//此行为注释
		{
			continue;
		}
		else if (szText[0]=='{')
		{
			strMsg="{";
		}
		else if(strText.Find("class ")==0)//类定义起始
		{
			//把":"后的继承属性去掉
			nPos=strText.Find(':');
			if (nPos>0)
			{
				nLocation=strText.Find("public ",nPos);
				if(nLocation<0)
					nLocation=strText.Find("protected ",nPos);
				if(nLocation<0)
					nLocation=strText.Find("private ",nPos);
				if(nLocation>0)
				{
					nPos=strText.Find(' ',nLocation);
					if(nPos>0)
						strMsg=strText.Mid(0,nLocation)+strText.Mid(nPos);
				}

			}
			else
				strMsg=strText;
		}
		else if (strText.Find(':')>0)//成员开始(protected:....)
		{
			nPos=strText.Find(':');
			bVisitPropertyOn=TRUE;
			strVisit=strText.Mid(0,nPos)+" ";
			continue;
		}
		else if (strText.Find("};")==0)//类定义结束
		{
			strMsg="}";
		}
		else
		{
			nPos=strText.Find(';');
			if (nPos<0)	//说明不是变量或者成员函数的声明
			{
				continue;
			}
			if(bVisitPropertyOn)
			{	
				//是变量或者成员函数的声明
			
				if(DelOneLineIfHasKey(strText))
					continue;
				DelCppKeyword(strText);
				DelDefaultArg(strText);
				DelVoidArg(strText);
				TransAbsFunc(strText);
				strMsg=strVisit+strText;
			}
			else
				strMsg=strText;
		}
		AppendMsg(&m_edtCSharp,strMsg+"\r\n");
	}
}

void CCppClassToCSharpDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDblClk(nFlags, point);
}

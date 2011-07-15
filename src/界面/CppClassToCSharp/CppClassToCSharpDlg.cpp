// CppClassToCSharpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CppClassToCSharp.h"
#include "CppClassToCSharpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************************
��������
��  ����
����ֵ��
˵  ����
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
************************************************************************/
static CString strKeyWord[]=
{
	" afx_msg ",
	" WINAPI "
	"const",
	"&"
};

/************************************************************************
��������
��  ����
����ֵ��
˵  ����
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
************************************************************************/
static CString strLineHas[]=
{
	"enum ",
	"DECLARE_DYNAMIC"
};


// CCppClassToCSharpDlg �Ի���
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


// CCppClassToCSharpDlg ��Ϣ�������
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	OldEditProc = (WNDPROC)SetWindowLong(m_edtCpp.m_hWnd, GWL_WNDPROC,(LONG)NewCallWindowProc);
	OldEditProc = (WNDPROC)SetWindowLong(m_edtCSharp.m_hWnd, GWL_WNDPROC,(LONG)NewCallWindowProc);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCppClassToCSharpDlg::OnPaint()
{
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CCppClassToCSharpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/************************************************************************
��������AppendMsg
��  ����edtMsg��strMsg
����ֵ��void
˵  �������ı���edtMsg��׷����Ϣ�����軻�����strMsg�ӣ�"\r\n"
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
************************************************************************/
void AppendMsg(CEdit*edtMsg,CString strMsg)
{
	LONG nLen=(LONG)edtMsg->SendMessage(WM_GETTEXTLENGTH);
	edtMsg->SetSel(nLen,-1);
	edtMsg->ReplaceSel(strMsg);
}

/************************************************************************
��������
��  ����
����ֵ��
˵  ����
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
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
��������
��  ����
����ֵ��
˵  ����ɾ����c#�в���ʶ��Ĺؼ���
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
************************************************************************/
void DelCppKeyword(CString &strText)
{
	for (int i=0;i<3/*sizeof(strKeyWord)*/;i++)
	{
		strText.Replace(strKeyWord[i]," ");
	}
}

/************************************************************************
��������
��  ����
����ֵ��
˵  ����ɾ��void����
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
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
��������
��  ����
����ֵ��
˵  ����ɾ�����������е�Ĭ�ϲ���
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
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
��������
��  ����
����ֵ��
˵  ����
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
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
��������
��  ����
����ֵ��
˵  ����ת��
��  �ڣ�2009-3-27
��  �ߣ�zhuxingxing
************************************************************************/
void CCppClassToCSharpDlg::OnBnClickedBtntranslate()
{
	int nLineCount=m_edtCpp.GetLineCount();
	if (nLineCount<1)//̫�٣�		
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

		if (szText[0]=='/')//����Ϊע��
		{
			continue;
		}
		else if (szText[0]=='{')
		{
			strMsg="{";
		}
		else if(strText.Find("class ")==0)//�ඨ����ʼ
		{
			//��":"��ļ̳�����ȥ��
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
		else if (strText.Find(':')>0)//��Ա��ʼ(protected:....)
		{
			nPos=strText.Find(':');
			bVisitPropertyOn=TRUE;
			strVisit=strText.Mid(0,nPos)+" ";
			continue;
		}
		else if (strText.Find("};")==0)//�ඨ�����
		{
			strMsg="}";
		}
		else
		{
			nPos=strText.Find(';');
			if (nPos<0)	//˵�����Ǳ������߳�Ա����������
			{
				continue;
			}
			if(bVisitPropertyOn)
			{	
				//�Ǳ������߳�Ա����������
			
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonDblClk(nFlags, point);
}

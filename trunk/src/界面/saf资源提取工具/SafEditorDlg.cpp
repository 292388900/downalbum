// SafEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SafEditor.h"
#include "SafEditorDlg.h"
#include "shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSafEditorDlg �Ի���




CSafEditorDlg::CSafEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSafEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSafEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edtMsg);
	DDX_Control(pDX, IDC_EDIT_SAF_FILE, m_edtSafFile);
}

BEGIN_MESSAGE_MAP(CSafEditorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT, &CSafEditorDlg::OnBnClickedButtonExtract)
END_MESSAGE_MAP()


// CSafEditorDlg ��Ϣ�������

BOOL CSafEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSafEditorDlg::OnPaint()
{
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSafEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSafEditorDlg::AppendMsg(CString strMsg)
{
	LONG nLen=(LONG)m_edtMsg.SendMessage(WM_GETTEXTLENGTH);
	m_edtMsg.SetSel(nLen,-1);
	m_edtMsg.ReplaceSel(strMsg+"\n");
}

//���ļ�srcFile�е�ƫ��nOffset��ȡnSize��С����ΪpFileName�ļ�
void SaveFile(CFile*srcFile,int nOffset,int nSize,CString strFileName)
{
	char buff[1024];
	int nReaded=0;
	ULONGLONG nReadPointer=srcFile->Seek(0,CFile::current);
	srcFile->Seek(nOffset,CFile::begin);
	CFile dstFile;
	dstFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite);
	
	while (nSize>sizeof(buff))
	{
		nReaded=srcFile->Read(buff,sizeof(buff));
		if(nReaded==0)
			break;
		dstFile.Write(buff,nReaded);
		nSize-=nReaded;
	}

	nReaded=srcFile->Read(buff,nSize);
	dstFile.Write(buff,nReaded);
	nSize-=nReaded;

	dstFile.Close();
	srcFile->Seek(nReadPointer,CFile::begin);
}

//C:\Documents and Settings\Administrator\����\29782171FeedingFrenzy\FFArchive.saf
void CSafEditorDlg::OnBnClickedButtonExtract()
{
	CString strFile;
	m_edtSafFile.GetWindowText(strFile);
	if (strFile=="")
		return;

	CString strFileRoot;
	int nPos=strFile.ReverseFind('.');
	strFileRoot=strFile.Left(nPos);
	CreateDirectory(strFileRoot,NULL);

	CString strText;
	SAFHEAD safHead;
	SAFTAIL safTail;
	SUBFILEINFO subFileInfo;
	CFile safFile;
	char szFileName[MAX_PATH*2];

	safFile.Open(strFile,CFile::modeRead);
	safFile.Seek(0,CFile::begin);
	safFile.Read(&safHead,sizeof(safHead));

	strText.Format("β��ƫ�ƣ�0x%x",safHead.nOffset);
	AppendMsg(strText);

	safFile.Seek(safHead.nOffset,CFile::begin);
	safFile.Read(&safTail,sizeof(safTail));

	strText.Format("������%d���ļ�",safTail.nFileCount);
	AppendMsg(strText);

	for (int i=0;i<safTail.nFileCount;i++)
	{
		safFile.Read(&subFileInfo,sizeof(subFileInfo));
		if (subFileInfo.nFileNameLen)
		{
			safFile.Read(szFileName,subFileInfo.nFileNameLen);
			szFileName[subFileInfo.nFileNameLen]=0;
			AppendMsg(szFileName);
		}
		CString strDstFile;
		strDstFile.Format("%s\\%s",strFileRoot,szFileName);
		strDstFile.Replace('/','\\');

		int nRootLen=strFileRoot.GetLength();
		while (TRUE)
		{
			nPos=strDstFile.Find('\\',nRootLen+1);
			if(nPos<=nRootLen)
				break;
			nRootLen=nPos;
			if (!PathFileExists(strDstFile.Left(nRootLen)))
			{
				CreateDirectory(strDstFile.Left(nRootLen),NULL);
			}
		}

		SaveFile(&safFile,subFileInfo.nFileOffset,subFileInfo.nFileSize,strDstFile);
		strText.Format("��ţ�%d �ļ�ƫ�ƣ�0x%x �ļ���С��%d",i+1,subFileInfo.nFileOffset,subFileInfo.nFileSize);
		AppendMsg(strText);
	}
	
	safFile.Close();
}

// SafEditorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SafEditor.h"
#include "SafEditorDlg.h"
#include "shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSafEditorDlg 对话框




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


// CSafEditorDlg 消息处理程序

BOOL CSafEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSafEditorDlg::OnPaint()
{
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
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

//从文件srcFile中的偏移nOffset读取nSize大小保存为pFileName文件
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

//C:\Documents and Settings\Administrator\桌面\29782171FeedingFrenzy\FFArchive.saf
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

	strText.Format("尾部偏移：0x%x",safHead.nOffset);
	AppendMsg(strText);

	safFile.Seek(safHead.nOffset,CFile::begin);
	safFile.Read(&safTail,sizeof(safTail));

	strText.Format("共含有%d个文件",safTail.nFileCount);
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
		strText.Format("编号：%d 文件偏移：0x%x 文件大小：%d",i+1,subFileInfo.nFileOffset,subFileInfo.nFileSize);
		AppendMsg(strText);
	}
	
	safFile.Close();
}

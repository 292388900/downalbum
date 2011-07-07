// �����Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�����.h"
#include "�����Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C�����Dlg �Ի���




C�����Dlg::C�����Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C�����Dlg::IDD, pParent)
	, m_nHandle(0)
	, m_strWndText(_T(""))
	, m_strClsName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C�����Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HANDLE, m_nHandle);
	DDX_Text(pDX, IDC_EDIT_WNDTEXT, m_strWndText);
	DDX_Text(pDX, IDC_EDIT_CLSNAME, m_strClsName);
}

BEGIN_MESSAGE_MAP(C�����Dlg, CDialog)
	ON_WM_TIMER()
// 	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// C�����Dlg ��Ϣ�������

BOOL C�����Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	/************************************************************************/
	SetTimer(1,500,NULL);
	/************************************************************************/

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C�����Dlg::OnPaint()
{
	CDialog::OnPaint();
}

void C�����Dlg::OnTimer(UINT_PTR nIDEvent)
{
	char lpClassName[MAX_PATH];

	POINT pt;
	GetCursorPos(&pt);
	CWnd*pWnd=WindowFromPoint(pt);
	if (pWnd!=NULL)
	{
		m_nHandle=(int)pWnd->m_hWnd;
		pWnd->GetWindowText(m_strWndText);
		GetClassName( pWnd->m_hWnd, lpClassName, sizeof(lpClassName) );
		m_strClsName=lpClassName;
	}

	UpdateData(FALSE);
}

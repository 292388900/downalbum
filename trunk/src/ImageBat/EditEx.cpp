#include "StdAfx.h"
#include "EditEx.h"



CEditEx::CEditEx(void)
{
}

CEditEx::~CEditEx(void)
{
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(ClistEx)
     ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEditEx::OnKillFocus( CWnd* pNewWnd )
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	CWnd* pParent = this->GetParent();

	::SendMessage(pParent->GetSafeHwnd(),ORANGE_LVN_ENDEDIT,0,0);
}
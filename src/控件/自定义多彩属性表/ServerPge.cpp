// ServerPge.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPropertySheetDemo.h"
#include "ServerPge.h"


// CServerPge 对话框

IMPLEMENT_DYNAMIC(CServerPge, CPropertyPage)

CServerPge::CServerPge()
	: CPropertyPage(CServerPge::IDD)
{

}

CServerPge::~CServerPge()
{
}

void CServerPge::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerPge, CPropertyPage)
END_MESSAGE_MAP()


// CServerPge 消息处理程序

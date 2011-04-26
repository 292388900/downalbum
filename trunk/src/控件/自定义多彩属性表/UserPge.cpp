// UserPge.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPropertySheetDemo.h"
#include "UserPge.h"


// CUserPge 对话框

IMPLEMENT_DYNAMIC(CUserPge, CPropertyPage)

CUserPge::CUserPge()
	: CPropertyPage(CUserPge::IDD)
{

}

CUserPge::~CUserPge()
{
}

void CUserPge::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserPge, CPropertyPage)
END_MESSAGE_MAP()


// CUserPge 消息处理程序

// StarLib.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Common/common.h"
#include "Update/Update.h"

int _tmain(int argc, _TCHAR* argv[])
{
	UPDATEINFO stUpdateInfo;
	//Star::Common::OpenUrl("www.baidu.com");
	CheckUpdateInfoIni("http://downalbum.googlecode.com/svn/trunk/url/webimagedown.xml",stUpdateInfo);
	return 0;
}


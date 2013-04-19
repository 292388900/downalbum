// StarLib.cpp : 定义控制台应用程序的入口点。
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


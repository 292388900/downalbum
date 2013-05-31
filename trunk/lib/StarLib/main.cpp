// StarLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Common/common.h"
#include "Update/Update.h"
#include "xls/XlsOperator.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//UPDATEINFO stUpdateInfo;
	//Star::Common::OpenUrl("www.baidu.com");
	//CheckUpdateInfoIni("http://downalbum.googlecode.com/svn/trunk/url/webimagedown.xml",stUpdateInfo);
	CXlsOperator xls;
	xls.Create();
	xls.AddTitle("1");
	xls.AddTitle("2");
	xls.AddTitle("3");



	xls.WriteStr(2,0,"123");
	xls.WriteStr(2,1,"456");
	xls.WriteStr(3,2,"789");

	xls.Save("d:/1.xls");

	return 0;
}


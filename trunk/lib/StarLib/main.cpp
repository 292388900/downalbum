
#pragma once

#include "stdafx.h"
#include "common\common.h"
#include "File\File.h"
#include "Process\Process.h"
#include "Encrypt\Encrypt.h"
//#include "SqliteDB\SqliteDB.h"

#include "htmldown/htmldown.h"
#include "update/Update.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//CSqliteDB sqlitedb;
	//sqlitedb.Connect( Star::Common::GetStartPath()+"sqlite.db" );
	CString strUrl="http://downalbum.googlecode.com/svn/trunk/url/QQAlbum.xml";
	UPDATEINFO stUpdate;
	CheckUpdateInfoIni(strUrl,stUpdate);


	return 0;
}


#pragma once

#include "common\common.h"
#include "File\File.h"
#include "Process\Process.h"
#include "Window\Window.h"
#include "Encrypt\Encrypt.h"
#include "SqliteDB\SqliteDB.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CSqliteDB sqlitedb;
	sqlitedb.Connect( Star::Common::GetStartPath()+"sqlite.db" );

	return 0;
}

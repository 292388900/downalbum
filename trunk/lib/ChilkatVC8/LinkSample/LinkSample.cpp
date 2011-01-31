// LinkSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// This example does nothing more than include a number of Chilkat headers
// and instantiating objects for the purpose of demonstrating a clean build
// and link (i.e. there are no unresolved externals).

// If you didn't ready the README.html that is included with the download,
// you should probably spend a few minutes reviewing it.  It tells what
// Chilkat lib should be linked, depending on the "Runtime Library" setting
// in your Project Properties --> C++ --> Code Generation settings.
// It also tells what additional Microsoft libs need to be added to your
// list of link libraries, such as ws2_32.lib, crypt32.lib, etc.

#include "../include/CkZip.h"
#include "../include/CkFtp2.h"
#include "../include/CkMailMan.h"
#include "../include/CkXml.h"
#include "../include/CkPrivateKey.h"
#include "../include/CkRsa.h"
#include "../include/CkHttp.h"
#include "../include/CkMime.h"
#include "../include/CkMht.h"
#include "../include/CkRar.h"
#include "../include/CkSsh.h"
#include "../include/CkSFtp.h"
#include "../include/CkDkim.h"
#include "../include/CkDirTree.h"
#include "../include/CkRarEntry.h"

void DoNothing(void)
{
	// Instantiate the objects...
	CkZip zip;
	CkMailMan mailman;
	CkFtp2 ftp2;
	CkXml xml;
	CkPrivateKey privKey;
	CkRsa rsa;
	CkHttp http;
	CkMime mime;
	CkMht mht;
	CkRar rar;
	CkRarEntry rarEntry;
	CkDkim dkim;
	CkDirTree dirTree;
	CkSsh ssh;
	CkSFtp sftp;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoNothing();

	return 0;
}


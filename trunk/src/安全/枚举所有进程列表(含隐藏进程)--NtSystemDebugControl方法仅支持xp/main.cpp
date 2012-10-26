// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ProcessViewer.h"
#include <locale.h>


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");

	OSVERSIONINFO os = {sizeof(os)};
	GetVersionEx(&os);
	printf("当前系统：%d.%d.%d\n",os.dwMajorVersion,os.dwMinorVersion,os.dwBuildNumber);
	if ( os.dwMajorVersion>5 ){
		printf("不支持该系统\n");
		//return 0;
	}

	vector<PROCESSINFO> vtProcesses;
	CProcessViewer viewer;
	viewer.GetProcessList(vtProcesses);
	for ( vector<PROCESSINFO>::iterator i=vtProcesses.begin(); i!=vtProcesses.end(); ++i ){
		_tprintf(_T("PID: %d ImageName: %s File: %s\n"), i->dwProcessId, i->strImageFileName, i->strImageFilePath );
	}

	system("pause");
	return 0;
}


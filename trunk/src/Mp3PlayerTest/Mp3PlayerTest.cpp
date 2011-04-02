// Mp3PlayerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Mp3PlayerTest.h"
#include "Mp3Player.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		CMp3Player mp3;
		mp3.LoadFile("c:\\1.mp3");
		//mp3.GetMp3Info();
		mp3.GetFirstMp3Frame();

		while ( mp3.GetNextMp3Frame() )
		{
		}
	}

	return nRetCode;
}

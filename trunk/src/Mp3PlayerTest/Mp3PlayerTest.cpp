// Mp3PlayerTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Mp3PlayerTest.h"
#include "Mp3Player.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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

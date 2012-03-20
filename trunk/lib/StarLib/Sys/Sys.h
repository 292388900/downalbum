
#pragma once
#include <afx.h>
#include <vector>
using namespace std;


//������Ϣ�ṹ
typedef struct TRAYINFO
{
	CString strDiscription;	//�����ı�
	CString strFilePath;		//�ļ�·��
}*PTRAYINFO;


namespace Star
{
	namespace Sys
	{
		BOOL IsWow64();
		int GetTrayInfo(HWND hWnd,vector<TRAYINFO>&vtTrays);

#if (WINVER >= 0x0600)

		//�жϲ���ϵͳ�ǲ���win7����vista
		BOOL IsVista();

		//��vista���жϵ�ǰ�û��Ƿ��ǹ���Ա
		BOOL IsAdmin();

#endif

	}
}
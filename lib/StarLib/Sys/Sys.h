
#pragma once
#include <afx.h>
#include <vector>
using namespace std;


//托盘信息结构
typedef struct TRAYINFO
{
	CString strDiscription;	//描述文本
	CString strFilePath;		//文件路径
}*PTRAYINFO;


namespace Star
{
	namespace Sys
	{
		BOOL IsWow64();
		int GetTrayInfo(HWND hWnd,vector<TRAYINFO>&vtTrays);

#if (WINVER >= 0x0600)

		//判断操作系统是不是win7或者vista
		BOOL IsVista();

		//在vista下判断当前用户是否是管理员
		BOOL IsAdmin();

#endif

	}
}
// msimg32.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"

#pragma comment(linker,"/align:4096") //减小文件大小

#pragma comment(linker,"/export:TransparentBlt=msimg.TransparentBlt")
#pragma comment(linker,"/export:GradientFill=msimg.GradientFill")
#pragma comment(linker,"/export:DllInitialize=msimg.DllInitialize")
#pragma comment(linker,"/export:AlphaBlend=msimg.AlphaBlend")
#pragma comment(linker,"/export:vSetDdrawflag=msimg.vSetDdrawflag")


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	MessageBox(0,L"injected",0,0);
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


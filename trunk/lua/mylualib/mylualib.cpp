// mylualib.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "msg/msg.h"


//#ifdef _DEBUG
#pragma comment(lib,"luaD.lib")
#pragma comment(lib,"User32.lib")
//#else
//#pragma comment(lib,"lua.lib")
//#endif


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

static const struct luaL_reg mylib [] = { 
	{"msgbox", msgbox},
	{NULL, NULL} /* sentinel */ 
}; 


extern "C" __declspec(dllexport) int luaopen_mylib (lua_State *L) { 
	// luaL_openlib(L, "mylib", mylib, 0);

	luaL_register(L, "mylib", mylib);
	return 1; 
}



#ifdef _MANAGED
#pragma managed(pop)
#endif


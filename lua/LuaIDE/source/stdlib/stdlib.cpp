// stdlib.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "../libapi/libapi.h"
#include "stdlib.h"


#ifdef _DEBUG
#pragma comment(lib,"../../lib/luaD.lib")
#pragma comment(lib,"../../lib/libapiD.lib")
#else
#pragma comment(lib,"../../lib/luaD.lib")
#pragma comment(lib,"../../lib/libapi.lib")
#endif




BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
    }
    return TRUE;
}

static int lua_trace(lua_State *L)
{
	Trace(luaL_checkstring(L, 1));
	return 0;
}

STDLIB_API int LuaRegister(lua_State* L, HWND hWnd)
{
	InitLibAPI(hWnd);

	AttachConsole();

	//lua_baselibopen(L);
	//lua_tablibopen(L);
	//lua_iolibopen(L);
	//lua_strlibopen(L);
	//lua_mathlibopen(L);

	lua_register(L, "Trace", lua_trace);

	return 0;
}



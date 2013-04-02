// mylualib.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "mylib/mylib.h"


#ifdef _DEBUG
#pragma comment(lib,"luaD.lib")
#pragma comment(lib,"User32.lib")
#else
#pragma comment(lib,"lua.lib")
#endif


#ifdef _MANAGED
#pragma managed(push, off)
#endif

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//					 )
//{
//    return TRUE;
//}

//////////////////////////////////////////////////////////////////////////

static const struct luaL_reg starlib [] = { 
	{"msgbox", msgbox},
	{"gethtml", gethtml},
	{"utf8s2ms", utf8s2ms},
	{"getluapath", getluapath},
	{"downloadfile", downloadfile},
	{"openurl", openurl},
	{"copytoclipboard", copytoclipboard},
	{"md5", md5},
	{"md5file", md5file},
	{"sleep", sleep},
	{"DecodeEscapeUsequence", DecodeEscapeUsequence},
	{"unescapexml", unescapexml},
	{"sendhttpdata", sendhttpdata},
	{"gettemppath", gettemppath},
	{"gettempfilepath", gettempfilepath},
	{"getdesktoppath", getdesktoppath},
	{"getcurrenttime", getcurrenttime},
	{"postmessage", postmessage},
	{"sendmessage", sendmessage},
	{"loadlibray", loadlibray},
	{"getprocaddress", getprocaddress},
	{"trim", trim},
	{NULL, NULL} /* sentinel */ 
}; 


extern "C" __declspec(dllexport) int luaopen_star(lua_State *L) { 
	// luaL_openlib(L, "mylib", mylib, 0);
	
	CoInitialize(NULL);
	luaL_register(L, "star", starlib);
	return 1; 
}



#ifdef _MANAGED
#pragma managed(pop)
#endif


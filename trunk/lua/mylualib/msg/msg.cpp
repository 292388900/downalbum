
#include "stdafx.h"
#include "msg.h"
#include <string>
using namespace std;

//²Î¿¼:luaB_print
int msgbox(lua_State *L) { 
	std::string str;

	int n = lua_gettop(L);  /* number of arguments */
	int i;
	lua_getglobal(L, "tostring");
	for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
			LUA_QL("msgbox"));
		if (i>1) str.append("\t");
		
		str.append(s);
		lua_pop(L, 1);  /* pop result */
	}

	::MessageBoxA(NULL,str.c_str(),"",MB_OK);

	return 0;
}


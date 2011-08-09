
#include "stdafx.h"
#include "mylib.h"
#include <StarLib/Common/common.h>
#include <StarLib/htmldown/htmldown.h>
#include <string>
using namespace std;

//参考:luaB_print
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

//html gethtml(url) 失败返回nil 
int gethtml(lua_State *L)
{
	int n =  lua_gettop(L);
	if ( n==0 ){
		lua_pushnil(L);
		return 1;
	}

	if ( lua_type(L,-1)!=LUA_TSTRING ){
		lua_pushnil(L);
		return 1;
	}
	
	const char *lpszUrl = lua_tostring(L,-1);
	if ( lpszUrl==NULL ){
		lua_pushnil(L);
		return 1;
	}
	
	CString strHtml;
	if ( GetHttpFileContent(lpszUrl,strHtml)!=0 ){
		lua_pushnil(L);
		return 1;
	}

	lua_pushstring(L,(const char *)(LPCTSTR)strHtml);
	return 1;
}

//utf8串转换为gbk的串
int utf8togbk(lua_State *L)
{
	int n = lua_gettop(L);
	if ( n==0 ){
		lua_pushnil(L);
		return 1;
	}

	if ( lua_type(L,-1)!=LUA_TSTRING ){
		lua_pushnil(L);
		return 1;
	}

	const char *lpszStr = lua_tostring(L,-1);
	if ( lpszStr==NULL ){
		lua_pushnil(L);
		return 1;
	}

	CString strText = lpszStr;
	Star::Common::ConvertUtf8ToGBK(strText);

	lua_pushstring(L,(const char *)(LPCTSTR)strText);
	return 1;
}
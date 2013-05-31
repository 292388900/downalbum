
#pragma once

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <StarLib/xls/XlsOperator.h>


extern "C" __declspec(dllexport) int luaopen_star (lua_State *L);

int msgbox(lua_State *L);
int gethtml(lua_State *L);
int utf8s2ms(lua_State *L);
int getluapath(lua_State *L);
int gettemppath(lua_State *L);
int gettempfilepath(lua_State *L);
int getdesktoppath(lua_State *L);
int downloadfile(lua_State *L);
int openurl(lua_State *L);
int copytoclipboard(lua_State *L);
int md5(lua_State *L);
int md5file(lua_State *L);
int sleep(lua_State *L);
int DecodeEscapeUsequence(lua_State *L);
int RemoveFileNameInvalidChar(lua_State *L);
int unescapexml(lua_State *L);
int sendhttpdata(lua_State *L);
int getcurrenttime(lua_State *L);
int postmessage(lua_State *L);
int sendmessage(lua_State *L);
int loadlibray(lua_State *L);
int getprocaddress(lua_State* L);
int trim(lua_State* L);

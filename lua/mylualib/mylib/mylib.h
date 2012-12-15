
#pragma once

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}


int msgbox(lua_State *L);
int gethtml(lua_State *L);
int utf8s2ms(lua_State *L);
int getluapath(lua_State *L);
int downloadfile(lua_State *L);
int openurl(lua_State *L);
int copytoclipboard(lua_State *L);
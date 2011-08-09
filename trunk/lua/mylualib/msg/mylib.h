
#pragma once

extern "C" {

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}


int msgbox(lua_State *L);
int gethtml(lua_State *L);
/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

#ifndef __XGC_LUA__
#define __XGC_LUA__

#include "commonspp.h"
#include "LuaBinder/lua.h"
#include "LuaBinder/Debug.h"
#include "LuaBinder/Util.h"
#include "LuaBinder/Manager.h"
#include "LuaBinder/ClassInfo.h"
#include "LuaBinder/PushGet.h"
#include "LuaBinder/Type.h"
#include "LuaBinder/Table.h"
#include "LuaBinder/Value.h"
#include "LuaBinder/Class.h"
#include "LuaBinder/LuaObject.h"
#include "LuaBinder/LuaTableIterator.h"
#include "LuaBinder/Iterator.h"
#include "LuaBinder/StatefulHybrid.h"
#include "LuaBinder/Script.h"

// just for help....
#define XGC_LUA_ON_LOAD( FuncName ) \
	XGC_STATIC_BLOCK( FuncName(); )

#define XGC_LUA_abs_index(L, i)     ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : \
		                    lua_gettop(L) + (i) + 1)


#endif

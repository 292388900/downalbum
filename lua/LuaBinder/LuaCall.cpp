#include "LuaBinder/Header.h"
#include "LuaBinder/LuaCall.h"
#include "LuaBinder/Script.h"

namespace XGC
{
	namespace lua
	{
		LuaCallBase::LuaCallBase( Script *L, int index) : _L(L->getState()) 
		{ 
			XGC_LUA_DEBUG_CALL; 
			getFunc(index); 
		}

		LuaCallBase::LuaCallBase( Script *L, const char *func) : _L(L->getState()) 
		{
			XGC_LUA_DEBUG_CALL;
			lua_getglobal(_L,func);
			getFunc(-1);
			lua_pop(_L,1); 
		}

		LuaCallBase::LuaCallBase( const LuaObject& obj )
			:_L(obj.m_state)
		{
			assert( obj.isfunction() );
			obj.push();
			int type = lua_type( _L, -1 );
			_ref = luaL_ref( obj.m_state, LUA_REGISTRYINDEX );
		}

		LuaCallBase::LuaCallBase( const LuaCallBase& rsh )
		{
			_L = rsh._L;
			lua_getref( rsh._L, rsh._ref );
			_ref = luaL_ref( rsh._L, LUA_REGISTRYINDEX );
			lua_pop(_L,1);
		}

		LuaCallBase& LuaCallBase::operator=( const LuaCallBase& rsh )
		{
			_L = rsh._L;
			lua_getref( rsh._L, rsh._ref );
			_ref = luaL_ref( rsh._L, LUA_REGISTRYINDEX );
			lua_pop(_L,1);

			return *this;
		}

		LuaCallBase::~LuaCallBase()
		{
			XGC_LUA_DEBUG_CALL;
			luaL_unref(_L, LUA_REGISTRYINDEX, _ref); 
		}

		bool LuaCallBase::valid()const
		{
			lua_getref( _L, _ref );
			bool ret = (_ref != LUA_REFNIL && lua_type( _L, -1 ) > LUA_TNIL );
			lua_pop(_L,1);
			return ret;
		}

		void LuaCallBase::getFunc(int index)
		{
			XGC_LUA_DEBUG_CALL;
			lua_pushvalue(_L,index);
			// assert("Invalid function!" && (lua_type(_L, -1) == LUA_TFUNCTION) );
			if( lua_type(_L, -1) == LUA_TFUNCTION )
				_ref = luaL_ref(_L, LUA_REGISTRYINDEX);
			else
				_ref = LUA_REFNIL;
		}

		int LuaCallBase::errorHandler(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			std::ostringstream out; // Use lua pushfstring and so on...
			lua_Debug debug;

			out << "SLB Exception: "
				<< std::endl << "-------------------------------------------------------"
				<< std::endl;
			out << "Lua Error:" << std::endl << "\t" 
				<<  lua_tostring(L, -1) << std::endl
				<< "Traceback:" << std::endl;
			for ( int level = 0; lua_getstack(L, level, &debug ); level++)
			{
				if (lua_getinfo(L, "Sln", &debug) )
				{
					//TODO use debug.name and debug.namewhat
					//make this more friendly
					out << "\t [ " << level << " (" << debug.what << ") ] ";
					if (debug.currentline > 0 )
					{
						out << debug.short_src << ":" << debug.currentline; 
						if (debug.name)
							out << " @ " << debug.name << "(" << debug.namewhat << ")";
					}
					out << std::endl;
				}
				else
				{
					out << "[ERROR using Lua DEBUG INTERFACE]" << std::endl;
				}
			}

			lua_pushstring(L, out.str().c_str()) ;
			return 1;
		}

		void LuaCallBase::execute(int numArgs, int numOutput, int top)
		{
			XGC_LUA_DEBUG_CALL;
			int base = lua_gettop(_L) - numArgs;
			lua_pushcfunction(_L, LuaCallBase::errorHandler);
			lua_insert(_L, base);

			if(lua_pcall(_L, numArgs, numOutput, base)) 
			{
				std::runtime_error exception( lua_tostring(_L, -1) );
				lua_remove(_L, base);
				lua_settop(_L,top); // TODO: Remove this.
				throw exception;
			}
			lua_remove(_L, base);
		}
	}
}

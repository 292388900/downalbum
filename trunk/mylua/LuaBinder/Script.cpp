#include "LuaBinder/Header.h"
#include "LuaBinder/Script.h"

namespace XGC
{
	namespace lua
	{
	#if XGC_LUA_DEBUG_LEVEL != 0
		/* Debugging function, see TODO on Script::getState */
		void ScriptHook(lua_State *L, lua_Debug *ar)
		{
			lua_getinfo(L, "Sl",ar);
			__XGC_LUA_ADJUST__();
			XGC_LUA_DEBUG_FUNC("SLB-X[%p] %s:%d",L,ar->short_src,ar->currentline );
			XGC_LUA_DEBUG_FUNC("\n");\
		}
	#endif

		Script::Script(bool default_libs) : _L(0), _loadDefaultLibs(default_libs)
		{
			XGC_LUA_DEBUG_CALL;
		}

		Script::~Script()
		{
			XGC_LUA_DEBUG_CALL;
			close();
		}
		
		lua_State* Script::getState()
		{
			XGC_LUA_DEBUG_CALL;
			if (!_L)
			{
				XGC_LUA_DEBUG(10, "Open default libs = %s", _loadDefaultLibs ? " true": " false");
				_L = luaL_newstate();
				assert("Can not create more lua_states" && (_L != 0L));
				if (_loadDefaultLibs) luaL_openlibs(_L);
				Manager::getInstance().registerSLB(_L);
			
				//TODO: Promote that functionality to a higher interface to allow proper
				//      debugging
				//
				/* if debug_level > 0 ........ */
				#if XGC_LUA_DEBUG_LEVEL != 0
				lua_sethook(_L, ScriptHook, LUA_MASKLINE, 0);
				#endif
				/* end debug */
			}
			return _L;
		}

		void Script::close()
		{
			XGC_LUA_DEBUG_CALL;
			if (_L)
			{
				lua_close(_L);
				_L = 0;
			}
		}

		void Script::callGC()
		{
			XGC_LUA_DEBUG_CALL;
			if (_L)
			{
				lua_gc(_L, LUA_GCCOLLECT, 0);
			}
		}

		void Script::doFile(const char* filename)
		{
			XGC_LUA_DEBUG_CALL;
			lua_State *L = getState();
			XGC_LUA_DEBUG(10, "filename %s = ", filename );
			if ( luaL_dofile(L, filename) )
			{
				throw std::runtime_error( lua_tostring(L,-1) );
			}
		}

		void Script::doFile( const wchar_t* filename )
		{
			XGC_LUA_DEBUG_CALL;
			lua_State *L = getState();
			XGC_LUA_DEBUG(10, "filename %s = ", filename);

			char ret[4096];
			size_t count = 0;
			wcstombs_s( &count, ret, sizeof(ret), filename, wcslen( filename ) );

			if ( luaL_dofile(L, ret) )
			{
				throw std::runtime_error( lua_tostring(L,-1) );
			}
		}

		void Script::doString(const char *o_code, const std::string &hint)
		{
			XGC_LUA_DEBUG_CALL;
			lua_State *L = getState();
			XGC_LUA_DEBUG(10, "code = %10s, hint = %s", o_code.c_str(), hint.c_str()); 
			std::stringstream code;
			code << "--" << hint << std::endl << o_code;

			if ( luaL_dostring(L, code.str().c_str()) )
			{
				throw std::runtime_error( lua_tostring(L,-1) );
			}
		}

		void Script::doString(const wchar_t *o_code, const std::wstring &hint)
		{
			XGC_LUA_DEBUG_CALL;
			lua_State *L = getState();
			XGC_LUA_DEBUG(10, "code = %10s, hint = %s", o_code.c_str(), hint.c_str()); 
			std::wstringstream code;
			code << "--" << hint << std::endl << o_code;

			char ret[4096];
			size_t count = 0;
			wcstombs_s( &count, ret, sizeof(ret), code.str().c_str(), code.str().length() );
			if ( luaL_dostring(L, ret) )
			{
				throw std::runtime_error( lua_tostring(L,-1) );
			}
		}
	}
}

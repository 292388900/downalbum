#ifndef __XGC_LUA_DEBUG__
#define __XGC_LUA_DEBUG__

#ifndef XGC_LUA_DEBUG_OUTPUT
	#define XGC_LUA_DEBUG_OUTPUT stderr
#endif

// you can redefine the function to be used to debug, should have a 
// printf-like interface.
#ifndef XGC_LUA_DEBUG_FUNC
	#include <cstdio>
	#include <cstring>
	#define XGC_LUA_DEBUG_FUNC(...) fprintf(XGC_LUA_DEBUG_OUTPUT, __VA_ARGS__);
#endif

//----------------------------------------------------------------------------
//-- Simple Debug (dissabled by default) -------------------------------------
//----------------------------------------------------------------------------
#ifndef XGC_LUA_DEBUG_LEVEL
#define XGC_LUA_DEBUG_LEVEL 0
#endif
	
#if XGC_LUA_DEBUG_LEVEL != 0
	#include "Export.h"

	extern XGC_LUA_EXPORT int XGC_LUA_DEBUG_LEVEL_TAB;

	inline void __XGC_LUA_ADJUST__(bool terminator = true)
	{
		if (XGC_LUA_DEBUG_LEVEL_TAB)
		{
			for(int i = 0; i < XGC_LUA_DEBUG_LEVEL_TAB-1; ++i) XGC_LUA_DEBUG_FUNC("| ");
			if (terminator) XGC_LUA_DEBUG_FUNC("|_");
		}
	}

	#define XGC_LUA_DEBUG(level,...) if (level <= XGC_LUA_DEBUG_LEVEL)\
		{\
			__dummy__XGC_LUA__debugcall.check(); /* to check a previous XGC_LUA_DEBUG_CALL */ \
			int __s = strlen(__FILE__); \
			int __offset = (__s > 18)? __s - 18: 0; \
			__XGC_LUA_ADJUST__();\
			XGC_LUA_DEBUG_FUNC("SLB-%-2d [%12s:%-4d] ", level, __FILE__+__offset, __LINE__);\
			XGC_LUA_DEBUG_FUNC(__VA_ARGS__);\
			XGC_LUA_DEBUG_FUNC("\n");\
		}

	#define XGC_LUA_DEBUG_STACK(level, L,  ... ) \
		{\
			XGC_LUA_DEBUG(level, " {stack} "  __VA_ARGS__ );\
			int top = lua_gettop(L); \
			for(int i = 1; i <= top; i++) \
			{ \
				if (lua_type(L,i) == LUA_TNONE) \
				{ \
					XGC_LUA_DEBUG(level, "\targ %d = (Invalid)", i);\
				} \
				else \
				{ \
					lua_pushvalue(L,i);\
					XGC_LUA_DEBUG(level+1, "\targ %d = %s -> %s", i, \
						lua_typename(L,lua_type(L,-1)), lua_tostring(L,-1) );\
					lua_pop(L,1);\
				} \
			}\
		}

		#include<sstream>
		#include<stdexcept>
		#include "lua.h"
	
		struct __XGC_LUA__cleanstack
		{
			__XGC_LUA__cleanstack(struct lua_State *L, int delta, const char *where, int line)
				: L(L), delta(delta), where(where), line(line)
			{
				top = lua_gettop(L);
			}
			~__XGC_LUA__cleanstack()
			{
				if (top+delta != lua_gettop(L))
				{
					std::ostringstream out;
					out << where << ":" << line << " -> ";
					out << "Invalid Stack Check. current = " << lua_gettop(L) << " expected = " << top + delta << std::endl;
					throw std::runtime_error(out.str());
				}
			}

			struct lua_State *L;
			int top;
			int delta;
			const char *where;
			int line;
		};

	#define XGC_LUA_DEBUG_CLEAN_STACK(Lua_Stack_L, delta)  \
		__XGC_LUA__cleanstack __dummy__XGC_LUA__cleanstack__(Lua_Stack_L, delta, __FILE__, __LINE__);

		struct __XGC_LUA__debugcall
		{
			__XGC_LUA__debugcall(const char *f, int l, const char *n)
				: file(f), line(l), name(n)
			{
				int s = ::strlen(__FILE__);
				int offset = (s > 18)? s - 18: 0;
				file = file + offset;
				__XGC_LUA_ADJUST__();
				XGC_LUA_DEBUG_FUNC("SLB >>> [%12s:%-4d] %s\n", file, line, name);
				XGC_LUA_DEBUG_LEVEL_TAB++;
			}

			~__XGC_LUA__debugcall()
			{
				__XGC_LUA_ADJUST__();
				XGC_LUA_DEBUG_FUNC("SLB <<< [%12s:%-4d] %s\n", file, line, name);
				__XGC_LUA_ADJUST__(false);
				XGC_LUA_DEBUG_FUNC("\n");
				XGC_LUA_DEBUG_LEVEL_TAB--;
			}

			void check() const {}


			const char *file;
			int line;
			const char *name;

		};
	#define XGC_LUA_DEBUG_CALL \
		__XGC_LUA__debugcall __dummy__XGC_LUA__debugcall(__FILE__,__LINE__,__FUNCTION__);

#else
	#define XGC_LUA_DEBUG(level,...)
	#define XGC_LUA_DEBUG_STACK(... ) 
	#define XGC_LUA_DEBUG_CLEAN_STACK(...)
	#define XGC_LUA_DEBUG_CALL
#endif

#endif

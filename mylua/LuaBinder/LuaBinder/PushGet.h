#ifndef __XGC_LUA_PUSH_GET__
#define __XGC_LUA_PUSH_GET__

// Fix Apple problems
#ifdef check
    #undef check
#endif

namespace XGC 
{
	namespace lua
	{

		//----------------------------------------------------------------------------
		//-- Push/Get/Check functions ------------------------------------------------
		//----------------------------------------------------------------------------
			template<class T> void push(lua_State *L, T v);
			template<class T> T get(lua_State *L, int pos);
			template<class T> bool check(lua_State *L, int pos);
			template<class T> void setGlobal(lua_State *L, T v, const char *name);
			template<class T> T getGlobal(lua_State *L, const char *name);
		//----------------------------------------------------------------------------

		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------

		namespace Private {
			template<class C>
			struct Type;
		}

		template<class T>
		inline void push(lua_State *L, T v)
		{
			XGC_LUA_DEBUG_CALL; 
			Private::Type<T>::push(L,v);
		}

		// get function based on Private::Type<class>
		template<class T>
		inline T get(lua_State *L, int pos)
		{
			XGC_LUA_DEBUG_CALL; 
			return Private::Type<T>::get(L,pos);
		}

		template<class T>
		inline bool check(lua_State *L, int pos)
		{
			XGC_LUA_DEBUG_CALL; 
			return Private::Type<T>::check(L,pos);
		}
		template<class T> 
		inline void setGlobal(lua_State *L, T v, const char *name)
		{
			XGC_LUA_DEBUG_CALL; 
			XGC::lua::push(L,v);
			lua_setglobal(L,name);
		}

		//#define XGC_LUA_REPEAT(N) \
		//template<class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)>\
		//template<>\
		//inline void setGlobal(lua_State *L, R (*func)(XGC_ENUM_D(N,T)), const char *name ) \
		//{\
		//	XGC_LUA_DEBUG_CALL;\
		//	xgc::lua::push(L,FuncCall::create(func));\
		//	lua_setglobal(L,name);\
		//};\
		//XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
		//#undef XGC_LUA_REPEAT

		template<class T>
		inline T getGlobal(lua_State *L, const char *name)
		{
			XGC_LUA_DEBUG_CALL; 
			lua_getglobal(L,name);
			T value = XGC::lua::get<T>(L, -1);
			lua_pop(L,1); // remove the value
			return value;
		}
	}
} // end of SLB namespace

#endif

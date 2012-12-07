#ifndef __XGC_LUA_LUACALL__
#define __XGC_LUA_LUACALL__

#include "LuaBinder/Export.h"
#include "LuaBinder/Object.h"
#include "LuaBinder/PushGet.h"
#include "LuaBinder/Type.h"

namespace XGC
{
	namespace lua
	{
		class Script;
		class XGC_LUA_EXPORT LuaCallBase 
		{ 
		public:
			// this allows to store a luaCall, mainly used by
			// Hybrid classes...
			virtual ~LuaCallBase();
			bool valid()const;
		protected: 
			LuaCallBase( Script *L, int index);
			LuaCallBase( Script *L, const char *func);
			LuaCallBase( const LuaObject& obj );
			LuaCallBase( const LuaCallBase& rsh );
			LuaCallBase& operator= ( const LuaCallBase& rsh );

			void execute(int numArgs, int numOutput, int top);

			lua_State *_L;
			int _ref; 
		private:
			void getFunc(int index);
			static int errorHandler(lua_State *L);
		}; 

		template<typename T>
		struct LuaCall;

		#define XGC_LUA_ARG(N) T##N arg_##N, 
		#define XGC_LUA_PUSH_ARGS(N) push<T##N>(_L, arg_##N );

		#define XGC_LUA_REPEAT(N) \
		\
			/* LuaCall: functions that return something  */ \
			template<class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
			struct XGC_LUA_EXPORT LuaCall<R( XGC_ENUM_D(N,T) )> : public LuaCallBase\
			{ \
				LuaCall(Script *L, int index) : LuaCallBase(L,index) {} \
				LuaCall(Script *L, const char *func) : LuaCallBase(L,func) {} \
				LuaCall(const LuaObject& obj ) : LuaCallBase( obj ) {} \
				LuaCall(const LuaCall& rsh ) : LuaCallBase( rsh ) {} \
				R operator()( XGC_REPEAT( N, XGC_LUA_ARG) char dummyARG = 0) /*TODO: REMOVE dummyARG */\
				{ \
					int top = lua_gettop(_L); \
					lua_rawgeti(_L, LUA_REGISTRYINDEX,_ref); \
					int type = lua_type( _L, -1 );\
					XGC_REPEAT( N, XGC_LUA_PUSH_ARGS ); \
					execute(N, 1, top); \
					R result = get<R>(_L, -1); \
					lua_settop(_L,top); \
					return result; \
				} \
				bool operator==(const LuaCall& lc) { return (_L == lc._L && _ref == lc._ref); }\
				LuaCall& operator=( const LuaCall& rsh ) { LuaCallBase::operator=( static_cast< const LuaCallBase& >(rsh) ); return *this; }\
			};
		XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
		#undef XGC_LUA_REPEAT

		#define XGC_LUA_REPEAT(N) \
		\
			/*LuaCall: functions that doesn't return anything */  \
			template<XGC_ENUM_D(N, class T)> \
			struct XGC_LUA_EXPORT LuaCall<void( XGC_ENUM_D(N,T) )> : public LuaCallBase\
			{ \
				LuaCall(Script *L, int index) : LuaCallBase(L,index) {} \
				LuaCall(Script *L, const char *func) : LuaCallBase(L,func) {} \
				LuaCall(const LuaObject& obj ) : LuaCallBase( obj ) {} \
				LuaCall(const LuaCall& rsh ) : LuaCallBase( rsh ) {} \
				void operator()( XGC_REPEAT( N, XGC_LUA_ARG) char dummyARG = 0) /*TODO: REMOVE dummyARG */\
				{ \
					int top = lua_gettop(_L); \
					lua_rawgeti(_L, LUA_REGISTRYINDEX,_ref); \
					XGC_REPEAT( N, XGC_LUA_PUSH_ARGS ); \
					execute(N, 0, top); \
					lua_settop(_L,top); \
				} \
				bool operator==(const LuaCall& lc) { return (_L == lc._L && _ref == lc._ref); }\
				LuaCall& operator=( const LuaCall& rsh ) { LuaCallBase::operator=( static_cast< const LuaCallBase& >(rsh) ); return *this; }\
			}; \

		XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
		#undef XGC_LUA_REPEAT
		#undef XGC_LUA_ARG
		#undef XGC_LUA_PUSH_ARGS
	}
} //end of SLB namespace

	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
#endif

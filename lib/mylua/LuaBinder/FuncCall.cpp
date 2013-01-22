#include "LuaBinder/Header.h"
#include "LuaBinder/FuncCall.h"

namespace XGC
{
	namespace lua
	{
		FuncCall::FuncCall() : _Treturn(0)
		{
			XGC_LUA_DEBUG_CALL;
			XGC_LUA_DEBUG(10, "Create FuncCall (%p)",this);
		}

		FuncCall::~FuncCall()
		{
			XGC_LUA_DEBUG_CALL;
			XGC_LUA_DEBUG(10, "Delete FuncCall (%p)",this);
		}

		
		void FuncCall::pushImplementation(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			lua_pushlightuserdata(L, (FuncCall*) this);
			lua_pushcclosure(L,FuncCall::_call, 1);
		}
		
		int FuncCall::_call(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			FuncCall *fc = (FuncCall*) lua_touserdata(L,lua_upvalueindex(1));
			assert("Invalid FuncCall" && fc);
			return fc->call(L);
		}
		
		void FuncCall::setArgComment(size_t p, const std::string& c)
		{
			XGC_LUA_DEBUG_CALL;
			if (p < _Targs.size())
			{
				_Targs[p].second = c;
			}
			else
			{
				//TODO warning or exception here.
			}
		}
		

		/* For lua functions.... */
		class LuaCFunction : public FuncCall
		{
		public:
			LuaCFunction(lua_CFunction f) : _func(f) { XGC_LUA_DEBUG_CALL; }
		protected:
			void pushImplementation(lua_State *L) {XGC_LUA_DEBUG_CALL; lua_pushcfunction(L,_func); }
			virtual int call(lua_State *L)
			{
				XGC_LUA_DEBUG_CALL;
				luaL_error(L, "Code should never be reached %s:%d",__FILE__,__LINE__);
				return 0;
			}
			virtual ~LuaCFunction() { XGC_LUA_DEBUG_CALL; }
			lua_CFunction _func;
		};

		FuncCall* FuncCall::create(lua_CFunction f)
		{
			XGC_LUA_DEBUG_CALL;
			return new LuaCFunction(f);
		}
	}
}

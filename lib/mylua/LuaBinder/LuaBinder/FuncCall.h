#ifndef __XGC_LUA_FUNCCALL__
#define __XGC_LUA_FUNCCALL__

#include "LuaBinder/Object.h"
#include "LuaBinder/Export.h"

namespace XGC
{
	namespace lua
	{
		class XGC_LUA_EXPORT FuncCall : public Object
		{
		public:

			#define XGC_LUA_REPEAT(N) \
			\
				/* FunCall for class Methods */ \
				template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
				static FuncCall* create(R (C::*func)(XGC_ENUM_D(N,T)) ); \
			\
				/* FunCall for CONST class Methods */ \
				template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
				static FuncCall* create(R (C::*func)(XGC_ENUM_D(N,T)) const ); \
			\
				/* (explicit) FunCall for CONST class Methods */ \
				template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
				static FuncCall* createConst(R (C::*func)(XGC_ENUM_D(N,T)) const ); \
			\
				/* (explicit) FunCall for NON-CONST class Methods */ \
				template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
				static FuncCall* createNonConst(R (C::*func)(XGC_ENUM_D(N,T))); \
			\
				/* FunCall for C-functions  */ \
				template<class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
				static FuncCall* create(R (func)(XGC_ENUM_D(N,T)) ); \
			\
				/* FunCall Class constructors  */ \
				template<class C XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
				static FuncCall* classConstructor(); \

			XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
			#undef XGC_LUA_REPEAT

			/* special case of a proper lua Function */
			static FuncCall* create(lua_CFunction f);

			size_t getNumArguments() const { return _Targs.size(); }
			const std::type_info* getArgType(size_t p) const { return _Targs[p].first; }
			const std::string& getArgComment(size_t p) const { return _Targs[p].second; }
			const std::type_info* getReturnedType() const { return _Treturn; }
			void setArgComment(size_t p, const std::string& c);

		protected:
			FuncCall();
			virtual ~FuncCall();
		
			void pushImplementation(lua_State *L);
			virtual int call(lua_State *L) = 0;

			std::vector< std::pair<const std::type_info*, std::string> > _Targs;
			const std::type_info* _Treturn;
		private:
			static int _call(lua_State *L);

		friend class Manager;	
		friend class ClassInfo;	
		};
	}
} //end of SLB namespace
//--------------------------------------------------------------------
// Inline implementations:
//--------------------------------------------------------------------
namespace XGC 
{
	namespace lua
	{
		#include "Private_FuncCall.h"

		#define XGC_LUA_REPEAT(N) \
		\
		template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		inline FuncCall* FuncCall::create(R (C::*func)(XGC_ENUM_D(N,T)) ) \
		{ \
			return createNonConst(func); \
		} \
		\
		template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		inline FuncCall* FuncCall::create(R (C::*func)(XGC_ENUM_D(N,T)) const ) \
		{ \
			return createConst(func); \
		} \
		\
		template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		inline FuncCall* FuncCall::createConst(R (C::*func)(XGC_ENUM_D(N,T)) const ) \
		{ \
			return new Private::FC_ConstMethod<C,R(XGC_ENUM_D(N,T))>(func); \
		} \
		template<class C,class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		inline FuncCall* FuncCall::createNonConst(R (C::*func)(XGC_ENUM_D(N,T)) ) \
		{ \
			return new Private::FC_Method<C,R(XGC_ENUM_D(N,T))>(func); \
		} \
			\
		template<class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		inline FuncCall* FuncCall::create(R (*func)(XGC_ENUM_D(N,T)) ) \
		{ \
			return new Private::FC_Function<R(XGC_ENUM_D(N,T))>(func);\
		} \
			\
		template<class C XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		inline FuncCall* FuncCall::classConstructor() \
		{ \
			return new Private::FC_ClassConstructor<C(XGC_ENUM_D(N,T))>;\
		} \

		XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
		#undef XGC_LUA_REPEAT
	}
}
#endif

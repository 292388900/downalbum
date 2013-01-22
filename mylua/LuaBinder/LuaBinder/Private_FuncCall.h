#ifndef __XGC_LUA_PRIVATE_FUNC_CALL__
#define __XGC_LUA_PRIVATE_FUNC_CALL__

namespace Private 
{
	//----------------------------------------------------------------------------
	//-- FuncCall Implementations ------------------------------------------------
	//----------------------------------------------------------------------------
	template<class T>
	class FC_Function; //> FuncCall to call functions (C static functions)

	template<class C, class T>
	class FC_Method; //> FuncCall to call Class methods

	template<class C, class T>
	class FC_ConstMethod; //> FuncCall to call Class const methods

	template<class> 
	class FC_ClassConstructor;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

	// XGC_LUA_INFO: Collects info of the arguments
	#define XGC_LUA_INFO_PARAMS(N) _Targs.push_back(\
			std::pair<const std::type_info*, std::string>( &typeid(T##N), "") ); 
	#define XGC_LUA_INFO(RETURN, N) \
		_Treturn = &typeid(RETURN);\
		XGC_REPEAT(N,XGC_LUA_INFO_PARAMS ) \

	// XGC_LUA_GET: Generates Code to get N parameters 
	//
	//    N       --> Number of parameters
	//    START   --> where to start getting parameters
	//                n=0   means start from the top
	//                n>0   start at top+n (i.e. with objects first parameter is the object)
	//
	//    For each paramter a param_n variable is generated with type Tn
	#define XGC_LUA_GET_PARAMS(N, START) T##N param_##N = XGC::lua::Private::Type<T##N>::get(L,N + (START) );
	#define XGC_LUA_GET(N,START) \
		if (lua_gettop(L) != N + (START) ) \
		{ \
			luaL_error(L, "Error number of arguments (given %d -> expected %d)", \
					lua_gettop(L)-(START), N); \
		}\
		XGC_REPEAT_BASE(N,XGC_LUA_GET_PARAMS, (START) ) \
		

	// FC_Method (BODY) 
	//    N       --> Number of parameters
	//
	// ( if is a const method )
	//    NAME    --> FC_Method   |  FC_ConstMethod
	//    CONST   --> /*nothing*/ |  const 
	// 
	// ( if returns or not a value)
	//    HEADER  --> class R    |  /*nothing*/
	//    RETURN  --> R          |  void 
	//
	//    ...     --> implementation of call function
	#define XGC_LUA_FC_METHOD_BODY(N,NAME, CONST, HEADER,RETURN, ...) \
	\
		template<class C HEADER XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		class NAME <C,RETURN (XGC_ENUM_D(N,T))> : public FuncCall { \
		public: \
			NAME( RETURN (C::*func)(XGC_ENUM_D(N,T)) CONST ) : _func(func) \
			{\
				XGC_LUA_INFO(RETURN, N) \
			}\
		protected: \
			int call(lua_State *L) \
			{ \
				__VA_ARGS__ \
			} \
		private: \
			RETURN (C::*_func)(XGC_ENUM_D(N,T)) CONST; \
		}; \


	// FC_Method (implementation with return or not a value):
	// ( if is a const method )
	//    NAME    --> FC_Method   |  FC_ConstMethod
	//    CONST   --> /*nothing*/ |  const 
	#define XGC_LUA_FC_METHOD(N, NAME, CONST) \
		XGC_LUA_FC_METHOD_BODY(N, NAME, CONST, XGC_COMMA class R ,R, \
				CONST C *obj = XGC::lua::get<CONST C*>(L,1); \
				if (obj == 0) luaL_error(L, "Invalid object for this method");\
				XGC_LUA_GET(N,1) \
				R value = (obj->*_func)(XGC_ENUM_D(N,param_)); \
				XGC::lua::push<R>(L, value); \
				return 1; \
			) \
		XGC_LUA_FC_METHOD_BODY(N, NAME, CONST, /*nothing*/ , void,	\
				CONST C *obj = XGC::lua::get<CONST C*>(L,1); \
				if (obj == 0) luaL_error(L, "Invalid object for this method");\
				XGC_LUA_GET(N,1) \
				(obj->*_func)(XGC_ENUM_D(N,param_)); \
				return 0; \
			)

	// FC_Method (with const methods or not)
	#define XGC_LUA_REPEAT(N) \
		XGC_LUA_FC_METHOD(N, FC_ConstMethod,  const) /* With const functions */ \
		XGC_LUA_FC_METHOD(N, FC_Method, /* nothing */ ) /* with non const functions */

	XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
	#undef XGC_LUA_REPEAT


	// FC_Function (Body)
	//    N       --> Number of parameters
	//
	// ( if returns or not a value)
	//    HEADER  --> class R    |  /*nothing*/
	//    RETURN  --> R          |  void 
	//
	//    ...     --> implementation of call function
	#define XGC_LUA_FC_FUNCTION_BODY(N, HEADER, RETURN, ...) \
	\
		template< HEADER  XGC_ENUM_D(N, class T)> \
		class FC_Function< RETURN (XGC_ENUM_D(N,T))> : public FuncCall { \
		public: \
			FC_Function( RETURN (*func)(XGC_ENUM_D(N,T)) ) : _func(func) {\
				XGC_LUA_INFO(RETURN, N) \
			} \
		protected: \
			virtual ~FC_Function() {} \
			int call(lua_State *L) \
			{ \
				__VA_ARGS__ \
			} \
		private: \
			RETURN (*_func)(XGC_ENUM_D(N,T)); \
		}; 
	
	#define XGC_LUA_FC_FUNCTION(N) \
		XGC_LUA_FC_FUNCTION_BODY( N, class R XGC_COMMA_IF(N), R, \
				XGC_LUA_GET(N,0) \
				R value = (_func)(XGC_ENUM_D(N,param_)); \
				XGC::lua::push<R>(L, value); \
				return 1; \
		)\
		XGC_LUA_FC_FUNCTION_BODY( N, /* nothing */ , void, \
				XGC_LUA_GET(N,0) \
				(_func)(XGC_ENUM_D(N,param_)); \
				return 0; \
		)

	XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_FC_FUNCTION)
	#undef XGC_LUA_FC_METHOD
	#undef XGC_LUA_FC_METHOD_BODY
	#undef XGC_LUA_FC_FUNCTION
	#undef XGC_LUA_FC_FUNCTION_BODY

	#define XGC_LUA_REPEAT(N) \
		template<class C XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
		struct FC_ClassConstructor<C(XGC_ENUM_D(N,T))> : public FuncCall\
		{\
		public:\
			FC_ClassConstructor() {} \
		protected: \
			int call(lua_State *L) \
			{ \
				ClassInfo *c = Manager::getInstance().getClass(typeid(C)); \
				if (c == 0) luaL_error(L, "Class %s is not avaliable! ", typeid(C).name()); \
				XGC_LUA_GET(N, 0); \
				Private::Type<C*>::push(L, new C( XGC_ENUM_D(N,param_) ), true ); \
				return 1; \
			} \
		}; \

	XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
	#undef XGC_LUA_REPEAT

	#undef XGC_LUA_GET
	#undef XGC_LUA_GET_PARAMS
} // end of xgc::lua::Private namespace	
#endif

#ifndef __XGC_LUA_HYBRID__
#define __XGC_LUA_HYBRID__

#include "LuaBinder/Export.h"
#include "LuaBinder/Manager.h"
#include "LuaBinder/LuaCall.h"
#include "LuaBinder/Value.h"
#include "LuaBinder/ClassInfo.h"
#include "LuaBinder/Instance.h"

namespace XGC 
{
	namespace lua
	{
		class HybridBase;
			
		struct XGC_LUA_EXPORT InvalidMethod : public std::exception
		{	
			InvalidMethod(const HybridBase*, const char *c);
			~InvalidMethod() throw() {}
			const char* what() const throw() { return _what.c_str(); }
			std::string _what;
		};

		class XGC_LUA_EXPORT HybridBase {
		public:

			/** Returns the lua_State, this function will be valid if the object is
			 * attached, otherwise will return 0 */
			lua_State* getLuaState() const { return _L; }

			/** Indicates where this instance will look for its hybrid methods; */
			void attach(lua_State *);
			bool isAttached() const { return (_L != 0); }

			/** use this to release memory allocated by the hybrid object, inside
			 * the lua_State.*/
			void unAttach(); 

			/** Use this function to register this class as hybrid, it will override
			 * ClassInfo metamethods of class__index, class__newindex and object__index
			 * if your class requires those methods contact me to see if it is possible
			 * to do it, by the moment this is the only way this works */
			static void registerAsHybrid(ClassInfo *ci);

		protected:
			typedef std::map< const char *, LuaCallBase *> MethodMap;

			HybridBase();
			virtual ~HybridBase();

			/** This method allows to lock the lua_State when the state is not owned
			 * by the instance (when the link was done using linkFromLuaTable). Here
			 * you can add lock mechanism to avoid two instances accessing the same
			 * lua_State */
			virtual void lockBegin(lua_State *) {}

			/** This method is called at the end of the call, see lockBegin */
			virtual void lockEnd(lua_State *) {}


			//-- Private data -----------------------------------------------------
			// [-1, (0|+1)]
			bool getMethod(const char *name) const;
			virtual ClassInfo* getClassInfo() const = 0;
			void clearMethodMap();


			mutable MethodMap _methods;
			mutable ref_ptr<Table> _subclassMethods;

			friend struct InternalHybridSubclass;
			friend struct InvalidMethod;
			//-- Private data -----------------------------------------------------

		private:
			lua_State * _L;
			int _global_environment;

			// pops a key,value from tom and sets as our method
			// [-2,0]
			static void setMethod(lua_State *L, ClassInfo *ci);

			static int call_lua_method(lua_State *L);
			static int class__newindex(lua_State *);
			static int class__index(lua_State *);
			static int object__index(lua_State *);

		public:
			// This class helps to handle the lockBegin, lockEnd. Using methods
			// directly will require to split LCall to handle return of void or not.
			// (this is a little trick). Also this class issues a garbage collect
			// operation, that's the only way to keep clean objects like smart
			// pointers and so on.
			struct XGC_LUA_EXPORT AutoLock
			{
				AutoLock(const HybridBase *hconst);
				~AutoLock();
				HybridBase* _hybrid;
			};

		};

		template<class BaseClass>
		class Hybrid : public virtual HybridBase {
		public:
			Hybrid()
			{
				ClassInfo *c;
				c = Manager::getInstance().getOrCreateClass( typeid(BaseClass) );
				if (!c->initialized())
				{
					// Give a default instance factory... that only is able
					// to handle push/get of pointers without handling 
					// construction, copy, delete, ...
					c->setInstanceFactory(
						new InstanceFactoryAdapter< BaseClass,
							Instance::NoCopyNoDestroy::Implementation<BaseClass> >()
					);
				}
			}
			virtual ~Hybrid() {}

		protected:
			ClassInfo* getClassInfo() const
			{
				return Manager::getInstance().getClass( typeid(BaseClass) );
			}
			
		#define XGC_LUA_ARG_H(N) ,T##N arg_##N
		#define XGC_LUA_ARG(N) , arg_##N
		#define XGC_LUA_BODY(N) \
				\
				AutoLock __dummy__lock(this); \
				LC *method = 0; \
				XGC_LUA_DEBUG(3,"Call Hybrid-method [%s]", name)\
				lua_State *L = getLuaState(); \
				{\
					XGC_LUA_DEBUG_CLEAN_STACK(L,0)\
					MethodMap::iterator it = _methods.find(name) ; \
					if (it != _methods.end()) \
					{ \
						method = reinterpret_cast<LC*>(it->second); \
						XGC_LUA_DEBUG(4,"method [%s] was found %p", name,method)\
					} \
					else \
					{ \
						if (getMethod(name)) \
						{ \
							method = new LC(L, -1);\
							lua_pop(L,1); /*method is stored in the luaCall*/\
							XGC_LUA_DEBUG(2,"method [%s] found in lua [OK] -> %p", name,method)\
							_methods[name] = method;\
						} \
						else \
						{ \
							_methods[name] = 0L; \
							XGC_LUA_DEBUG(2,"method [%s] found in lua [FAIL!]", name)\
						}\
					}\
					if (!method) throw InvalidMethod(this, name);\
				}\

		#define XGC_LUA_REPEAT(N) \
		\
			/* non const version */\
			template<class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
			R LCall( const char *name XGC_REPEAT(N, XGC_LUA_ARG_H) ) \
			{ \
				XGC_LUA_DEBUG_CALL;\
				typedef XGC::lua::LuaCall<R(BaseClass* XGC_COMMA_IF(N) XGC_ENUM_D(N,T))> LC;\
				XGC_LUA_BODY(N) \
				return (*method)(static_cast<BaseClass*>(this) XGC_REPEAT(N, XGC_LUA_ARG) ); \
			} \
			/* const version */\
			template<class R XGC_COMMA_IF(N) XGC_ENUM_D(N, class T)> \
			R LCall( const char *name XGC_REPEAT(N, XGC_LUA_ARG_H) ) const \
			{ \
				XGC_LUA_DEBUG_CALL;\
				typedef XGC::lua::LuaCall<R(const BaseClass* XGC_COMMA_IF(N) XGC_ENUM_D(N,T))> LC;\
				XGC_LUA_BODY(N) \
				return (*method)(static_cast<const BaseClass*>(this) XGC_REPEAT(N, XGC_LUA_ARG) ); \
			} \

		XGC_MAIN_REPEAT_Z(MAX,XGC_LUA_REPEAT)
		#undef XGC_LUA_REPEAT
		#undef XGC_LUA_BODY
		#undef XGC_LUA_ARG
		#undef XGC_LUA_ARG_H
		};
	}
}

#define HYBRID_method_0(name,ret_T) \
	ret_T name() { XGC_LUA_DEBUG_CALL; return LCall<ret_T>(#name); }
#define HYBRID_method_1(name,ret_T, T1) \
	ret_T name(T1 p1) { XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1>(#name,p1); }
#define HYBRID_method_2(name,ret_T, T1, T2) \
	ret_T name(T1 p1,T2 p2) { XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2); }
#define HYBRID_method_3(name,ret_T, T1, T2, T3) \
	ret_T name(T1 p1,T2 p2, T3 p3) { XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2, p3); }
#define HYBRID_method_4(name,ret_T, T1, T2, T3, T4) \
	ret_T name(T1 p1,T2 p2, T3 p3, T4 p4) {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2, p3,p4); }
#define HYBRID_method_5(name,ret_T, T1, T2, T3, T4,T5) \
	ret_T name(T1 p1,T2 p2, T3 p3, T4 p4, T5 p5) {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2, p3,p4,p5); }

#define HYBRID_const_method_0(name,ret_T) \
	ret_T name() const {  XGC_LUA_DEBUG_CALL; return LCall<ret_T>(#name); }
#define HYBRID_const_method_1(name,ret_T, T1) \
	ret_T name(T1 p1) const {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1>(#name,p1); }
#define HYBRID_const_method_2(name,ret_T, T1, T2) \
	ret_T name(T1 p1,T2 p2) const {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2); }
#define HYBRID_const_method_3(name,ret_T, T1, T2, T3) \
	ret_T name(T1 p1,T2 p2, T3 p3) const {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2, p3); }
#define HYBRID_const_method_4(name,ret_T, T1, T2, T3, T4) \
	ret_T name(T1 p1,T2 p2, T3 p3, T4 p4) const {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2, p3,p4); }
#define HYBRID_const_method_5(name,ret_T, T1, T2, T3, T4,T5) \
	ret_T name(T1 p1,T2 p2, T3 p3, T4 p4, T5 p5) const {  XGC_LUA_DEBUG_CALL; return LCall<ret_T,T1,T2>(#name,p1,p2, p3,p4,p5); }

#endif

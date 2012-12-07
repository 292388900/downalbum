#ifndef __XGC_LUA_TYPE__
#define __XGC_LUA_TYPE__

#include "LuaBinder/Manager.h"
#include "LuaBinder/ClassInfo.h"
#include "LuaBinder/LuaObject.h"

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
namespace XGC 
{
	namespace lua
	{
		namespace Private 
		{
			// Default implementation
			template<class T>
			struct Type
			{
				static ClassInfo *getClass(lua_State *L)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"getClass '%s'", typeid(T).name());
					ClassInfo *c = Manager::getInstance().getClass(typeid(T));
					if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
					return c;
				}

				static void push(lua_State *L,const T &obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, &obj);
					getClass(L)->push_copy(L, (void*) &obj);
				}

				static T get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					T* obj = reinterpret_cast<T*>( getClass(L)->get_ptr(L, pos) );	
					XGC_LUA_DEBUG(9,"obj = %p", obj);
					return *obj;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"check '%s' at pos %d", typeid(T).name(), pos);
					return getClass(L)->check(L, pos);
				}
				
			};

			template<class T>
			struct Type<T*>
			{
				static ClassInfo *getClass(lua_State *L)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"getClass '%s'", typeid(T).name());
					ClassInfo *c = Manager::getInstance().getClass(typeid(T));
					if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
					return c;
				}

				static void push(lua_State *L, T *obj, bool fromConstructor = false)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"push '%s' of %p (from constructor=%s)",
							typeid(T).name(),
							obj,
							fromConstructor? "true" : "false" );

					if (obj == 0)
					{
						lua_pushnil(L);
						return;
					}

					const std::type_info &t_T = typeid(T);
					const std::type_info &t_obj = typeid(*obj);
					
					assert("Invalid typeinfo!!! (type)" && (&t_T) );
					assert("Invalid typeinfo!!! (object)" && (&t_obj) );

					if (t_obj != t_T)
					{
						// check if the internal class exists...
						ClassInfo *c = Manager::getInstance().getClass(t_obj);
						if ( c ) 
						{
							XGC_LUA_DEBUG(8,"Push<T*=%s> with conversion from "
								"T(%p)->T(%p) (L=%p, obj =%p)",
								c->getName().c_str(), t_obj.name(), t_T.name(),L, obj);
							// covert the object to the internal class...
							void *real_obj = Manager::getInstance().convert( &t_T, &t_obj, obj );
							c->push_ptr(L, real_obj, fromConstructor);
							return;
						}
					}
					// use this class...	
					getClass(L)->push_ptr(L, (void*) obj, fromConstructor);
				}

				static T* get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"get '%s' at pos %d", typeid(T).name(), pos);
					return reinterpret_cast<T*>( getClass(L)->get_ptr(L, pos) );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"check '%s' at pos %d", typeid(T).name(), pos);
					return getClass(L)->check(L, pos);
				}
			};
			
			template<class T>
			struct Type<const T*>
			{
				static ClassInfo *getClass(lua_State *L)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"getClass '%s'", typeid(T).name());
					ClassInfo *c = Manager::getInstance().getClass(typeid(T));
					if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
					return c;
				}

				static void push(lua_State *L,const T *obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"push '%s' of %p", typeid(T).name(), obj);
					if (obj == 0)
					{
						lua_pushnil(L);
						return;
					}
					if (typeid(*obj) != typeid(T))
					{
						// check if the internal class exists...
						ClassInfo *c = Manager::getInstance().getClass(typeid(*obj));
						if ( c ) 
						{
							XGC_LUA_DEBUG(8,"Push<const T*=%s> with conversion from "
								"T(%p)->T(%p) (L=%p, obj =%p)",
								c->getName().c_str(), typeid(*obj).name(), typeid(T).name(),L, obj);
							// covert the object to the internal class...
							const void *real_obj = Manager::getInstance().convert( &typeid(T), &typeid(*obj), obj );
							c->push_const_ptr(L, real_obj);
							return;
						}
					}
					getClass(L)->push_const_ptr(L, (const void*) obj);
				}

				static const T* get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"get '%s' at pos %d", typeid(T).name(), pos);
					return reinterpret_cast<const T*>( getClass(L)->get_const_ptr(L, pos) );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"check '%s' at pos %d", typeid(T).name(), pos);
					return getClass(L)->check(L, pos);
				}
			};

			template<class T>
			struct Type<const T&>
			{
				static void push(lua_State *L,const T &obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"push '%s' of %p(const ref)", typeid(T).name(), &obj);
					Type<const T*>::push(L, &obj);
				}

				static const T& get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"get '%s' at pos %d", typeid(T).name(), pos);
					const T* obj = Type<const T*>::get(L,pos);
					//TODO: remove the typeid(T).getName() and use classInfo :)
					if (obj == 0L) luaL_error(L, "Can not get a reference of class %s", typeid(T).name());
					return *(obj);
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"check '%s' at pos %d", typeid(T).name(), pos);
					return Type<const T*>::check(L,pos);
				}
			};
			
			template<class T>
			struct Type<T&>
			{
				static ClassInfo *getClass(lua_State *L)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"getClass '%s'", typeid(T).name());
					ClassInfo *c = Manager::getInstance().getClass(typeid(T));
					if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
					return c;
				}

				static void push(lua_State *L,T &obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"push '%s' of %p (reference)", typeid(T).name(), &obj);
					getClass(L)->push_ref(L, (void*) &obj);
				}

				static T& get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"get '%s' at pos %d", typeid(T).name(), pos);
					return *(Type<T*>::get(L,pos));
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(10,"check '%s' at pos %d", typeid(T).name(), pos);
					return Type<T*>::check(L,pos);
				}
			};

			//--- Specializations ---------------------------------------------------

			template<>
			struct Type<void*>
			{
				static void push(lua_State *L,void* obj)
				{
					XGC_LUA_DEBUG_CALL; 
					if (obj == 0) lua_pushnil(L);
					else
					{
						XGC_LUA_DEBUG(8,"Push<void*> (L=%p, obj =%p)",L, obj);
						lua_pushlightuserdata(L, obj);
					}
				}

				static void *get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<void*> (L=%p, pos=%i ) =%p)",L, pos, lua_touserdata(L,pos));
					if (check(L,pos)) return lua_touserdata(L,pos);
					//TODO: Check here if is an userdata and convert it to void
					return 0;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_islightuserdata(L,pos) != 0);
				}
			};

			template< class T >
			struct TypeImpl_Integer
			{
				static void push(lua_State *L, T v)
				{
					lua_pushinteger(L,v);
				}
				static T get(lua_State *L, int p)
				{
					T v = (T) lua_tointeger(L,p);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					return (lua_isnumber(L,pos) != 0);
				}
			};

			template< class T >
			struct TypeImpl_Integer< T& >
			{
				static void push(lua_State *L, T& v)
				{
					lua_pushinteger(L,v);
				}
				static T get(lua_State *L, int p)
				{
					T v = (T) lua_tointeger(L,p);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					return (lua_isnumber(L,pos) != 0);
				}
			};

			template< class T >
			struct TypeImpl_Number
			{
				static void push(lua_State *L, T v)
				{
					lua_pushnumber(L,v);
				}

				static T get(lua_State *L, int p)
				{
					T v = (T)lua_tonumber(L,p);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					return (lua_isnumber(L,pos) != 0);
				}
			};

			template< class T >
			struct TypeImpl_Number< T& >
			{
				static void push(lua_State *L, T& v)
				{
					lua_pushnumber(L,v);
				}
				static T get(lua_State *L, int p)
				{
					T v = (T) lua_tonumber(L,p);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					return (lua_isnumber(L,pos) != 0);
				}
			};

		#define	DEFINE_TYPE_INTEGER( _type ) template<> struct Type< _type >	:	public TypeImpl_Integer< _type >{}
		#define	DEFINE_TYPE_NUMBER( _type ) template<> struct Type< _type >	:	public TypeImpl_Number< _type >{}

		#define DEFINE_INTEGER( _type )\
			DEFINE_TYPE_INTEGER( _type );\
			DEFINE_TYPE_INTEGER( const _type );\
			DEFINE_TYPE_INTEGER( _type& );\
			DEFINE_TYPE_INTEGER( const _type& );\
			DEFINE_TYPE_INTEGER( _type* );\
			DEFINE_TYPE_INTEGER( const _type* );\

		#define DEFINE_NUMBER( _type )\
			DEFINE_TYPE_NUMBER( _type );\
			DEFINE_TYPE_NUMBER( const _type );\
			DEFINE_TYPE_NUMBER( _type& );\
			DEFINE_TYPE_NUMBER( const _type& );\

		#define DEFINE_UNSIGNED( _type )\
			DEFINE_TYPE_INTEGER( unsigned _type );\
			DEFINE_TYPE_INTEGER( const unsigned _type );\
			DEFINE_TYPE_INTEGER( unsigned _type& );\
			DEFINE_TYPE_INTEGER( const unsigned _type& );\

			//////////////////////////////////////////////////////////////////////////
			// define type
			DEFINE_TYPE_INTEGER( char );
			DEFINE_TYPE_INTEGER( unsigned char );
			DEFINE_TYPE_INTEGER( char& );
			DEFINE_TYPE_INTEGER( unsigned char& );

			DEFINE_INTEGER( short );
			DEFINE_INTEGER( int );
			DEFINE_INTEGER( long );
			DEFINE_INTEGER( __int64 );

			DEFINE_UNSIGNED( short );
			DEFINE_UNSIGNED( int );
			DEFINE_UNSIGNED( long );
			DEFINE_UNSIGNED( __int64 );
			//DEFINE_UNSIGNED( __w64 int );
			//DEFINE_UNSIGNED( __w64 long );

			DEFINE_NUMBER( float );
			DEFINE_NUMBER( double );

		
			// Type specialization for <bool>
			template<>
			struct Type<bool>
			{
				static void push(lua_State *L, bool v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push bool = %d",(int)v);
					lua_pushboolean(L,v);
				}
				static bool get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					bool v = (lua_toboolean(L,p) != 0);
					XGC_LUA_DEBUG(6,"Get bool (pos %d) = %d",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return lua_isboolean(L,pos);
				}
			};

			template<>
			struct Type<bool&>
			{
				static void push(lua_State *L, bool v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push bool = %d",(int)v);
					lua_pushboolean(L,v);
				}
				static bool get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					bool v = (lua_toboolean(L,p) != 0);
					XGC_LUA_DEBUG(6,"Get bool (pos %d) = %d",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return lua_isboolean(L,pos);
				}
			};

			template<>
			struct Type< const bool& >
			{
				static void push(lua_State *L, bool v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push bool = %d",(int)v);
					lua_pushboolean(L,v);
				}
				static bool get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					bool v = (lua_toboolean(L,p) != 0);
					XGC_LUA_DEBUG(6,"Get bool (pos %d) = %d",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return lua_isboolean(L,pos);
				}
			};

			template<>
			struct Type<std::string>
			{
				static void push(lua_State *L, const std::string &v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const std::string& = %s",v.c_str());
					lua_pushstring(L, v.c_str());
				}

				static std::string get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);
					if( v == NULL ) luaL_error(L, "request string.");

					XGC_LUA_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type<std::string&>
			{
				static void push(lua_State *L, const std::string &v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const std::string& = %s",v.c_str());
					lua_pushstring(L, v.c_str());
				}

				static std::string get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);
					if( v == NULL ) luaL_error(L, "request string.");

					XGC_LUA_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type<const std::string &>
			{
				static void push(lua_State *L, const std::string &v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const std::string& = %s",v.c_str());
					lua_pushstring(L, v.c_str());
				}

				// let the compiler do the conversion...
				static const std::string get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);
					if( v == NULL ) luaL_error(L, "request string.");

					XGC_LUA_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
					return std::string(v);
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type<std::wstring>
			{
				static void push(lua_State *L, const std::wstring &v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const std::string& = %s",v.c_str());
					char* lpa = (char*)_alloca( v.length() + 1 );
					size_t convert = 0;
					size_t ret = wcstombs_s( &convert, lpa, v.length() + 1, v.c_str(), v.length() );
					if( ret != 0 )
					{
						lua_pushnil(L);
					}
					else
					{
						lua_pushstring(L, lpa );
					}
				}

				static std::wstring get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);

					if( v == NULL ) luaL_error(L, "request string.");

					XGC_LUA_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
					int chars = (int)strlen( v ) + 1;
					wchar_t* lpw = (wchar_t*)_alloca( chars*sizeof(wchar_t) );

					size_t convert = 0;
					size_t ret = mbstowcs_s( &convert, lpw, chars, v, chars );
					if( ret != 0 ) return L"";
					return lpw;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type<std::wstring&>
			{
				static void push(lua_State *L, const std::wstring &v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const std::string& = %s",v.c_str());
					char* lpa = (char*)_alloca( v.length() + 1 );
					size_t convert = 0;
					size_t ret = wcstombs_s( &convert, lpa, v.length() + 1, v.c_str(), v.length() );
					if( ret != 0 )
					{
						lua_pushnil(L);
					}
					else
					{
						lua_pushstring(L, lpa );
					}
				}

				static std::wstring get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);

					if( v == NULL ) luaL_error(L, "request string.");

					XGC_LUA_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
					int chars = (int)strlen( v ) + 1;
					wchar_t* lpw = (wchar_t*)_alloca( chars*sizeof(wchar_t) );

					size_t convert = 0;
					size_t ret = mbstowcs_s( &convert, lpw, chars, v, chars );
					if( ret != 0 ) return L"";
					return lpw;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type<const std::wstring &>
			{
				static void push(lua_State *L, const std::wstring &v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const std::wstring& = %s",v.c_str());
					char* lpa = (char*)_alloca( v.length() + 1 );
					size_t convert = 0;
					size_t ret = wcstombs_s( &convert, lpa, v.length() + 1, v.c_str(), v.length() );
					if( ret != 0 )
					{
						lua_pushnil(L);
					}
					else
					{
						lua_pushstring(L, lpa );
					}
				}

				// let the compiler do the conversion...
				static const std::wstring get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);

					if( v == NULL ) luaL_error(L, "request string.");
					XGC_LUA_DEBUG(6,"Get std::wstring (pos %d) = %s",p,v);
					int chars = (int)strlen( v ) + 1;
					wchar_t* lpw = (wchar_t*)_alloca( chars*sizeof(wchar_t) );

					size_t convert = 0;
					size_t ret = mbstowcs_s( &convert, lpw, chars, v, chars );
					if( ret != 0 ) return L"";
					return lpw;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			// Type specialization for <const char*>
			template<>
			struct Type<char*>
			{
				static void push(lua_State *L, char* v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push char* = %s",v);
					lua_pushstring(L,v);
				}

				static const char* get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);
					XGC_LUA_DEBUG(6,"Get const char* (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			// Type specialization for <const char*>
			template<>
			struct Type<const char*>
			{
				static void push(lua_State *L, const char* v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const char* = %s",v);
					lua_pushstring(L,v);
				}

				static const char* get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const char* v = (const char*) lua_tostring(L,p);
					XGC_LUA_DEBUG(6,"Get const char* (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type<const unsigned char*>
			{
				static void push(lua_State *L, const unsigned char* v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const unsigned char* = %s",v);
					lua_pushstring(L,(const char*)v);
				}

				static const unsigned char* get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const unsigned char* v = (const unsigned char*) lua_tostring(L,p);
					XGC_LUA_DEBUG(6,"Get const unsigned char* (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			// Type specialization for <const wchar_t*>
			template<>
			struct Type<wchar_t*>
			{
				static void push(lua_State *L, wchar_t* v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push char* = %s",v);
					size_t s = wcslen(v) + 1 ;
					lua_pushlstring(L,(const char*)v,s*sizeof(wchar_t));
				}

				static const wchar_t* get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const wchar_t* v = (const wchar_t*) lua_tolstring(L,p,NULL);
					XGC_LUA_DEBUG(6,"Get const char* (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			// Type specialization for <const char*>
			template<>
			struct Type<const wchar_t*>
			{
				static void push(lua_State *L, const wchar_t* v)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(6, "Push const char* = %s",v);
					size_t s = wcslen(v) + 1 ;
					lua_pushlstring(L,(const char*)v,s*sizeof(wchar_t));
				}

				static const wchar_t* get(lua_State *L, int p)
				{
					XGC_LUA_DEBUG_CALL; 
					const wchar_t* v = (const wchar_t*) lua_tolstring(L,p,NULL);
					XGC_LUA_DEBUG(6,"Get const char* (pos %d) = %s",p,v);
					return v;
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					return (lua_isstring(L,pos) != 0);
				}
			};

			template<>
			struct Type< LuaObject >
			{
				static void push(lua_State *L,const LuaObject &obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, &obj);
					obj.push();
				}

				static LuaObject get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					lua_pushvalue( L, pos );
					int ref = luaL_ref( L, LUA_REGISTRYINDEX );
					int type = lua_type( L, pos );
					return LuaObject( L, ref );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL;
					return (lua_istable( L, pos ) != 0);
				}
			};

			template<>
			struct Type< LuaObject& >
			{
				static void push(lua_State *L,const LuaObject &obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, &obj);
					obj.push();
				}

				static LuaObject get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					int type = lua_type( L, pos );
					lua_pushvalue(L,pos);
					int ref = luaL_ref( L, LUA_REGISTRYINDEX );
					return LuaObject( L, ref );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL;
					return true;
				}
			};

			template<>
			struct Type< const LuaObject& >
			{
				static void push(lua_State *L,const LuaObject &obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, &obj);
					obj.push();
				}

				static LuaObject get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					lua_pushvalue( L, pos );
					int ref = luaL_ref( L, LUA_REGISTRYINDEX );
					return LuaObject( L, ref );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL;
					return (lua_istable( L, pos ) != 0);
				}
			};

			template<>
			struct Type< LuaObject* >
			{
				static void push(lua_State *L,const LuaObject *obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, obj);
					obj->push();
				}

				static LuaObject get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					lua_pushvalue( L, pos );
					int ref = luaL_ref( L, LUA_REGISTRYINDEX );
					return LuaObject( L, ref );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL;
					return (lua_istable( L, pos ) != 0);
				}

			};

			template<>
			struct Type< const LuaObject* >
			{
				static void push(lua_State *L,const LuaObject *obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, obj);
					obj->push();
				}

				static LuaObject get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					int ref = luaL_ref( L, pos );
					return LuaObject( L, ref );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL;
					return (lua_istable( L, pos ) != 0);
				}
			};

			template<>
			struct Type< FuncCall* >
			{
				static void push(lua_State *L, FuncCall* obj)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Push<T=%s>(L=%p, obj =%p)", typeid(T).name(), L, obj);
					obj->push(L);
				}

				static FuncCall* get(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL; 
					XGC_LUA_DEBUG(8,"Get<T=%s>(L=%p, pos = %i)", typeid(T).name(), L, pos);
					return (FuncCall*)lua_touserdata( L, pos );
				}

				static bool check(lua_State *L, int pos)
				{
					XGC_LUA_DEBUG_CALL;
					return (lua_isuserdata( L, pos ) != 0);
				}
			};
		}
	}
}

#endif

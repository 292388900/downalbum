#include "LuaBinder/Header.h"
#include "LuaBinder/Manager.h"
#include "LuaBinder/ClassInfo.h"

namespace XGC
{
	namespace lua
	{
		/* Global functions */
		int XGC_LUA_type(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			const ClassInfo *ci = Manager::getInstance().getClass(L,-1);
			if (ci)
			{
				lua_pushstring(L, ci->getName().c_str());
				return 1;
			}
			return 0;
		}

		int XGC_LUA_rawptr(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			int top = lua_gettop(L);
			if (lua_getmetatable(L,1))
			{
				lua_getfield(L, -1, "__class_ptr");
				if (!lua_isnil(L,-1))
				{
					ClassInfo* ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
					void* raw = ci->get_ptr(L, 1);
					lua_settop(L, top);
					lua_pushlightuserdata(L, raw);
					return 1;
				}
			}
			lua_settop(L, top);
			return 0;
		}

		int XGC_LUA_copy(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			int top = lua_gettop(L);
			if (lua_getmetatable(L,1))
			{
				lua_getfield(L, -1, "__class_ptr");
				if (!lua_isnil(L,-1))
				{
					ClassInfo* ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
					lua_settop(L, top);
					ci->push_copy(L, lua_touserdata(L,1));
					return 1;
				}
			}
			lua_settop(L, top);
			return 0;
		}

		int XGC_LUA_using_index(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			lua_pushnil(L);
			while( lua_next(L, lua_upvalueindex(1)) )
			{
				lua_pushvalue(L,2); // key
				lua_gettable(L, -2); 
				if (!lua_isnil(L,-1))
				{
					return 1;
				}
				lua_pop(L,2);
			}
			return 0;
		}

		int XGC_LUA_using(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			int top = lua_gettop(L);
			luaL_checktype(L, 1, LUA_TTABLE);

			lua_getfield(L, LUA_REGISTRYINDEX, "XGC_LUA_using");
			if ( lua_isnil(L,-1) )
			{
				// pop nil value
				lua_pop(L,1);

				// get or create _G's metatable 
				if(!lua_getmetatable(L, LUA_GLOBALSINDEX))
				{
					lua_newtable(L);
					// set as metatable of _B 
					lua_pushvalue(L,-1);
					lua_setmetatable(L, LUA_GLOBALSINDEX);
				}
				else
				{
					luaL_error(L, "Can not use SLB.using,"
						" _G already has a metatable");
				}

				lua_newtable(L); // create the "XGC_LUA_using" table
				lua_pushvalue(L,-1); // keep a copy in registry 
				lua_setfield(L, LUA_REGISTRYINDEX, "XGC_LUA_using");

				// push the using_index func with the XGC_LUA_using table as closure
				lua_pushvalue(L,-1);
				lua_pushcclosure(L, XGC_LUA_using_index, 1); // push the closure
				// set this functions as "__index" of the metatable (at -3)
				lua_setfield(L, -3, "__index");

				// leave the XGC_LUA_using table at the top
			}
			lua_pushvalue(L,1); // get the first argument
			//TODO check not repeating tables...
			luaL_ref(L, -2); // add the table

			lua_settop(L,top);
			return 0;
		}

		int XGC_LUA_isA(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			int top = lua_gettop(L);
			if (top != 2)
				luaL_error(L, "Invalid number of arguments (instance, class)");

			ClassInfo* ci_a = 0;
			if (lua_getmetatable(L,1))
			{
				lua_getfield(L, -1, "__class_ptr");
				if (!lua_isnil(L,-1))
				{
					ci_a = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
				}
			}
			ClassInfo* ci_b = 0;
			if (lua_getmetatable(L,2))
			{
				lua_getfield(L, -1, "__class_ptr");
				if (!lua_isnil(L,-1))
				{
					ci_b = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
				}
			}
			lua_settop(L, top);
			if ( ci_a && ci_b ) 
			{
				lua_pushboolean(L, ci_a->isSubClassOf(ci_b) );
			}
			else
			{
				lua_pushboolean(L, false);
			}
			return 1;
		}
		
		int XGC_LUA_allTypes(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			Manager::ClassMap &map =  Manager::getInstance().getClasses();

			lua_newtable(L);
			for(Manager::ClassMap::iterator i = map.begin(); i != map.end(); ++i)
			{
				lua_pushstring(L, i->second->getName().c_str());
				i->second->push(L);
				lua_rawset(L,-3);
			}
			return 1;
		}

		int XGC_LUA_print(lua_State *L)
		{
			int top = lua_gettop(L);
			const char* str = lua_tostring( L, -1 );
			// OutputDebugStringA( str );
			puts( str );
			lua_settop( L, top );
			return 0;
		}

		static const luaL_Reg XGC_LUA_funcs[] = {
			{"type", XGC_LUA_type},
			{"copy", XGC_LUA_copy},
			{"using", XGC_LUA_using},
			{"rawptr", XGC_LUA_rawptr},
			{"isA", XGC_LUA_isA},
			{"allTypes", XGC_LUA_allTypes},
			{"print", XGC_LUA_print},
			{NULL, NULL}
		};

		Manager* Manager::_singleton = 0;

		Manager::Manager()
		{
			XGC_LUA_DEBUG_CALL;
			XGC_LUA_DEBUG(0, "Manager initialization");
			_global = new Namespace();
		}

		Manager::~Manager()
		{
			XGC_LUA_DEBUG_CALL;
			XGC_LUA_DEBUG(0, "Manager destruction");
		}
		
		void Manager::registerSLB(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL;
			int top = lua_gettop(L);

			// Register global functions
			luaL_register(L, "slb", XGC_LUA_funcs);
			// metatable of "SLB"
			lua_newtable(L);
			lua_pushstring(L,"__index");
			_global->push(L);
			lua_rawset(L,-3);
			lua_setmetatable(L,-2); // SLB table

			lua_settop(L,top);
		}
		
		void Manager::reset()
		{
			XGC_LUA_DEBUG_CALL;
			delete _singleton;
			_singleton = 0;
		}
		
		void Manager::addClass( ClassInfo *c )
		{
			XGC_LUA_DEBUG_CALL;
			_classes[ c->getTypeid() ] = c;
		}

		const ClassInfo *Manager::getClass(const std::type_info &ti) const
		{
			XGC_LUA_DEBUG_CALL;
			assert("Invalid type_info" && (&ti) );
			ClassMap::const_iterator i = _classes.find(&ti);
			if ( i != _classes.end() ) return i->second.get();
			return 0;
		}

		const ClassInfo *Manager::getClass(const std::string &name) const
		{
			XGC_LUA_DEBUG_CALL;
			NameMap::const_iterator i = _names.find(name);
			if ( i != _names.end() ) return getClass( *i->second );
			return 0;
		}

		ClassInfo *Manager::getClass(lua_State *L, int pos) const
		{
			XGC_LUA_DEBUG_CALL;
			pos = L_abs_index(L,pos);
			int top = lua_gettop(L);
			ClassInfo* ci = 0L;
			if (lua_getmetatable(L,pos))
			{
				lua_getfield(L, -1, "__class_ptr");
				if (!lua_isnil(L,-1))
				{
					ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
				}
			}
			lua_settop(L, top);
			return ci;
		}


		ClassInfo *Manager::getClass(const std::string &name)
		{
			XGC_LUA_DEBUG_CALL;
			NameMap::iterator i = _names.find(name);
			if ( i != _names.end() ) return getClass( *i->second );
			return 0;
		}

		ClassInfo *Manager::getClass(const std::type_info &ti)
		{
			XGC_LUA_DEBUG_CALL;
			assert("Invalid type_info" && (&ti) );
			ClassMap::iterator i = _classes.find(&ti);
			if ( i != _classes.end() ) return i->second.get();
			return 0;
		}
			
		bool Manager::copy(lua_State *from, int pos, lua_State *to)
		{
			XGC_LUA_DEBUG_CALL;
			XGC_LUA_DEBUG_CLEAN_STACK(from,0);
			
			if (from == to)
			{
				lua_pushvalue(from, pos);
				return true;
			}

			switch(lua_type(from, pos))
			{
				case LUA_TNIL:
					{
						XGC_LUA_DEBUG(20, "copy from %p(%d)->%p [nil]", from,pos,to);
						lua_pushnil(to);
					}
					return true;
				case LUA_TNUMBER:
					{
						XGC_LUA_DEBUG(20, "copy from %p(%d)->%p [number]", from,pos,to);
						lua_Number n = lua_tonumber(from,pos);
						lua_pushnumber(to, n);
					}
					return true;
				case LUA_TBOOLEAN:
					{
						XGC_LUA_DEBUG(20, "copy from %p(%d)->%p [boolean]", from,pos,to);
						int b = lua_toboolean(from,pos);
						lua_pushboolean(to,b);
					}
					return true;
				case LUA_TSTRING:
					{
						XGC_LUA_DEBUG(20, "copy from %p(%d)->%p [string]", from,pos,to);
						const char *s = lua_tostring(from,pos);
						lua_pushstring(to,s);
					}
					return true;
				case LUA_TTABLE:
					{
						XGC_LUA_DEBUG(0, "*WARNING* copy of tables Not yet Implemented!!!");
					}
					return false;
				case LUA_TUSERDATA:
					{
						XGC_LUA_DEBUG(20, "copy from %p(%d)->%p [Object]", from,pos,to);
						ClassInfo *ci = getClass(from, pos);
						if (ci != 0L)
						{
							const void* ptr = ci->get_const_ptr(from, pos);
							XGC_LUA_DEBUG(25, "making a copy of the object %p", ptr);
							// now copy it
							ci->push_copy(to,ptr);
							return true;
						}
						else
						{
							XGC_LUA_DEBUG(25, "Could not recognize the object");
							return false;
						}
					}
			}
			XGC_LUA_DEBUG(10,
				"Invalid copy from %p(%d)->%p %s",
				from,pos,to, lua_typename(from, lua_type(from,pos)));
			return false;
		}

		ClassInfo *Manager::getOrCreateClass(const std::type_info &ti)
		{
			XGC_LUA_DEBUG_CALL;
			assert("Invalid type_info" && (&ti) );
			ClassInfo *c = 0;
			ClassMap::iterator i = _classes.find(&ti);
			if ( i != _classes.end() )
			{
				c = i->second.get();
			}
			else
			{
				c = new ClassInfo(ti);
			}
			return c;
		}
		
		void Manager::set( const char* name, Object *obj)
		{
			XGC_LUA_DEBUG_CALL;
			_global->set(name, obj);
		}
		
		void Manager::rename(ClassInfo *ci, const std::string &new_name)
		{
			XGC_LUA_DEBUG_CALL;
			const std::string old_name = ci->getName();

			NameMap::iterator i = _names.find(old_name);
			if ( i != _names.end() )
			{
				_global->erase(old_name);
				_names.erase(i);
			}

			_global->set(new_name, ci);
			_names[ new_name ] = ci->getTypeid();

		}

		void* Manager::convert( const std::type_info *C1, const std::type_info *C2, void *obj)
		{
			XGC_LUA_DEBUG_CALL;
			XGC_LUA_DEBUG(10, "C1 = %s", C1->name());
			XGC_LUA_DEBUG(10, "C2 = %s", C2->name());
			XGC_LUA_DEBUG(10, "obj = %p", obj);
			if (C1 == C2)
			{
				XGC_LUA_DEBUG(11, "same class");
				return obj; 
			}

			ConversionsMap::iterator i = _conversions.find( ConversionsMap::key_type(C1,C2) );
			if (i != _conversions.end())
			{
				XGC_LUA_DEBUG(11, "convertible");
				return i->second( obj );
			}
			else
			{
				ClassInfo* c1 = getClass( *C1 );
				ClassInfo* c2 = getClass( *C2 );

				ClassInfo::ConverList l;
				if( c1 && c1->isSubClassOf( c2, &l ) )
				{
					while( obj && !l.empty() )
					{
						ConversionsMap::key_type& p = l.back();
						ConversionsMap::iterator i = _conversions.find( p );
						if( i != _conversions.end() )
						{
							XGC_LUA_DEBUG(11, "convertible");
							obj = i->second( obj );
						}
						else
						{
							XGC_LUA_DEBUG(11, "fail");
							return 0;
						}
						l.pop_back();
					}
				}
				else if( c2 && c2->isSubClassOf( c1, &l, true ) )
				{
					while( obj && !l.empty() )
					{
						ConversionsMap::key_type& p = l.front();
						ConversionsMap::iterator i = _conversions.find( p );
						if (i != _conversions.end())
						{
							XGC_LUA_DEBUG(11, "convertible");
							obj = i->second( obj );
						}
						else
						{
							XGC_LUA_DEBUG(11, "fail");
							return 0;
						}
						l.pop_front();
					}
				}
			}
			XGC_LUA_DEBUG(11, "fail");
			return obj;
		}
	}
}


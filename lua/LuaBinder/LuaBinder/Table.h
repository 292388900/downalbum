#ifndef __XGC_LUA_TABLE__
#define __XGC_LUA_TABLE__

#include "LuaBinder/Export.h"
#include "LuaBinder/Object.h"
#include "LuaBinder/RefPtr.h"

namespace XGC
{
	namespace lua
	{
		class XGC_LUA_EXPORT Table : public virtual Object 
		{
		public:
			typedef std::map< std::string, ref_ptr<Object> > Elements;
			Table(const std::string &separator = "", bool cacheable = false);

			void erase(const std::string &);
			Object* get(const std::string &);
			void getElements( std::list< const char* > &l )const;
			void set(const std::string &, Object*);

			bool isCacheable() { return _cacheable; }

			// [ -2, 0, - ] pops two elements (key, value) from the top and pushes it into
			// the cache. If _cacheable == false this won't make much sense, but you can use
			// it anyway (you can recover the values with getCache).
			void setCache(lua_State *L);

			// [ -1, +1, - ] will pop a key, and push the value or nil if not found.
			void getCache(lua_State *L);

		protected:
			virtual ~Table();

			Object* rawGet(const std::string &);
			void rawSet(const std::string &, Object*);
			
			void pushImplementation(lua_State *);

			/** will try to find the object, if not present will return -1. If this
			 * function is not overriden not finding an object will raise an error
			 * It is highly recommended to call this method in subclasses of Table
			 * first.*/
			virtual int __index(lua_State *L); 
			virtual int __newindex(lua_State *L);

			virtual int __call(lua_State *L);
			virtual int __garbageCollector(lua_State *L);
			virtual int __tostring(lua_State *L);

			Elements _elements;
			/** this function returns the index where to find the cache table that
			 * __index method uses if _cacheable is true. This method must NOT be called
			 * outside metamethod's implementation. */
			int cacheTableIndex() { return lua_upvalueindex(1); }

		private:
			typedef std::pair<Table*,const std::string> TableFind;
			typedef int (Table::*TableMember)(lua_State*);

			int __indexProxy(lua_State *L);
			static int __meta(lua_State*);
			void pushMeta(lua_State *L, TableMember) const;

			TableFind getTable(const std::string &key, bool create);

			bool _cacheable;
			std::string _separator;

			Table(const Table&);
			Table& operator=(const Table&);
		};

		//--------------------------------------------------------------------
		// Inline implementations:
		//--------------------------------------------------------------------
		template< size_t S, class C, class V >
		class XGC_LUA_EXPORT Array : public virtual Object
		{
			V (C::* _OffsetArray)[S];
		public:
			Array( V (C::* offset)[S] )
				: _OffsetArray( offset )
			{
			}

			~Array()
			{

			}

			bool set(){ return false; }

		protected:
			void pushImplementation(lua_State *L)
			{
				XGC_LUA_DEBUG_CALL;
				lua_newtable(L); // an NEW empty table

				lua_newtable(L); // and its metatable:

				lua_newtable(L); // cache

				lua_pushvalue(L, -1); 
				lua_setfield(L, -3, "__indexCache");

				pushMeta(L, &Array::__index);
				lua_setfield(L,-3, "__index");
				pushMeta(L, &Array::__newindex);
				lua_setfield(L, -3, "__newindex");
				pushMeta(L, &Array::__tostring);
				lua_setfield(L, -3, "__tostring");

				lua_pop(L,1); // remove the cache table

				lua_setmetatable(L,-2);
			}

			int __index(lua_State *L, InstanceBase** Instance)
			{
				XGC_LUA_DEBUG_CALL;
				XGC_LUA_DEBUG_STACK(10,L,"Table::__index (%p)",this);
				int result = -1;

				if (lua_type(L, 2) == LUA_TNUMBER)
				{
					int nIndex = lua_tointeger(L,2);
					if( nIndex >= 0 && nIndex < S )
					{
						if( (*Instance) )
						{
							C* Ptr = (C*)(*Instance)->get_ptr();

							if( Ptr )
							{
								Private::Type< V >::push(L,(Ptr->*_OffsetArray)[nIndex]);
							}
							else
							{
								luaL_error(L, "invalid property value index(%08x).", Ptr);
							}
						}
					}
					else
					{
						luaL_error(L, "Invalid array index(%d).", nIndex);
					}
				}
				else
				{
					luaL_error(L, "array not suppost string index.");
				}

				return result;
			}

			int __newindex(lua_State *L, InstanceBase** Instance)
			{
				int result = 0;
				XGC_LUA_DEBUG_CALL;
				XGC_LUA_DEBUG_STACK(10,L,"Table::__newindex (%p)",this);

				int top = lua_gettop(L);
				if (lua_type(L, 2) == LUA_TNUMBER)
				{
					int nIndex = lua_tointeger(L,2);
					if( nIndex >= 0 && nIndex < S )
					{
						if( (*Instance) )
						{
							C* Ptr = (C*)(*Instance)->get_ptr();

							if( Ptr )
							{
								(Ptr->*_OffsetArray)[nIndex] = Private::Type< V >::get(L,-1);
							}
							else
							{
								luaL_error(L, "invalid property value index(%08x).", Ptr);
							}
						}
						else
						{
							luaL_error(L, "Invalid array index(%d).", nIndex);
						}
					}
					else
					{
						luaL_error(L, "array not suppost string index.");
					}
				}
				return result;
			}

			virtual int __tostring(lua_State *L, InstanceBase** Instance)
			{
				XGC_LUA_DEBUG_CALL;
				XGC_LUA_DEBUG_STACK(10,L,"Table::__tostring (%p)",this);
				int top = lua_gettop(L);
				lua_pushfstring(L, "");
				return 1;
			}

		private:
			typedef int (Array::* TableMember)(lua_State*,InstanceBase**);

			static int __meta(lua_State *L)
			{
				XGC_LUA_DEBUG_CALL;
				// upvalue(1) is the cache table...
				Array *table = reinterpret_cast<Array*>(lua_touserdata(L, lua_upvalueindex(2)));
				TableMember member = *reinterpret_cast<TableMember*>(lua_touserdata(L, lua_upvalueindex(3)));
				InstanceBase** Instance = (InstanceBase**)lua_touserdata(L,lua_upvalueindex(4));
				return (table->*member)(L, Instance);
			}

			/** Pushmeta exepects a cache-table at the top, and creates a metamethod with 3 upvalues,
			* first the cache table, second the current object, and last the member to call */
			void pushMeta(lua_State *L, TableMember member) const
			{
				assert("Invalid pushMeta, expected a table at the top (the cache-table)" &&
					lua_type(L,-1) == LUA_TTABLE);
				lua_pushvalue(L,-1); // copy of cache table
				lua_pushlightuserdata(L, (void*) this);
				void *p = lua_newuserdata(L, sizeof(TableMember)); 
				memcpy(p,&member,sizeof(TableMember));
				lua_pushlightuserdata(L,lua_touserdata(L,1));

				lua_pushcclosure(L, __meta, 4 );
			}
		};

		template< size_t S, class C >
		class XGC_LUA_EXPORT Array< S, C, char > : public virtual Object
		{
			char (C::* _OffsetArray)[S];
		public:
			Array( char (C::* offset)[S] )
				: _OffsetArray( offset )
			{
			}

			~Array()
			{

			}

			bool set()
			{ 

				return false; 
			}

		protected:
			void pushImplementation(lua_State *L)
			{
				XGC_LUA_DEBUG_CALL;
				lua_newtable(L); // an NEW empty table

				lua_newtable(L); // and its metatable:

				lua_newtable(L); // cache

				lua_pushvalue(L, -1); 
				lua_setfield(L, -3, "__indexCache");

				pushMeta(L, &Array::__index);
				lua_setfield(L,-3, "__index");
				pushMeta(L, &Array::__newindex);
				lua_setfield(L, -3, "__newindex");
				pushMeta(L, &Array::__tostring);
				lua_setfield(L, -3, "__tostring");

				lua_pop(L,1); // remove the cache table

				lua_setmetatable(L,-2);
			}

			int __index(lua_State *L, InstanceBase** Instance)
			{
				XGC_LUA_DEBUG_CALL;
				XGC_LUA_DEBUG_STACK(10,L,"Table::__index (%p)",this);
				int result = -1;

				if( (*Instance) )
				{
					C* Ptr = (C*)(*Instance)->get_ptr();
					if( !Ptr )
					{
						luaL_error(L, "invalid property value index(%08x).", Ptr);
						return -1;
					}

					if (lua_type(L, 2) == LUA_TNUMBER)
					{
						int nIndex = lua_tointeger(L,2);
						if( nIndex >= 0 && nIndex < S )
						{
							Private::Type< char >::push(L,(Ptr->*_OffsetArray)[nIndex]);
						}
						else
						{
							lua_pushnil(L);
						}
					}
					else if(lua_type(L, 2) == LUA_TSTRING)
					{
						const char* name = lua_tostring(L, 2);
						if( name && stricmp( "value", name ) == 0 )
						{
							lua_pushstring(L, (Ptr->*_OffsetArray));
						}
						else
						{
							lua_pushnil(L);
						}
					}
					else
					{
						lua_pushnil(L);
					}
				}

				return result;
			}

			int __newindex(lua_State *L, InstanceBase** Instance)
			{
				int result = 0;
				XGC_LUA_DEBUG_CALL;
				XGC_LUA_DEBUG_STACK(10,L,"Table::__newindex (%p)",this);

				int top = lua_gettop(L);
				if( (*Instance) )
				{
					C* Ptr = (C*)(*Instance)->get_ptr();

					if( !Ptr )
					{
						luaL_error(L, "invalid property value index(%08x).", Ptr);
						return -1;
					}

					if (lua_type(L, 2) == LUA_TNUMBER)
					{
						int nIndex = lua_tointeger(L,2);
						if( nIndex >= 0 && nIndex < S )
						{
							(Ptr->*_OffsetArray)[nIndex] = Private::Type< char >::get(L,-1);
						}
						else
						{
							lua_pushnil(L);
						}
					}
					else if( lua_type(L, 2) == LUA_TSTRING)
					{
						const char* name = lua_tostring(L, 2);
						if( name && stricmp( "value", name ) == 0 )
						{
							strncpy( (Ptr->*_OffsetArray), lua_tostring(L,-1), S );
						}
						else
						{
							lua_pushnil(L);
						}
					}
				}
				else
				{
					lua_pushnil(L);
				}

				return result;
			}

			virtual int __tostring(lua_State *L, InstanceBase** Instance)
			{
				XGC_LUA_DEBUG_CALL;
				XGC_LUA_DEBUG_STACK(10,L,"Table::__tostring (%p)",this);
				int top = lua_gettop(L);
				lua_pushfstring(L, "");
				return 1;
			}

		private:
			typedef int (Array::* TableMember)(lua_State*,InstanceBase**);

			static int __meta(lua_State *L)
			{
				XGC_LUA_DEBUG_CALL;
				// upvalue(1) is the cache table...
				Array *table = reinterpret_cast<Array*>(lua_touserdata(L, lua_upvalueindex(2)));
				TableMember member = *reinterpret_cast<TableMember*>(lua_touserdata(L, lua_upvalueindex(3)));
				InstanceBase** Instance = (InstanceBase**)lua_touserdata(L,lua_upvalueindex(4));
				return (table->*member)(L, Instance);
			}

			/** Pushmeta exepects a cache-table at the top, and creates a metamethod with 3 upvalues,
			* first the cache table, second the current object, and last the member to call */
			void pushMeta(lua_State *L, TableMember member) const
			{
				assert("Invalid pushMeta, expected a table at the top (the cache-table)" &&
					lua_type(L,-1) == LUA_TTABLE);
				lua_pushvalue(L,-1); // copy of cache table
				lua_pushlightuserdata(L, (void*) this);
				void *p = lua_newuserdata(L, sizeof(TableMember)); 
				memcpy(p,&member,sizeof(TableMember));
				lua_pushlightuserdata(L,lua_touserdata(L,1));

				lua_pushcclosure(L, __meta, 4 );
			}
		};

	}
}
#endif

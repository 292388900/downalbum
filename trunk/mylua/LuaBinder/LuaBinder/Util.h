#ifndef __XGC_LUA_UTIL__
#define __XGC_LUA_UTIL__
#include "LuaBinder/Class.h"

#define L_abs_index(L, i)     ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : \
		                    lua_gettop(L) + (i) + 1)

namespace XGC
{
	namespace lua
	{
		template< class T >
		struct luaArray
		{
			luaArray( int size )
				: _array( new T[size] )
				, _size( size )
			{

			}

			~luaArray()
			{
				delete[] _array;
			}

			void Set( size_t idx, const T& v )
			{
				if( idx >= _size )
					return;

				_array[idx] = v;
			}

			T* Get( size_t idx )
			{
				if( idx >= _size )
					return NULL;

				return _array + idx;
			}

			operator const T*()const
			{
				return _array;
			}

			operator T*()
			{
				return _array;
			}

			T* _array;
			size_t _size;
		};

		template< class T >
		void RegisterArray( const char* name )
		{
			Class< luaArray< T > >( name )
				.constructor< size_t >()
				.set( "set", &luaArray< T >::Set )
				.set( "get", &luaArray< T >::Get )
				.object__index( &luaArray< T >::Get )
				.object__newindex( &luaArray< T >::Set )
				;
		}
	}
}

#endif

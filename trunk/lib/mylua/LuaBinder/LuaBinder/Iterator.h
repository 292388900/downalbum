#ifndef __XGC_LUA_ITERATOR__
#define __XGC_LUA_ITERATOR__

#include "LuaBinder/Export.h"
#include "LuaBinder/Object.h"
#include "LuaBinder/PushGet.h"

namespace XGC
{
	namespace lua
	{
		class XGC_LUA_EXPORT IteratorBase
		{
		public:
			virtual int push(lua_State *L) = 0;
			virtual ~IteratorBase() {}
		};

		class XGC_LUA_EXPORT Iterator : public Object
		{
		public: 
			Iterator(IteratorBase *b);

		protected:
			void pushImplementation(lua_State *L);
			virtual ~Iterator();
		private:
			static int iterator_call(lua_State *L);
			IteratorBase *_iterator;
			Iterator( const Iterator &slbo);
			Iterator& operator=( const Iterator &slbo);
		};


		// Standard iterator
		template<class T, class T_iterator>
		struct StdIteratorTraits
		{
			typedef T Container;
			typedef T_iterator Iterator;
			typedef Iterator (Container::*GetIteratorMember)();
			//How unref iterators:
			static typename Iterator::value_type unref(Iterator& i) { return *i; }
		};

		template<class T, class T_iterator>
		struct StdConstIteratorTraits
		{
			typedef T Container;
			typedef T_iterator Iterator;
			typedef Iterator (Container::*GetIteratorMember)() const;

			//How unref iterators:
			static const typename Iterator::value_type unref(const Iterator& i) { return *i; }
		};

		template<typename Traits>
		class StdIterator : public IteratorBase
		{
		public:
			typedef typename Traits::GetIteratorMember MemberFuncs ;
			typedef typename Traits::Container Container;
			typedef typename Traits::Iterator  Iterator;

			StdIterator(MemberFuncs m_first, MemberFuncs m_end );
			int push(lua_State *L);
		protected:
			static int next(lua_State *L) ;
		private:
			MemberFuncs _begin, _end;

			StdIterator( const StdIterator &slbo);
			StdIterator& operator=( const StdIterator &slbo);
		};

		// ------------------------------------------------------------
		// ------------------------------------------------------------
		// ------------------------------------------------------------
		
		template<class T>
		inline StdIterator<T>::StdIterator(MemberFuncs m_first, MemberFuncs m_end)
			: _begin(m_first), _end(m_end)
		{
		}
		
		template<class T>
		inline int StdIterator<T>::push(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL
			Container* container = XGC::lua::get<Container*>(L,1);
			lua_pushcclosure(L, StdIterator<T>::next, 0);
			Iterator *d = reinterpret_cast<Iterator*>(lua_newuserdata(L, sizeof(Iterator)*2));
			d[0] = (container->*_begin)();
			d[1] = (container->*_end)();
			return 2;
		}

		template<class T>
		inline int StdIterator<T>::next(lua_State *L)
		{
			XGC_LUA_DEBUG_CALL
			Iterator *d = reinterpret_cast<Iterator*>(lua_touserdata(L,1));
			
			if ( d[0] != d[1] )
			{
				XGC::lua::push(L, T::unref(d[0]) );
				++d[0]; // inc iterator
			}
			else
			{
				lua_pushnil(L);
			}
			return 1;
		}
	}
} //end of SLB namespace

#endif

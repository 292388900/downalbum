#ifndef __XGC_LUA_OBJECT__
#define __XGC_LUA_OBJECT__

#include "LuaBinder/Export.h"

namespace XGC
{
	namespace lua
	{
		class XGC_LUA_EXPORT Object 
		{
		public:
			unsigned int referenceCount() const { return _refCounter; }
			void ref();
			void unref();

			void push(lua_State *L);
			void setInfo(const std::string&);
			const std::string& getInfo() const;

		protected:

			Object();
			virtual ~Object();

			virtual void pushImplementation(lua_State *) = 0;
			virtual void onGarbageCollection(lua_State *) {}

		private:
			void initialize(lua_State *) const;
			static int GC_callback(lua_State *);
			unsigned int _refCounter;
			std::string _info; // for metadata, documentation, ...

			
			Object( const Object &slbo);
			Object& operator=( const Object &slbo);
		};

		// ------------------------------------------------------------
		// ------------------------------------------------------------
		// ------------------------------------------------------------
			
		inline void Object::ref()
		{
			++_refCounter;
		}

		inline void Object::unref()
		{
			assert(_refCounter > 0);
			--_refCounter; 
			if (_refCounter == 0) delete this;
		}

		inline void Object::setInfo(const std::string& s) {_info = s;}
		inline const std::string& Object::getInfo() const {return _info;}
	}
} //end of SLB namespace

#endif

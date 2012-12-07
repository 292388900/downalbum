#ifndef __XGC_LUA_SCRIPT__
#define __XGC_LUA_SCRIPT__

#include "LuaBinder/PushGet.h"
#include "LuaBinder/Type.h"

namespace XGC
{
	namespace lua
	{
		class XGC_LUA_EXPORT Script
		{
			friend class LuaCallBase;
		public:
			Script(bool loadDefaultLibs = true);
			virtual ~Script();
			void doFile(const char *filename);
			void doFile(const wchar_t *filename);
			void doString(const char *codeChunk, const std::string &where_hint ="[SLB]");
			void doString(const wchar_t *codeChunk, const std::wstring &where_hint =L"[SLB]");
			 /* ************************* WARNING *********************************
			  * Sometines you need to manually call Garbage Collector(GC), to be sure
			  * that all objects are destroyed. This is mandatory when using smartPointers
			  * be very carefull. GC operations are really expensive, avoid calling GC
			  * too frequently.
			  * ************************* WARNING *********************************/
			void callGC();

			template<class T>
			void set(const char *name, T value)
			{ XGC::lua::setGlobal<T>(getState(), value, name );}

			template<class T>
			T get(const char* name)
			{ return XGC::lua::getGlobal<T>(getState(), name ); }

		protected:
			lua_State* getState();
			void close(); // will close lua_state

		private:
			Script(const Script &s);
			Script& operator=(const Script&);
			lua_State *_L;
			bool _loadDefaultLibs;
		};
	}
}

#endif

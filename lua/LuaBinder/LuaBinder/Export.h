#ifndef __XGC_LUA_EXPORT__
#define __XGC_LUA_EXPORT__

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
	#  if defined( _LIB ) || defined( XGC_LUA_USE_STATIC )
	#    define XGC_LUA_EXPORT
	#  elif defined( XGC_LUA_LIBRARY )
	#    define XGC_LUA_EXPORT   __declspec(dllexport)

	#  else
	#    define XGC_LUA_EXPORT   __declspec(dllimport)
	#  endif /* XGC_LUA_LIBRARY */
	#pragma warning( disable: 4251 )	
#else
	#  define XGC_LUA_EXPORT
#endif  

#endif

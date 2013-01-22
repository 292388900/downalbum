// header.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

// crt
#include <tchar.h>
#include <time.h>

// stl container
#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <bitset>

// stl helper
#include <typeinfo>
#include <cassert>
#include <climits>
#include <sstream>
#include <iostream>
#include <stdexcept>

// stl algorithm
#include <algorithm>

// user
#include "defines.h"
#include "commonspp.h"

#include "LuaBinder/lua.h"
#include "LuaBinder/Debug.h"
#include "LuaBinder/Util.h"

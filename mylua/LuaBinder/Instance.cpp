#include "LuaBinder/Header.h"
#include "LuaBinder/Instance.h"
#include "LuaBinder/ClassInfo.h"
#include "LuaBinder/Manager.h"

namespace XGC
{
	namespace lua
	{
		InstanceBase::InstanceBase(Type t, const std::type_info &type) : _flags(t), _class(0)
		{
			XGC_LUA_DEBUG_CALL;
			_class = Manager::getInstance().getClass( type );
		}

		InstanceBase::~InstanceBase() {XGC_LUA_DEBUG_CALL;}

		InstanceFactory::~InstanceFactory() {XGC_LUA_DEBUG_CALL;}
	}
}

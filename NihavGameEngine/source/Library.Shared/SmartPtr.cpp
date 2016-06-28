#include "pch.h"
#include "SmartPtr.h"

namespace Library
{
	std::set<std::map<void*, std::vector<SmartPtr*>>*> SmartPtr::sSmartPointerList;

	void SmartPtr::ClearStaticMembers()
	{
		sSmartPointerList.clear();
	}
}


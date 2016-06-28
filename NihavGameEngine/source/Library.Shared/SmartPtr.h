#pragma once
#include <set>
#include <map>
#include <vector>

namespace Library
{
	class SmartPtr
	{
	public:
		SmartPtr() = default;
		virtual ~SmartPtr() = default;

		virtual void* GetRawPtr() = 0;
		virtual void SetRawPtr(void* ptr) = 0;

		static void ClearStaticMembers();
		static std::map<void*, std::vector<SmartPtr*>> sSmartPointerList;
	};
}


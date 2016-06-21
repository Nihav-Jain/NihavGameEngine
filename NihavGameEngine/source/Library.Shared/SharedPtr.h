#pragma once
#include <map>
#include <list>

namespace Library
{
	template <typename T>
	class SharedPtr
	{
	public:
		SharedPtr();
		~SharedPtr();
		SharedPtr(const SharedPtr& rhs);
		SharedPtr(SharedPtr&& rhs);

		SharedPtr& operator=(const SharedPtr& rhs);
		SharedPtr& operator=(SharedPtr&& rhs);

		T* operator->();
		const T* operator->() const;

		T& operator*();
		const T& operator*() const;

		T* RawPtr();
		const T* RawPtr() const;

		template<typename... ArgTypes>
		static SharedPtr MakeShared(ArgTypes&&... Args);
		static void ClearStaticMembers();
		static std::uint32_t ReferenceCount(SharedPtr<T>& sharedPtr);

	private:
		//template<typename... ArgTypes>
		//SharedPtr(ArgTypes&&... Args);
		SharedPtr(T* rawPtr);

		T* mRawPtr;
		static std::map<T*, std::list<SharedPtr<T>*>> mReferenceCount;
	};


}
#include "SharedPtr.inl"


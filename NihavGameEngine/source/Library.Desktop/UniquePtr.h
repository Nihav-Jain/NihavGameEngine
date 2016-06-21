#pragma once
#include <map>
#include <vector>
#include <list>

namespace Library
{
	template <typename T>
	class UniquePtr
	{
	public:
		UniquePtr();
		~UniquePtr();
		UniquePtr(UniquePtr& rhs);
		UniquePtr(UniquePtr&& rhs);

		UniquePtr& operator=(UniquePtr& rhs);
		UniquePtr& operator=(UniquePtr&& rhs);

		T* operator->();
		const T* operator->() const;

		T& operator*();
		const T& operator*() const;

		T* RawPtr();
		const T* RawPtr() const;

		template<typename... ArgTypes>
		static UniquePtr MakeUnique(ArgTypes&&... Args);
		static void ClearStaticMembers();

	private:
		//template<typename... ArgTypes>
		//SharedPtr(ArgTypes&&... Args);
		UniquePtr(T* rawPtr);

		T* mRawPtr;
		static std::map<T*, UniquePtr<T>*> mReferences;
	};


}
#include "UniquePtr.inl"


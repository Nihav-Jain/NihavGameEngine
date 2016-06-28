#pragma once
#include <map>
#include <vector>
#include "SmartPtr.h"

namespace Library
{
	template <typename T>
	class UniquePtr : public SmartPtr
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

		virtual void* GetRawPtr() override;
		virtual void SetRawPtr(void* ptr) override;

		template<typename... ArgTypes>
		static UniquePtr MakeUnique(ArgTypes&&... Args);

	private:
		UniquePtr(T* rawPtr);

		T* mRawPtr;
	};


}
#include "UniquePtr.inl"


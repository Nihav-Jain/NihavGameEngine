#pragma once
#include <map>
#include <list>
#include "SmartPtr.h"

namespace Library
{
	template <typename T>
	class SharedPtr : public SmartPtr
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

		void Delete();

		virtual void* GetRawPtr() override;
		virtual void SetRawPtr(void* ptr) override;

		template<typename... ArgTypes>
		static SharedPtr MakeShared(ArgTypes&&... Args);
		static std::uint32_t ReferenceCount(SharedPtr<T>& sharedPtr);

	private:
		SharedPtr(T* rawPtr);

		T* mRawPtr;
	};


}
#include "SharedPtr.inl"


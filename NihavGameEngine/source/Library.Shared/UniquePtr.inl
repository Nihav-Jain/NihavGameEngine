#include "pch.h"
#include "UniquePtr.h"

namespace Library
{
	template <typename T>
	std::map<void*, std::vector<SmartPtr*>> UniquePtr<T>::mReferences;

	template<typename T>
	template<typename ...ArgTypes>
	UniquePtr<T> UniquePtr<T>::MakeUnique(ArgTypes&& ...Args)
	{
		SmartPtr::sSmartPointerList.insert(&mReferences);
		T* ptr = NewObject<T>(std::forward<ArgTypes>(Args)...);
		UniquePtr<T> uniquePtr(ptr);
		return uniquePtr;
	}

	template<typename T>
	UniquePtr<T>::UniquePtr(T* rawPtr) :
		mRawPtr(rawPtr)
	{}

	template<typename T>
	UniquePtr<T>::UniquePtr() :
		mRawPtr(nullptr)
	{}

	template <typename T>
	UniquePtr<T>::~UniquePtr()
	{
		if (mRawPtr != nullptr)
		{
			mReferences.erase(reinterpret_cast<void*>(mRawPtr));
			DeleteObject(mRawPtr);
		}
	}

	template<typename T>
	UniquePtr<T>::UniquePtr(UniquePtr& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		rhs.mRawPtr = nullptr;
		if (mRawPtr != nullptr)
		{
			void* ptr = reinterpret_cast<void*>(mRawPtr);
			if (mReferences[ptr].size() == 0)
				mReferences[ptr].push_back(this);
			else
				mReferences[ptr][0] = this;
		}
	}

	template<typename T>
	UniquePtr<T>::UniquePtr(UniquePtr&& rhs) :
		UniquePtr(rhs)
	{}

	template<typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr& rhs)
	{
		if (this != &rhs)
		{
			DeleteObject(mRawPtr);
			mRawPtr = rhs.mRawPtr;
			rhs.mRawPtr = nullptr;
			if (mRawPtr != nullptr)
			{
				void* ptr = reinterpret_cast<void*>(mRawPtr);
				if (mReferences[ptr].size() == 0)
					mReferences[ptr].push_back(this);
				else
					mReferences[ptr][0] = this;
			}
		}
		return *this;
	}

	template<typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& rhs)
	{
		return operator=(rhs);
	}

	template<typename T>
	T* UniquePtr<T>::operator->()
	{
		if (mRawPtr == nullptr)
			throw std::exception("Null pointer exception");
		return mRawPtr;
	}

	template<typename T>
	const T* UniquePtr<T>::operator->() const
	{
		if (mRawPtr == nullptr)
			throw std::exception("Null pointer exception");
		return mRawPtr;
	}

	template<typename T>
	T& UniquePtr<T>::operator*()
	{
		return *(operator->());
	}

	template<typename T>
	const T& UniquePtr<T>::operator*() const
	{
		return *(operator->());
	}

	template <typename T>
	T* UniquePtr<T>::RawPtr()
	{
		return mRawPtr;
	}

	template <typename T>
	const T* UniquePtr<T>::RawPtr() const
	{
		return mRawPtr;
	}

	template<typename T>
	void* UniquePtr<T>::GetRawPtr()
	{
		return reinterpret_cast<void*>(mRawPtr);
	}

	template<typename T>
	void UniquePtr<T>::SetRawPtr(void* ptr)
	{
		mReferences[ptr] = mReferences[reinterpret_cast<void*>(mRawPtr)];
		mReferences.erase(reinterpret_cast<void*>(mRawPtr));
		mRawPtr = reinterpret_cast<T*>(ptr);
	}

	template<typename T>
	void UniquePtr<T>::ClearStaticMembers()
	{
		mReferences.clear();
	}
}
#include "pch.h"
#include "UniquePtr.h"

namespace Library
{
	template <typename T>
	std::map<T*, UniquePtr<T>*> UniquePtr<T>::mReferences;

	template<typename T>
	template<typename ...ArgTypes>
	UniquePtr<T> UniquePtr<T>::MakeUnique(ArgTypes&& ...Args)
	{
		T* ptr = new T(Args...);
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
			mReferences.erase(mRawPtr);
			delete mRawPtr;
		}
	}

	template<typename T>
	UniquePtr<T>::UniquePtr(UniquePtr& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		rhs.mRawPtr = nullptr;
		if(mRawPtr != nullptr)
			mReferences[mRawPtr] = this;
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
			delete mRawPtr
			mRawPtr = rhs.mRawPtr;
			rhs.mRawPtr = nullptr;
			if (mRawPtr != nullptr)
				mReferences[mRawPtr] = this;
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
	void UniquePtr<T>::ClearStaticMembers()
	{
		mReferences.clear();
	}
}
#include "pch.h"
#include "SharedPtr.h"
#include "Memory.h"

namespace Library
{
	template <typename T>
	std::map<void*, std::vector<SmartPtr*>> SharedPtr<T>::mReferenceCount;

	template<typename T>
	template<typename ...ArgTypes>
	SharedPtr<T> SharedPtr<T>::MakeShared(ArgTypes&& ...Args)
	{
		SmartPtr::sSmartPointerList.insert(&mReferenceCount);
		T* ptr = NewObject<T>(std::forward<ArgTypes>(Args)...);
		SharedPtr<T> sharedPtr(ptr);
		return sharedPtr;
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(T* rawPtr) :
		mRawPtr(rawPtr)
	{}

	template<typename T>
	SharedPtr<T>::SharedPtr() :
		mRawPtr(nullptr)
	{}

	template <typename T>
	SharedPtr<T>::~SharedPtr()
	{
		if (mRawPtr != nullptr)
		{
			std::vector<SmartPtr*>::iterator itr = std::find(mReferenceCount[reinterpret_cast<void*>(mRawPtr)].begin(), mReferenceCount[reinterpret_cast<void*>(mRawPtr)].end(), this);
			if (itr != mReferenceCount[reinterpret_cast<void*>(mRawPtr)].end())
				mReferenceCount[reinterpret_cast<void*>(mRawPtr)].erase(itr);
			if (mReferenceCount[reinterpret_cast<void*>(mRawPtr)].size() == 0)
				DeleteObject(mRawPtr);
		}
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		if(mRawPtr != nullptr)
			mReferenceCount[reinterpret_cast<void*>(mRawPtr)].push_back(this);
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr&& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		rhs.mRawPtr = nullptr;

		if (mRawPtr != nullptr)
		{
			std::vector<SmartPtr*>::iterator itr = std::find(mReferenceCount[reinterpret_cast<void*>(mRawPtr)].begin(), mReferenceCount[reinterpret_cast<void*>(mRawPtr)].end(), &rhs);
			if (itr != mReferenceCount[reinterpret_cast<void*>(mRawPtr)].end())
				mReferenceCount[reinterpret_cast<void*>(mRawPtr)].erase(itr);
			else
				mReferenceCount[reinterpret_cast<void*>(mRawPtr)].push_back(this);
		}
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& rhs)
	{
		if (this != &rhs && mRawPtr != rhs.mRawPtr)
		{
			if (mRawPtr != nullptr)
			{
				void* ptr = reinterpret_cast<void*>(mRawPtr);
				mReferenceCount[ptr].erase(std::find(mReferenceCount[ptr].begin(), mReferenceCount[ptr].end(), this));
				if (mReferenceCount[ptr].size() == 0)
					DeleteObject(mRawPtr);
			}
			if(rhs.mRawPtr != nullptr)
				mReferenceCount[reinterpret_cast<void*>(rhs.mRawPtr)].push_back(this);
			mRawPtr = rhs.mRawPtr;
		}
		return *this;
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& rhs)
	{
		if (this != &rhs)
		{
			if (rhs.mRawPtr != nullptr)
			{
				void* rhsPtr = reinterpret_cast<void*>(rhs.mRawPtr);
				mReferenceCount[rhsPtr].erase(std::find(mReferenceCount[rhsPtr].begin(), mReferenceCount[rhsPtr].end(), &rhs));
				mReferenceCount[rhsPtr].push_back(this);
			}
			if (mRawPtr != nullptr)
			{
				void* ptr = reinterpret_cast<void*>(mRawPtr);
				mReferenceCount[ptr].erase(std::find(mReferenceCount[ptr].begin(), mReferenceCount[ptr].end(), this));
				if (mReferenceCount[ptr].size() == 0)
					DeleteObject(mRawPtr);
			}
			mRawPtr = rhs.mRawPtr;
			rhs.mRawPtr = nullptr;
		}
		return *this;
	}

	template<typename T>
	T* SharedPtr<T>::operator->()
	{
		if (mRawPtr == nullptr)
			throw std::exception("Null pointer exception");
		return mRawPtr;
	}

	template<typename T>
	const T* SharedPtr<T>::operator->() const
	{
		if (mRawPtr == nullptr)
			throw std::exception("Null pointer exception");
		return mRawPtr;
	}

	template<typename T>
	T& SharedPtr<T>::operator*()
	{
		return *(operator->());
	}

	template<typename T>
	const T& SharedPtr<T>::operator*() const
	{
		return *(operator->());
	}

	template <typename T>
	T* SharedPtr<T>::RawPtr()
	{
		return mRawPtr;
	}

	template <typename T>
	const T* SharedPtr<T>::RawPtr() const
	{
		return mRawPtr;
	}

	template<typename T>
	void* SharedPtr<T>::GetRawPtr()
	{
		return reinterpret_cast<void*>(mRawPtr);
	}

	template<typename T>
	void SharedPtr<T>::SetRawPtr(void* ptr)
	{
		mReferenceCount[ptr] = mReferenceCount[reinterpret_cast<void*>(mRawPtr)];
		mReferenceCount.erase(reinterpret_cast<void*>(mRawPtr));
		mRawPtr = reinterpret_cast<T*>(ptr);
	}

	template<typename T>
	void SharedPtr<T>::ClearStaticMembers()
	{
		for(auto& value : mReferenceCount)
		{
			value.second.clear();
		}
		mReferenceCount.clear();
	}

	template<typename T>
	std::uint32_t SharedPtr<T>::ReferenceCount(SharedPtr<T>& sharedPtr)
	{
		if(sharedPtr.RawPtr() != nullptr)
			return static_cast<std::uint32_t>(mReferenceCount[reinterpret_cast<void*>(sharedPtr.RawPtr())].size());
		return 0;
	}
}
#include "pch.h"
#include "SharedPtr.h"

namespace Library
{
	template <typename T>
	std::map<T*, std::list<SharedPtr<T>*>> SharedPtr<T>::mReferenceCount;

	template<typename T>
	template<typename ...ArgTypes>
	SharedPtr<T> SharedPtr<T>::MakeShared(ArgTypes&& ...Args)
	{
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
			std::list<SharedPtr<T>*>::iterator itr = std::find(mReferenceCount[mRawPtr].begin(), mReferenceCount[mRawPtr].end(), this);
			if (itr != mReferenceCount[mRawPtr].end())
				mReferenceCount[mRawPtr].erase(itr);
			if (mReferenceCount[mRawPtr].size() == 0)
				DeleteObject(mRawPtr);
		}
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		if(mRawPtr != nullptr)
			mReferenceCount[mRawPtr].push_back(this);
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr&& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		rhs.mRawPtr = nullptr;

		if (mRawPtr != nullptr)
		{
			std::list<SharedPtr<T>*>::iterator itr = std::find(mReferenceCount[mRawPtr].begin(), mReferenceCount[mRawPtr].end(), &rhs);
			if (itr != mReferenceCount[mRawPtr].end())
				mReferenceCount[mRawPtr].erase(itr);
			else
				mReferenceCount[mRawPtr].push_back(this);
		}
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& rhs)
	{
		if (this != &rhs && mRawPtr != rhs.mRawPtr)
		{
			if (mRawPtr != nullptr)
			{
				mReferenceCount[mRawPtr].erase(std::find(mReferenceCount[mRawPtr].begin(), mReferenceCount[mRawPtr].end(), this));
				if (mReferenceCount[mRawPtr].size() == 0)
					DeleteObject(mRawPtr);
			}
			if(rhs.mRawPtr != nullptr)
				mReferenceCount[rhs.mRawPtr].push_back(this);
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
				mReferenceCount[rhs.mRawPtr].erase(std::find(mReferenceCount[rhs.mRawPtr].begin(), mReferenceCount[rhs.mRawPtr].end(), &rhs));
				mReferenceCount[rhs.mRawPtr].push_back(this);
			}
			if (mRawPtr != nullptr)
			{
				mReferenceCount[mRawPtr].erase(std::find(mReferenceCount[mRawPtr].begin(), mReferenceCount[mRawPtr].end(), this));
				if (mReferenceCount[mRawPtr].size() == 0)
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
			return static_cast<std::uint32_t>(mReferenceCount[sharedPtr.RawPtr()].size());
		return 0;
	}
}
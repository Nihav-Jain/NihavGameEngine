#include "pch.h"
#include "SharedPtr.h"
#include "Memory.h"

namespace Library
{

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
		Delete();
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		if(mRawPtr != nullptr)
			SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].push_back(this);
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr&& rhs) :
		mRawPtr(rhs.mRawPtr)
	{
		rhs.mRawPtr = nullptr;

		if (mRawPtr != nullptr)
		{
			std::vector<SmartPtr*>::iterator itr = std::find(SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].begin(), SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].end(), &rhs);
			if (itr != SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].end())
				SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].erase(itr);
			else
				SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].push_back(this);
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
				SmartPtr::sSmartPointerList[ptr].erase(std::find(SmartPtr::sSmartPointerList[ptr].begin(), SmartPtr::sSmartPointerList[ptr].end(), this));
				if (SmartPtr::sSmartPointerList[ptr].size() == 0)
					DeleteObject(mRawPtr);
			}
			if(rhs.mRawPtr != nullptr)
				SmartPtr::sSmartPointerList[reinterpret_cast<void*>(rhs.mRawPtr)].push_back(this);
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
				SmartPtr::sSmartPointerList[rhsPtr].erase(std::find(SmartPtr::sSmartPointerList[rhsPtr].begin(), SmartPtr::sSmartPointerList[rhsPtr].end(), &rhs));
				SmartPtr::sSmartPointerList[rhsPtr].push_back(this);
			}
			if (mRawPtr != nullptr)
			{
				void* ptr = reinterpret_cast<void*>(mRawPtr);
				SmartPtr::sSmartPointerList[ptr].erase(std::find(SmartPtr::sSmartPointerList[ptr].begin(), SmartPtr::sSmartPointerList[ptr].end(), this));
				if (SmartPtr::sSmartPointerList[ptr].size() == 0)
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
	void SharedPtr<T>::Delete()
	{
		if (mRawPtr != nullptr)
		{
			std::vector<SmartPtr*>::iterator itr = std::find(SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].begin(), SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].end(), this);
			if (itr != SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].end())
				SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].erase(itr);
			if (SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)].size() == 0)
				DeleteObject(mRawPtr);
			mRawPtr = nullptr;
		}
	}

	template<typename T>
	void* SharedPtr<T>::GetRawPtr()
	{
		return reinterpret_cast<void*>(mRawPtr);
	}

	template<typename T>
	void SharedPtr<T>::SetRawPtr(void* ptr)
	{
		SmartPtr::sSmartPointerList[ptr] = SmartPtr::sSmartPointerList[reinterpret_cast<void*>(mRawPtr)];
		SmartPtr::sSmartPointerList.erase(reinterpret_cast<void*>(mRawPtr));
		mRawPtr = reinterpret_cast<T*>(ptr);
	}

	template<typename T>
	std::uint32_t SharedPtr<T>::ReferenceCount(SharedPtr<T>& sharedPtr)
	{
		if(sharedPtr.RawPtr() != nullptr)
			return static_cast<std::uint32_t>(SmartPtr::sSmartPointerList[reinterpret_cast<void*>(sharedPtr.RawPtr())].size());
		return 0;
	}
}
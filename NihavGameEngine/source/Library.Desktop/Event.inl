#include "pch.h"
#include "Event.h"

namespace Library
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	Event<T>::Event(const T& payload) :
		mMessage(&payload), EventPublisher(&mSubscriberList)
	{}

	template<typename T>
	Event<T>::Event(const Event& rhs) :
		mMessage(rhs.mMessage), EventPublisher(rhs)
	{}

	template<typename T>
	Event<T>::Event(Event&& rhs) :
		mMessage(rhs.mMessage), EventPublisher(std::move(rhs))
	{
		rhs.mMessage = nullptr;
	}

	template<typename T>
	Event<T>& Event<T>::operator=(const Event& rhs)
	{
		if (this != &rhs)
		{
			mMessage = rhs.mMessage;
			EventPublisher::operator=(rhs);
		}
		return *this;
	}

	template<typename T>
	Event<T>& Event<T>::operator=(Event&& rhs)
	{
		if (this != &rhs)
		{
			mMessage = rhs.mMessage;
			EventPublisher::operator=(std::move(rhs));

			rhs.mMessage = nullptr;
		}
		return *this;
	}

	template<typename T>
	void Event<T>::Subscribe(const IEventSubscriber& subscriber)
	{
		mSubscriberList.PushBack(&subscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(const IEventSubscriber& subscriber)
	{
		mSubscriberList.Remove(&subscriber);
	}

	template<typename T>
	void Event<T>::UnsubscriberAll()
	{
		mSubscriberList.Clear();
	}

	template<typename T>
	const T& Event<T>::Message() const
	{
		return *mMessage;
	}
}

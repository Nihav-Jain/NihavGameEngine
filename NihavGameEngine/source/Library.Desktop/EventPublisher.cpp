#include "pch.h"
#include "EventPublisher.h"

namespace Library
{
	using namespace std::chrono;

	RTTI_DEFINITIONS(EventPublisher);

	EventPublisher::EventPublisher(const Vector<IEventSubscriber*>& subscriberList, std::mutex& eventMutex) :
		mSubscriberList(&subscriberList), mTimeEnqueued(), mDelay(), mMutexPtr(&eventMutex)
	{}

	EventPublisher::EventPublisher(const EventPublisher& rhs) :
		mSubscriberList(rhs.mSubscriberList), mTimeEnqueued(rhs.mTimeEnqueued), mDelay(mDelay), mMutexPtr(rhs.mMutexPtr)
	{}

	EventPublisher::EventPublisher(EventPublisher&& rhs) :
		mSubscriberList(rhs.mSubscriberList), mTimeEnqueued(std::move(rhs.mTimeEnqueued)), mDelay(std::move(mDelay)), mMutexPtr(rhs.mMutexPtr)
	{}

	EventPublisher& EventPublisher::operator=(const EventPublisher& rhs)
	{
		if (this != &rhs)
		{
			mSubscriberList = rhs.mSubscriberList;
			mTimeEnqueued = rhs.mTimeEnqueued;
			mDelay = rhs.mDelay;
			mMutexPtr = rhs.mMutexPtr;
		}
		return *this;
	}

	EventPublisher& EventPublisher::operator=(EventPublisher&& rhs)
	{
		if (this != &rhs)
		{
			mSubscriberList = rhs.mSubscriberList;
			mTimeEnqueued = std::move(rhs.mTimeEnqueued);
			mDelay = std::move(rhs.mDelay);
			mMutexPtr = rhs.mMutexPtr;
		}
		return *this;
	}

	void EventPublisher::SetTime(const std::chrono::milliseconds& timeEnqueued, std::chrono::milliseconds delay)
	{
		mTimeEnqueued = timeEnqueued;
		mDelay = delay;
	}

	const std::chrono::milliseconds& EventPublisher::TimeEnqueued() const
	{
		return mTimeEnqueued;
	}

	const std::chrono::milliseconds& EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const std::chrono::milliseconds& currentTime) const
	{
		if ((currentTime - mTimeEnqueued) > mDelay)
			return true;
		return false;
	}

	void EventPublisher::Deliver() const
	{
		std::lock_guard<std::mutex> lock(*mMutexPtr);

		std::vector<std::future<void>> futures;
		for (auto& subscriber : *mSubscriberList)
		{
			futures.emplace_back(std::async([&](IEventSubscriber* subscriber) {
				subscriber->Notify(*this);
			}, subscriber));
		}

		for (auto& f : futures)
		{
			f.get();
		}
	}

}

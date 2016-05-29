#include "pch.h"
#include "EventQueue.h"

namespace Library
{
	EventQueue::EventQueue() :
		mPublishers(), mExpiredEvents()
	{}

	void EventQueue::Enqueue(const std::shared_ptr<EventPublisher>& publisher, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (mPublishers.Find(publisher) == mPublishers.end())
		{
			publisher->SetTime(gameTime.ElapsedGameTime(), delay);
			mPublishers.PushBack(publisher);
		}
	}

	void EventQueue::Send(const std::shared_ptr<EventPublisher>& publisher)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		publisher->Deliver();
		mPublishers.Remove(publisher);
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		std::vector<std::future<void>> futures;
		DeliverExpiredEvents(gameTime, futures);

		// wait for the events to be dispatched before removing them so that if an exception is thrown, the mEvents vector is not modified
		for (auto& f : futures)
		{
			f.get();
		}

		std::lock_guard<std::mutex> lock(mMutex);
		for (auto& expiredEvent : mExpiredEvents)
		{
			mPublishers.Remove(expiredEvent);
		}
		mExpiredEvents.Clear();
	}

	void EventQueue::Clear(const GameTime& gameTime)
	{
		std::vector<std::future<void>> futures;
		DeliverExpiredEvents(gameTime, futures);

		for (auto& f : futures)
		{
			f.get();
		}

		std::lock_guard<std::mutex> lock(mMutex);
		mPublishers.Clear();
		mExpiredEvents.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mPublishers.IsEmpty();
	}

	std::uint32_t EventQueue::Size() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mPublishers.Size();
	}

	void EventQueue::CancelEvent(const std::shared_ptr<EventPublisher>& publisher)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mPublishers.Remove(publisher);
	}

	void EventQueue::DeliverExpiredEvents(const GameTime& gameTime, std::vector<std::future<void>>& futures)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		for (const auto& publisher : mPublishers)
		{
			futures.emplace_back(std::async([&](std::shared_ptr<EventPublisher>& publisher) {
				if (publisher->IsExpired(gameTime.ElapsedGameTime()))
				{
					publisher->Deliver();
					mExpiredEvents.PushBack(publisher);
				}
			}, publisher));
		}
	}

}

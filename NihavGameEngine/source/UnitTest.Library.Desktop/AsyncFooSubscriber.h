#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "EventQueue.h"

class AsyncFooSubscriber : public Library::IEventSubscriber
{
public:
	AsyncFooSubscriber(Library::EventQueue& queue, const Library::GameTime& gameTime);
	virtual ~AsyncFooSubscriber() = default;

	virtual void Notify(const Library::EventPublisher& publisher) override;

	bool wasNotified;
	std::int32_t data;

private:
	Library::EventQueue* mEventQueuePtr;
	const Library::GameTime* mGameTimePtr;
};


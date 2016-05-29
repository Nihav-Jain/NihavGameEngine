#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "EventQueue.h"

class AsyncBarSubscriber : public Library::IEventSubscriber
{
public:
	AsyncBarSubscriber(Library::EventQueue& queue, const Library::GameTime& gameTime);
	virtual ~AsyncBarSubscriber() = default;

	virtual void Notify(const Library::EventPublisher& publisher) override;

	bool wasNotified;
	std::int32_t data;
	bool queueWasEmpty;
private:
	Library::EventQueue* mEventQueuePtr;
	const Library::GameTime* mGameTimePtr;
};


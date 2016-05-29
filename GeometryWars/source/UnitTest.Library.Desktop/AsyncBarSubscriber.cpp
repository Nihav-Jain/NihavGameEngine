#include "pch.h"
#include "AsyncBarSubscriber.h"

using namespace Library;

AsyncBarSubscriber::AsyncBarSubscriber(EventQueue& queue, const GameTime& gameTime) :
	wasNotified(false), data(0), mEventQueuePtr(&queue), mGameTimePtr(&gameTime), queueWasEmpty(false)
{}

void AsyncBarSubscriber::Notify(const EventPublisher& publisher)
{
	assert(publisher.Is(Event<Foo>::TypeIdClass()));

	const Foo& foo = publisher.As<Event<Foo>>()->Message();
	data = foo.GetData();
	wasNotified = true;

	publisher.TimeEnqueued();
	publisher.Delay();
	//mEventQueuePtr->Size();
	////mEventQueuePtr->Clear(*mGameTimePtr);
	////queueWasEmpty = mEventQueuePtr->IsEmpty();
}

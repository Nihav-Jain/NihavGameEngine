#include "pch.h"
#include "AsyncFooSubscriber.h"

using namespace Library;

AsyncFooSubscriber::AsyncFooSubscriber(EventQueue& queue, const GameTime& gameTime) :
	wasNotified(false), data(0), mEventQueuePtr(&queue), mGameTimePtr(&gameTime)
{}

void AsyncFooSubscriber::Notify(const EventPublisher& publisher)
{
	assert(publisher.Is(Event<Foo>::TypeIdClass()));

	const Foo& foo = publisher.As<Event<Foo>>()->Message();
	data = foo.GetData();
	wasNotified = true;

	Foo anotherFoo(100);
	std::shared_ptr<EventPublisher> anotherEvent = std::make_shared<Event<Foo>>(anotherFoo);
	mEventQueuePtr->Enqueue(anotherEvent, *mGameTimePtr);
}

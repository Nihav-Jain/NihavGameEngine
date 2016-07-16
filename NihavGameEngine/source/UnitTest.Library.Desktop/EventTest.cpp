#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include "Game.h"

#include "IEventSubscriber.h"
#include "Event.h"
#include "EventQueue.h"

#include "Foo.h"
#include "FooSubscriber.h"
#include "Bar.h"
#include "BarSubscriber.h"

#include "AsyncFooSubscriber.h"
#include "AsyncBarSubscriber.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std::chrono;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(EventTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);

			const std::pair<const std::uint64_t*, EngineModule**> ptr = *DesktopAudioManager::Itr;
			UNREFERENCED_PARAMETER(ptr);

			Engine::CreateEngine();
			Engine::Get().Activate();
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Engine::Get().Deactivate();
			Engine::Destroy();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(EventTestPublisher)
		{
			FooSubscriber subscriber;
			Foo foo(20);
			
			Event<Foo> fooEvent(foo);
			fooEvent.Deliver();

			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);

			fooEvent.Subscribe(subscriber);
			fooEvent.Deliver();

			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			fooEvent.Unsubscribe(subscriber);
			fooEvent.Deliver();

			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);


			fooEvent.Subscribe(subscriber);
			fooEvent.Deliver();

			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			fooEvent.UnsubscribeAll();
			fooEvent.Deliver();

			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventTestEventQueue)
		{
			Game game;
			EventQueue& queue = game.GetWorld().GetEventQueue();

			FooSubscriber subscriber;
			Foo foo(20);

			std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);
			
			fooEvent->Subscribe(subscriber);

			game.Start();
			queue.Enqueue(fooEvent, game.GetGameTime());

			Sleep(201);

			game.Update();
			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			game.Update();
			
			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);

			Event<Foo>::Unsubscribe(subscriber);

			FooSubscriber anotherFooSubscriber;
			Event<Foo>::Subscribe(anotherFooSubscriber);

			Bar bar(30);
			BarSubscriber barSubsciber;
			std::shared_ptr<Event<Bar>> barEvent = std::make_shared<Event<Bar>>(bar);
			Event<Bar>::Subscribe(barSubsciber);
			
			queue.Enqueue(fooEvent, game.GetGameTime(), milliseconds(400));
			queue.Enqueue(barEvent, game.GetGameTime(), milliseconds(200));

			Assert::IsFalse(anotherFooSubscriber.wasNotified);
			Assert::AreEqual(0, anotherFooSubscriber.data);
			Assert::IsFalse(barSubsciber.wasNotified);
			Assert::AreEqual(0, barSubsciber.data);


			Sleep(201);

			game.Update();
			Assert::IsFalse(anotherFooSubscriber.wasNotified);
			Assert::AreEqual(0, anotherFooSubscriber.data);

			Assert::IsTrue(barSubsciber.wasNotified);
			Assert::AreEqual(30, barSubsciber.data);

			barSubsciber.wasNotified = false;
			barSubsciber.data = 0;

			Sleep(401);

			game.Update();
			Assert::IsTrue(anotherFooSubscriber.wasNotified);
			Assert::AreEqual(20, anotherFooSubscriber.data);
			Assert::IsFalse(barSubsciber.wasNotified);
			Assert::AreEqual(0, barSubsciber.data);

			Event<Foo>::UnsubscribeAll();
			Event<Bar>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueTestSend)
		{
			Game game;
			EventQueue& queue = game.GetWorld().GetEventQueue();

			Foo foo(20);
			FooSubscriber subscriber;
			std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);
			Event<Foo>::Subscribe(subscriber);

			Assert::IsTrue(queue.IsEmpty());
			Assert::AreEqual(0U, queue.Size());

			game.Start();
			queue.Enqueue(fooEvent, game.GetGameTime(), milliseconds(200));

			Assert::IsFalse(queue.IsEmpty());
			Assert::AreEqual(1U, queue.Size());

			game.Update();
			Sleep(100);

			game.Update();
			Assert::IsFalse(subscriber.wasNotified);
			Assert::AreEqual(0, subscriber.data);

			queue.Send(fooEvent);

			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			Sleep(300);
			game.Update();

			Assert::IsFalse(subscriber.wasNotified);
			Assert::AreEqual(0, subscriber.data);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueTestClear)
		{
			GameClock clock;
			GameTime gameTime;

			EventQueue queue;

			Foo foo(20);
			FooSubscriber subscriber;
			std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);
			Event<Foo>::Subscribe(subscriber);

			clock.Reset();
			clock.UpdateGameTime(gameTime);
			queue.Enqueue(fooEvent, gameTime, milliseconds(100));

			Sleep(101);
			clock.UpdateGameTime(gameTime);
			queue.Clear(gameTime);

			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueTestCancelEvent)
		{
			Game game;
			EventQueue& queue = game.GetWorld().GetEventQueue();

			Foo foo(20);
			FooSubscriber subscriber;
			std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);
			Event<Foo>::Subscribe(subscriber);

			game.Start();
			queue.Enqueue(fooEvent, game.GetGameTime(), milliseconds(200));

			Sleep(100);
			game.Update();

			Assert::IsFalse(subscriber.wasNotified);
			Assert::AreEqual(0, subscriber.data);

			queue.CancelEvent(fooEvent);

			Sleep(300);
			game.Update();

			Assert::IsFalse(subscriber.wasNotified);
			Assert::AreEqual(0, subscriber.data);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventTestCopyMoveSemantics)
		{
			FooSubscriber subscriber;
			Foo foo(20);

			std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(std::move(foo));
			Assert::AreEqual(0, foo.GetData());

			Event<Foo> fooEventCopy(*fooEvent);
			Assert::AreEqual(20, fooEventCopy.Message().GetData());

			Foo tempFoo(30);
			Event<Foo> fooEventCopy2(tempFoo);
			fooEventCopy2 = *fooEvent;
			Assert::AreEqual(20, fooEventCopy2.Message().GetData());
			
			Event<Foo> fooEventMove(std::move(fooEventCopy));
			Assert::AreEqual(20, fooEventMove.Message().GetData());
			Assert::AreEqual(0, fooEventCopy.Message().GetData());
			
			Event<Foo> fooEventMove2(tempFoo);
			Assert::AreEqual(30, fooEventMove2.Message().GetData());
			fooEventMove2 = std::move(fooEventMove);
			Assert::AreEqual(20, fooEventMove2.Message().GetData());
			Assert::AreEqual(0, fooEventMove.Message().GetData());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventTestRTTI)
		{
			Foo foo(10);
			Event<Foo> fooEvent(foo);

			Assert::IsTrue(fooEvent.Is(EventPublisher::TypeIdClass()));
			
			EventPublisher* publisher = fooEvent.As<EventPublisher>();
			Assert::IsNotNull(publisher);
			
			EventPublisher* anotherPublisher = fooEvent.AssertiveAs<EventPublisher>();
			Assert::IsNotNull(anotherPublisher);

			Assert::AreEqual(fooEvent.TypeIdInstance(), Event<Foo>::TypeIdClass());

			RTTI* rtti = fooEvent.QueryInterface(Event<Bar>::TypeIdClass());
			Assert::IsNull(rtti);
			rtti = fooEvent.QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(rtti);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventTestAsycReEnqueue)
		{
			for (std::uint32_t k = 0; k < 5; k++)
			{
				std::uint32_t size = 5;
				Foo foo(10);
				Game game;
				EventQueue& queue = game.GetWorld().GetEventQueue();
				const GameTime& gameTime = game.GetGameTime();

				std::uint32_t i;
				AsyncFooSubscriber templateFooSubscriber(queue, gameTime);
				// 100 AsyncFooSubscribers
				AsyncFooSubscriber** fooSubscribers = new AsyncFooSubscriber*[size];
				for (i = 0; i < size; i++)
				{
					fooSubscribers[i] = new AsyncFooSubscriber(queue, gameTime);
					Event<Foo>::Subscribe(*fooSubscribers[i]);
				}
				// 100 AsyncBarSubscribers

				// 100 Foo Events
				for (i = 0; i < size; i++)
				{
					std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);
					queue.Enqueue(fooEvent, gameTime);
				}
				// 100 Bar Events

				game.Start();
				// sleep
				Sleep(100);

				// Test non-notification
				for (i = 0; i < size; i++)
				{
					Assert::IsFalse(fooSubscribers[i]->wasNotified);
					Assert::AreEqual(0, fooSubscribers[i]->data);
				}

				// Update
				game.Update();
				//// Test notification
				for (i = 0; i < size; i++)
				{
					Assert::IsTrue(fooSubscribers[i]->wasNotified);
					Assert::AreEqual(10, fooSubscribers[i]->data);
				}

				// reset all subscribers
				for (i = 0; i < size; i++)
				{
					fooSubscribers[i]->wasNotified = false;
					fooSubscribers[i]->data = 0;
				}

				//sleep
				Sleep(100);

				// Update
				game.Update();
				// Test for new values according to events queued by subscribers
				for (i = 0; i < size; i++)
				{
					Assert::IsTrue(fooSubscribers[i]->wasNotified);
					Assert::AreEqual(100, fooSubscribers[i]->data);
				}

				Event<Foo>::UnsubscribeAll();
				for (i = 0; i < size; i++)
				{
					delete fooSubscribers[i];
				}
				delete[] fooSubscribers;
			}

		}

		//TEST_METHOD(EventTestAsync)
		//{
		//	for (std::uint32_t k = 0; k < 30; k++)
		//	{
		//		std::uint32_t size = 10;
		//		Foo foo(10);
		//		Game game;
		//		EventQueue& queue = game.GetWorld().GetEventQueue();
		//		const GameTime& gameTime = game.GetGameTime();

		//		std::uint32_t i;
		//		AsyncBarSubscriber templateFooSubscriber(queue, gameTime);
		//		// 100 AsyncFooSubscribers
		//		AsyncBarSubscriber** fooSubscribers = new AsyncBarSubscriber*[size];
		//		for (i = 0; i < size; i++)
		//		{
		//			fooSubscribers[i] = new AsyncBarSubscriber(queue, gameTime);
		//			Event<Foo>::Subscribe(*fooSubscribers[i]);
		//		}
		//		// 100 AsyncBarSubscribers

		//		// 100 Foo Events
		//		for (i = 0; i < size; i++)
		//		{
		//			std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);
		//			queue.Enqueue(fooEvent, gameTime);
		//		}
		//		// 100 Bar Events

		//		game.Start();
		//		// sleep
		//		Sleep(100);

		//		// Test non-notification
		//		for (i = 0; i < size; i++)
		//		{
		//			Assert::IsFalse(fooSubscribers[i]->wasNotified);
		//			Assert::AreEqual(0, fooSubscribers[i]->data);
		//		}

		//		// Update
		//		game.Update();
		//		//// Test notification
		//		for (i = 0; i < size; i++)
		//		{
		//			Assert::IsTrue(fooSubscribers[i]->wasNotified);
		//			Assert::AreEqual(10, fooSubscribers[i]->data);
		//		}

		//		Event<Foo>::UnsubscribeAll();
		//		for (i = 0; i < size; i++)
		//		{
		//			delete fooSubscribers[i];
		//		}
		//		delete[] fooSubscribers;
		//	}
		//}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EventTest::sStartMemState;
#endif
}
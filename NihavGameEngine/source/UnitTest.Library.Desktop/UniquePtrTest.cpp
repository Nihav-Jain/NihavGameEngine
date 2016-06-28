#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(UniquePtrTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			SmartPtr::ClearStaticMembers();
			UniquePtr<int>::ClearStaticMembers();
			//UniquePtr<Foo>::ClearStaticMembers();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(UniquePtrAlloc)
		{
			UniquePtr<int> ptr = UniquePtr<int>::MakeUnique();
			*ptr = 10;

			Assert::AreEqual(10, *ptr);
			Assert::AreEqual(10, *ptr.RawPtr());
		}

		TEST_METHOD(UniquePtrAssign)
		{
			UniquePtr<int> ptr = UniquePtr<int>::MakeUnique(10);
			UniquePtr<int> anotherPtr = ptr;

			Assert::AreEqual(10, *anotherPtr);
			Assert::IsNull(ptr.RawPtr());
		}

	/*	TEST_METHOD(SharedPtrAutoDestruct)
		{
			SharedPtr<int> ptr = SharedPtr<int>::MakeShared();
			*ptr = 10;

			Assert::AreEqual(10, *ptr);
			Assert::AreEqual(1U, SharedPtr<int>::ReferenceCount(ptr));

			{
				SharedPtr<int> anotherPtr = ptr;
				Assert::AreEqual(10, *anotherPtr);
				Assert::AreEqual(2U, SharedPtr<int>::ReferenceCount(ptr));
			}

			Assert::AreEqual(1U, SharedPtr<int>::ReferenceCount(ptr));
		}

		TEST_METHOD(SharedPtrParamCtor)
		{
			SharedPtr<int> ptr = SharedPtr<int>::MakeShared(10);
			Assert::AreEqual(10, *ptr);

			SharedPtr<Foo> fooPtr = SharedPtr<Foo>::MakeShared(20);
			Assert::AreEqual(20, fooPtr->GetData());

			SharedPtr<Foo> anotherFooPtr = SharedPtr<Foo>::MakeShared(20, 30.30f);
			Assert::AreEqual(20, anotherFooPtr->GetData());
			Assert::AreEqual(30.30f, anotherFooPtr->GetAnotherData());
		}*/

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState UniquePtrTest::sStartMemState;
#endif
}
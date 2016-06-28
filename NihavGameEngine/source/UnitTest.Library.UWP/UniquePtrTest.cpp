#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "../Library.Shared/HeapManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTest_Library_UWP
{

	TEST_CLASS(UWPUniquePtrTest)
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

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(UWP_UniquePtrAlloc)
		{
			UniquePtr<int> ptr = UniquePtr<int>::MakeUnique();
			*ptr = 10;

			Assert::AreEqual(10, *ptr);
			Assert::AreEqual(10, *ptr.RawPtr());
		}

		TEST_METHOD(UWP_UniquePtrAssign)
		{
			UniquePtr<int> ptr = UniquePtr<int>::MakeUnique(10);
			UniquePtr<int> anotherPtr = ptr;

			Assert::AreEqual(10, *anotherPtr);
			Assert::IsNull(ptr.RawPtr());
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState UWPUniquePtrTest::sStartMemState;
#endif
}
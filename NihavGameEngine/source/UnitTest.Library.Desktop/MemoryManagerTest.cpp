#include "pch.h"
#include "CppUnitTest.h"
#include "../Library.Shared/HeapManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(MemoryManagerTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);

			std::vector<HeapManager::HeapDef> heapDefs;
			heapDefs.push_back(HeapManager::HeapDef());
			heapDefs.push_back(HeapManager::HeapDef());

			heapDefs[0].HeapID = 3;
			heapDefs[0].HeapSize = 5000000;
			heapDefs[0].Name = "HeapOne";

			heapDefs[1].HeapID = 4;
			heapDefs[1].HeapSize = 5000000;
			heapDefs[1].Name = "HeapTwo";

			HeapManager::CreateHeapManager(memory);
			HeapManager::GetHeapManager()->Init(heapDefs, 2);

		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			HeapManager::DestroyHeapManager();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				//Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(HeapManagerTest)
		{
			std::uint32_t id = 3;

			char *string1 = (char *)ENGINE_ALLOC(13, id, "TST");
			strcpy_s(string1, 13, "Hello World!");
			ENGINE_FREE(string1);

			// pointer arithmetic works
			char *randomNumbers = (char *)ENGINE_ALLOC(100, id, "TST");
			char *randomNumbers1 = randomNumbers;
			for (int i = 0; i<100; ++i)
			{
				*randomNumbers++ = (char)(i);
			}
			ENGINE_FREE(randomNumbers1);

			char *tempBlock1, *tempBlock2;
			// checking heap consistency i.e. number of blocks in the linked list is the same as the actual number of blocks in the heap
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));

			tempBlock1 = (char *)ENGINE_ALLOC(500, id, "TST");
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(2U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			tempBlock2 = (char *)ENGINE_ALLOC(500, id, "TST");
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			// another test, similar to last one that by allocating another block, the total size of the heap goes down by more than the allocated size
			// [tempBlock1, tempBlock2, Free]
											  // another block allocated so now we should have 3 blocks in total, 2 allocated, 1 free
			Assert::AreEqual(3U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			// freeing a block should result in 2 blocks in total (the new freed block should be merged with the old free block)
			// [tempBlock1, Free]
			ENGINE_FREE(tempBlock2);
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(2U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			// free another block and all blocks whould be merged
			// [Free]
			ENGINE_FREE(tempBlock1);
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(1U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			tempBlock1 = (char *)ENGINE_ALLOC(500, id, "TST");
			tempBlock2 = (char *)ENGINE_ALLOC(500, id, "TST");
			char* tempBlock3 = (char *)ENGINE_ALLOC(500, id, "TST");
			Assert::AreEqual(4U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			// freeing tempBlock1 will still result in 4 blocks, the order being -> [Free, tempBlock2, tempBlock3, Free]
			ENGINE_FREE(tempBlock1);
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			// freeing tempBlock3 will result in 3 blocks, the order being -> [Free, tempBlock2, Free]
			ENGINE_FREE(tempBlock3);
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(3U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			// freeing tempBlock2 will result in just 1 block, since all of the free blocks will be merged, the order being -> [Free]
			ENGINE_FREE(tempBlock2);
			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(1U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

		}

		TEST_METHOD(MemManagementDtorTest)
		{
			Foo* foo = ENGINE_NEW(3, "TST") Foo(20);
			Assert::AreEqual(20, foo->GetData());
			DeleteObject<Foo>(foo);
		}

		TEST_METHOD(DefragmentationTest)
		{
			std::uint32_t id = 3;
			SharedPtr<Foo> foo1 = SharedPtr<Foo>::MakeShared(5);
			SharedPtr<Foo> foo2 = SharedPtr<Foo>::MakeShared(10);
			SharedPtr<Foo> foo3 = SharedPtr<Foo>::MakeShared(15);
			SharedPtr<Foo> foo4 = SharedPtr<Foo>::MakeShared(20);
			SharedPtr<Foo> foo5 = SharedPtr<Foo>::MakeShared(25);

			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(6U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			Assert::AreEqual(5, foo1->GetData());
			Assert::AreEqual(10, foo2->GetData());
			Assert::AreEqual(15, foo3->GetData());
			Assert::AreEqual(20, foo4->GetData());
			Assert::AreEqual(25, foo5->GetData());

			foo2.Delete();

			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(6U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			auto expression = [&] {foo2->GetData(); };
			Assert::ExpectException<std::exception>(expression);

			foo4.Delete();

			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(6U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			HeapManager::GetHeapManager()->DefragmentHeap(id);

			Assert::IsTrue(HeapManager::GetHeapManager()->IsHeapConsistent(id));
			Assert::AreEqual(4U, HeapManager::GetHeapManager()->GetHeapNumBlocks(id));

			Assert::AreEqual(5, foo1->GetData());
			Assert::AreEqual(15, foo3->GetData());
			Assert::AreEqual(25, foo5->GetData());
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
		static DesktopMemory memory;
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState MemoryManagerTest::sStartMemState;
#endif
	DesktopMemory MemoryManagerTest::memory;
}
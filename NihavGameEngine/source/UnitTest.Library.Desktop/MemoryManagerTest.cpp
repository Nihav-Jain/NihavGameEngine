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
			delete HeapManager::GetHeapManager();

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

			//// pointer arithmetic works
			//char *randomNumbers = (char *)allocateBlock(100, 4);
			//char *randomNumbers1 = randomNumbers;
			//for (int i = 0; i<100; ++i)
			//{
			//	*randomNumbers++ = (char)getRangedRandom(0, 255);
			//}
			//freeBlock(randomNumbers1);
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
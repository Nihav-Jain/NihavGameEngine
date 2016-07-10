#include "pch.h"
#include "CppUnitTest.h"
#include "FileManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(EngineModuleTest)
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
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(ModuleLifecycle)
		{
			Engine::CreateEngine();
			Assert::AreEqual(10, FileManager::Get().TestValue);

			Engine::Get().Activate();
			Assert::AreEqual(20, FileManager::Get().TestValue);

			Engine::Get().Deactivate();
			Assert::AreEqual(30, FileManager::Get().TestValue);

			Engine::Destroy();

			auto expression = [&] {
				FileManager& something = FileManager::Get();
				UNREFERENCED_PARAMETER(something);
			};
			Assert::ExpectException<std::exception>(expression);
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EngineModuleTest::sStartMemState;
#endif
}
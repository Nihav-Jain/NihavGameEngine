#include "pch.h"
#include "CppUnitTest.h"
#include "FileManager.h"
#include "FileHandle.h"
#include "Engine.h"
#include "UWPFileManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTest_Library_UWP
{

	TEST_CLASS(UWPFileManagerTest)
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
				//Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(UWP_FileManagerCtorTest)
		{
			Engine::CreateEngine();

			Assert::IsTrue(FileManager::Get().Is(UWPFileManager::TypeIdClass()));

			UWPFileManager* fileManager = FileManager::Get().AssertiveAs<UWPFileManager>();

			while(!fileManager->AssetsFolderFound())
			{ }

			Engine::Destroy();
		}

		TEST_METHOD(UWP_GetFileAsyncTest)
		{
			Engine::CreateEngine();

			Assert::IsTrue(FileManager::Get().Is(UWPFileManager::TypeIdClass()));
			UWPFileManager* fileManager = FileManager::Get().AssertiveAs<UWPFileManager>();

			while (!fileManager->AssetsFolderFound())
			{}

			bool allDone = false;
			FileHandle* testXml = nullptr;
			std::function<void(FileHandle*)> getFileCallback = [&](FileHandle* testXmlFileHandle) {
				testXml = testXmlFileHandle;
				Assert::IsNotNull(testXml);
				Assert::IsFalse(testXml->IsOpen());
				allDone = true;
			};
			FileManager::Get().GetFileAsync("test.xml", getFileCallback);
			while (!allDone)
			{}

			Engine::Destroy();
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState UWPFileManagerTest::sStartMemState;
#endif
}
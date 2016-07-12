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
			delete testXml;
		}

		TEST_METHOD(UWP_ReadTextAsyncTest)
		{
			Engine::CreateEngine();

			UWPFileManager* fileManager = FileManager::Get().AssertiveAs<UWPFileManager>();
			while (!fileManager->AssetsFolderFound())
			{}

			bool allDone = false;
			FileHandle* testXml = nullptr;
			std::string text;
			std::function<void(std::string)> readTextCallback = [&](std::string textFromCallback) {
				text = textFromCallback;
				Assert::IsTrue("<something></something>" == text);
				allDone = true;
			};
			std::function<void(FileHandle*)> getFileCallback = [&](FileHandle* testXmlFileHandle) {
				testXml = testXmlFileHandle;
				Assert::IsNotNull(testXml);
				Assert::IsFalse(testXml->IsOpen());
				testXml->ReadTextAsync(readTextCallback);
			};
			FileManager::Get().GetFileAsync("test.xml", getFileCallback);
			while (!allDone)
			{}

			Engine::Destroy();
			delete testXml;

		}

		TEST_METHOD(UWP_ReadLineTest)
		{
			Engine::CreateEngine();

			bool allDone = false;
			FileHandle* testXml = nullptr;
			std::function<void(void)> openFileCallback = [&]() {
				Assert::IsFalse(testXml->IsEndOfFile());
				Assert::IsTrue("<something></something>" == testXml->ReadLine());
				Assert::IsFalse(testXml->IsEndOfFile());
				Assert::IsTrue("<everything></everything>" == testXml->ReadLine());
				Assert::IsTrue(testXml->IsEndOfFile());

				allDone = true;
			};
			std::function<void(FileHandle*)> getFileCallback = [&](FileHandle* testXmlFileHandle) {
				testXml = testXmlFileHandle;
				Assert::IsNotNull(testXml);
				Assert::IsFalse(testXml->IsOpen());

				testXml->OpenFileAsync(openFileCallback);
			};
			FileManager::Get().GetFileAsync("test2.xml", getFileCallback);


			while (!allDone)
			{
			}

			Engine::Destroy();
			delete testXml;
		}


#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState UWPFileManagerTest::sStartMemState;
#endif
}
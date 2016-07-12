#include "pch.h"
#include "CppUnitTest.h"
#include "DesktopFileManager.h"
#include "DesktopFileHandle.h"
#include "Engine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(FileManagerTest)
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

		TEST_METHOD(FileManagerCtorTest)
		{
			Engine::CreateEngine();
			Assert::IsTrue(FileManager::Get().Is(DesktopFileManager::TypeIdClass()));
			Engine::Destroy();
		}

		TEST_METHOD(GetFileAsyncTest)
		{
			Engine::CreateEngine();

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

		TEST_METHOD(ReadTextAsyncTest)
		{
			Engine::CreateEngine();

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
	_CrtMemState FileManagerTest::sStartMemState;
#endif
}
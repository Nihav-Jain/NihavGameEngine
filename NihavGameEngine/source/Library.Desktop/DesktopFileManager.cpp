#include "pch.h"
#include "DesktopFileManager.h"
#include "DesktopFileHandle.h"

namespace Library
{
	DesktopFileManagerFactory DesktopFileManager::sFactory;
	const std::string DesktopFileManager::ASSETS_FOLDER_PREFIX = "Content/";
	RTTI_DEFINITIONS(DesktopFileManager, FileManager);
	ENGINE_MODULE_DEFINITIONS(DesktopFileManager, &DesktopFileManager::sRunTimeTypeId, &FileManager::sInstance);

	DesktopFileManager::DesktopFileManager()
	{
	}


	DesktopFileManager::~DesktopFileManager()
	{
	}

	void DesktopFileManager::GetFileAsync(const std::string& fileName, std::function<void(FileHandle*)>& callback)
	{
		std::future<void> fut = std::async([&]() {
			std::ifstream* fileStream = new std::ifstream();
			fileStream->open(ASSETS_FOLDER_PREFIX + fileName, std::ifstream::in);
			FileHandle* fileHandle = new DesktopFileHandle(*fileStream);
			callback(fileHandle);
		});
	}

	void DesktopFileManager::CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
	}

}

#include "pch.h"
#include <ppltasks.h>
#include <codecvt>
#include "UWPFileManager.h"
#include "Engine.h"
#include "EngineModule.h"
#include "UWPFileHandle.h"

using namespace Windows::Storage;

namespace Library
{
	std::wstring UWPFileManager::ASSETS_FOLDER = L"Assets";
	UWPFileManagerFactory UWPFileManager::sFactory;
	RTTI_DEFINITIONS(UWPFileManager, FileManager);
	ENGINE_MODULE_DEFINITIONS(UWPFileManager, &UWPFileManager::sRunTimeTypeId, &FileManager::sInstance);

	UWPFileManager::UWPFileManager() :
		bAssetsFolderFound(false), mAssetsFolder(Windows::ApplicationModel::Package::Current->InstalledLocation),
		mMutex()
	{
		Platform::String^ assetsFolder = ref new Platform::String(ASSETS_FOLDER.c_str());
		concurrency::create_task(Windows::ApplicationModel::Package::Current->InstalledLocation->GetFolderAsync(assetsFolder)).then([&](StorageFolder^ foundFolder)
			{
				std::lock_guard<std::mutex> lock(mMutex);			
				mAssetsFolder = foundFolder;
				bAssetsFolderFound = true;
			}
		);
	}


	UWPFileManager::~UWPFileManager()
	{
		delete mAssetsFolder;
	}

	void UWPFileManager::GetFileAsync(const std::string& fileName, const std::function<void(FileHandle*)>& callback)
	{
		{
			std::lock_guard<std::mutex> lock(mMutex);
			if (!bAssetsFolderFound)
				throw std::exception("FileManager::OpenFileAsync() - Assets folder not loaded yet, delay this function call.");
		}

		std::wstring wFileName;
		wFileName.assign(fileName.begin(), fileName.end());

		Platform::String^ pFileName = ref new Platform::String(wFileName.c_str());
		concurrency::create_task(mAssetsFolder->GetFileAsync(pFileName)).then([&](StorageFile^ foundFile)
		{
			FileHandle* fileHandle = new UWPFileHandle(foundFile);
			callback(fileHandle);
		});
	}

	void UWPFileManager::CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
	}

	bool UWPFileManager::AssetsFolderFound() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return bAssetsFolderFound;
	}
}

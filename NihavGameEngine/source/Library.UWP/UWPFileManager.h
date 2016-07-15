#pragma once
#include <mutex>
#include "FileManager.h"

namespace Library
{
	class UWPFileManager : public FileManager
	{
		RTTI_DECLARATIONS(UWPFileManager, FileManager);
		ENGINE_MODULE_DECLARATIONS();
	public:
		UWPFileManager();
		virtual ~UWPFileManager();

		virtual void GetFileAsync(const std::string& fileName, const std::function<void(FileHandle*)>& callback) override;
		virtual void CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName) override;

		bool AssetsFolderFound() const;

	private:
		static class UWPFileManagerFactory sFactory;
		Windows::Storage::StorageFolder^ mAssetsFolder;
		bool bAssetsFolderFound;
		mutable std::mutex mMutex;

		static std::wstring ASSETS_FOLDER;

	};

	CONCRETE_MODULE_FACTORY(UWPFileManager);

}


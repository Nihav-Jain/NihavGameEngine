#pragma once
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

		virtual void OpenFileAsync(FileHandle* outFileHandle, const std::string& fileName, FileHandle::FileMode mode = FileHandle::FileMode::READ_ONLY) override;
		virtual void CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName) override;

	private:
		static class UWPFileManagerFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(UWPFileManager);

}


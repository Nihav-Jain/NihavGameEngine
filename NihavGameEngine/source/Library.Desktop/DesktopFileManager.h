#pragma once
#include "FileManager.h"

namespace Library
{
	class DesktopFileManager : public FileManager
	{
		RTTI_DECLARATIONS(DesktopFileManager, FileManager);
		ENGINE_MODULE_DECLARATIONS();
	public:
		DesktopFileManager();
		virtual ~DesktopFileManager();

		virtual void OpenFileAsync(FileHandle* outFileHandle, const std::string& fileName, FileHandle::FileMode mode = FileHandle::FileMode::READ_ONLY) override;
		virtual void CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName) override;

	private:
		static class DesktopFileManagerFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(DesktopFileManager);
}

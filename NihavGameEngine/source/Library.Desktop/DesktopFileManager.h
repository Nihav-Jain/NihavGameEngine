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

		virtual void GetFileAsync(const std::string& fileName, std::function<void(FileHandle*)>& callback) override;
		virtual void CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName) override;

	private:
		static class DesktopFileManagerFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(DesktopFileManager);
}

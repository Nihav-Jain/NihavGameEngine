#pragma once
#include "EngineModule.h"
#include "FileHandle.h"

namespace Library
{
	class FileManager : public EngineModule
	{
		RTTI_DECLARATIONS(FileManager, EngineModule);
	public:
		FileManager();
		virtual ~FileManager();

		static FileManager& Get();

		virtual void OpenFileAsync(FileHandle* outFileHandle, const std::string& fileName, FileHandle::FileMode mode = FileHandle::FileMode::READ_ONLY) = 0;
		virtual void CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName) = 0;

	protected:
		static FileManager* sInstance;
	};
}


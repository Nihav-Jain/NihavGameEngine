#pragma once
#include "EngineModule.h"
#include "FileHandle.h"
#include <functional>

namespace Library
{
	class FileManager : public EngineModule
	{
		RTTI_DECLARATIONS(FileManager, EngineModule);
	public:
		FileManager();
		virtual ~FileManager();

		static FileManager& Get();

		virtual void GetFileAsync(const std::string& fileName, const std::function<void(FileHandle*)>& callback) = 0;
		virtual void CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName) = 0;

	protected:
		static FileManager* sInstance;
	};
}


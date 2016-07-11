#include "pch.h"
#include "FileManager.h"

namespace Library
{
	FileManager* FileManager::sInstance = nullptr;
	RTTI_DEFINITIONS(FileManager, EngineModule);

	FileManager::FileManager()
	{}

	FileManager::~FileManager()
	{}

	FileManager& FileManager::Get()
	{
		return *sInstance;
	}
}

#include "pch.h"
#include "FileManager.h"
#include "Engine.h"
#include "EngineModule.h"

namespace Library
{
	FileManager* FileManager::sInstance;
	FileManagerFactory FileManager::sFactory;
	RTTI_DEFINITIONS(FileManager, EngineModule);
	ENGINE_MODULE_DEFINITIONS(FileManager, &FileManager::sRunTimeTypeId, &FileManager::sInstance);

	FileManager::FileManager()
	{
	}

	FileManager::~FileManager()
	{
	}

	void FileManager::Activate()
	{}

	void FileManager::Deactivate()
	{}
}

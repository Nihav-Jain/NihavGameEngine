#include "pch.h"
#include "FileManager.h"
#include "Engine.h"
#include "EngineModule.h"

namespace Library
{
	FileManager* FileManager::sInstance = nullptr;
	FileManagerFactory FileManager::sFactory;
	RTTI_DEFINITIONS(FileManager, EngineModule);
	ENGINE_MODULE_DEFINITIONS(FileManager, &FileManager::sRunTimeTypeId, &FileManager::sInstance);

	FileManager::FileManager() :
		TestValue(10)
	{
	}

	FileManager::~FileManager()
	{
	}

	void FileManager::Activate()
	{
		TestValue = 20;
	}

	void FileManager::Deactivate()
	{
		TestValue = 30;
	}

	FileManager& FileManager::Get()
	{
		if (sInstance == nullptr)
			throw std::exception("FileManager not instantiated");
		return *sInstance;
	}
}

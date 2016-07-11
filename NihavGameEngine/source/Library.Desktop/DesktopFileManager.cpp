#include "pch.h"
#include "DesktopFileManager.h"

namespace Library
{
	DesktopFileManagerFactory DesktopFileManager::sFactory;
	RTTI_DEFINITIONS(DesktopFileManager, FileManager);
	ENGINE_MODULE_DEFINITIONS(DesktopFileManager, &DesktopFileManager::sRunTimeTypeId, &FileManager::sInstance);

	DesktopFileManager::DesktopFileManager()
	{
	}


	DesktopFileManager::~DesktopFileManager()
	{
	}
}

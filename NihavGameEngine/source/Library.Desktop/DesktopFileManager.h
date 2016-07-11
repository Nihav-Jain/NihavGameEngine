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

	private:
		static class DesktopFileManagerFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(DesktopFileManager);
}

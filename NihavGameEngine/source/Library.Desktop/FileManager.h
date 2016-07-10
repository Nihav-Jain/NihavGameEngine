#pragma once
#include "EngineModule.h"

namespace Library
{

	class FileManager final : public EngineModule
	{
	private:
		static FileManager* sInstance;

		RTTI_DECLARATIONS(FileManager, EngineModule);
		ENGINE_MODULE_DECLARATIONS();

	public:
		FileManager();
		~FileManager();

		virtual void Activate() override;
		virtual void Deactivate() override;

	private:
		static class FileManagerFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(FileManager);
}


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
		virtual ~FileManager();

		virtual void Activate() override;
		virtual void Deactivate() override;

		std::int32_t TestValue;

		static FileManager& Get();

	private:
		static class FileManagerFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(FileManager);
}


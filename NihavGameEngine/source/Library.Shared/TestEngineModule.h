#pragma once
#include "EngineModule.h"

namespace Library
{

	class TestEngineModule final : public EngineModule
	{
	private:
		static TestEngineModule* sInstance;

		RTTI_DECLARATIONS(TestEngineModule, EngineModule);
		ENGINE_MODULE_DECLARATIONS();

	public:
		TestEngineModule();
		virtual ~TestEngineModule();

		virtual void Activate() override;
		virtual void Deactivate() override;

		std::int32_t TestValue;

		static TestEngineModule& Get();

	private:
		static class TestEngineModuleFactory sFactory;
	};

	CONCRETE_MODULE_FACTORY(TestEngineModule);
}


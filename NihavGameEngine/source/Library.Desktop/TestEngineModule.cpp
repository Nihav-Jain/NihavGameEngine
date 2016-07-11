#include "pch.h"
#include "TestEngineModule.h"
#include "Engine.h"
#include "EngineModule.h"

namespace Library
{
	TestEngineModule* TestEngineModule::sInstance = nullptr;
	TestEngineModuleFactory TestEngineModule::sFactory;
	RTTI_DEFINITIONS(TestEngineModule, EngineModule);
	ENGINE_MODULE_DEFINITIONS(TestEngineModule, &TestEngineModule::sRunTimeTypeId, &TestEngineModule::sInstance);

	TestEngineModule::TestEngineModule() :
		TestValue(10)
	{
	}

	TestEngineModule::~TestEngineModule()
	{
	}

	void TestEngineModule::Activate()
	{
		TestValue = 20;
	}

	void TestEngineModule::Deactivate()
	{
		TestValue = 30;
	}

	TestEngineModule& TestEngineModule::Get()
	{
		if (sInstance == nullptr)
			throw std::exception("TestEngineModule not instantiated");
		return *sInstance;
	}
}

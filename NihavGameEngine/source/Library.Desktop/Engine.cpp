#include "pch.h"
#include "Engine.h"
#include "EngineModule.h"

namespace Library
{
	Engine* Engine::sInstance = nullptr;
	const Engine::EngineModuleListDeleter Engine::sDeleter;

	Engine::Engine()
	{}

	Engine& Engine::GetInstance()
	{
		if (sInstance == nullptr)
			sInstance = new Engine();
		return *sInstance;
	}

	void Engine::DestroyInstance()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	Hashmap<const std::uint64_t*, EngineModule**>& Engine::ModuleList()
	{
		static Hashmap<const std::uint64_t*, EngineModule**>* map = new Hashmap<const std::uint64_t*, EngineModule**>();
		return *map;
	}

	Engine::~Engine()
	{}

	Engine::EngineModuleListDeleter::EngineModuleListDeleter() :
		// this ensures that the static hashmap is constructed before the completetion of the constrcutor of EngineModuleListDeleter
		// which, in turn, ensures that the static ModuleList hashmap ptr will not be deleted before the destructor of EngineModuleListDeleter
		JustAPtr(&Engine::ModuleList())
	{}

	Engine::EngineModuleListDeleter::~EngineModuleListDeleter()
	{
		delete &Engine::ModuleList();
	}
}

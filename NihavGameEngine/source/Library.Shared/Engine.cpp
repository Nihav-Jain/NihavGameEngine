#include "pch.h"
#include "Engine.h"
#include "EngineModule.h"

namespace Library
{
	Engine* Engine::sInstance = nullptr;
	const Engine::EngineModuleListDeleter Engine::sDeleter;

	Engine::Engine() :
		mModules()
	{
		Hashmap<const std::uint64_t*, EngineModule**>& moduleList = ModuleList();

		for (auto& pair : moduleList)
		{
			*(pair.second) = Factory<EngineModule>::Create(*(pair.first));
			mModules.Insert(*(pair.first), *(pair.second));
		}
	}

	void Engine::CreateEngine()
	{
		if (sInstance == nullptr)
			sInstance = new Engine();
	}

	Engine& Engine::Get()
	{
		if (sInstance == nullptr)
			sInstance = new Engine();
		return *sInstance;
	}

	void Engine::Destroy()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	Hashmap<const std::uint64_t*, EngineModule**>& Engine::ModuleList()
	{
		static Hashmap<const std::uint64_t*, EngineModule**>* map = new Hashmap<const std::uint64_t*, EngineModule**>();
		return *map;
	}

	void Engine::Activate()
	{
		for (auto& pair : mModules)
		{
			pair.second->Activate();
		}
	}

	void Engine::Deactivate()
	{
		for (auto& pair : mModules)
		{
			pair.second->Deactivate();
		}
	}

	EngineModule* Engine::GetEngineModule(std::uint64_t classTypeId)
	{
		if (mModules.ContainsKey(classTypeId))
			return mModules[classTypeId];
		return nullptr;
	}

	Engine::~Engine()
	{
		for (auto& pair : mModules)
		{
			std::uint64_t* moduleTypeIdPtr = reinterpret_cast<std::uint64_t*>(pair.second->TypeIdInstance());
			delete pair.second;

			assert(ModuleList().ContainsKey(moduleTypeIdPtr));
			*(ModuleList().operator[](moduleTypeIdPtr)) = nullptr;
		}
	}

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

#pragma once
#include "EngineModule.h"

namespace Library
{

	class Engine final
	{

	public:
		static void CreateEngine();
		static Engine& Get();
		static void Destroy();

		static Hashmap<const std::uint64_t*, EngineModule**>& ModuleList();

		void Activate();
		void Deactivate();
		EngineModule* GetEngineModule(std::uint64_t classTypeId);

	private:
		static Engine* sInstance;
		Engine();
		~Engine();

		class EngineModuleListDeleter
		{
		public:
			EngineModuleListDeleter();
			~EngineModuleListDeleter();
		private:
			Hashmap<const std::uint64_t*, EngineModule**>* JustAPtr;
		};

		static const EngineModuleListDeleter sDeleter;
		Hashmap<std::uint64_t, EngineModule*> mModules;
	};
}


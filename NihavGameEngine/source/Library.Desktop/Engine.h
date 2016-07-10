#pragma once
#include "EngineModule.h"

namespace Library
{

	class Engine final
	{

	public:
		static Engine& GetInstance();
		static void DestroyInstance();

		static Hashmap<const std::uint64_t*, EngineModule**>& ModuleList();
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
	};
}


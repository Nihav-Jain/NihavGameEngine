#pragma once
#include "RTTI.h"
#include "Factory.h"

namespace Library
{
	class EngineModule : public RTTI
	{
		RTTI_DECLARATIONS(EngineModule, RTTI);

	public:
		EngineModule();
		virtual ~EngineModule();

		virtual void Activate() = 0;
		virtual void Deactivate() = 0;

		friend class Engine;
	};

#define CONCRETE_MODULE_FACTORY(ConcreteModuleType) CONCRETE_FACTORY(ConcreteModuleType, EngineModule);
#define ENGINE_MODULE_DECLARATIONS()										\
private:																	\
	static const Library::Hashmap<const std::uint64_t*, EngineModule**>::Iterator Itr;

#define ENGINE_MODULE_DEFINITIONS(ModuleType, PointerToTypeId, PointerToSingletonPtr)	\
	const Library::Hashmap<const std::uint64_t*, EngineModule**>::Iterator ModuleType::Itr = Engine::ModuleList().Insert(PointerToTypeId, reinterpret_cast<EngineModule**>(PointerToSingletonPtr));
}

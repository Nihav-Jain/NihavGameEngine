#pragma once
#include "pch.h"
#include "Factory.h"

namespace Library
{
	template<typename AbstractProductT>
	Hashmap<std::string, const Factory<AbstractProductT>* const> Factory<AbstractProductT>::sFactories;
	template<typename AbstractProductT>
	Hashmap<std::uint64_t, const Factory<AbstractProductT>* const> Factory<AbstractProductT>::sFactoriesByTypeId;

	template<typename AbstractProductT>
	const Factory<AbstractProductT>* const Factory<AbstractProductT>::Find(const std::string& className)
	{
		Iterator itr = sFactories.Find(className);
		if (itr == sFactories.end())
			return nullptr;
		return (*itr).second;
	}

	template<typename AbstractProductT>
	const Factory<AbstractProductT>* const Factory<AbstractProductT>::Find(std::uint64_t classTypeId)
	{
		Hashmap<std::uint64_t, const Factory* const>::Iterator itr = sFactoriesByTypeId.Find(classTypeId);
		if (itr == sFactoriesByTypeId.end())
			return nullptr;
		return (*itr).second;
	}

	template<typename AbstractProductT>
	AbstractProductT* Factory<AbstractProductT>::Create(const std::string& className)
	{
		const Factory* const factoryInstance = Find(className);
		if (factoryInstance == nullptr)
			return nullptr;
		return factoryInstance->Create();
	}

	template<typename AbstractProductT>
	AbstractProductT* Factory<AbstractProductT>::Create(std::uint64_t classTypeId)
	{
		const Factory* const factoryInstance = Find(classTypeId);
		if (factoryInstance == nullptr)
			return nullptr;
		return factoryInstance->Create();
	}

	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::Iterator Factory<AbstractProductT>::begin()
	{
		return sFactories.begin();
	}

	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::Iterator Factory<AbstractProductT>::end()
	{
		return sFactories.end();
	}

	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Add(Factory& factory, std::uint64_t classTypeId)
	{
		if (!sFactories.ContainsKey(factory.ClassName()))
		{
			sFactories.Insert(factory.ClassName(), &factory);
			sFactoriesByTypeId.Insert(classTypeId, &factory);
		}
	}

	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Remove(const Factory& factory, std::uint64_t classTypeId)
	{
		sFactories.Remove(factory.ClassName());
		sFactoriesByTypeId.Remove(classTypeId);
	}
}
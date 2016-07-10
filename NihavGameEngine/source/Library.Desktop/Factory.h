#pragma once
#include <string>
#include "Hashmap.h"

namespace Library
{
	/**
	 *	Templated Factory class to create objects by class name
	 *	AbsrtackProductT is the typename of the Factory group
	 */
	template <typename AbstractProductT>
	class Factory
	{
	public:
		typedef typename Hashmap<std::string, const Factory* const>::Iterator Iterator;

		/**
		 *	Default constructor
		 */
		Factory() = default;

		/**
		 *	Default destructor
		 */
		virtual ~Factory() = default;

		/**
		 *	Getter for the name of the class registered with the Factory
		 *	@return name of the class
		 */
		virtual std::string ClassName() const = 0;

		/**
		 *	Virtual constructor for the class registered with the Factory
		 *	IMPORTANT - the caller owns the memory for this object and thus needs to delete it
		 *	@return pointer to the class object
		 */
		virtual AbstractProductT* Create() const = 0;

		/**
		 *	Getter for the Factory instance for the given class name
		 *	@param name of the class whose factory is to be found
		 *	@return pointer to the factory instance of the given class name
		 */
		static const Factory* const Find(const std::string& className);

		/**
		 *	Getter for the Factory instance for the given class type id
		 *	@param type id of the class whose factory is to be found
		 *	@return pointer to the factory instance of the given class name
		 */
		static const Factory* const Find(std::uint64_t classTypeId);

		/**
		 *	Getter for the object of the given class name
		 *	@param name of the class whose object is to be created
		 *	@return pointer to the class object
		 */
		static AbstractProductT* Create(const std::string& className);

		/**
		 *	Getter for the object of the given class type id
		 *	@param type id of the class whose object is to be created
		 *	@return pointer to the class object
		 */
		static AbstractProductT* Create(std::uint64_t classTypeId);

		/**
		 *	Gets the Iterator pointing to first factory of this abstract factory group
		 *	@return Iterator
		 */
		static Iterator begin();

		/**
		 *	Gets the Iterator pointing to end list of factories of this abstract factory group
		 *	@return Iterator
		 */
		static Iterator end();

	protected:
		
		/**
		 *	Registers the given factory with the abstract factory group
		 *	@param reference to the concrete factory
		 */
		void Add(Factory& factory, std::uint64_t classTypeId);

		/**
		 *	Un-registers the given factory with the abstract factory group
		 *	@param reference to the concrete factory to be removed
		 */
		void Remove(const Factory& factory, std::uint64_t classTypeId);

	private:
		static Hashmap<std::string, const Factory* const> sFactories;
		static Hashmap<std::uint64_t, const Factory* const> sFactoriesByTypeId;
	};

		/**
		 *	Use this macro to create and register a Factory for the given concrete type
		 *	@param concrete type for who the factory is to be created
		 *	@param abstract type of the group this Factory will belong to
		 */
#define CONCRETE_FACTORY(ConcreteProductType, AbstractProductType)				\
	class ConcreteProductType ## Factory : public Factory<AbstractProductType>	\
	{																			\
		public:																	\
			ConcreteProductType ## Factory()									\
			{																	\
				Add(*this, ConcreteProductType::TypeIdClass());					\
			}																	\
			virtual ~ConcreteProductType ## Factory()							\
			{																	\
				Remove(*this, ConcreteProductType::TypeIdClass());				\
			}																	\
			virtual std::string ClassName() const override						\
			{																	\
				return #ConcreteProductType;									\
			}																	\
			virtual AbstractProductType* Create() const override				\
			{																	\
				return new ConcreteProductType();								\
			}																	\
	};
}

#include "Factory.inl"
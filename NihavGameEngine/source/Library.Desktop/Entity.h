#pragma once
#include "RTTI.h"
#include "Attributed.h"
#include "Factory.h"
#include "WorldState.h"
#include "Action.h"

namespace Library
{
	class Sector;

	/**
	 *	Defines a game entity
	 *	Implement the CONCRETE_ENTITY_FACTORY macro with the attribute <ChildClassName> to create a factory for your derived Entity class
	 *	Has 1 prescribed attribute - name (STRING)
	 *	requires XmlParseHelperEntity to being created via XML
	 *	@inherits Attributed
	 */
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed);
	public:

		/**
	 	 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		Entity();

		/**
		 *	Copy Constructor - deep copies the member variables and attributes
		 *	@param constant reference to the Entity to be copied
		 */
		Entity(const Entity& rhs);
		
		/**
		 *	Move Constructor
		 *	@param R value reference to the Entity to be moved
		 */
		Entity(Entity&& rhs);

		/**
		 *	Default destructor
		 */
		virtual ~Entity();

		/**
		 *	Copy assignment operator - deep copies the member variables and attributes
		 *	@param constant reference to the Entity to be copied
		 *	@return reference to the updated Entity
		 */
		Entity& operator=(const Entity& rhs);
		
		/**
		 *	Move assignment operator
		 *	@param R value reference to the Entity to be moved
		 *	@return reference to the updated Entity
		 */
		Entity& operator=(Entity&& rhs);

		/**
		 *	Getter for the name of this Entity
		 *	@return constance reference to the string representing the name
		 */
		const std::string& Name() const;

		/**
		 *	Setter for the name of this Entity
		 *	@param constance reference to the string representing the name
		 */
		void SetName(const std::string& name);

		/**
		 *	Getter for the Datum which contains the array of Entities of this Sector
		 *	@return reference to the Datum
		 */
		Datum& Actions();

		/**
		 *	const override for the Getter for the Datum which contains the array of Entities of this Sector
		 *	@return constant reference to the Datum
		 */
		const Datum& Actions() const;

		/**
		 *	Adds an Entity of the given name to this Sector
		 *	@param name of the derived Entity class which is to be created
		 *	@param  instance name of the Entity
		 *	@return reference to the newly created Entity
		 */
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/**
		 *	Searches for the Entity of the given name in this Sector
		 *	@param name of the Entity to be searched
		 *	@return pointer to the Entity if found, nullptr if there is no sector of this name
		 */
		Action* FindAction(const std::string& actionName) const;

		/**
		 *	Getter for the Sector which contains this Entity
		 *	@return pointer to the Sector
		 */
		Sector* GetSector() const;

		/**
		 *	Setter for the Sector which contains this Entity
		 *	@param reference to the Sector which adopts this Entity
		 */
		void SetSector(Sector& parent);

		/**
		 *	Calls the update method on all of its child Actions, called by the parent Sector's Update method
		 *	Must be overriden by children of they want to dp anything extra
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState);

		/**
		 *	acts as a virtual copy constructor
		 *	@param constant reference to the Entity to be copied / cloned
		 *	@return pointer to the newly instantiated Entity
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::uint32_t NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		static const std::string ATTRIBUTE_NAME;
		static const std::string ATTRIBUTE_ACTIONS;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;
	};

#define CONCRETE_ENTITY_FACTORY(ConcreteEntityType) CONCRETE_FACTORY(ConcreteEntityType, Entity);

	CONCRETE_ENTITY_FACTORY(Entity);
}


#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace Library
{
	/**
	 *	Defines an abstract Action
	 *	Implement the CONCRETE_ACTION_FACTORY macro with the attribute <ChildClassName> to create a factory for your derived Action class
	 *	Has 1 prescribed attribute - name (STRING)
	 *	requires XmlParseHelperAction to being created via XML
	 *	@inherits Attributed
	 */
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

	public:
		
		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		Action();

		/**
		 *	Default destructor
		 */
		virtual ~Action() = default;

		/**
		 *	disallow copy construtor
		 */
		Action(const Action& rhs) = delete;

		/**
		 *	disallow copy assignment operator
		 */
		Action& operator=(const Action& rhs) = delete;

		/**
		 *	Getter for the name of this Action
		 *	@return constance reference to the string representing the name
		 */
		const std::string& Name() const;

		/**
		 *	Setter for the name of this Action
		 *	@param constance reference to the string representing the name
		 */
		void SetName(const std::string& name);

		/**
		 *	Will be called by the parent Entity / Sector / World's Update method every frame
		 *	Should be used to implement the main logic of a apecialized Action / Component
		 *	Pure virual function, needs to be implemented by the children
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState) = 0;

		/**
		 *	Will be called by the EndElementHandler of Action parser
		 *	Can be used for processing the Action after the whole element has been parsed
		 */
		virtual void PostParsingProcess();

		static const std::uint32_t NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		static const std::string ATTRIBUTE_NAME;

	private:
		std::string mName;
	};

#define CONCRETE_ACTION_FACTORY(ConcreteActionType) CONCRETE_FACTORY(ConcreteActionType, Action);

}

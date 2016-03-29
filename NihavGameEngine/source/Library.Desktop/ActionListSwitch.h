#pragma once
#include "Hashmap.h"
#include "ActionList.h"

namespace Library
{
	class ActionListSwitch : public ActionList
	{
		RTTI_DECLARATIONS(ActionListSwitch, ActionList);

	public:

		class ActionListSwitchCase : public ActionList
		{
			RTTI_DECLARATIONS(ActionListSwitchCase, ActionList);

		public:
			ActionListSwitchCase();
			ActionListSwitchCase(const ActionListSwitchCase& rhs) = delete;
			ActionListSwitchCase& operator=(const ActionListSwitchCase& rhs) = delete;

			virtual ~ActionListSwitchCase() = default;

			static const std::string ATTRIBUTE_CASE_VALUE;
		protected:
			virtual void DefinePrescribedAttributes();
		private:
		};

		CONCRETE_ACTION_FACTORY(ActionListSwitchCase);

		ActionListSwitch();
		ActionListSwitch(const ActionListSwitch& rhs);
		ActionListSwitch(ActionListSwitch&& rhs);

		virtual ~ActionListSwitch();

		ActionListSwitch& operator=(const ActionListSwitch& rhs);
		ActionListSwitch& operator=(ActionListSwitch&& rhs);

		virtual void PostParsingProcess() override;
		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;
	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		void GenerateCaseMap();

		static const std::string ATTRIBUTE_SWITCH_VALUE;
		Hashmap<std::string, ActionListSwitchCase*>* mCaseMap;
	};

	CONCRETE_ACTION_FACTORY(ActionListSwitch);
}


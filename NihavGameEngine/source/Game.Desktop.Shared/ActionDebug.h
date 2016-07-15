#pragma once
#include "Game.h"
#include "Action.h"

namespace Library
{
	class ActionDebug : public ActionList
	{
		RTTI_DECLARATIONS(ActionDebug, ActionList)
	public:
		ActionDebug();
		ActionDebug(const ActionDebug& other) = default;
		ActionDebug(ActionDebug&& other) = default;
		ActionDebug& operator=(const ActionDebug& rhs) = default;
		ActionDebug& operator=(ActionDebug&& rhs) = default;
		~ActionDebug();
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(ActionDebug)
}


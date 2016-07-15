#include "pch.h"
#include "ActionTogglePauseMusic.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionTogglePauseMusic, ActionList);

		const std::string ActionTogglePauseMusic::ATTRIBUTE_NAME = "musicName";

	ActionTogglePauseMusic::ActionTogglePauseMusic()
	{
		AddInternalAttribute(ATTRIBUTE_NAME, "", 0);

	}
	Scope * ActionTogglePauseMusic::Clone(const Scope & rhs) const
	{
		ActionTogglePauseMusic& entity = *rhs.AssertiveAs<ActionTogglePauseMusic>();
		return new ActionTogglePauseMusic(entity);
	}


	void ActionTogglePauseMusic::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		AudioManager::Get().TogglePauseMusic((*this)[ATTRIBUTE_NAME].Get<std::string>());
	}
}


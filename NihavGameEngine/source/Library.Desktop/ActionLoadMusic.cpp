#include "pch.h"
#include "ActionLoadMusic.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionLoadMusic, ActionList);

	const std::string ActionLoadMusic::ATTRIBUTE_NAME = "musicName";

	ActionLoadMusic::ActionLoadMusic()
	{
		AddInternalAttribute(ATTRIBUTE_NAME, "", 0);
	}


	void ActionLoadMusic::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		std::uint32_t totalMusicFiles = (*this)[ATTRIBUTE_NAME].Size();
		for (std::uint32_t currentMusic = 0; currentMusic < totalMusicFiles; ++currentMusic)
		{
			AudioManager::Get().LoadMusic((*this)[ATTRIBUTE_NAME].Get<std::string>(currentMusic));
		}
	}
}

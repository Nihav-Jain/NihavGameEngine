#include "pch.h"
#include "AudioManager.h"

namespace Library
{
	AudioManager* AudioManager::sInstance = nullptr;
	RTTI_DEFINITIONS(AudioManager, EngineModule);


	AudioManager& AudioManager::Get()
	{
		return *sInstance;
	}

}

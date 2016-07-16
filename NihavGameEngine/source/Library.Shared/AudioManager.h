#pragma once
#include "EngineModule.h"

namespace Library
{
	class AudioManager : public EngineModule
	{
		RTTI_DECLARATIONS(AudioManager, EngineModule);
	public:
		AudioManager() = default;
		virtual ~AudioManager() = default;

		static AudioManager& Get();

		/**
		*	LoadMusic - Load music of given name with extension
		*	@param audioNameWithExtension	just name no path
		*/
		virtual void LoadMusic(const std::string& audioNameWithExtension) = 0;

		/**
		*	PlayMusic - Play music of given name with extension
		*	@param audioNameWithExtension	just name no path
		*	@param loopsOneToN	zero for continous looping and N for N loops
		*	@param volumeZeroToOne	volume in float from zero to 1
		*/
		virtual void PlayMusic(const std::string& audioNameWithExtension, std::int32_t loopsOneToN = 1, std::float_t volumeZeroToOne = 1.0f) = 0;

		/**
		*	TogglePauseMusic - Pause if playing and vice verca
		*	@param audioNameWithExtension	just name no path
		*/
		virtual void TogglePauseMusic(const std::string& audioNameWithExtension) = 0;

		/**
		*	StopMusic - Stop Music
		*	@param audioNameWithExtension	just name no path
		*/
		virtual void StopMusic(const std::string& audioNameWithExtension) = 0;

		virtual void Update() {}

	protected:
		static AudioManager* sInstance;
	};
}


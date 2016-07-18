#pragma once

#include "Hashmap.h"
#include "RTTI.h"
#include "AudioManager.h"

namespace Library
{
	const std::int32_t MAX_CHANNELS = 256;

	/**
	 *	Class UWPAudioManager
	 */
	class UWPAudioManager final : public AudioManager
	{
		RTTI_DECLARATIONS(UWPAudioManager, AudioManager);
		ENGINE_MODULE_DECLARATIONS();

	public:
		UWPAudioManager();
		virtual ~UWPAudioManager();

		/**
		*	LoadMusic - Load music of given name with extension
		*	@param audioNameWithExtension	just name no path
		*/
		virtual void LoadMusic (const std::string& audioNameWithExtension) override;

		/**
		*	PlayMusic - Play music of given name with extension
		*	@param audioNameWithExtension	just name no path
		*	@param loopsOneToN	zero for continous looping and N for N loops
		*	@param volumeZeroToOne	volume in float from zero to 1
		*/
		virtual void PlayMusic (const std::string& audioNameWithExtension, std::int32_t loopsOneToN = 1, std::float_t volumeZeroToOne = 1.0f) override;

		/**
		*	TogglePauseMusic - Pause if playing and vice verca
		*	@param audioNameWithExtension	just name no path
		*/
		virtual void TogglePauseMusic(const std::string& audioNameWithExtension) override;

		/**
		*	StopMusic - Stop Music
		*	@param audioNameWithExtension	just name no path
		*/
		virtual void StopMusic(const std::string& audioNameWithExtension) override;

		virtual void Update() override;
	
	private:
	
		const std::string PATH = "Content/Music/";
		const std::int32_t VIRTUAL_CHANNELS = 256;
		const std::int32_t MIN_PRIORITY = 0;
		const std::int32_t MAX_PRIORITY = 256;

		int32_t mNumberOfDrivers;
		int mChannelCount;

		static class UWPAudioManagerFactory sFactory;

	};
	CONCRETE_MODULE_FACTORY(UWPAudioManager);

}



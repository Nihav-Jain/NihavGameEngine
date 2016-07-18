#include "pch.h"
#include "UWPAudioManager.h"

namespace Library
{
	UWPAudioManagerFactory UWPAudioManager::sFactory;
	RTTI_DEFINITIONS(UWPAudioManager, AudioManager);
	ENGINE_MODULE_DEFINITIONS(UWPAudioManager, &UWPAudioManager::sRunTimeTypeId, &AudioManager::sInstance);

	UWPAudioManager::UWPAudioManager() :
		mNumberOfDrivers(0),
		mChannelCount(0)
	{
	}


	UWPAudioManager::~UWPAudioManager()
	{

	}

	void UWPAudioManager::LoadMusic(const std::string & audioNameWithExtension)
	{
		UNREFERENCED_PARAMETER(audioNameWithExtension);
	}

	void UWPAudioManager::PlayMusic(const std::string & audioNameWithExtension, std::int32_t loopsOneToN, std::float_t volumeZeroToOne)
	{
		UNREFERENCED_PARAMETER(audioNameWithExtension);
		UNREFERENCED_PARAMETER(loopsOneToN);
		UNREFERENCED_PARAMETER(volumeZeroToOne);
	}


	void UWPAudioManager::TogglePauseMusic(const std::string & audioNameWithExtension)
	{
		UNREFERENCED_PARAMETER(audioNameWithExtension);
	}

	void UWPAudioManager::StopMusic(const std::string & audioNameWithExtension)
	{
		UNREFERENCED_PARAMETER(audioNameWithExtension);
	}

	void UWPAudioManager::Update()
	{
	}

}

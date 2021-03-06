#pragma once

#include "Game.h"
#include "SpriteManager.h"

#include <vector>

namespace Library
{
	class ScoreManager final : public SpriteManager
	{
	public:
		static ScoreManager* CreateInstance();
		static ScoreManager* GetInstance();
		static void DeleteInstance();

		ScoreManager();
		~ScoreManager();

		virtual void Init(WorldState& worldState) override;
		virtual void RefreshSprites() override;
		virtual void CleanupSprites() override;

		void SetNumberBase(std::int32_t base);

	private:
		static ScoreManager *sInstance;

		std::int32_t mNumberBase;

		std::vector<std::vector<class SpriteRenderer*>> mSprites;
		std::vector<std::vector<class Image*>> mDigits;
		std::vector<Library::SpriteRenderer*> mCurrentSprites;

	};
}

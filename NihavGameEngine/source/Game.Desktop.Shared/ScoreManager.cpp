#include "pch.h"
#include "ScoreManager.h"
#include "RenderDevice.h"
#include "Renderer.h"
#include "Image.h"
#include "SpriteRenderer.h"

namespace Library
{
	ScoreManager* ScoreManager::sInstance = nullptr;

	ScoreManager * ScoreManager::CreateInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new ScoreManager();
		}

		return sInstance;
	}

	ScoreManager * ScoreManager::GetInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new ScoreManager();
		}
		return sInstance;
	}

	void ScoreManager::DeleteInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	ScoreManager::ScoreManager()
		: mNumberBase(10), mSprites(), mDigits(), mCurrentSprites()
	{
	}

	ScoreManager::~ScoreManager()
	{
#ifndef PLATFORM_XBOX
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			for (std::int32_t j = 0; j < mNumberBase; ++j)
			{
				delete mSprites[i][j];
				mSprites[i][j] = nullptr;
			}
		}
#endif
	}

	void ScoreManager::Init(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
#ifndef PLATFORM_XBOX
		mSprites.resize(mNumSprites);
		mDigits.resize(mNumSprites);
		mCurrentSprites.resize(mNumSprites);

		std::int32_t padding = 0;
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			// Determine position
			std::int32_t order = (mOrderLeftToRight) ? i : (mNumSprites - i);
			glm::vec4 pos = glm::vec4(order * mImageSize + mHorizontalOffset + padding, mVerticalOffset, 1, 1);
			padding = (mOrderLeftToRight) ? padding - mPadding : padding + mPadding;

			mDigits[i].resize(mNumberBase);
			mSprites[i].resize(mNumberBase);

			for (std::int32_t j = 0; j < mNumberBase; ++j)
			{
				// Create sprite renderer for each image
				mSprites[i][j] = new SpriteRenderer();
				SpriteRenderer* renderer = mSprites[i][j];

				// Create image per digit per location
				mDigits[i][j] = renderer->CreateAction("Image", "image", *renderer, ActionList::ATTRIBUTE_ACTIONS).AssertiveAs<Image>();
				mDigits[i][j]->SetPath(mBasePath + mFolderPath + std::to_string(j) + mExtension);
				mDigits[i][j]->SetSize(glm::vec4( (float)(mImageSize) ));

				renderer->SetTransform(pos, glm::vec4(), glm::vec4(1));
				renderer->BeginPlay(worldState);
			}

			mCurrentSprites[i] = mSprites[i][0];
		}

#endif
	}

	void ScoreManager::RefreshSprites()
	{
#ifndef PLATFORM_XBOX
		std::int32_t value = mValue;

		if (mNumberBase > 0)
		{
			for (std::int32_t i = 0; i < mNumSprites; ++i)
			{
				std::int32_t digit = value % mNumberBase;

				// Set new image
				Renderer::GetInstance()->RemoveRenderable(mCurrentSprites[i], 101);
				mCurrentSprites[i] = mSprites[i][digit];
				Renderer::GetInstance()->AddRenderable(mCurrentSprites[i], 101);

				value = value / mNumberBase;
			}
		}
#endif
	}

	void ScoreManager::CleanupSprites()
	{
#ifndef PLATFORM_XBOX
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			Renderer::GetInstance()->RemoveRenderable(mCurrentSprites[i], 101);
		}
#endif
	}

	void ScoreManager::SetNumberBase(std::int32_t base)
	{
		mNumberBase = base;
	}
}
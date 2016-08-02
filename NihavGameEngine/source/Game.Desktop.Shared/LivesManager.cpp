#include "pch.h"
#include "LivesManager.h"
#include "RenderDevice.h"
#include "Renderer.h"
#include "Image.h"
#include "SpriteRenderer.h"

namespace Library
{
	const std::string LivesManager::IMAGE_NAME = "life";

	LivesManager* LivesManager::sInstance = nullptr;

	LivesManager* LivesManager::CreateInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new LivesManager();
		}

		return sInstance;
	}

	LivesManager* LivesManager::GetInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new LivesManager();
		}
		return sInstance;
	}

	void LivesManager::DeleteInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	LivesManager::LivesManager()
		: mSprites(), mImages()
	{
	}

	LivesManager::~LivesManager()
	{
#ifndef PLATFORM_XBOX
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			delete mSprites[i];
			mSprites[i] = nullptr;
		}
#endif
	}

	void LivesManager::Init(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		mSprites.resize(mNumSprites);
		mImages.resize(mNumSprites);

#ifndef PLATFORM_XBOX
		std::int32_t padding = 0;
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			// Determine position
			std::int32_t order = (mOrderLeftToRight) ? i : (mNumSprites - i);
			glm::vec4 pos = glm::vec4(order * mImageSize + mHorizontalOffset + padding, mVerticalOffset, 1, 1);
			padding = (mOrderLeftToRight) ? padding - mPadding : padding + mPadding;

			mSprites[i] = new SpriteRenderer();
			SpriteRenderer* renderer = mSprites[i];

			mImages[i] = renderer->CreateAction("Image", "image", *renderer, ActionList::ATTRIBUTE_ACTIONS).AssertiveAs<Image>();
			mImages[i]->SetPath(mBasePath + mFolderPath + IMAGE_NAME + mExtension);
			mImages[i]->SetSize(glm::vec4((float)(mImageSize)));

			renderer->SetTransform(pos, glm::vec4(), glm::vec4(1));

			renderer->BeginPlay(worldState);
		}
#endif
	}

	void LivesManager::RefreshSprites()
	{
#ifndef PLATFORM_XBOX
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			Renderer::GetInstance()->RemoveRenderable(mSprites[i], 101);

			if (i < mValue)
			{
				Renderer::GetInstance()->AddRenderable(mSprites[i], 101);
			}
		}
#endif
	}

	void LivesManager::CleanupSprites()
	{
#ifndef PLATFORM_XBOX
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			Renderer::GetInstance()->RemoveRenderable(mSprites[i], 101);
		}
#endif
	}
}
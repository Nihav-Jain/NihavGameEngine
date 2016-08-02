#include "pch.h"
#include "Game.h"

namespace Library
{

	Game::Game() :
		mSharedData(), mParseMaster(mSharedData),
		mGameClock(), mGameTime(), mWorld(mGameTime, mParseMaster), mRenderer(nullptr),
		mLoadingLevel(false), mMutex(), bWorldUpdated(false), mGameLoopState(GameLoopState::WORLD_PENDING)
	{
		//HeapManager::CreateHeapManager(mMemory);
		mSharedData.SetRootScope(mWorld);
		AddParseHelpers();
	}

	Game::~Game()
	{
		SharedDataTable::ClearStateGraph();
		ActionExpression::ClearStaticMemebers();
		Attributed::ClearStaticMembers();
	}

	World& Game::GetWorld()
	{
		return mWorld;
	}

	XmlParseMaster& Game::ParseMaster()
	{
		return mParseMaster;
	}

	const GameTime& Game::GetGameTime() const
	{
		return mGameTime;
	}

	void Game::Start()
	{
		mGameClock.Reset();
		mGameClock.UpdateGameTime(mGameTime); 
		//mWorld.SetAudioManager(mAudioManager);
		mWorld.BeginPlay();
	}

	void Game::Start(const std::string& config)
	{
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			mLoadingLevel = true;
		}

		mParseMaster.ParseFromFileAsync(config, [&](bool parsingSuccesfull) {
			if (!parsingSuccesfull)
				throw std::exception("Error while parsing Level");
			//callback();
			mGameClock.Reset();
			mGameClock.UpdateGameTime(mGameTime);
			mWorld.BeginPlay();
	
			{
				std::lock_guard<std::recursive_mutex> lock(mMutex);
				mLoadingLevel = false;
			}
		});
	}

	void Game::Update()
	{
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			if (mLoadingLevel)
				return;
		}

		//mGameClock.UpdateGameTime(mGameTime);
		//mWorld.Update();
		//if (mRenderer != nullptr)
		//{
		//	mRenderer->Update();
		//}

		UpdateAsync();

		AudioManager::Get().Update();
	}

	void Game::SetRenderer(Renderer* renderer)
	{
		mRenderer = renderer;
	}

	void Game::Destroy()
	{
		mWorld.OnDestroy();
	}

	void Game::UpdateAsync()
	{
		GameLoopState isWorldUpdating = GameLoopState::NONE;

		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			isWorldUpdating = mGameLoopState;
		}

		if (isWorldUpdating != GameLoopState::WORLD_UPDATING)
		{
			std::future<void> fut = std::async(std::launch::async, [&]() {

				{
					std::lock_guard<std::recursive_mutex> lock(mMutex);
					mGameLoopState = GameLoopState::WORLD_UPDATING;
				}

				mGameClock.UpdateGameTime(mGameTime);

				try
				{
					mWorld.Update();
				}
				catch (std::exception ex)
				{
					ex;
				}

				{
					std::lock_guard<std::recursive_mutex> lock(mMutex);
					mGameLoopState = GameLoopState::WORLD_UPDATED;
				}
			});

			UNREFERENCED_PARAMETER(fut);
		}

		GameLoopState worldUpdated = GameLoopState::NONE;

		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			worldUpdated = mGameLoopState;
		}

		if (mRenderer != nullptr && worldUpdated != GameLoopState::WORLD_UPDATING)
		{
			mRenderer->Update();

			{
				std::lock_guard<std::recursive_mutex> lock(mMutex);
				mGameLoopState = GameLoopState::WORLD_PENDING;
			}
		}

	}

	void Game::AddParseHelpers()
	{
		mParseMaster.AddHelper(mDefineParser);
		mParseMaster.AddHelper(mDefineSectorParser);

		mParseMaster.AddHelper(mWorldParser);
		mParseMaster.AddHelper(mSectorParser);
		mParseMaster.AddHelper(mEntityParser);
		mParseMaster.AddHelper(mGameObjectParser);
		mParseMaster.AddHelper(mActionParser);
		mParseMaster.AddHelper(mTableParser);
		mParseMaster.AddHelper(mPrimitivesParser);
		mParseMaster.AddHelper(mNameValueParser);
		mParseMaster.AddHelper(mSwitchParser);
		mParseMaster.AddHelper(mCaseParser);
		mParseMaster.AddHelper(mExpressionParser);
		mParseMaster.AddHelper(mReactionParser);
		mParseMaster.AddHelper(mActionEvent);

		mParseMaster.AddHelper(mActionIf);
		mParseMaster.AddHelper(mActionIfCondition);
		mParseMaster.AddHelper(mActionIfThen);
		mParseMaster.AddHelper(mActionIfElse);

		mParseMaster.AddHelper(mActionWhile);
		mParseMaster.AddHelper(mActionWhileLoop);

		mParseMaster.AddHelper(mActionBeginPlay);
		mParseMaster.AddHelper(mActionOnDestroy);
		mParseMaster.AddHelper(mSpriteParser);
		mParseMaster.AddHelper(mPolygonParser);
		mParseMaster.AddHelper(mImageParser);
		mParseMaster.AddHelper(mAnimatorParser);
		mParseMaster.AddHelper(mAnimationStateParser);
		mParseMaster.AddHelper(mAnimationFrameParser);
		mParseMaster.AddHelper(mCircleColliderComponent);
	}
}

#include "pch.h"
#include "Game.h"

namespace Library
{
	Game* Game::sInstance = nullptr;

	Game::Game() :
		mRootScope(), mSharedData(), mParseMaster(mSharedData), 
		mWorldParser(), mSectorParser(), mEntityParser(), mActionParser(),
		mTableParser(), mPrimitivesParser(), mNameValueParser(),
		mGameClock(), mGameTime(), mWorld(nullptr)
	{
		mSharedData.SetRootScope(mRootScope);

		mParseMaster.AddHelper(mWorldParser);
		mParseMaster.AddHelper(mSectorParser);
		mParseMaster.AddHelper(mEntityParser);
		mParseMaster.AddHelper(mActionParser);
		mParseMaster.AddHelper(mTableParser);
		mParseMaster.AddHelper(mPrimitivesParser);
		mParseMaster.AddHelper(mNameValueParser);
	}


	Game::~Game()
	{
		SharedDataTable::ClearStateGraph();
		XmlParseHelperPrimitives::ClearStaticMembers();
	}

	Game& Game::Instance()
	{
		if (sInstance == nullptr)
			sInstance = new Game();
		return *sInstance;
	}

	void Game::SetWorld(World& world)
	{
		mWorld = &world;
		mWorld->SetGameTime(mGameTime);
	}

	World& Game::GetWorld()
	{
		if (mWorld == nullptr)
			throw std::exception("World has not been created yet.");
		return *mWorld;
	}

	void Game::AddXmlParseHelper(IXmlParseHelper& helper)
	{
		mParseMaster.AddHelper(helper);
	}

	bool Game::ParseFromFile(const std::string& fileName)
	{
		return mParseMaster.ParseFromFile(fileName);
	}

	void Game::Start()
	{
		mGameClock.Reset();
	}

	void Game::Update()
	{
		if (mWorld != nullptr)
		{
			mGameClock.UpdateGameTime(mGameTime);
			mWorld->Update();
		}
	}
}
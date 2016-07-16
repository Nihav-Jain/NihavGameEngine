#include "pch.h"
#include "Engine.h"
#include "EngineModule.h"
#include "DesktopFileManager.h"
#include "Game.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySquare.h"
#include "Collectible.h"
#include "Bullet.h"
#include "BloomPostProcessing.h"
#include "DesktopAudioManager.h"

using namespace Library;
using namespace OpenGLImplmentation;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);
	
	const std::pair<const std::uint64_t*, EngineModule**> ptr = *DesktopFileManager::Itr;
	const std::pair<const std::uint64_t*, EngineModule**> ptr2 = *DesktopAudioManager::Itr;

	UNREFERENCED_PARAMETER(ptr);
	UNREFERENCED_PARAMETER(ptr2);

	Engine::CreateEngine();
	Engine::Get().Activate();

	OpenGLRenderDevice renderDevice;

	/// Game-specific factories that cannot live as members in the Game class ///
	XBoxControllerHandlerFactory xchf;
	ActionDebugFactory adf;
	PlayerFactory mPlayerFactory;
	EnemyFactory mEnemyFactory;
	EnemySquareFactory mEnemySquareFactory;
	BulletFactory mBulletFactory;
	CollectibleFactory mCollectibleFactory;

	BloomPostProcessing bloom;

	// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
	Renderer * renderer = Renderer::GetInstance(&renderDevice);

	renderer->AddPostPostProcessing(&bloom);

	/// Game ///
	Game game;
	game.SetRenderer(renderer);
	game.Start("config/geometrywars_test.xml");
	renderDevice.InitOpenGl("Geometry Wars", game.GetWorld().GetWidth(), game.GetWorld().GetHeight());

#pragma warning(push)
#pragma warning(disable : 4127)
	while (renderDevice.IsValid())
	{
#pragma warning(pop) 
		game.Update();
	}

	Engine::Get().Deactivate();
	Engine::Destroy();
	return 0;
}



#pragma once

#include "pch.h"
#include <Xinput.h>
#include <Windows.h>
#include "Common\DeviceResources.h"
#include "Game_UWP_DirectX11Main.h"
#include "Game.h"
#include "UWPRenderDevice.h"
#include "ReactionAttributed.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemySquare.h"
#include "Collectible.h"


namespace Game_UWP_DirectX11
{
	// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		App();
		virtual ~App();

		// IFrameworkView Methods.
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		// Application lifecycle event handlers.
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		void OnResuming(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

	private:
		//std::shared_ptr<DX::DeviceResources> m_deviceResources;
		//std::unique_ptr<Game_UWP_DirectX11Main> m_main;
		bool m_windowClosed;
		bool m_windowVisible;

		std::unique_ptr<Library::Game> mGame;
		Library::Renderer* mRenderer;
		std::shared_ptr<Library::UWPRenderDevice> mRenderDevice;

		Library::XBoxControllerHandlerFactory xchf;
		Library::ActionDebugFactory actionDebugFactory;
		Library::PlayerFactory mPlayerFactory;
		Library::EnemyFactory mEnemyFactory;
		Library::EnemySquareFactory mEnemySquareFactory;
		Library::BulletFactory mBulletFactory;
		Library::CollectibleFactory mCollectibleFactory;
	};
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};

#pragma once
#include <mutex>
#include "RenderDevice.h"

namespace Library
{
	class Viewport;
	class Texture;
	class Shader;
	class Buffer;
	class D3DTexture;
	class D3DShader;
	class D3DRenderBuffer;

	struct IDeviceNotify
	{
	public:
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};

	class UWPRenderDevice : public RenderDevice
	{
	public:
		UWPRenderDevice();
		virtual ~UWPRenderDevice();

		virtual std::int32_t GetViewportWidth() override;
		virtual std::int32_t GetViewportHeight() override;
		virtual Texture* CreateTexture(const std::string & imagePath) override;
		virtual Shader* CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath) override;
		virtual Buffer* CreateBuffer(bool createIndicesBuffer) override;
		virtual FrameBuffer* CreateFrameBuffer(std::uint32_t textureCnt) override { UNREFERENCED_PARAMETER(textureCnt); return nullptr; };
		virtual FrameBuffer* GetDefaultFrameBuffer() override { return nullptr; };
		virtual void Draw(DrawMode mode, std::uint32_t counts, bool useIndices) override;
		virtual void ClearScreen() override;

		void SetWindow(Windows::UI::Core::CoreWindow^ window);
		void RegisterDeviceNotify(IDeviceNotify& deviceNotify);

		virtual bool AllResourcesLoaded() const;
		virtual void ResourceLoaded();
	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void UpdateRenderTargetSize();
		DXGI_MODE_ROTATION ComputeDisplayRotation();
		void HandleDeviceLost();
		void SwapSwapChain();

		D3D_FEATURE_LEVEL mFeatureLevel;
		Microsoft::WRL::ComPtr<ID3D11Device1> mDirect3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> mDirect3DDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		D3D11_VIEWPORT									mScreenViewport;

		std::vector<std::shared_ptr<D3DShader>> mShaders;
		std::vector<std::shared_ptr<D3DTexture>> mTextures;
		std::vector<std::shared_ptr<D3DRenderBuffer>> mBuffers;

		Platform::Agile<Windows::UI::Core::CoreWindow> mWindow;
		Windows::Foundation::Size						mD3DRenderTargetSize;
		Windows::Foundation::Size						mOutputSize;
		Windows::Foundation::Size						mLogicalSize;
		Windows::Graphics::Display::DisplayOrientations	mNativeOrientation;
		Windows::Graphics::Display::DisplayOrientations	mCurrentOrientation;
		float											mDpi;

		// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
		float mEffectiveDpi;

		// Transforms used for display orientation.
		DirectX::XMFLOAT4X4	m_orientationTransform3D;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		IDeviceNotify* mDeviceNotify;
		mutable std::recursive_mutex mMutex;
		std::int32_t mResourcesPendingLoadCount;
	};
}


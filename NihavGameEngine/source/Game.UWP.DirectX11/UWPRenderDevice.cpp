#include "pch.h"
#include "UWPRenderDevice.h"
#include "Common\DirectXHelper.h"
#include "D3DRenderBuffer.h"
#include "D3DShader.h"
#include "D3DTexture.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Graphics::Display;

namespace DisplayMetrics
{
	// High resolution displays can require a lot of GPU and battery power to render.
	// High resolution phones, for example, may suffer from poor battery life if
	// games attempt to render at 60 frames per second at full fidelity.
	// The decision to render at full fidelity across all platforms and form factors
	// should be deliberate.
	static const bool SupportHighResolutions = false;

	// The default thresholds that define a "high resolution" display. If the thresholds
	// are exceeded and SupportHighResolutions is false, the dimensions will be scaled
	// by 50%.
	static const float DpiThreshold = 192.0f;		// 200% of standard desktop display.
	static const float WidthThreshold = 1920.0f;	// 1080p width.
	static const float HeightThreshold = 1080.0f;	// 1080p height.
};

// Constants used to calculate screen rotations
namespace ScreenRotation
{
	// 0-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 90-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation90(
		0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 180-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation180(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 270-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation270(
		0.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
};

namespace Library
{
	UWPRenderDevice::UWPRenderDevice() :
		mScreenViewport(),
		mFeatureLevel(D3D_FEATURE_LEVEL_9_1),
		mRenderTargetView(),
		mOutputSize(),
		mLogicalSize(),
		mNativeOrientation(DisplayOrientations::None),
		mCurrentOrientation(DisplayOrientations::None),
		mDpi(-1.0f),
		mEffectiveDpi(-1.0f),
		mDeviceNotify(nullptr),
		mMutex(), mResourcesPendingLoadCount(0)
	{
		CreateDeviceResources();
	}


	UWPRenderDevice::~UWPRenderDevice()
	{
	}

	std::int32_t UWPRenderDevice::GetViewportWidth()
	{
		return static_cast<std::int32_t>(mOutputSize.Width);
	}

	std::int32_t UWPRenderDevice::GetViewportHeight()
	{
		return static_cast<std::int32_t>(mOutputSize.Height);
	}

	Texture* UWPRenderDevice::CreateTexture(const std::string& imagePath)
	{
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			++mResourcesPendingLoadCount;
		}
		D3DTexture* texture = new D3DTexture(*mDirect3DDevice.Get(), *mDirect3DDeviceContext.Get());
		texture->Init(imagePath, *this);
		mTextures.push_back(texture);
		return texture;
	}

	Shader* UWPRenderDevice::CreateShader(const std::string& vPath, const std::string& fPath, const std::string& gPath)
	{
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			++mResourcesPendingLoadCount;
		}
		D3DShader* shader = new D3DShader(*mDirect3DDevice.Get(), *mDirect3DDeviceContext.Get());
		shader->Init(vPath, fPath, gPath, *this);
		mShaders.push_back(shader);
		return shader;
	}

	Buffer* UWPRenderDevice::CreateBuffer(bool createIndicesBuffer)
	{
		D3DRenderBuffer* buffer = new D3DRenderBuffer(*mDirect3DDevice.Get(), *mDirect3DDeviceContext.Get());
		buffer->Init(createIndicesBuffer);
		mBuffers.push_back(buffer);
		return buffer;
	}

	void UWPRenderDevice::Draw(DrawMode mode, std::uint32_t counts, bool useIndices)
	{

		switch (mode)
		{
		case Library::RenderDevice::DrawMode::TRIANGLES:
			mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case Library::RenderDevice::DrawMode::LINES:
			mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case Library::RenderDevice::DrawMode::POINTS:
			mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		default:
			break;
		}
		if (useIndices)
			mDirect3DDeviceContext->DrawIndexed(counts, 0, 0);
		else
			mDirect3DDeviceContext->Draw(counts, 0);

		//SwapSwapChain();
	}

	void UWPRenderDevice::ClearScreen()
	{
		static DirectX::XMVECTORF32 BackgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		mDirect3DDeviceContext->RSSetViewports(1, &mScreenViewport);

		// Reset render targets to the screen.
		ID3D11RenderTargetView *const targets[1] = { mRenderTargetView.Get() };
		mDirect3DDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		UNREFERENCED_PARAMETER(targets);

		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	}

	void UWPRenderDevice::Present()
	{
		ThrowIfFailed(mSwapChain->Present(1, 0), "IDXGISwapChain::Present() failed.");
	}

	void UWPRenderDevice::SetWindow(CoreWindow^ window)
	{
		DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
		mWindow = window;

		mLogicalSize = Windows::Foundation::Size(window->Bounds.Width, window->Bounds.Height);
		mNativeOrientation = currentDisplayInformation->NativeOrientation;
		mCurrentOrientation = currentDisplayInformation->CurrentOrientation;
		mDpi = currentDisplayInformation->LogicalDpi;

		CreateWindowSizeDependentResources();
	}

	void UWPRenderDevice::RegisterDeviceNotify(IDeviceNotify& deviceNotify)
	{
		mDeviceNotify = &deviceNotify;
	}

	bool UWPRenderDevice::AllResourcesLoaded() const
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		return (mResourcesPendingLoadCount == 0);
	}

	void UWPRenderDevice::ResourceLoaded()
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		assert(mResourcesPendingLoadCount > 0);
		--mResourcesPendingLoadCount;
	}

	void UWPRenderDevice::CreateDeviceResources()
	{
		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		if (DX::SdkLayersAvailable())
		{
			// If the project is in a debug build, enable debugging via SDK Layers with this flag.
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		// This array defines the set of DirectX hardware feature levels this app will support.
		// Note the ordering should be preserved.
		// Don't forget to declare your application's minimum required feature level in its
		// description.  All applications are assumed to support 9.1 unless otherwise stated.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// Create the Direct3D 11 API device object and a corresponding context.
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		HRESULT hr = D3D11CreateDevice(
			nullptr,					// Specify nullptr to use the default adapter.
			D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
			0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
			creationFlags,				// Set debug and Direct2D compatibility flags.
			featureLevels,				// List of feature levels this app can support.
			ARRAYSIZE(featureLevels),	// Size of the list above.
			D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps.
			&device,					// Returns the Direct3D device created.
			&mFeatureLevel,			// Returns feature level of device created.
			&context					// Returns the device immediate context.
		);

		if (FAILED(hr))
		{
			// If the initialization fails, fall back to the WARP device.
			// For more information on WARP, see: 
			// http://go.microsoft.com/fwlink/?LinkId=286690
			DX::ThrowIfFailed(
				D3D11CreateDevice(
					nullptr,
					D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
					0,
					creationFlags,
					featureLevels,
					ARRAYSIZE(featureLevels),
					D3D11_SDK_VERSION,
					&device,
					&mFeatureLevel,
					&context
				)
			);
		}

		// Store pointers to the Direct3D 11.3 API device and immediate context.
		DX::ThrowIfFailed(
			device.As(&mDirect3DDevice)
		);

		DX::ThrowIfFailed(
			context.As(&mDirect3DDeviceContext)
		);
	}

	void UWPRenderDevice::CreateWindowSizeDependentResources()
	{
		// Clear the previous window size specific context.
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		mDirect3DDeviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		mRenderTargetView = nullptr;
		mDepthStencilView = nullptr;
		mDirect3DDeviceContext->Flush();

		UpdateRenderTargetSize();

		// The width and height of the swap chain must be based on the window's
		// natively-oriented width and height. If the window is not in the native
		// orientation, the dimensions must be reversed.
		DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

		bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
		mD3DRenderTargetSize.Width = swapDimensions ? mOutputSize.Height : mOutputSize.Width;
		mD3DRenderTargetSize.Height = swapDimensions ? mOutputSize.Width : mOutputSize.Height;

		if (mSwapChain != nullptr)
		{
			// If the swap chain already exists, resize it.
			HRESULT hr = mSwapChain->ResizeBuffers(
				2, // Double-buffered swap chain.
				lround(mD3DRenderTargetSize.Width),
				lround(mD3DRenderTargetSize.Height),
				DXGI_FORMAT_B8G8R8A8_UNORM,
				0
			);

			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			{
				// If the device was removed for any reason, a new device and swap chain will need to be created.
				HandleDeviceLost();

				// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
				// and correctly set up the new device.
				return;
			}
			else
			{
				DX::ThrowIfFailed(hr);
			}
		}
		else
		{
			// Otherwise, create a new one using the same adapter as the existing Direct3D device.
			DXGI_SCALING scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

			swapChainDesc.Width = lround(mD3DRenderTargetSize.Width);		// Match the size of the window.
			swapChainDesc.Height = lround(mD3DRenderTargetSize.Height);
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				// This is the most common swap chain format.
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;								// Don't use multi-sampling.
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;									// Use double-buffering to minimize latency.
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// All Windows Store apps must use this SwapEffect.
			swapChainDesc.Flags = 0;
			swapChainDesc.Scaling = scaling;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

			// This sequence obtains the DXGI factory that was used to create the Direct3D device above.
			ComPtr<IDXGIDevice3> dxgiDevice;
			DX::ThrowIfFailed(
				mDirect3DDevice.As(&dxgiDevice)
			);

			ComPtr<IDXGIAdapter> dxgiAdapter;
			DX::ThrowIfFailed(
				dxgiDevice->GetAdapter(&dxgiAdapter)
			);

			ComPtr<IDXGIFactory4> dxgiFactory;
			DX::ThrowIfFailed(
				dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
			);

			ComPtr<IDXGISwapChain1> swapChain;
			DX::ThrowIfFailed(
				dxgiFactory->CreateSwapChainForCoreWindow(
					mDirect3DDevice.Get(),
					reinterpret_cast<IUnknown*>(mWindow.Get()),
					&swapChainDesc,
					nullptr,
					&swapChain
				)
			);
			DX::ThrowIfFailed(
				swapChain.As(&mSwapChain)
			);

			// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
			// ensures that the application will only render after each VSync, minimizing power consumption.
			DX::ThrowIfFailed(
				dxgiDevice->SetMaximumFrameLatency(1)
			);
		}

		// Set the proper orientation for the swap chain, and generate 2D and
		// 3D matrix transformations for rendering to the rotated swap chain.
		// Note the rotation angle for the 2D and 3D transforms are different.
		// This is due to the difference in coordinate spaces.  Additionally,
		// the 3D matrix is specified explicitly to avoid rounding errors.

		switch (displayRotation)
		{
		case DXGI_MODE_ROTATION_IDENTITY:
			m_orientationTransform3D = ScreenRotation::Rotation0;
			break;

		case DXGI_MODE_ROTATION_ROTATE90:
			m_orientationTransform3D = ScreenRotation::Rotation270;
			break;

		case DXGI_MODE_ROTATION_ROTATE180:
			m_orientationTransform3D = ScreenRotation::Rotation180;
			break;

		case DXGI_MODE_ROTATION_ROTATE270:
			m_orientationTransform3D = ScreenRotation::Rotation90;
			break;

		default:
			throw std::exception("Invalid display rotation");
		}

		DX::ThrowIfFailed(
			mSwapChain->SetRotation(displayRotation)
		);

		// Create a render target view of the swap chain back buffer.
		ComPtr<ID3D11Texture2D1> backBuffer;
		DX::ThrowIfFailed(
			mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

		DX::ThrowIfFailed(
			mDirect3DDevice->CreateRenderTargetView(
				backBuffer.Get(),
				nullptr,
				&mRenderTargetView
			)
		);

		// Create a depth stencil view for use with 3D rendering if needed.
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			lround(mD3DRenderTargetSize.Width),
			lround(mD3DRenderTargetSize.Height),
			1, // This depth stencil view has only one texture.
			1, // Use a single mipmap level.
			D3D11_BIND_DEPTH_STENCIL
		);

		ComPtr<ID3D11Texture2D> depthStencil;
		DX::ThrowIfFailed(
			mDirect3DDevice->CreateTexture2D(
				&depthStencilDesc,
				nullptr,
				&depthStencil
			)
		);

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		DX::ThrowIfFailed(
			mDirect3DDevice->CreateDepthStencilView(
				depthStencil.Get(),
				&depthStencilViewDesc,
				&mDepthStencilView
			)
		);

		// Set the 3D rendering viewport to target the entire window.
		mScreenViewport = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			mD3DRenderTargetSize.Width,
			mD3DRenderTargetSize.Height
		);

		mDirect3DDeviceContext->RSSetViewports(1, &mScreenViewport);

		// Create a Direct2D target bitmap associated with the
		// swap chain back buffer and set it as the current target.
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				mDpi,
				mDpi
			);

		ComPtr<IDXGISurface2> dxgiBackBuffer;
		DX::ThrowIfFailed(
			mSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
		);
	}

	void UWPRenderDevice::UpdateRenderTargetSize()
	{
		mEffectiveDpi = mDpi;

		// To improve battery life on high resolution devices, render to a smaller render target
		// and allow the GPU to scale the output when it is presented.
		if (!DisplayMetrics::SupportHighResolutions && mDpi > DisplayMetrics::DpiThreshold)
		{
			float width = DX::ConvertDipsToPixels(mLogicalSize.Width, mDpi);
			float height = DX::ConvertDipsToPixels(mLogicalSize.Height, mDpi);

			// When the device is in portrait orientation, height > width. Compare the
			// larger dimension against the width threshold and the smaller dimension
			// against the height threshold.
			if (std::max(width, height) > DisplayMetrics::WidthThreshold && std::min(width, height) > DisplayMetrics::HeightThreshold)
			{
				// To scale the app we change the effective DPI. Logical size does not change.
				mEffectiveDpi /= 2.0f;
			}
		}

		// Calculate the necessary render target size in pixels.
		mOutputSize.Width = DX::ConvertDipsToPixels(mLogicalSize.Width, mEffectiveDpi);
		mOutputSize.Height = DX::ConvertDipsToPixels(mLogicalSize.Height, mEffectiveDpi);

		// Prevent zero size DirectX content from being created.
		mOutputSize.Width = std::max(mOutputSize.Width, 1.0f);
		mOutputSize.Height = std::max(mOutputSize.Height, 1.0f);
	}

	DXGI_MODE_ROTATION UWPRenderDevice::ComputeDisplayRotation()
	{
		DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

		// Note: NativeOrientation can only be Landscape or Portrait even though
		// the DisplayOrientations enum has other values.
		switch (mNativeOrientation)
		{
		case DisplayOrientations::Landscape:
			switch (mCurrentOrientation)
			{
			case DisplayOrientations::Landscape:
				rotation = DXGI_MODE_ROTATION_IDENTITY;
				break;

			case DisplayOrientations::Portrait:
				rotation = DXGI_MODE_ROTATION_ROTATE270;
				break;

			case DisplayOrientations::LandscapeFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE180;
				break;

			case DisplayOrientations::PortraitFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE90;
				break;
			}
			break;

		case DisplayOrientations::Portrait:
			switch (mCurrentOrientation)
			{
			case DisplayOrientations::Landscape:
				rotation = DXGI_MODE_ROTATION_ROTATE90;
				break;

			case DisplayOrientations::Portrait:
				rotation = DXGI_MODE_ROTATION_IDENTITY;
				break;

			case DisplayOrientations::LandscapeFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE270;
				break;

			case DisplayOrientations::PortraitFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE180;
				break;
			}
			break;
		}
		return rotation;
	}

	void UWPRenderDevice::HandleDeviceLost()
	{
		mSwapChain = nullptr;

		if (mDeviceNotify != nullptr)
		{
			mDeviceNotify->OnDeviceLost();
		}

		CreateDeviceResources();
		CreateWindowSizeDependentResources();

		if (mDeviceNotify != nullptr)
		{
			mDeviceNotify->OnDeviceRestored();
		}
	}

	void UWPRenderDevice::SwapSwapChain()
	{
		// The first argument instructs DXGI to block until VSync, putting the application
		// to sleep until the next VSync. This ensures we don't waste any cycles rendering
		// frames that will never be displayed to the screen.
		DXGI_PRESENT_PARAMETERS parameters = { 0 };
		HRESULT hr = mSwapChain->Present1(1, 0, &parameters);

		// Discard the contents of the render target.
		// This is a valid operation only when the existing contents will be entirely
		// overwritten. If dirty or scroll rects are used, this call should be removed.
		mDirect3DDeviceContext->DiscardView1(mRenderTargetView.Get(), nullptr, 0);

		// Discard the contents of the depth stencil.
		mDirect3DDeviceContext->DiscardView1(mDepthStencilView.Get(), nullptr, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we 
		// must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
}

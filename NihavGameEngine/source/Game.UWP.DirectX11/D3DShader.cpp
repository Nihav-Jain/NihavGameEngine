#include "pch.h"
#include <DirectXMath.h>
#include "D3DShader.h"

using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace concurrency;

namespace Library
{
	D3DShader::D3DShader(ID3D11Device1& device, ID3D11DeviceContext& context) : mDevice(&device), mContext(&context),
		mVertexShader(nullptr), mGeometryShader(nullptr), mPixelShader(nullptr), mInputLayout(nullptr), mConstantGeometryBuffer(nullptr), mConstantPixelBuffer(nullptr), mGeoBufferInstance()
	{
	}


	D3DShader::~D3DShader()
	{
		ReleaseObject(mVertexShader);
		ReleaseObject(mGeometryShader);
		ReleaseObject(mPixelShader);
		ReleaseObject(mInputLayout);
		ReleaseObject(mConstantGeometryBuffer);
		ReleaseObject(mConstantPixelBuffer);
	}
	void D3DShader::Init(const std::string & vPath, const std::string & fPath, const std::string & gPath)
	{
		UNREFERENCED_PARAMETER(vPath);
		UNREFERENCED_PARAMETER(fPath);
		bool IsASprite = gPath == "";
		std::vector<char> compiledVertexShader;
		if(IsASprite)
			ReadData(L"SpriteVertexShader.cso", compiledVertexShader);
		else
			ReadData(L"VertexShader.cso", compiledVertexShader);
		ThrowIfFailed(mDevice->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &mVertexShader), "ID3D11Device::CreatedVertexShader() failed.");

		if (!IsASprite)
		{
			// Load a compiled pixel shader
			std::vector<char> compiledGeometryShader;
			ReadData(L"GeometryShader.cso", compiledGeometryShader);
			ThrowIfFailed(mDevice->CreateGeometryShader(&compiledGeometryShader[0], compiledGeometryShader.size(), nullptr, &mGeometryShader), "ID3D11Device::CreatedGeometryShader() failed.");
		}

		std::vector<char> compiledPixelShader;
		if (IsASprite)
			ReadData(L"SpritePixelShader.cso", compiledPixelShader);
		else
			ReadData(L"PixelShader.cso", compiledPixelShader);
		ThrowIfFailed(mDevice->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, &mPixelShader), "ID3D11Device::CreatedPixelShader() failed.");


		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
		if (IsASprite)
		{
			D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			ThrowIfFailed(mDevice->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), &mInputLayout), "ID3D11Device::CreateInputLayout() failed.");

			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
			ThrowIfFailed(mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantGeometryBuffer), "ID3D11Device::CreateBuffer() failed.");

		}
		else
		{
			D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			ThrowIfFailed(mDevice->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), &mInputLayout), "ID3D11Device::CreateInputLayout() failed.");

			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			constantBufferDesc.ByteWidth = sizeof(CGeometryBufferPerObject);
			ThrowIfFailed(mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantGeometryBuffer), "ID3D11Device::CreateBuffer() failed.");

			constantBufferDesc.ByteWidth = sizeof(CPixelBufferPerObject);
			ThrowIfFailed(mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantPixelBuffer), "ID3D11Device::CreateBuffer() failed.");
		}
	}
	void D3DShader::Use()
	{
		mContext->VSSetShader(mVertexShader, nullptr, 0);
		mContext->GSSetShader(mGeometryShader, nullptr, 0);
		mContext->PSSetShader(mPixelShader, nullptr, 0);

		mContext->IASetInputLayout(mInputLayout);

		if (mGeometryShader)
		{
			mContext->GSSetConstantBuffers(0, 1, &mConstantGeometryBuffer);
			mContext->PSSetConstantBuffers(0, 1, &mConstantPixelBuffer);
		}
		else
		{
			mContext->VSSetConstantBuffers(0, 1, &mConstantGeometryBuffer);
		}
	}
	void D3DShader::SetMatrix4(const std::string & name, const glm::mat4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		//mGeoBufferInstance
		mGeoBufferInstance.WorldViewProjection =
			DirectX::XMFLOAT4X4(
				value[0][0], value[1][0], value[2][0], value[3][0],
				value[0][1], value[1][1], value[2][1], value[3][1],
				value[0][2], value[1][2], value[2][2], value[3][2],
				value[0][3], value[1][3], value[2][3], value[3][3]);
		if(mGeometryShader)
			mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &mGeoBufferInstance, 0, 0);
		else
			mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &mGeoBufferInstance.WorldViewProjection, 0, 0);
	}
	void D3DShader::SetVector4(const std::string & name, const glm::vec4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		if (mGeometryShader)
			mContext->UpdateSubresource(mConstantPixelBuffer, 0, nullptr, &value, 0, 0);
	}
	void D3DShader::SetFloat(const std::string & name, float value)
	{
		UNREFERENCED_PARAMETER(name);
		if (mGeometryShader)
		{
			mGeoBufferInstance.width = value;
			mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &mGeoBufferInstance, 0, 0);
		}
	}

	void D3DShader::ReadData(const std::wstring& filename, std::vector<char>& fileDataVector)
	{
		Platform::String^ pFileName = ref new Platform::String(filename.c_str());

		CREATEFILE2_EXTENDED_PARAMETERS extendedParams = { 0 };
		extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
		extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
		extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
		extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
		extendedParams.lpSecurityAttributes = nullptr;
		extendedParams.hTemplateFile = nullptr;

		Wrappers::FileHandle file(
			CreateFile2(
				Platform::String::Concat("\\", pFileName)->Data(),
				GENERIC_READ,
				FILE_SHARE_READ,
				OPEN_EXISTING,
				&extendedParams
			)
		);
		if (file.Get() == INVALID_HANDLE_VALUE)
		{
			throw ref new Platform::FailureException();
		}

		FILE_STANDARD_INFO fileInfo = { 0 };
		if (!GetFileInformationByHandleEx(
			file.Get(),
			FileStandardInfo,
			&fileInfo,
			sizeof(fileInfo)
		))
		{
			throw ref new Platform::FailureException();
		}

		if (fileInfo.EndOfFile.HighPart != 0)
		{
			throw ref new Platform::OutOfMemoryException();
		}

		Platform::Array<byte>^ fileData = ref new Platform::Array<byte>(fileInfo.EndOfFile.LowPart);

		if (!ReadFile(
			file.Get(),
			fileData->Data,
			fileData->Length,
			nullptr,
			nullptr
		))
		{
			throw ref new Platform::FailureException();
		}

		fileDataVector.clear();
		for (std::uint32_t i = 0; i < fileData->Length; i++)
		{
			fileDataVector.push_back(fileData->get(i));
		}
	}
}
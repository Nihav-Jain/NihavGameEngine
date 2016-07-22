#pragma once
#include <mutex>
#include "Shader.h"
#include "FileManager.h"
#include "FileHandle.h"

struct ID3D11Device1;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
namespace Library
{
	class RenderDevice;
	class D3DShader : public Library::Shader
	{
	public:
		D3DShader(ID3D11Device1& device, ID3D11DeviceContext& context);
		virtual ~D3DShader();

		virtual void Init(const std::string & vPath, const std::string & fPath, const std::string & gPath, RenderDevice& device) override;
		virtual bool Use() override;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) override;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) override;
		virtual void SetFloat(const std::string & name, float value) override;
		virtual void SetBool(const std::string & name, bool value) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(value); };
		virtual void SetInt(const std::string & name, std::int32_t value) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(value); };
	private:
		void AllShadersLoaded();

		struct CGeometryBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;
			float width;
			DirectX::XMFLOAT3 bullshit;
		};
		struct CPixelBufferPerObject
		{
			DirectX::XMFLOAT4 Color;
		};

		ID3D11Device1* mDevice;
		ID3D11DeviceContext* mContext;
		ID3D11VertexShader* mVertexShader;
		ID3D11GeometryShader* mGeometryShader;
		ID3D11PixelShader* mPixelShader;

		ID3D11Buffer* mConstantGeometryBuffer;
		ID3D11Buffer* mConstantPixelBuffer;
		CGeometryBufferPerObject mGeoBufferInstance;

		ID3D11InputLayout* mInputLayout;

		//void ReadData(const std::wstring& filename, std::vector<char>& fileDataVector);
		std::recursive_mutex mMutex;
		bool bIsSprite;
		RenderDevice* mRenderDevice;

		bool mVSLoaded;
		bool mGSLoaded;
		bool mPSLoaded;

		std::function<void(FileHandle*)> mVSGetFileCallback;
		std::function<void(std::vector<std::uint8_t>)> mVSReadBufferCallback;

		std::function<void(FileHandle*)> mGSGetFileCallback;
		std::function<void(std::vector<std::uint8_t>)> mGSReadBufferCallback;

		std::function<void(FileHandle*)> mPSGetFileCallback;
		std::function<void(std::vector<std::uint8_t>)> mPSReadBufferCallback;
	};
}
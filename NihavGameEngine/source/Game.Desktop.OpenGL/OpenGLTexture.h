#pragma once
#include "Texture.h"
#include "GL/gl3w.h"

class Library::RenderDevice;
namespace OpenGLImplmentation {


	class OpenGLTexture :
		public Library::Texture
	{

	public:
		OpenGLTexture();
		OpenGLTexture(GLuint textureId);
		~OpenGLTexture();

		virtual void Init(const std::string & imagePath, Library::RenderDevice& device) override;
		virtual void Use(std::uint32_t useAsTextureIndex) override;
	private:
		GLuint mTextureId;
	};

}

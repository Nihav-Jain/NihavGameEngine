#include "pch.h"
#include "OpenGLRenderDevice.h"
#include "GL/gl3w.h"
#include "SOIL/SOIL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGLImplmentation {

	OpenGLRenderDevice::OpenGLRenderDevice() :
		mWindow(nullptr),
		mWidth(800),
		mHeight(600), mResourcesPendingLoadCount(0)
	{
	}


	OpenGLRenderDevice::~OpenGLRenderDevice()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void OpenGLRenderDevice::InitOpenGl(const std::string & title, ::int32_t width, std::int32_t height)
	{
		mWidth = width;
		mHeight = height;

		if (mWidth < 400)
			mWidth = 400;
		if (mHeight < 300)
			mHeight = 300;

		if (!glfwInit())
		{
			throw std::exception("glfwInit falied");
		}

		mWindow = glfwCreateWindow(mWidth, mHeight, title.c_str(), nullptr, nullptr);
		if (mWindow == nullptr)
		{
			throw std::exception("glfwCreateWindow falied");
		}

		glfwMakeContextCurrent(mWindow);

		if (gl3wInit() != 0)
		{
			throw std::exception("gl3wInit falied");
		}

		glViewport(0, 0, mWidth, mHeight);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	std::int32_t OpenGLRenderDevice::GetViewportWidth()
	{
		return mWidth;
	}

	std::int32_t OpenGLRenderDevice::GetViewportHeight()
	{
		return mHeight;
	}

	Library::Texture * OpenGLRenderDevice::CreateTexture(const std::string & imagePath)
	{
		++mResourcesPendingLoadCount;
		OpenGLTexture * texture = new OpenGLTexture();
		texture->Init(imagePath, *this);
		mTextures.push_back(texture);
		return texture;
	}

	void OpenGLRenderDevice::ClearScreen()
	{
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
		const glm::vec4 Black = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glClearBufferfv(GL_COLOR, 0, &Black[0]);
	}

	Library::Shader * OpenGLRenderDevice::CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath)
	{
		++mResourcesPendingLoadCount;
		OpenGLShader * shader = new OpenGLShader();
		shader->Init(vPath, fPath, gPath, *this);
		mShaders.push_back(shader);
		return shader;
	}

	void OpenGLRenderDevice::Draw(DrawMode mode, std::uint32_t counts, bool useIndices)
	{
		UNREFERENCED_PARAMETER(useIndices);
		switch (mode) {
		case DrawMode::TRIANGLES:
			glDrawArrays(GL_TRIANGLES, 0, counts);
			break;
		case DrawMode::LINES:
			glDrawElements(GL_LINES, counts, GL_UNSIGNED_INT, 0);
			break;
		case DrawMode::POINTS:
			glDrawArrays(GL_POINTS, 0, counts);
			break;
		default:
			break;
		}
		
		glBindVertexArray(0);
	}

	bool OpenGLRenderDevice::IsValid()
	{
		return !glfwWindowShouldClose(mWindow);
	}

	bool OpenGLRenderDevice::AllResourcesLoaded() const
	{
		return (mResourcesPendingLoadCount == 0);
	}

	void OpenGLRenderDevice::ResourceLoaded()
	{
		assert(mResourcesPendingLoadCount > 0);
		--mResourcesPendingLoadCount;
	}

	Library::Buffer * OpenGLRenderDevice::CreateBuffer(bool createIndicesBuffer)
	{
		OpenGLRenderBuffer * buffer = new OpenGLRenderBuffer();
		buffer->Init(createIndicesBuffer);
		mBuffers.push_back(buffer);
		return buffer;
	}
	Library::FrameBuffer * OpenGLRenderDevice::CreateFrameBuffer(std::uint32_t textureCnt)
	{
		OpenGLFrameBuffer * fb = new OpenGLFrameBuffer();
		fb->Init(textureCnt, mWidth, mHeight);
		return fb;
	}
	Library::FrameBuffer * OpenGLRenderDevice::GetDefaultFrameBuffer()
	{
		OpenGLFrameBuffer * fb = new OpenGLFrameBuffer(0);
		return fb;
	}
}

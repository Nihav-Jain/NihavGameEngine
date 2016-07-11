#pragma once
#include "FileHandle.h"

namespace Library
{
	class DesktopFileHandle : public FileHandle
	{
	public:
		DesktopFileHandle();
		virtual ~DesktopFileHandle();

		virtual void ReadTextAsync(std::string& outFileText) = 0;
		virtual void ReadBufferAsync(Vector<std::uint8_t>& outBuffer) = 0;

		virtual void WriteTextAsync(const std::string& fileText) = 0;
		virtual void WriteBufferAsync(const Vector<std::uint8_t>& buffer) = 0;

		virtual void CloseFileAsync() = 0;

	};
}


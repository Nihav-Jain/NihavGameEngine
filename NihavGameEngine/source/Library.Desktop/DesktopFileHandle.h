#pragma once
#include <fstream>
#include "FileHandle.h"

namespace Library
{
	class DesktopFileHandle : public FileHandle
	{
	public:
		DesktopFileHandle(std::ifstream& file);
		virtual ~DesktopFileHandle();

		virtual void ReadTextAsync(std::function<void(std::string)>& callback) override;
		virtual void ReadBufferAsync(Vector<std::uint8_t>& outBuffer) override;

		virtual void WriteTextAsync(const std::string& fileText) override;
		virtual void WriteBufferAsync(const Vector<std::uint8_t>& buffer);

		virtual void CloseFileAsync() override;

	private:
		std::ifstream* mFile;
	};
}


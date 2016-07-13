#pragma once
#include <fstream>
#include "FileHandle.h"

namespace Library
{
	class DesktopFileHandle : public FileHandle
	{
	public:
		DesktopFileHandle(const std::string& fileName);
		virtual ~DesktopFileHandle();

		virtual void ReadTextAsync(const std::function<void(std::string)>& callback) override;
		virtual void ReadBufferAsync(Vector<std::uint8_t>& outBuffer) override;

		virtual void WriteTextAsync(const std::string& fileText) override;
		virtual void WriteBufferAsync(const Vector<std::uint8_t>& buffer);

		virtual void OpenFileAsync(const std::function<void(void)>& callback, FileMode mode = FileMode::READ_ONLY) override;
		virtual void CloseFile() override;

		virtual std::string ReadLine() override;

		virtual bool IsEndOfFile() const override;

	private:
		std::fstream mFile;
		std::string mFileName;

		mutable std::recursive_mutex mMutex;
	};
}


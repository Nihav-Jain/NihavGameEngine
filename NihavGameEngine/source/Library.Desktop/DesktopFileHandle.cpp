#include "pch.h"
#include <sstream>
#include "DesktopFileHandle.h"


namespace Library
{
	DesktopFileHandle::DesktopFileHandle(std::ifstream& file) :
		mFile(&file)
	{
	}


	DesktopFileHandle::~DesktopFileHandle()
	{
		if (mFile->is_open())
			mFile->close();
		delete mFile;
	}

	void DesktopFileHandle::ReadTextAsync(std::function<void(std::string)>& callback)
	{
		std::future<void> fut = std::async([&]() {
			std::string str((std::istreambuf_iterator<char>(*mFile)), (std::istreambuf_iterator<char>()));
			callback(str);
		});
	}

	void DesktopFileHandle::ReadBufferAsync(Vector<std::uint8_t>& outBuffer)
	{
		UNREFERENCED_PARAMETER(outBuffer);
	}

	void DesktopFileHandle::WriteTextAsync(const std::string& fileText)
	{
		UNREFERENCED_PARAMETER(fileText);
	}

	void DesktopFileHandle::WriteBufferAsync(const Vector<std::uint8_t>& buffer)
	{
		UNREFERENCED_PARAMETER(buffer);
	}

	void DesktopFileHandle::CloseFileAsync()
	{
		if (mFile->is_open())
			mFile->close();
	}
}

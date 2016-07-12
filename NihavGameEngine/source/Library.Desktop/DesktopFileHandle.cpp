#include "pch.h"
#include <sstream>
#include "DesktopFileHandle.h"


namespace Library
{
	DesktopFileHandle::DesktopFileHandle(const std::string& fileName) :
		mFileName(fileName), mFile(), mMutex()
	{}

	DesktopFileHandle::~DesktopFileHandle()
	{
		CloseFile();
	}

	void DesktopFileHandle::ReadTextAsync(std::function<void(std::string)>& callback)
	{
		std::function<void(void)> openCallback = [&]() {
			std::lock_guard<std::recursive_mutex> recLock(mMutex);
			std::string str((std::istreambuf_iterator<char>(mFile)), (std::istreambuf_iterator<char>()));
			callback(str);
		};

		bool open = false;

		{
			std::lock_guard<std::recursive_mutex> recLock(mMutex);
			open = bIsOpen;
		}

		if (!open)
			OpenFileAsync(openCallback, FileMode::READ_ONLY);
		else
			std::future<void> fut = std::async(openCallback);
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

	void DesktopFileHandle::OpenFileAsync(std::function<void(void)>& callback, FileMode mode)
	{
		std::future<void> fut =  std::async([&]() {
			{
				std::lock_guard<std::recursive_mutex> recLock(mMutex);
				if (bIsOpen)
					CloseFile();

				switch (mode)
				{
				case FileMode::READ_ONLY:
					mFile.open(mFileName, std::fstream::in);
					break;
				case FileMode::WRITE_ONLY:
					mFile.open(mFileName, std::fstream::out);
					break;
				case FileMode::READ_WRITE:
					mFile.open(mFileName, std::fstream::in | std::fstream::out);
				default:
					throw std::exception("Invalid File mode");
				}

				bIsOpen = true;
			}
			callback();
		});
	}

	void DesktopFileHandle::CloseFile()
	{
		std::lock_guard<std::recursive_mutex> recLock(mMutex);
		if (mFile.is_open())
			mFile.close();
		bIsOpen = false;
	}

	std::string DesktopFileHandle::ReadLine()
	{
		{
			std::lock_guard<std::recursive_mutex> recLock(mMutex);
			if (!bIsOpen)
				throw std::exception("File is not open");
		}

		if (IsEndOfFile())
			throw std::exception("End of Stream encountered.");

		std::string str;
		
		std::lock_guard<std::recursive_mutex> recLock(mMutex);
		std::getline(mFile, str);
		
		return str;
	}

	bool DesktopFileHandle::IsEndOfFile() const
	{
		std::lock_guard<std::recursive_mutex> recLock(mMutex);
		return !mFile.good();
	}
}

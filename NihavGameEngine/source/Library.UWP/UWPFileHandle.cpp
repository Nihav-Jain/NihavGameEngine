#include "pch.h"
#include "UWPFileHandle.h"

using namespace Windows::Storage;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation;

namespace Library
{
	RTTI_DEFINITIONS(UWPFileHandle, FileHandle);

	UWPFileHandle::UWPFileHandle(Windows::Storage::StorageFile^ file) :
		mFile(file), mMutex(), mFileLines(0), mLineItr()
	{}

	UWPFileHandle::~UWPFileHandle()
	{
		CloseFile();
	}

	void UWPFileHandle::ReadTextAsync(std::function<void(std::string)>& callback)
	{
		concurrency::create_task(FileIO::ReadTextAsync(mFile)).then([&](Platform::String^ fileContent)
		{
			std::string str;
			str.assign(fileContent->Begin(), fileContent->End());
			callback(str);
		});
	}

	void UWPFileHandle::ReadBufferAsync(Vector<std::uint8_t>& outBuffer)
	{
		UNREFERENCED_PARAMETER(outBuffer);
	}

	void UWPFileHandle::WriteTextAsync(const std::string& fileText)
	{
		UNREFERENCED_PARAMETER(fileText);
	}

	void UWPFileHandle::WriteBufferAsync(const Vector<std::uint8_t>& buffer)
	{
		UNREFERENCED_PARAMETER(buffer);
	}

	std::string UWPFileHandle::ReadLine()
	{
		if (!bIsOpen)
			throw std::exception("File is not open");
		if (IsEndOfFile())
			throw std::exception("End of Stream encountered.");
		return *(mLineItr++);
	}

	void UWPFileHandle::OpenFileAsync(std::function<void(void)>& callback, FileMode mode)
	{
		if (bIsOpen)
			CloseFile();

		bIsOpen = true;
		if (mode != FileMode::WRITE_ONLY)
		{
			mFileLines.Clear();
			concurrency::create_task(FileIO::ReadLinesAsync(mFile)).then([&](IVector<Platform::String^>^ list) {
				std::uint32_t i;
				for (i = 0; i < list->Size; i++)
				{
					std::string str;
					str.assign(list->GetAt(i)->Begin(), list->GetAt(i)->End());
					mFileLines.PushBack(str);
				}
				mLineItr = mFileLines.begin();
				callback();
			});
		}
	}

	void UWPFileHandle::CloseFile()
	{
		mFileLines.Clear();
		mLineItr = mFileLines.end();
		bIsOpen = false;
	}

	bool UWPFileHandle::IsEndOfFile() const
	{
		return mLineItr == mFileLines.end();
	}
}

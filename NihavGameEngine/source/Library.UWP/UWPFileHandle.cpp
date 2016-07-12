#include "pch.h"
#include "UWPFileHandle.h"

using namespace Windows::Storage;
namespace Library
{
	RTTI_DEFINITIONS(UWPFileHandle, FileHandle);

	UWPFileHandle::UWPFileHandle(Windows::Storage::StorageFile^ file) :
		mFile(file)
	{}

	UWPFileHandle::~UWPFileHandle()
	{
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

	void UWPFileHandle::CloseFileAsync()
	{

	}
}

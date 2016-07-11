#include "pch.h"
#include "UWPFileHandle.h"

namespace Library
{
	UWPFileHandle::UWPFileHandle()
	{
	}

	UWPFileHandle::~UWPFileHandle()
	{
	}

	void UWPFileHandle::ReadTextAsync(std::string& outFileText)
	{
		UNREFERENCED_PARAMETER(outFileText);
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

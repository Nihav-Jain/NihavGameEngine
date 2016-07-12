#pragma once
#include <ppltasks.h>
#include <string>
#include <memory>
#include "FileHandle.h"


namespace Library
{
	class UWPFileHandle : public FileHandle
	{
	public:
		UWPFileHandle(Windows::Storage::StorageFile^ file);
		virtual ~UWPFileHandle();

		virtual void ReadTextAsync(std::function<void(std::string)>& callback) override;
		virtual void ReadBufferAsync(Vector<std::uint8_t>& outBuffer) override;

		virtual void WriteTextAsync(const std::string& fileText) override;
		virtual void WriteBufferAsync(const Vector<std::uint8_t>& buffer) override;

		virtual void CloseFileAsync() override;

	private:
		Windows::Storage::StorageFile^ mFile;
	};
}


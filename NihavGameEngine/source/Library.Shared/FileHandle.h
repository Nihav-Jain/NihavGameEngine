#pragma once
#include <functional>
#include "Vector.h"

namespace Library
{
	class FileHandle
	{
	public:

		enum class FileMode
		{
			READ_ONLY,
			WRITE_ONLY,
			READ_WRITE
		};

		virtual ~FileHandle();

		virtual void ReadTextAsync(std::function<void(std::string)>& callback) = 0;
		virtual void ReadBufferAsync(Vector<std::uint8_t>& outBuffer) = 0;

		virtual void WriteTextAsync(const std::string& fileText) = 0;
		virtual void WriteBufferAsync(const Vector<std::uint8_t>& buffer) = 0;

		virtual void CloseFileAsync() = 0;
		bool IsOpen() const;

	protected:
		FileHandle();

		bool bIsOpen;
		friend class FileManager;
	};
}

#pragma once
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

		virtual void ReadTextAsync(std::string& outFileText) = 0;
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

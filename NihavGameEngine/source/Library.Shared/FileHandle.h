#pragma once
#include <functional>
#include <vector>
#include "Vector.h"
#include "RTTI.h"

namespace Library
{
	class FileHandle : public RTTI
	{
		RTTI_DECLARATIONS(FileHandle, RTTI);

	public:
		enum class FileMode
		{
			READ_ONLY,
			WRITE_ONLY,
			READ_WRITE
		};

		virtual ~FileHandle();

		virtual void ReadTextAsync(const std::function<void(std::string)>& callback) = 0;
		virtual void ReadBufferAsync(const std::function<void(std::vector<std::uint8_t>)>& callback) = 0;

		virtual void WriteTextAsync(const std::string& fileText) = 0;
		virtual void WriteBufferAsync(const Vector<std::uint8_t>& buffer) = 0;

		virtual void OpenFileAsync(const std::function<void(void)>& callback, FileMode mode = FileMode::READ_ONLY) = 0;
		virtual void CloseFile() = 0;

		virtual std::string ReadLine() = 0;

		virtual bool IsEndOfFile() const = 0;

		bool IsOpen() const;

	protected:
		FileHandle();

		bool bIsOpen;
		friend class FileManager;
	};
}

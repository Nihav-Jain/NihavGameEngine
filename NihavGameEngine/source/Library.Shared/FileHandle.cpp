#include "pch.h"
#include "FileHandle.h"

namespace Library
{
	RTTI_DEFINITIONS(FileHandle, RTTI);

	FileHandle::FileHandle() :
		bIsOpen(false)
	{
	}


	FileHandle::~FileHandle()
	{
	}

	bool FileHandle::IsOpen() const
	{
		return bIsOpen;
	}
}

#include "pch.h"
#include "FileHandle.h"

namespace Library
{
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

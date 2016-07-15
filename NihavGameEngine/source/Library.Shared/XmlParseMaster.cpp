#include "pch.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"
#include "FileManager.h"

namespace Library
{
	XmlParseMaster::XmlParseMaster(SharedData& sharedData) :
		mSharedData(&sharedData), mHelpers(), mLastClonedHelper(0), mIsCloned(false), mFileName(std::string()), mXmlParser(nullptr),
		mFileHandles(), mFileHandleCounter(0), mMutex(), bIncludeFileOpen(false)
	{
		mSharedData->SetXmlParseMaster(this);

		fOpenFileCallback = [this]() {
			bool isFirstChunk = true;
			bool isLastChunk = false;

			std::string fileData;
			while (!mFileHandles.IsEmpty())
			{
				while (!mFileHandles.Top()->IsEndOfFile())
				{
					fileData = mFileHandles.Top()->ReadLine();

					std::uint32_t fileLength = static_cast<std::uint32_t>(fileData.length());

					if (mFileHandleCounter == 1)
						isLastChunk = mFileHandles.Top()->IsEndOfFile();

					if (!Parse(fileData.c_str(), fileLength, isFirstChunk, isLastChunk))
					{
						CloseTopFileHandle();
						fParseFromFileCallback(false);
					}
					isFirstChunk = false;
				}
				CloseTopFileHandle();
			}
			fParseFromFileCallback(true);
		};

		fGetFileCallback = [this](FileHandle* handle) {
			{
				std::lock_guard<std::recursive_mutex> lock(mMutex);
				mFileHandles.Push(handle);
				mFileHandleCounter++;
			}
			handle->OpenFileAsync(fTempCallback);
		};

		fOpenFileCallbackForIncludes = [&]() {
			bIncludeFileOpen = true;
		};

	}

	XmlParseMaster::~XmlParseMaster()
	{
		if (mIsCloned)
		{
			delete mSharedData;
			for (std::uint32_t i = 0; i < mLastClonedHelper; i++)
			{
				delete mHelpers[i];
				mHelpers[i] = nullptr;
			}
		}
		XML_ParserFree(mXmlParser);

		while (!mFileHandles.IsEmpty())
		{
			delete mFileHandles.Top();
			mFileHandles.Pop();
		}
	}

	XmlParseMaster* XmlParseMaster::Clone() const
	{
		SharedData* clonedSharedData = mSharedData->Clone();
		XmlParseMaster* clonedParseMaster = new XmlParseMaster(*clonedSharedData);

		for (std::uint32_t i = 0; i < mHelpers.Size(); i++)
		{
			clonedParseMaster->AddHelper(*(mHelpers[i]->Clone()));
		}
		clonedParseMaster->mIsCloned = true;
		clonedParseMaster->mLastClonedHelper = mHelpers.Size();

		return clonedParseMaster;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper& helper)
	{
		mHelpers.PushBack(&helper);
	}

	void XmlParseMaster::RemoveHelper(IXmlParseHelper& helper)
	{
		mHelpers.Remove(mHelpers.Find(&helper));
	}

	bool XmlParseMaster::Parse(const char* document, std::int32_t length, bool isFirstChunk, bool isLastChunk)
	{
		if (isFirstChunk)
		{
			InitializeXmlParser();

			for (auto helper : mHelpers)
			{
				helper->Initialize();
			}
		}
		XML_Status parseCode = XML_Parse(mXmlParser, document, length, isLastChunk);

		if (parseCode == XML_STATUS_ERROR || parseCode == XML_STATUS_SUSPENDED)
			return false;
		return true;
	}

	void XmlParseMaster::ParseFromFileAsync(const std::string& fileName, const std::function<void(bool)>& callback)
	{
		mFileName = fileName;
		fParseFromFileCallback = callback;
		fTempCallback = fOpenFileCallback;
		OpenFileHandleAsync(fileName, fOpenFileCallback);
	}

	const std::string& XmlParseMaster::GetFileName() const
	{
		return mFileName;
	}

	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	void XmlParseMaster::SetSharedData(SharedData& sharedData)
	{
		if (mIsCloned)
			delete mSharedData;
		mSharedData = &sharedData;
		mSharedData->SetXmlParseMaster(this);
	}

	const Vector<IXmlParseHelper*>& XmlParseMaster::Helpers() const
	{
		return mHelpers;
	}

	void XmlParseMaster::StartElementHandler(void* userData, const char* elementName, const char** attributes)
	{
		XmlParseMaster* xmlParseMaster = reinterpret_cast<XmlParseMaster*>(userData);
		assert(xmlParseMaster != nullptr);

		Hashmap<std::string, std::string> attributeMap;
		xmlParseMaster->GetSharedData()->IncrementDepth();

		std::uint32_t i = 0;
		while (attributes[i] != '\0')
		{
			attributeMap[attributes[i]] = attributes[i + 1];
			i += 2;
		}
		std::string element = elementName;
		if (element == "include")
		{
			if (!attributeMap.ContainsKey("file"))
				throw std::exception("<include> tag has missing attribute: file");
			bool done = false;
			xmlParseMaster->bIncludeFileOpen = false;
			xmlParseMaster->fTempCallback = xmlParseMaster->fOpenFileCallbackForIncludes;

			xmlParseMaster->OpenFileHandleAsync(attributeMap["file"], [&]() {
				done = true;
			});

			while(!xmlParseMaster->bIncludeFileOpen){}
			return;
		}

		for (i = 0; i < xmlParseMaster->mHelpers.Size(); i++)
		{
			if (xmlParseMaster->mHelpers[i]->StartElementHandler(*xmlParseMaster->GetSharedData(), elementName, attributeMap))
			{
				break;
			}
		}
	}

	void XmlParseMaster::EndElementHandler(void* userData, const char* elementName)
	{
		XmlParseMaster* xmlParseMaster = reinterpret_cast<XmlParseMaster*>(userData);
		assert(xmlParseMaster != nullptr);

		for (std::uint32_t i = 0; i < xmlParseMaster->mHelpers.Size(); i++)
		{
			if (xmlParseMaster->mHelpers[i]->EndElementHandler(*xmlParseMaster->GetSharedData(), elementName))
			{
				break;
			}
		}
		xmlParseMaster->GetSharedData()->DecrementDepth();
	}

	void XmlParseMaster::CharDataHandler(void* userData, const char* characterStream, int length)
	{
		XmlParseMaster* xmlParseMaster = reinterpret_cast<XmlParseMaster*>(userData);
		assert(xmlParseMaster != nullptr);

		std::string characterData(characterStream, characterStream + length);
		characterData = xmlParseMaster->TrimInplace(characterData);
		if (!characterData.empty())
		{
			for (std::uint32_t i = 0; i < xmlParseMaster->mHelpers.Size(); i++)
			{
				if (xmlParseMaster->mHelpers[i]->CharDataHandler(*xmlParseMaster->GetSharedData(), characterData))
				{
					break;
				}
			}
		}
	}

	void XmlParseMaster::InitializeXmlParser()
	{
		if (mXmlParser == nullptr)
			mXmlParser = XML_ParserCreate(NULL);
		else
			XML_ParserReset(mXmlParser, NULL);
		XML_SetElementHandler(mXmlParser, XmlParseMaster::StartElementHandler, XmlParseMaster::EndElementHandler);
		XML_SetCharacterDataHandler(mXmlParser, XmlParseMaster::CharDataHandler);
		XML_SetUserData(mXmlParser, this);
	}


	std::string& XmlParseMaster::TrimRightInplace(std::string& s, const std::string& delimiters)
	{
		return s.erase(s.find_last_not_of(delimiters) + 1);
	}

	std::string& XmlParseMaster::TrimLeftInplace(std::string& s, const std::string& delimiters)
	{
		return s.erase(0, s.find_first_not_of(delimiters));
	}

	std::string& XmlParseMaster::TrimInplace(std::string& s, const std::string& delimiters)
	{
		return TrimLeftInplace(TrimRightInplace(s, delimiters), delimiters);
	}

	void XmlParseMaster::OpenFileHandleAsync(const std::string& fileName, const std::function<void(void)>& callback)
	{
		UNREFERENCED_PARAMETER(callback);
		FileManager::Get().GetFileAsync(fileName, fGetFileCallback);
	}

	void XmlParseMaster::CloseTopFileHandle()
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		mFileHandles.Top()->CloseFile();
		delete mFileHandles.Top();
		mFileHandles.Pop();
		mFileHandleCounter--;
	}

#pragma region SharedData

	XmlParseMaster::SharedData::SharedData() :
		SharedData(nullptr, 0)
	{}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone() const
	{
		SharedData* clonedSharedData = new SharedData(mParseMaster, 0);

		return clonedSharedData;
	}

	XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster() const
	{
		return mParseMaster;
	}

	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster* parseMaster)
	{
		mParseMaster = parseMaster;
	}

	void XmlParseMaster::SharedData::IncrementDepth()
	{
		mDepth++;
	}

	void XmlParseMaster::SharedData::DecrementDepth()
	{
		mDepth--;
	}

	std::uint32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

	XmlParseMaster::SharedData::SharedData(XmlParseMaster* parseMaster, std::uint32_t depth) :
		mParseMaster(parseMaster), mDepth(depth)
	{}

	RTTI_DEFINITIONS(XmlParseMaster::SharedData, RTTI);

#pragma endregion
}

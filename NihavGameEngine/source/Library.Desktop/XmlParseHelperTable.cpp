#include "pch.h"
#include "XmlParseHelperTable.h"

namespace Library
{

	bool XmlParseHelperTable::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != "scope")
			return false;

		if (!attributes.ContainsKey("name"))
			throw std::exception("Invalid syntax for <scope>. Missing attribute: name");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SCOPE_START, true))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->CurrentScopePtr =  &(sharedDataPtr->CurrentScopePtr->AppendScope(attributes["name"]));

		return true;
	}

	bool XmlParseHelperTable::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != "scope")
			return false;

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SCOPE_END, true))
			throw std::exception("Invalid script syntax");
		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::END_STATE_ROUTER))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();
		return true;
	}

	IXmlParseHelper* XmlParseHelperTable::Clone() const
	{
		return new XmlParseHelperTable();
	}

}

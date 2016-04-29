#include "pch.h"
#include "XmlParseHelperPolygon.h"
#include "PolygonRenderer.h"
#include "Renderer.h"
#include "Entity.h"

namespace Library {
	const std::string XmlParseHelperPolygon::ELEMENT_NAME = "polygon_renderer";

	XmlParseHelperPolygon::XmlParseHelperPolygon()
	{
	}

	bool XmlParseHelperPolygon::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName, const Hashmap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(attributes);
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != ELEMENT_NAME)
			return false;

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_START))
			throw std::exception("Invalid script syntax");
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
		Scope* parent = sharedDataPtr->CurrentScopePtr;
		PolygonRenderer * polygon = new PolygonRenderer();
		parent->Adopt(Entity::ATTRIBUTE_ACTIONS, *polygon);

		Renderer::GetInstance()->AddRenderable(polygon);
		sharedDataPtr->CurrentScopePtr = polygon;

		return true;
	}

	bool XmlParseHelperPolygon::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != ELEMENT_NAME)
			return false;

		bool transitionToActionEnd = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::ACTION_END);
		UNREFERENCED_PARAMETER(transitionToActionEnd);
		assert(transitionToActionEnd);
		bool transitionToStateRouter = sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER);
		UNREFERENCED_PARAMETER(transitionToStateRouter);
		assert(transitionToStateRouter);

		sharedDataPtr->CurrentScopePtr = sharedDataPtr->CurrentScopePtr->GetParent();

		return true;
	}

	IXmlParseHelper * XmlParseHelperPolygon::Clone() const
	{
		return new XmlParseHelperPolygon();
	}
}
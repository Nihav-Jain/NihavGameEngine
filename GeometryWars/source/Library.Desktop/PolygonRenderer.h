#pragma once
#include "glm\vec4.hpp"
#include "RTTI.h"
#include "Attributed.h"
#include "Renderable.h"

namespace Library {

	class Texture;
	class Shader;
	class RenderBuffer;

	/**
	* The renderable polygon class
	* TODO: Add trasnformation to the entiy trasnformation accordingly
	*/
	class PolygonRenderer : public ActionList, public Renderable
	{
		RTTI_DECLARATIONS(PolygonRenderer, ActionList);

	public:
		static const std::string ATTRIBUTE_INDICES;
		static const std::string ATTRIBUTE_POINTS;

		PolygonRenderer();
		virtual ~PolygonRenderer() = default;

		PolygonRenderer(const PolygonRenderer & rhs) = delete;
		PolygonRenderer & operator=(const PolygonRenderer & rhs) = delete;

		virtual void Render(RenderDevice * device) override;

	private:
		void Init(RenderDevice * device);
		bool mInited;
		Shader * mShader;
		RenderBuffer * mBuffer;

		float mWidth;
		glm::vec4 mColor;

		Datum * mPosition;
		Datum * mRotation;
		Datum * mScale;
		Datum * mIndices;
		Datum * mPoints;
	};

	CONCRETE_ACTION_FACTORY(PolygonRenderer);
}


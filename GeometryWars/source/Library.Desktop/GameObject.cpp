#include "pch.h"
#include "GameObject.h"

namespace Library
{
	RTTI_DEFINITIONS(GameObject);

	const std::string GameObject::ATTRIBUTE_POSITION = "position";
	const std::string GameObject::ATTRIBUTE_ROTATION = "rotation";
	const std::string GameObject::ATTRIBUTE_SCALE = "scale";

	// TODO : Make these sector names static const somewhere?
#pragma warning (disable:4592)
	const Hashmap<std::string, GameObject::GameObjectType> GameObject::SectorTypeStrings =
	{
		{ "playerSector",	GameObject::GameObjectType::PLAYER },
		{ "enemiesSector",	GameObject::GameObjectType::ENEMY },
		{ "bulletsSector",	GameObject::GameObjectType::BULLET }
	};
#pragma warning (default:4592)

	GameObject::GameObject()
		: mType(GameObjectType::INVALID), mPosition(), mRotation(), mScale(1.0f)
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_ROTATION, 1, &mRotation);
		AddExternalAttribute(ATTRIBUTE_SCALE, 1, &mScale);
	}

	const glm::vec4 & GameObject::Position() const
	{
		return mPosition;
	}

	void GameObject::SetPosition(const glm::vec4 & position)
	{
		mPosition = position;
	}

	const glm::vec4 & GameObject::Rotation() const
	{
		return mRotation;
	}

	void GameObject::SetRotation(const glm::vec4 & rotation)
	{
		mRotation = rotation;
	}

	const glm::vec4 & GameObject::Scale() const
	{
		return mScale;
	}

	void GameObject::SetScale(const glm::vec4 & scale)
	{
		mScale = scale;
	}

	GameObject::GameObjectType GameObject::Type() const
	{
		return mType;
	}

	void GameObject::SetType(const std::string & type)
	{
		mType = SectorTypeStrings[type];
	}

	void GameObject::SetType(GameObjectType type)
	{
		mType = type;
	}
}


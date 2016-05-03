#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Collectible : public GameObject
	{
		RTTI_DECLARATIONS(Collectible, GameObject);

	public:

		Collectible();
		virtual ~Collectible() = default;

		Collectible(const Collectible& rhs);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, WorldState& worldState) override;

		static const std::string ATTRIBUTE_ISCOLLECTED;
		static const std::string ATTRIBUTE_CHANNEL;

	protected:

		bool mIsCollected;
		std::string mCollisionChannel;

	private:

		void ResetAttributePointers();
		static const class CollectibleFactory cf;
	};

	CONCRETE_ENTITY_FACTORY(Collectible);
}


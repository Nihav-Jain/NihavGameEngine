#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Bullet : public GameObject
	{
		RTTI_DECLARATIONS(Bullet, GameObject);

	public:

		Bullet();
		virtual ~Bullet() = default;

		static const std::string ATTRIBUTE_VELOCITY;

		// TODO: Remove these once bullet prototype exists in xml
		static const std::float_t DEFAULT_SPEED;
		static const glm::vec4 DEFAULT_SIZE;
		static const std::string DEFAULT_IMAGE;

		const glm::vec4 & Velocity() const;
		void SetVelocity(const glm::vec4 & velocity);

		void BulletDeath(WorldState& worldState);

		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, WorldState& worldState) override;

	protected:

		glm::vec4 mVelocity;
	};

	CONCRETE_ENTITY_FACTORY(Bullet);
}

#pragma once

namespace Library
{

	class ActionPlayMusic : public ActionList
	{
		RTTI_DECLARATIONS(ActionPlayMusic,ActionList)

	public:
		ActionPlayMusic();

		virtual ~ActionPlayMusic() = default;

		ActionPlayMusic(const ActionPlayMusic& rhs) = delete;
		ActionPlayMusic& operator=(const ActionPlayMusic& rhs) = delete;

		/**
		*	Calls the update method on all of its child Actions, called by the parent Action / Entity / Sector / World's Update method
		*	@param reference to the WorldState
		*/
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_NAME;
		static const std::string ATTRIBUTE_VOLUME;
		static const std::string ATTRIBUTE_LOOP;

	};

	CONCRETE_ACTION_FACTORY(ActionPlayMusic);
}
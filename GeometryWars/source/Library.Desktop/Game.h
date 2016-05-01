#pragma once

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#include "GameClock.h"
#include "GameTime.h"

#include "Renderer.h"
#include "RenderDevice.h"
#include "Animator.h"
#include "AnimationState.h"
#include "AnimationFrame.h"

#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"

#include "ActionList.h"
#include "ActionListSwitch.h"
#include "ActionExpression.h"
#include "ActionIfThenElse.h"
#include "ActionWhileLoop.h"
#include "ActionCreateEntity.h"
#include "ActionDestroyEntity.h"
#include "ActionCreateEntityFromFile.h"

#include "ActionEvent.h"
#include "Reaction.h"
#include "ReactionAttributed.h"

#include "SharedDataTable.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperWorld.h"
#include "XmlParseHelperSector.h"
#include "XmlParseHelperEntity.h"
#include "XmlParseHelperAction.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperPrimitives.h"
#include "XmlParseHelperNameValue.h"
#include "XmlParseHelperActionSwitch.h"
#include "XmlParseHelperActionExpression.h"
#include "XmlParseHelperReaction.h"
#include "XmlParseHelperActionEvent.h"
#include "XmlParseHelperActionIf.h"
#include "XmlParseHelperActionWhile.h"
#include "XmlParseHelperBeginPlay.h"
#include "XmlParseHelperSprite.h"
#include "XmlParseHelperPolygon.h"
#include "XmlParseHelperImage.h"
#include "XmlParseHelperOnDestroy.h"
#include "XmlParseHelperAnimator.h"
#include "XmlParseHelperAnimationState.h"
#include "XmlParseHelperAnimationFrame.h"

namespace Library
{
	/**
	 *	Game class which contains the world, manages the game clock
	 *	Call Start() before the game loop
	 *	Call Update() in the game loop
	 *	Call Destroy() after the game loop
	 */
	class Game final
	{
	public:

		/**
		 *	parameterless constructor, initializes the members
		 */
		Game();

		/**
		 *	disallow copy constructor
		 */
		Game(const Game& rhs) = delete;

		/**
		 *	destructor
		 */
		~Game();

		/**
		 *	disallow assignment operator
		 */
		Game& operator=(const Game& rhs) = delete;

		/**
		 *	Getter for the game World
		 *	@return reference to World
		 */
		World& GetWorld();

		/**
		 *	Getter for the XML parse master of the Game
		 *	@return reference to Xml Parse Master
		 */
		XmlParseMaster& ParseMaster();

		/**
		 *	Getter of Game time
		 *	@return const reference to game time
		 */
		const GameTime& GetGameTime() const;

		/**
		*	Resets the game clock and other things to be initialized before starting the game loop
		*	Must be called before entering the game loop
		*/
		void Start();

		/**
		 *	Resets the game clock and other things to be initialized before starting the game loop
		 *	Must be called before entering the game loop
		 */
		void Start(const std::string & config);

		/**
		 *	Updates the game clock and calls Update on the game World
		 *	Must be called every frame, inside the game loop
		 */
		void Update();

		/**
		 *	Will close off any lose ends before the Game ends
		 *	Must be called after exiting from the game loop
		 */
		void Destroy();

		void SetRenderer(Renderer * device);

	private:
		void AddParseHelpers();

		Renderer* mRenderer;

		SharedDataTable mSharedData;
		XmlParseMaster mParseMaster;

		GameClock mGameClock;
		GameTime mGameTime;
		World mWorld;

		XmlParseHelperWorld mWorldParser;
		XmlParseHelperSector mSectorParser;
		XmlParseHelperEntity mEntityParser;
		XmlParseHelperAction mActionParser;
		XmlParseHelperTable mTableParser;
		XmlParseHelperPrimitives mPrimitivesParser;
		XmlParseHelperNameValue mNameValueParser;
		XmlParseHelperActionSwitch mSwitchParser;
		XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase mCaseParser;
		XmlParseHelperActionExpression mExpressionParser;
		XmlParseHelperReaction mReactionParser;
		XmlParseHelperActionEvent mActionEvent;

		XmlParseHelperActionIf mActionIf;
		XmlParseHelperActionIf::XmlParseHelperActionIfCondition mActionIfCondition;
		XmlParseHelperActionIf::XmlParseHelperActionIfThen mActionIfThen;
		XmlParseHelperActionIf::XmlParseHelperActionIfElse mActionIfElse;

		XmlParseHelperActionWhile mActionWhile;
		XmlParseHelperActionWhile::XmlParseHelperActionWhileLoopBody mActionWhileLoop;
		XmlParseHelperBeginPlay mActionBeginPlay;
		XmlParseHelperOnDestroy mActionOnDestroy;

		XmlParseHelperSprite mSpriteParser;
		XmlParseHelperPolygon mPolygonParser;
		XmlParseHelperImage mImageParser;
		XmlParseHelperAnimator mAnimatorParser;
		XmlParseHelperAnimationState mAnimationStateParser;
		XmlParseHelperAnimationFrame mAnimationFrameParser;

		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		ActionListSwitchFactory mActionSwitchFactory;
		ActionListSwitch::ActionListSwitchCaseFactory mActionCaseFactory;
		ActionWhileLoopFactory mActionWhileFactory;
		ActionIfThenElseFactory mActionIfFactory;
		ActionExpressionFactory mActionExpressionFactory;
		ActionEventFactory mActionEventFactory;
		ReactionAttributedFactory mReactionFactory;
		ActionCreateEntityFactory mCreateEntityFactory;
		ActionDestroyEntityFactory mDestroyEntityFactory;
		ActionCreateEntityFromFileFactory mCreateEntityFromFileFactory;
		AnimatorFactory mAnimatorFactory;
		AnimationStateFactory mAnimationStateFactory;
		AnimationFrameFactory mAnimationFrameFactory;
	};
}

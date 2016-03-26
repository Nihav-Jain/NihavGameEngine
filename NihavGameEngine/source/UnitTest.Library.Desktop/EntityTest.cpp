#include "pch.h"
#include "CppUnitTest.h"

#include "GameClock.h"
#include "GameTime.h"
#include "Factory.h"
#include "RTTI.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

#include "XmlParseMaster.h"
#include "SharedDataTable.h"
#include "XmlParseHelperWorld.h"
#include "XmlParseHelperSector.h"
#include "XmlParseHelperEntity.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperPrimitives.h"
#include "XmlParseHelperNameValue.h"

#include "ActorEntity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(EntityTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			SharedDataTable::ClearStateGraph();
			XmlParseHelperPrimitives::ClearStaticMembers();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(EntityTestSimple)
		{
			EntityFactory entityFactory;

			GameClock gameClock;
			gameClock.Reset();
			GameTime gameTime;

			std::string worldName = "World";
			World world;
			world.SetName("World");
			world.SetGameTime(gameTime);

			Assert::AreEqual(worldName, world.Name());
			Assert::AreEqual(worldName, world["name"].Get<std::string>());
			Assert::IsTrue(world.IsPrescribedAttribute(World::ATTRIBUTE_NAME_SECTOR));

			Sector& sector1 = world.CreateSector("sector1");
			Sector& sector2 = world.CreateSector("sector2");

			Assert::IsFalse(world.IsAttribute("sector1"));
			Datum& worldSectors = world.Sectors();
			Assert::IsTrue(worldSectors.Get<Scope*>(0) == &sector1);
			Assert::IsTrue(worldSectors.Get<Scope*>(1) == &sector2);
			Assert::IsTrue(sector1.Name() == "sector1");
			Assert::IsTrue(sector2.Name() == "sector2");

			Assert::IsTrue(&world == sector1.GetWorld());

			Entity& entity11 = sector1.CreateEntity("Entity", "entity11");
			Entity& entity12 = sector1.CreateEntity("Entity", "entity12");

			Entity& entity21 = sector2.CreateEntity("Entity", "entity21");
			Entity& entity22 = sector2.CreateEntity("Entity", "entity22");

			Assert::IsFalse(sector1.IsAttribute("entity11"));
			Datum& sector1Entities = sector1.Entities();
			Assert::IsTrue(sector1Entities.Get<Scope*>(0) == &entity11);
			Assert::IsTrue(sector1Entities.Get<Scope*>(1) == &entity12);
			Assert::IsTrue(entity11.Name() == "entity11");
			Assert::IsTrue(entity12.Name() == "entity12");

			Assert::IsFalse(sector2.IsAttribute("entity21"));
			Datum& sector2Entities = sector2.Entities();
			Assert::IsTrue(sector2Entities.Get<Scope*>(0) == &entity21);
			Assert::IsTrue(sector2Entities.Get<Scope*>(1) == &entity22);
			Assert::IsTrue(entity21.Name() == "entity21");
			Assert::IsTrue(entity22.Name() == "entity22");

			Assert::IsTrue(&sector1 == entity11.GetSector());
			Assert::IsTrue(&sector2 == entity21.GetSector());

			gameClock.UpdateGameTime(gameTime);
			world.Update();

			Assert::IsTrue(world.FindSector("sector1") == &sector1);
			Assert::IsTrue(world.FindSector("sector1")->FindEntity("entity12") == &entity12);

			World anotherWorld = world;
			anotherWorld.SetName("anotherWorld");

			Assert::IsTrue(world.Name() == "World");
			Assert::IsTrue(anotherWorld.Name() == "anotherWorld");
			Datum& anotherWorldSectors = anotherWorld.Sectors();
			Assert::AreEqual(2U, anotherWorldSectors.Size());
			Assert::IsFalse(anotherWorldSectors.Get<Scope*>(0) == worldSectors.Get<Scope*>(0));
			Assert::IsTrue(anotherWorld.IsPrescribedAttribute("sectors"));
			
			//Sector* anotherSector1 = anotherWorldSectors.Get<Scope*>(0)->As<Sector>();
			Assert::IsTrue(anotherWorldSectors.Get<Scope*>(0)->Is(Scope::TypeIdClass()));
		}

		TEST_METHOD(EntityTestXML)
		{
			EntityFactory entityFactory;
			ActorEntityFactory actorEntityFactory;

			SharedDataTable sharedData;
			Scope rootScope;
			sharedData.SetRootScope(rootScope);

			XmlParseMaster master(sharedData);
			XmlParseHelperWorld worldParser;
			XmlParseHelperSector sectorParser;
			XmlParseHelperEntity entityParser;
			XmlParseHelperTable tableParser;
			XmlParseHelperPrimitives primitivesParser;
			XmlParseHelperNameValue nameValueParser;

			master.AddHelper(worldParser);
			master.AddHelper(sectorParser);
			master.AddHelper(entityParser);
			master.AddHelper(tableParser);
			master.AddHelper(primitivesParser);
			master.AddHelper(nameValueParser);

			Assert::IsTrue(master.ParseFromFile("Content/config/xml_entity_test.xml"));

			Datum* worldDatum = rootScope.Find("world");
			Assert::IsNotNull(worldDatum);
			Assert::IsTrue(worldDatum->Type() == Datum::DatumType::TABLE);
			Assert::AreEqual(1U, worldDatum->Size());

			Scope* worldScope = worldDatum->Get<Scope*>();
			World* world = worldScope->As<World>();
			Assert::IsNotNull(world);

			Assert::IsTrue(world->Name() == "NihavWorld");
			Assert::IsTrue(world->IsPrescribedAttribute("sectors"));

			Assert::IsTrue(world->IsAuxiliaryAttribute("worldInt1"));
			Assert::IsTrue(world->IsAuxiliaryAttribute("worldInt2"));
			Assert::IsTrue(world->IsAuxiliaryAttribute("worldInt3"));

			Assert::AreEqual(10, world->operator[]("worldInt1").Get<std::int32_t>());
			Assert::AreEqual(20, world->operator[]("worldInt2").Get<std::int32_t>());
			Assert::AreEqual(30, world->operator[]("worldInt3").Get<std::int32_t>());

			Assert::IsTrue(world->IsAuxiliaryAttribute("worldScope"));

			Scope* worldNestedScope = world->operator[]("worldScope").Get<Scope*>();
			Assert::IsNotNull(worldNestedScope->Find("worldScopeFloat"));
			Assert::IsNotNull(worldNestedScope->Find("worldScopeString"));
			Assert::IsNotNull(worldNestedScope->Find("worldScopeVector"));

			Assert::AreEqual(10.10f, worldNestedScope->operator[]("worldScopeFloat").Get<std::float_t>());
			Assert::IsTrue("scopestring" == worldNestedScope->operator[]("worldScopeString").Get<std::string>());
			Assert::IsTrue(worldNestedScope->operator[]("worldScopeVector").Get<glm::vec4>() == glm::vec4(10, 20, 30, 40));

			Assert::IsFalse(world->IsAttribute("worldSector1"));

			Datum& worldSectors = world->Sectors();
			Assert::AreEqual(2U, worldSectors.Size());
			
				Sector* worldSector1 = worldSectors.Get<Scope*>(0)->As<Sector>();
				Assert::IsNotNull(worldSector1);
				Assert::IsTrue(worldSector1->Name() == "worldSector1");
				Assert::IsTrue(worldSector1->IsPrescribedAttribute("entities"));

				Assert::IsFalse(worldSector1->IsAuxiliaryAttribute("sector1Entity1"));
				Assert::IsTrue(worldSector1->IsAuxiliaryAttribute("sector1Scope"));

				Scope* sector1Scope = worldSector1->operator[]("sector1Scope").Get<Scope*>();
				Assert::IsNotNull(sector1Scope->Find("sector1Int"));
				Assert::AreEqual(5, sector1Scope->Find("sector1Int")->Get<std::int32_t>());

				Assert::IsTrue(worldSector1->IsAuxiliaryAttribute("sector1Mat"));
				Assert::IsTrue(worldSector1->Find("sector1Mat")->Get<glm::mat4>() == glm::mat4x4(10));

				Assert::IsFalse(worldSector1->IsAttribute("sector1Entity1"));

				Datum& sector1Entities = worldSector1->Entities();
				Assert::AreEqual(2U, sector1Entities.Size());

					Entity* sector1Entity = sector1Entities.Get<Scope*>(0)->As<Entity>();
					Assert::IsNotNull(sector1Entity);
					Assert::IsTrue(sector1Entity->Name() == "sector1Entity1");

					Assert::IsTrue(sector1Entity->IsAuxiliaryAttribute("entity1Str"));
					Assert::IsTrue(sector1Entity->IsAuxiliaryAttribute("entity1Scope"));

					Assert::IsTrue(sector1Entity->operator[]("entity1Str") == "stringvalue");
					Scope* sector1Entity1Scope = sector1Entity->operator[]("entity1Scope").Get<Scope*>();
					Assert::IsNotNull(sector1Entity1Scope->Find("entity1Int"));
					Assert::AreEqual(50, sector1Entity1Scope->Find("entity1Int")->Get<std::int32_t>());

					Entity* sector1Entity2 = sector1Entities.Get<Scope*>(1)->As<Entity>();
					Assert::IsNotNull(sector1Entity2);
					Assert::IsTrue(sector1Entity2->Is(ActorEntity::TypeIdClass()));
					Assert::IsTrue(sector1Entity2->IsPrescribedAttribute("position"));

					std::int32_t positionIteration = sector1Entity2->operator[]("positionIteration").Get<std::int32_t>();
					Assert::AreEqual(3, positionIteration);

				Sector* worldSector2 = worldSectors.Get<Scope*>(1)->As<Sector>();
				Assert::IsNotNull(worldSector2);
				Assert::AreEqual(1U, worldSector2->Entities().Size());
					Entity* sector2Entity = worldSector2->Entities().Get<Scope*>()->As<Entity>();
					Assert::IsNotNull(sector2Entity);
					Assert::IsTrue(sector2Entity->Is(ActorEntity::TypeIdClass()));
					Assert::IsTrue(sector2Entity->IsPrescribedAttribute("position"));

					std::int32_t positionIteration2 = sector2Entity->operator[]("positionIteration").Get<std::int32_t>();
					Assert::AreEqual(5, positionIteration2);

			while (positionIteration > 0)
			{
				world->Update();
				positionIteration--;
				positionIteration2--;
			}
			Assert::IsTrue(sector1Entity2->operator[]("position").Get<glm::vec4>() == glm::vec4(30, 0, 0, 0));
			while (positionIteration2 > 0)
			{
				world->Update();
				positionIteration2--;
			}
			Assert::IsTrue(sector2Entity->operator[]("position").Get<glm::vec4>() == glm::vec4(50, 0, 0, 0));

		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EntityTest::sStartMemState;
#endif
}
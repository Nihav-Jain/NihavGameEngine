#include "pch.h"
#include "SharedDataTable.h"


namespace Library
{
	RTTI_DEFINITIONS(SharedDataTable);

	//Graph<SharedDataTable::ParserState> SharedDataTable::ParserStateAutomata;

	SharedDataTable::SharedDataTable() :
		mRootScope(nullptr), CurrentScopePtr(nullptr), DataName(), DataValue(), NameValueElementDataParsed(false), mStateTraversor()
	{

		// prepare the state diagram graph here
		if (ParserStateAutomata.IsEmpty())
		{
			ParserState root = ParserState::ROOT_STATE;
			ParserState stateRouter = ParserState::STATE_ROUTER;

			ParserState worldStart = ParserState::WORLD_START;
			ParserState worldEnd = ParserState::WORLD_END;

			ParserState sectorStart = ParserState::SECTOR_START;
			ParserState sectorEnd = ParserState::SECTOR_END;

			ParserState entityStart = ParserState::ENTITY_START;
			ParserState entityEnd = ParserState::ENTITY_END;
			
			ParserState scopeStart = ParserState::SCOPE_START;
			ParserState scopeEnd = ParserState::SCOPE_END;
			
			ParserState primitiveStart = ParserState::PRIMITIVE_START;
			ParserState primitiveEnd = ParserState::PRIMITIVE_END;
			
			ParserState nameStart = ParserState::NAME_START;
			ParserState nameEnd = ParserState::NAME_END;
			
			ParserState valueStart = ParserState::VALUE_START;
			ParserState valueEnd = ParserState::VALUE_END;

			Graph<ParserState>::Traversor rootState = ParserStateAutomata.AddVertex(root);

			/** World */
			// root -> world_start -> world_router -> world_end
			Graph<ParserState>::Traversor worldStartState = ParserStateAutomata.AddVertex(worldStart, rootState);
			Graph<ParserState>::Traversor worldRouterState = ParserStateAutomata.AddVertex(stateRouter, worldStartState);
			Graph<ParserState>::Traversor worldEndState = ParserStateAutomata.AddVertex(worldEnd, worldRouterState);

			// world_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> world_router
			Graph<ParserState>::Traversor wPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, worldRouterState);
			Graph<ParserState>::Traversor wNameStart = ParserStateAutomata.AddVertex(nameStart, wPrimitiveStartState);
			Graph<ParserState>::Traversor wNameEnd = ParserStateAutomata.AddVertex(nameEnd, wNameStart);
			Graph<ParserState>::Traversor wValueStart = ParserStateAutomata.AddVertex(valueStart, wNameEnd);
			Graph<ParserState>::Traversor wValueEnd = ParserStateAutomata.AddVertex(valueEnd, wValueStart);
			Graph<ParserState>::Traversor wPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, wValueEnd);
			ParserStateAutomata.CreateEdge(wPrimitiveEndState, worldRouterState);

			// world_router -> scope_start -> world_router
			Graph<ParserState>::Traversor wscopeStartState = ParserStateAutomata.AddVertex(scopeStart, worldRouterState);
			ParserStateAutomata.CreateEdge(wscopeStartState, worldRouterState);

			// world_router -> scope_end -> world_router
			Graph<ParserState>::Traversor wscopeEndState = ParserStateAutomata.AddVertex(scopeEnd, worldRouterState);
			ParserStateAutomata.CreateEdge(wscopeEndState, worldRouterState);
		
			/** Sector */
			// world_router -> sector_start -> sector_router -> sector_end -> world_router
			Graph<ParserState>::Traversor sectorStartState = ParserStateAutomata.AddVertex(sectorStart, worldRouterState);
			Graph<ParserState>::Traversor sectorRouterState = ParserStateAutomata.AddVertex(stateRouter, sectorStartState);
			Graph<ParserState>::Traversor sectorEndState = ParserStateAutomata.AddVertex(sectorEnd, sectorRouterState);
			ParserStateAutomata.CreateEdge(sectorEndState, worldRouterState);

			// sector_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> sector_router
			Graph<ParserState>::Traversor sPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, sectorRouterState);
			Graph<ParserState>::Traversor sNameStart = ParserStateAutomata.AddVertex(nameStart, sPrimitiveStartState);
			Graph<ParserState>::Traversor sNameEnd = ParserStateAutomata.AddVertex(nameEnd, sNameStart);
			Graph<ParserState>::Traversor sValueStart = ParserStateAutomata.AddVertex(valueStart, sNameEnd);
			Graph<ParserState>::Traversor sValueEnd = ParserStateAutomata.AddVertex(valueEnd, sValueStart);
			Graph<ParserState>::Traversor sPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, sValueEnd);
			ParserStateAutomata.CreateEdge(sPrimitiveEndState, sectorRouterState);

			// sector_router -> scope_start -> sector_router
			Graph<ParserState>::Traversor sScopeStartState = ParserStateAutomata.AddVertex(scopeStart, sectorRouterState);
			ParserStateAutomata.CreateEdge(sScopeStartState, sectorRouterState);

			// sector_router -> scope_end -> sector_router
			Graph<ParserState>::Traversor sScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, sectorRouterState);
			ParserStateAutomata.CreateEdge(sScopeEndState, sectorRouterState);

			/** Entity */
			// sector_router -> entity_start -> entity_router -> entity_end -> sector_router
			Graph<ParserState>::Traversor entityStartState = ParserStateAutomata.AddVertex(entityStart, sectorRouterState);
			Graph<ParserState>::Traversor entityRouterState = ParserStateAutomata.AddVertex(stateRouter, entityStartState);
			Graph<ParserState>::Traversor entityEndState = ParserStateAutomata.AddVertex(entityEnd, entityRouterState);
			ParserStateAutomata.CreateEdge(entityEndState, sectorRouterState);

			// entity_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> entity_router
			Graph<ParserState>::Traversor ePrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, entityRouterState);
			Graph<ParserState>::Traversor eNameStart = ParserStateAutomata.AddVertex(nameStart, ePrimitiveStartState);
			Graph<ParserState>::Traversor eNameEnd = ParserStateAutomata.AddVertex(nameEnd, eNameStart);
			Graph<ParserState>::Traversor eValueStart = ParserStateAutomata.AddVertex(valueStart, eNameEnd);
			Graph<ParserState>::Traversor eValueEnd = ParserStateAutomata.AddVertex(valueEnd, eValueStart);
			Graph<ParserState>::Traversor ePrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, eValueEnd);
			ParserStateAutomata.CreateEdge(ePrimitiveEndState, entityRouterState);

			// entity_router -> scope_start -> entity_router
			Graph<ParserState>::Traversor eScopeStartState = ParserStateAutomata.AddVertex(scopeStart, entityRouterState);
			ParserStateAutomata.CreateEdge(eScopeStartState, entityRouterState);

			// entity_router -> scope_end -> entity_router
			Graph<ParserState>::Traversor eScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, entityRouterState);
			ParserStateAutomata.CreateEdge(eScopeEndState, entityRouterState);
		}

		mStateTraversor = ParserStateAutomata.Begin();
	}

	XmlParseMaster::SharedData* SharedDataTable::Clone() const
	{
		return new SharedDataTable();
	}

	void SharedDataTable::ClearStateGraph()
	{
		//ParserStateAutomata.Clear();
	}

	bool SharedDataTable::CheckStateTransition(ParserState expectedState)
	{
		bool hasValidStateTransition = false;

		mStateTraversor.ResetChildrenIterator();
		while (mStateTraversor.HasMoreChildren())
		{
			if (mStateTraversor.GetCurrentChildVertex() == expectedState)
			{
				hasValidStateTransition = true;
				mStateTraversor.TraverseToCurrentChild();
				break;
			}
			mStateTraversor.MoveToNextChild();
		}
		return hasValidStateTransition;
	}

	const Graph<SharedDataTable::ParserState>::Traversor & SharedDataTable::StateTraversor() const
	{
		return mStateTraversor;
	}

	Scope& SharedDataTable::RootScope()
	{
		return *mRootScope;
	}

	void SharedDataTable::SetRootScope(Scope& rootScope)
	{
		mRootScope = &rootScope;
		CurrentScopePtr = mRootScope;
	}

}

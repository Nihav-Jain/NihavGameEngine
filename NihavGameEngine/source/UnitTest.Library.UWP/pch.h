//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <exception>
#include <cstdint>
#include <string>

#include <collection.h>
#include <ppltasks.h>

#include "UnitTestApp.xaml.h"
#include "HeapManager.h"
#include "SharedPtr.h"
#include "UniquePtr.h"

#include "RTTI.h"
#include "Engine.h"
#include "EngineModule.h"

#pragma region Containers

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#pragma endregion

#include "glm\glm.hpp"

#include "Datum.h"
#include "Scope.h"

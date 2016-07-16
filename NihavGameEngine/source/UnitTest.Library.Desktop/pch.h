// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Headers for CppUnitTest
#include "CppUnitTest.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          (P)
#endif

#include <exception>
#include <cstdint>
#include <utility>
#include <string>

#include <iostream>
#include <fstream>
#include <functional>

#include "RTTI.h"
#include "Engine.h"
#include "EngineModule.h"
#include "DesktopMemory.h"
#include "HeapManager.h"
#include "SharedPtr.h"
#include "UniquePtr.h"
#include "glm\glm.hpp"
#include "expat.h"

#pragma region Containers

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#pragma endregion

#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

#include "Foo.h"
#include "FooRTTI.h"
#include "AttributedFoo.h"
#include "AttributedFooChild.h"
#include "AttributedNative.h"
#include "DesktopAudioManager.h"



#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <assert.h>
#include <cstdint>
#include <exception>
#include <string>
#include <functional>

#include "HeapManager.h"
#include "SharedPtr.h"
#include "UniquePtr.h"

#include "RTTI.h"

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#include "Engine.h"
#include "EngineModule.h"

#include "expat.h"
#include "glm\glm.hpp"

#include "Datum.h"
#include "Scope.h"
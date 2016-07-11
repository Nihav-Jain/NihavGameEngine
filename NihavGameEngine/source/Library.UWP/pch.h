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

#include "../Library.Shared/HeapManager.h"
#include "../Library.Shared/SharedPtr.h"
#include "../Library.Shared/UniquePtr.h"

#include "RTTI.h"

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"

#include "Engine.h"
#include "EngineModule.h"
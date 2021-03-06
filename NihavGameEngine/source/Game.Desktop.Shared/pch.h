// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          (P)
#endif

#include <exception>
#include <cstdint>
#include <utility>
#include <string>

#include "RTTI.h"
#include "glm\glm.hpp"

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
#include "GameObject.h"

#include "SpriteManager.h"
#include "ScoreManager.h"
#include "LivesManager.h"
#include "BombManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
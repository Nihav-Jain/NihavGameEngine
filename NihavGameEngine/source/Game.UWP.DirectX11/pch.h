#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <map>
#include <exception>
#include <WICTextureLoader.h>

#include "GameException.h"
#include "glm\glm.hpp"

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


#define DeleteObjects(objects) if((objects) != nullptr) { delete[] objects; objects = nullptr; }
#define ReleaseObject(object) if((object) != nullptr) { object->Release(); object = nullptr; }

#define PLATFORM_XBOX 1
#pragma once

#define WIN32_LEAN_AND_MEAN		 // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "Define.h"

// STL
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <filesystem>
#include <directxtk/SimpleMath.h>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// ImGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

// Libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Engine
#include "Game.h"
#include "Graphics.h"
#include "GameProcessor.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ConstantBuffer.h"

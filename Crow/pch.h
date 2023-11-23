﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#include <comdef.h>

// STL
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
using namespace std;

// ImGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// WIN
#include <windows.h>
#include <assert.h>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <directxtk/SimpleMath.h>
#include <DirectXTex.h>
#include <DirectXTex.inl>
//#include <DirectXTex/DirectXTex.h>
//#include <DirectXTex/DirectXTex.inl>

#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

//#include <DirectXMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "Types.h"
#include "Values.h"
//#include "Structs.h"
#include "Helper.h"

#include "TimeSystem.h"

#endif //PCH_H

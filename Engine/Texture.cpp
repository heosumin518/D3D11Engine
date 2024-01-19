#include "pch.h"
#include "Texture.h"

Engine::Texture::Texture() : Super(ResourceType::Texture)
{
}

Engine::Texture::~Texture()
{
}

void Engine::Texture::Load(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	filesystem::path srcPath(path.c_str());
	wstring strExtension = srcPath.extension();
	transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);

	// LoadFromWICFile을 사용하기전에 CoInitializeEx를 호출하여 COM초기화를 해야합니다.
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
	}// error

	if (strExtension == L".dds")
	{
		CHECK(::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &md, img));
	}
	else if (strExtension == L".tga")
	{
		CHECK(::LoadFromTGAFile(path.c_str(), TGA_FLAGS_NONE, &md, img));
	}
	else if (strExtension == L".hdr")
	{
		CHECK(::LoadFromHDRFile(path.c_str() ,&md, img));
	}
	else
	{
		CHECK(::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img));
	}

	CHECK(::CreateShaderResourceView(DEVICE.Get(), img.GetImages(), img.GetImageCount(), md, m_shaderResourceView.GetAddressOf()));

	CoUninitialize(); // TODO: 좀 더 알아보기.
}

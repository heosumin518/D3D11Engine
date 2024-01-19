#include "Texture.h"
#include "pch.h"
#include "../Engine/Define.h"

Texture::Texture() : Super(ResourceType::Texture)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	if (path.find(L".tga") != wstring::npos)
		CHECK(::LoadFromTGAFile(path.c_str(), TGA_FLAGS_NONE, &md, img));
	else if (path.find(L".dds") != wstring::npos)
		CHECK(::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &md, img));
	else
		CHECK(::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img));

	CHECK(::CreateShaderResourceView())
}

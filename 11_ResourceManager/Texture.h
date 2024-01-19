#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture();
	~Texture();

	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return m_shaderResourceView; }

	void Load(const wstring& path) override;

public:
	Vector2 GetSize() { return m_size; }

private:
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	Vector2 m_size = { 0.f, 0.f };
};


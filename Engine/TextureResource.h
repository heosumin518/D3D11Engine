#pragma once

namespace Engine
{
	class TextureResource
	{
	public:
		TextureResource() {}
		~TextureResource() {}

		void Create(const wstring& filePath);

	private:
		wstring m_filePath;
		ComPtr<ID3D11ShaderResourceView> m_texture;
	};
}



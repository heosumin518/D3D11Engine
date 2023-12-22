#pragma once
#include "ModelLoader.h"

enum TextureType
{
	Diffuse, Normal, Specular, Emissive, Opacity, End,
};

class Material
{
public:
	Material();
	virtual ~Material();

	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	void SetConstantBufferData(CB_UseTextureMap data, ComPtr<ID3D11Buffer> buffer, ComPtr<ID3D11BlendState> alphaBlendState) { m_CBUseTextureMap = data; m_CBUseTextureMapBuffer = buffer; m_alphaBlendState = alphaBlendState; }

public:
	ComPtr<ID3D11ShaderResourceView> GetDiffuseRV() { return m_diffuseRV; }
	ComPtr<ID3D11ShaderResourceView> GetNormalRV() { return m_normalRV; }
	ComPtr<ID3D11ShaderResourceView> GetSpecularRV() { return m_specularRV; }
	ComPtr<ID3D11ShaderResourceView> GetEmissiveRV() { return m_emissiveRV; }
	ComPtr<ID3D11ShaderResourceView> GetOpacityRV() { return m_opacityRV; }

private:
	friend ModelLoader;
	friend Model;

	CB_UseTextureMap m_CBUseTextureMap;
	ComPtr<ID3D11Buffer> m_CBUseTextureMapBuffer;

	ComPtr<ID3D11BlendState> m_alphaBlendState;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// �̹̽ú�� ���ҽ� ��.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// ����� ���ҽ� ��.

	// now use 23/12/22
	string name;
	ComPtr<ID3D11ShaderResourceView> m_textures[TextureType::End];
};


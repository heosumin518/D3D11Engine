#pragma once
#include "ModelLoader.h"

class Material
{
public:
	Material();
	virtual ~Material();

public:
	ComPtr<ID3D11ShaderResourceView> GetDiffuseRV() { return m_diffuseRV; }
	ComPtr<ID3D11ShaderResourceView> GetNormalRV() { return m_normalRV; }
	ComPtr<ID3D11ShaderResourceView> GetSpecularRV() { return m_specularRV; }
	ComPtr<ID3D11ShaderResourceView> GetEmissiveRV() { return m_emissiveRV; }
	ComPtr<ID3D11ShaderResourceView> GetOpacityRV() { return m_opacityRV; }
	ComPtr<ID3D11ShaderResourceView> GetMetalnessRV() { return m_metalnessRV; }
	ComPtr<ID3D11ShaderResourceView> GetRoughnessRV() { return m_roughnessRV; }

private:
	friend ModelLoader;
	friend Model;

	string m_name;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// �̹̽ú�� ���ҽ� ��.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// ����� ���ҽ� ��.	
	ComPtr<ID3D11ShaderResourceView> m_metalnessRV = nullptr;	// ��Ż�� ���ҽ� ��.	
	ComPtr<ID3D11ShaderResourceView> m_roughnessRV = nullptr;	// �����Ͻ��� ���ҽ� ��.	
};


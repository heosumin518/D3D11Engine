#pragma once
#include "GameProcessor.h"

struct Vertex
{
	Vector3 position;	// ���� ��ġ ����
	Vector2 uv;			// �ؽ�ó ��ǥ
	Vector3 normal;
	Vector3 tangent;
};

struct CB_Transform
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

struct CB_DirectionLight
{
	Vector4 direction = { 0.f, 0.f, 1.0f, 1.0f };
	Vector4 ambient = { 0.1f, 0.1f, 0.1f, 0.1f };
	Vector4 diffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 specular = { 0.6f, 0.6f, 0.6f, 0.6f };
	Vector3 eyePos;
	float pad0;
};

struct CB_Material
{
	Vector4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 specular = { 1.f, 1.f, 1.f, 1.f };
	float specularPower = 80.f;
	bool useNormalMap = true;
	bool pad2[3];
	bool useSpecularMap = true;
	bool pad3[3];
	float pad0;
};

static_assert((sizeof(CB_Material) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

class NormalMapping :
	public GameProcessor
{
public:
	NormalMapping(const int32& width, const int32& height, const std::wstring& name);
	virtual ~NormalMapping();

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateGeometry() override;
	void CreateInputLayout() override;
	void CreateConstantBuffer() override;
	void CreateShaderResourceView() override;

	void RenderImGUI() override;

private:
	vector<Vertex> m_vertices;
	vector<WORD> m_indices;

	Matrix m_world;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_view;				// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	// Constant Buffers
	ComPtr<ID3D11Buffer> m_CBTransformBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBMaterialBuffer = nullptr;

	CB_Transform m_CBCube;
	CB_DirectionLight m_CBLight;
	CB_Material m_CBMaterial;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ���ҽ� ��
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;		// specular ���ҽ� ��

	// ImGUI �� ������ camera ���� ���
	Vector3 m_cameraPos = Vector3(0.0f, 0.0f, -6.5f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector3 m_cubeRotateInfo = { 0.f, 0.f, 0.f };
	float m_cubeScale = 1.f;
};


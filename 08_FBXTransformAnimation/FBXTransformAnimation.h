#pragma once
#include "GameProcessor.h"
#include "Model.h"

struct CB_Transform
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

static_assert((sizeof(CB_Transform) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_DirectionLight
{
	Vector4 direction = { 0.f, 0.f, 1.0f, 1.0f };
	Vector4 ambient = { 0.1f, 0.1f, 0.1f, 0.1f };
	Vector4 diffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 specular = { 0.6f, 0.6f, 0.6f, 0.6f };
	Vector3 eyePos;
	float pad0;
};

static_assert((sizeof(CB_DirectionLight) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Material
{
	Vector4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specularPower = 80.f;
	bool useDiffuseMap = true;
	bool pad1[3];
	bool useNormalMap = true;
	bool pad2[3];
	bool useSpecularMap = true;
	bool pad3[3];
	bool useEmissiveMap = true;
	bool pad4[3];
	bool useOpacityMap = true;
	bool pad5[3];
	Vector2 pad6;
};

static_assert((sizeof(CB_Material) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

class FBXTransformAnimation :
	public GameProcessor
{
public:
	FBXTransformAnimation(const int32& width, const int32& height, const std::wstring& name);
	virtual ~FBXTransformAnimation();

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateInputLayout() override;
	void CreateConstantBuffer() override;

	void RenderImGUI() override;

private:
	Matrix m_world = Matrix::Identity;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_view = Matrix::Identity;				// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_projection = Matrix::Identity;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.
	// -> ���� world �� ������Ʈ���� ������ �ϰ� view �� projection �� ī�޶� �������� �Ѵ�.

	// Constant Buffers
	ComPtr<ID3D11Buffer> m_CBTransformBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBMaterialBuffer = nullptr;

	CB_Transform m_CBModel;
	CB_DirectionLight m_CBLight;
	CB_Material m_CBMaterial;

	// FBX
	vector<shared_ptr<Model>> m_models;		// ������ �ִ� �𵨵�


	// ImGUI �� ������ camera ���� ���
	Vector3 m_cameraPos = Vector3(0.0f, 300.0f, -1000.0f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector2 m_rotation = Vector2(0.0f, 0.0f);
	float m_modelScale = 3.f;
};


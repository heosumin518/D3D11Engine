#pragma once
#include "GameProcessor.h"
#include "Material.h"
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
	Vector3 direction = { 0.5f, 0.0f, 0.8f };
	float pad0 = 0.f;
	Vector4 ambient = { 0.1f, 0.1f, 0.1f, 0.1f };
	Vector4 diffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 specular = { 1.f, 1.f, 1.f, 1.f };
	Vector3 eyePos = {};
	float pad1 = 0.f;
};

static_assert((sizeof(CB_DirectionLight) % 16) == 0,
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

public:
	void InitMatrix();

private:
	Matrix m_world = Matrix::Identity;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_view = Matrix::Identity;				// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix m_projection = Matrix::Identity;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	// -> 이후 world 는 오브젝트마다 가지게 하고 view 와 projection 은 카메라가 가지도록 한다.

	// Constant Buffers
	ComPtr<ID3D11Buffer> m_CBTransformBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBMaterialBuffer = nullptr;

	CB_Transform m_CBModel;
	CB_DirectionLight m_CBLight;
	CB_Material m_CBMaterial;

	// FBX Model
	Model m_model;


	// ImGUI 로 조절할 camera 변수 목록
	Vector3 m_cameraPos = Vector3(0.0f, 300.0f, -1000.0f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector2 m_rotation = Vector2(0.0f, 0.0f);
	float m_modelScale = 3.f;
};


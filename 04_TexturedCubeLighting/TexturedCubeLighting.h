#pragma once
#include "GameProcessor.h"

struct Vertex
{
	Vector3 position;	// 정점 위치 정보
	Vector2 uv;			// 텍스처 좌표
	Vector3 normal;
};

struct CBTransformData
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

struct CBLightData
{
	Vector4 lightDir;
	Vector4 lightColor;
};

class TexturedCubeLighting :
	public GameProcessor
{
public:
	TexturedCubeLighting(const int32& width, const int32& height, const std::wstring& name);
	virtual ~TexturedCubeLighting();

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

	Matrix m_world;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_view;				// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix m_projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	Vector4 m_lightColor = { 1.f, 1.f, 1.f, 1.0f };
	Vector4 m_lightDir = { 1.f, -1.f, 1.f, 1.0f };

	ComPtr<ID3D11Buffer> m_CBTransformBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;

	CBTransformData m_CBCubeData;
	CBLightData m_CBLight;

	// ImGUI 로 조절할 camera 변수 목록
	float m_cameraPos[3] = { 0.f, 1.8, -4.f };
	float m_cameraFOV = 100.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 100.0f;
	Vector3 m_cubeRotateInfo = {0.f, 0.f, 0.f};
};


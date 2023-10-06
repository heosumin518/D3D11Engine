#pragma once
#include "GameProcessor.h"

class TexturedCubeLighting :
	public GameProcessor
{
public:
	TexturedCubeLighting(const int32& width, const int32& height, const std::wstring& name);
	virtual ~TexturedCubeLighting();

	void Initialize() override;
	void Update() override;
	void Render() override;

	void RenderImGUI() override;

	void CreateGeometry() override;

	void CreateVertexShader() override;
	void CreatePixelShader() override;

private:
	ComPtr<ID3D11PixelShader> m_pixelShaderSolid = nullptr;		// �ȼ� ���̴� ����Ʈ ǥ�ÿ�.

	Matrix                m_world;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix                m_view;				// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix                m_projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	Vector4 m_lightColor = { 1.f, 1.f, 1.f, 1.0f };
	Vector4 m_lightDir = { 1.f, -1.f, 1.f, 1.0f };

	TransformData m_transformData;

	// ImGUI �� ������ camera ���� ���
	float m_cameraPos[3] = { 0.f, 1.8, -4.f };
	float m_cameraFOV = 100.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 100.0f;
	Vector3 m_cubeRotateInfo = {0.f, 0.f, 0.f};
};


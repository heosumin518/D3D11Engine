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

	void CreateGeometry() override;

	void CreateVertexShader() override;
	void CreatePixelShader() override;

private:
	ComPtr<ID3D11PixelShader> m_pixelShaderSolid = nullptr;		// �ȼ� ���̴� ����Ʈ ǥ�ÿ�.

	Matrix                m_world;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix                m_view;				// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix                m_projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	XMFLOAT4 m_lightColors[2] =
	{
		XMFLOAT4(1.f, 1.f, 1.f, 1.0f),
		XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f)
	};
	XMFLOAT4 m_initialLightDirs[2] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 m_lightDirsEvaluated[2] = {};		// ���� ����Ʈ ����

	// ť��� ����Ʈ�� ����
	TransformData m_cubeTransformData;
	TransformData m_lightTransformData;

	// ImGUI �� ������ ���� ���
	float m_cameraPos[3] = { 0.f, 4.0, -12.f };
	float m_cameraFOV = 90.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 100.0f;
};


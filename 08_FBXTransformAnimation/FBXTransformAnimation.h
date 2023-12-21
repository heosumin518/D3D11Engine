#pragma once
#include "GameProcessor.h"
#include "Model.h"


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
	

	// FBX
	vector<shared_ptr<Model>> m_models;		// ������ �ִ� �𵨵�


	// ImGUI �� ������ camera ���� ���
	Vector4 m_cameraPos = Vector4(0.0f, 300.0f, -1000.0f, 0.f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector2 m_rotation = Vector2(0.0f, 0.0f);
	float m_modelScale = 3.f;


	/// new
	CB_Coordinate m_CBCoordinate;
	ComPtr<ID3D11Buffer> m_CBCoordinateBuffer;

	ComPtr<ID3D11Buffer> m_CBModelTransformBuffer;

	CB_UseTextureMap m_CBUseTextureMap;
	ComPtr<ID3D11Buffer> m_CBUseTextureMapBuffer;

	CB_Camera m_CBCamera;
	ComPtr<ID3D11Buffer> m_CBCameraBuffer;

	CB_Light m_CBLight;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;

};


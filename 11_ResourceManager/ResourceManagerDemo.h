#pragma once
#include "GameProcessor.h"
#include "Model.h"



class ResourceManagerDemo :
	public GameProcessor
{
public:
	ResourceManagerDemo(const int32& width, const int32& height, const std::wstring& name);
	virtual ~ResourceManagerDemo();

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
	ComPtr<ID3D11Buffer> m_CBMatPaletteBuffer = nullptr;

	CB_Transform m_CBModel;
	CB_DirectionLight m_CBLight;
	CB_Material m_CBMaterial;
	CB_MatrixPalette m_CBMatPalette;

	// FBX
	shared_ptr<Model> m_model;		// ������ �ִ� ��


	// ImGUI �� ������ camera ���� ���
	Vector3 m_cameraPos = Vector3(50.0f, 0.0f, -100.0f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector3 m_rotation = Vector3(0.0f, 0.0f, 82.0f);
	float m_modelScale = 1.f;
};

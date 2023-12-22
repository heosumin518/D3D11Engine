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

	/// new
	CB_Transform m_cbTransform;
	ComPtr<ID3D11Buffer> m_pTransformBuffer;

	CB_Light m_cbLight;
	ComPtr<ID3D11Buffer> m_pLightBuffer;

	CB_Material m_cbMaterial;
	ComPtr<ID3D11Buffer> m_pMaterialBuffer;

	CB_UseTextureMap m_cbUseTextureMap;
	ComPtr<ID3D11Buffer> m_pUseTextureMapBuffer;

	CameraInfo m_camera;
};


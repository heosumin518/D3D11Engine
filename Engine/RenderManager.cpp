#include "pch.h"
#include "RenderManager.h"

void Engine::RenderManager::Init()
{
	// Create Constant Buffer
	{
		m_cameraBuffer = make_shared<ConstantBuffer<CameraDesc>>();
		m_cameraBuffer->Create();
		m_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();
		m_transformBuffer->Create();
		m_lightBuffer = make_shared<ConstantBuffer<LightDesc>>();
		m_lightBuffer->Create();
		m_materialBuffer = make_shared<ConstantBuffer<MaterialDesc>>();
		m_materialBuffer->Create();
		m_matrixPaletteBuffer = make_shared<ConstantBuffer<MatrixPaletteDesc>>();
		m_matrixPaletteBuffer->Create();

		// ���ؽ� ���̴� ����
		DC->VSSetConstantBuffers(0, 1, m_transformBuffer->GetComPtr().GetAddressOf());
		DC->VSSetConstantBuffers(1, 1, m_cameraBuffer->GetComPtr().GetAddressOf());
		DC->VSSetConstantBuffers(2, 1, m_lightBuffer->GetComPtr().GetAddressOf());

		DC->VSSetConstantBuffers(4, 1, m_matrixPaletteBuffer->GetComPtr().GetAddressOf());

		// �ȼ� ���̴� ����
		DC->PSSetConstantBuffers(0, 1, m_transformBuffer->GetComPtr().GetAddressOf());
		DC->PSSetConstantBuffers(1, 1, m_cameraBuffer->GetComPtr().GetAddressOf());
		DC->PSSetConstantBuffers(2, 1, m_lightBuffer->GetComPtr().GetAddressOf());
		DC->PSSetConstantBuffers(3, 1, m_materialBuffer->GetComPtr().GetAddressOf());
	}

	// Create Sampler State
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		//desc.BorderColor[0] = 1;	// Address �� BORDER �� ������ ������ ��Ÿ��.
		//desc.BorderColor[1] = 0;
		//desc.BorderColor[2] = 0;
		//desc.BorderColor[3] = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// TODO : D3D11_COMPARISON_NEVER �ʹ� ��� �ٸ���.
		//desc.MaxAnisotropy = 16;	// �̰� ����.
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		desc.MipLODBias = 0.0f;
		CHECK(DEVICE->CreateSamplerState(&desc, m_samplerState.GetAddressOf()));
	}

	// Create Blend State
	// ���ĺ����� ���� ���� ���� ����
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;

		D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
		rtBlendDesc.BlendEnable = true; // ���� ��� ����
		// FinalRGB = SrcRGB *SrcBlend + DestRGB*DestBlend
		rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;	// SrcBlend�� SrcColor�� ���İ�
		rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// DestBlend�� (1-SourceColor.a)
		// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
		rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;	// SrcBlendAlpha = 1
		rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;	// DestBlendAlpha = 1	
		rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // ����Ÿ�ٿ� RGBA ��� Write
		blendDesc.RenderTarget[0] = rtBlendDesc;

		CHECK(DEVICE->CreateBlendState(&blendDesc, m_blendState.GetAddressOf()));
	}

	// Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = true;	// TODO : false �� �θ�.. ��� �Ǵ��� �˾ƺ���
		desc.DepthClipEnable = true;
		desc.AntialiasedLineEnable = true;

		CHECK(DEVICE->CreateRasterizerState(&desc, m_rasterizerState.GetAddressOf()));
	}

	// Initialize the world matrix
	{
		XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -1000.0f, 0.0f);
		XMVECTOR at = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_view = XMMatrixLookAtLH(eye, at, up);
		m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, GAME->GetGameDesc().width / GAME->GetGameDesc().height, 1.0f, 10000.0f);
	}

	// TODO : temp model load
	// fbx ���� �ε��Ͽ� �� ����
	ModelLoader loader(DEVICE);
	m_model = loader.LoadModelFile("../Resources/cerberus2.fbx"); // GOSEGU

	// TODO : TEMP ps vs layout
	{
		CreateInputLayout();
		CreatePixelShader();
		CreateVeretexShader();
	}
}

void Engine::RenderManager::Update()
{
	// camera update
	{
		XMVECTOR eye = XMVectorSet(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z, 0.f);
		XMVECTOR at = XMVectorSet(m_cameraPos.x, m_cameraPos.y + 1.f, 100.f, 0.f);
		XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		m_view = XMMatrixLookAtLH(eye, at, up);
		m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, GAME->GetGameDesc().width / GAME->GetGameDesc().height, 1.0f, 10000.0f);
	}

	// light update
	{
		m_lightData.eyePos = m_cameraPos;
	}

	// model update
	{
		m_model->Update(DT);
	}

	// constant Buffer update
	{
		m_transformBuffer->CopyData(m_transformData);
		m_cameraBuffer->CopyData(m_cameraData);
		m_lightBuffer->CopyData(m_lightData);
		m_materialBuffer->CopyData(m_materialData);
	}
}

void Engine::RenderManager::Render()
{
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//DC->RSSetState(m_rasterizerState.Get());
	DC->RSSetState(nullptr);

	// render model
	for (size_t i = 0; i < m_model->GetMeshes().size(); i++)
	{
		size_t mi = m_model->GetMeshes()[i]->GetMaterialIndex();

		DC->PSSetShaderResources(0, 1, m_model->GetMaterials()[mi]->GetDiffuseRV().GetAddressOf());
		DC->PSSetShaderResources(1, 1, m_model->GetMaterials()[mi]->GetNormalRV().GetAddressOf());
		DC->PSSetShaderResources(2, 1, m_model->GetMaterials()[mi]->GetSpecularRV().GetAddressOf());
		DC->PSSetShaderResources(3, 1, m_model->GetMaterials()[mi]->GetEmissiveRV().GetAddressOf());
		DC->PSSetShaderResources(4, 1, m_model->GetMaterials()[mi]->GetOpacityRV().GetAddressOf());
		DC->PSSetShaderResources(5, 1, m_model->GetMaterials()[mi]->GetMetalnessRV().GetAddressOf());
		DC->PSSetShaderResources(6, 1, m_model->GetMaterials()[mi]->GetRoughnessRV().GetAddressOf());

		m_materialData.isUseBaseColorMap = m_model->GetMaterials()[mi]->GetDiffuseRV() != nullptr ? true : false;
		m_materialData.isUseNormalMap = m_model->GetMaterials()[mi]->GetNormalRV() != nullptr ? true : false;
		m_materialData.isUseSpecularMap = m_model->GetMaterials()[mi]->GetSpecularRV() != nullptr ? true : false;
		m_materialData.isUseEmissiveMap = m_model->GetMaterials()[mi]->GetEmissiveRV() != nullptr ? true : false;
		m_materialData.isUseOpacityMap = m_model->GetMaterials()[mi]->GetOpacityRV() != nullptr ? true : false;
		m_materialData.isUseMetalnessMap = m_model->GetMaterials()[mi]->GetMetalnessRV() != nullptr ? true : false;
		m_materialData.isUseRoughnessMap = m_model->GetMaterials()[mi]->GetRoughnessRV() != nullptr ? true : false;

		if (m_materialData.isUseOpacityMap)
			DC->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);  // ���ĺ��� ���¼��� , �ٸ��ɼ��� �⺻��
		else
			DC->OMSetBlendState(nullptr, nullptr, 0xffffffff);		// ��������, �ٸ��ɼ��� �⺻��

		m_transformData.world = m_model->GetMeshes()[i]->GetNodeTransform().Transpose() * XMMatrixTranspose(m_world);

		m_model->GetMeshes()[i]->UpdateMatrixPalette(m_model->GetBones(), m_matrixPaletteData.array);

		DC->UpdateSubresource(m_matrixPaletteBuffer->GetComPtr().Get(), 0, nullptr, &m_matrixPaletteData, 0, 0);
		DC->UpdateSubresource(m_transformBuffer->GetComPtr().Get(), 0, nullptr, &m_transformData, 0, 0);
		DC->UpdateSubresource(m_materialBuffer->GetComPtr().Get(), 0, nullptr, &m_materialData, 0, 0);

		DC->IASetIndexBuffer(m_model->GetMeshes()[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		DC->IASetVertexBuffers(
			0, 1,
			m_model->GetMeshes()[i]->GetVertexBuffer().GetAddressOf(),
			m_model->GetMeshes()[i]->GetVertexBufferStride(),
			m_model->GetMeshes()[i]->GetVertexBufferOffset()
		);
		DC->DrawIndexed(m_model->GetMeshes()[i]->GetIndexCount(), 0, 0);
	}
}

void Engine::RenderManager::CreateVeretexShader()
{
	CHECK(CompileShaderFromFile(L"VertexShader.hlsl", "vs_main", "vs_5_0", m_vsBlob));
	CHECK(DEVICE->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
}

void Engine::RenderManager::CreatePixelShader()
{
	CHECK(CompileShaderFromFile(L"PixelShader.hlsl", "ps_main", "ps_5_0", m_psBlob));
	CHECK(DEVICE->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}

void Engine::RenderManager::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	// AlignedByteOffset ���� D3D11_APPEND_ALIGNED_ELEMENT �� �����ϸ� ���ۿ� �����Ͱ� ��� �迭�Ǵ����� �ڵ����� �˾Ƴ����� �� �� �ִ�.

	const int32_t count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	CHECK(DEVICE->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

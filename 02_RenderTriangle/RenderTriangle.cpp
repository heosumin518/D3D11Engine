#include "pch.h"
#include "RenderTriangle.h"

RenderTriangle::RenderTriangle(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

void RenderTriangle::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();

	CreateGeometry();
	GameProcessor::CreateVertexShader();
	CreateInputLayout();
	GameProcessor::CreatePixelShader();
}

void RenderTriangle::Update()
{
	GameProcessor::Update();
}

void RenderTriangle::Render()
{
	RenderBegin();

	// TO DO
	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		// IA
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderEnd();
}

void RenderTriangle::CreateGeometry()
{
	// ������ VertexShader�� World, View, Projection ��ȯ�� ������� �����Ƿ� 
	// ���� Normalized Device Coordinate(��ǥ��)�� ��ġ�� �����Ѵ�.
	//      /---------------------(1,1,1)   z���� ���̰�
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       �߾��� (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)


	// VertexData
	{
		// m_vertices.resize(3);
		m_vertices.resize(4);		// �簢��

		m_vertices[0].position = Vector3(-0.5f, -0.5f, 0.f);
		m_vertices[0].color = Color(1.0f, 0.f, 0.f, 1.f);

		m_vertices[1].position = Vector3(-0.5f, 0.5f, 0.f);
		m_vertices[1].color = Color(0.f, 1.0f, 0.f, 1.f);

		m_vertices[2].position = Vector3(0.5f, -0.5f, 0.f);
		m_vertices[2].color = Color(0.f, 1.f, 1.0f, 1.f);

		m_vertices[3].position = Vector3(0.5f, 0.5f, 0.f);
		m_vertices[3].color = Color(0.f, 0.f, 1.0f, 1.f);
	}

	// IndexData
	{
		//m_indices = { 0, 1, 3 };	// �ﰢ��
		m_indices = { 0, 1, 2, 2, 1, 3 };	// �簢��
	}


	// VertexBuffer
	{
		// ���� ���� ���� ����
		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;		// GPU���� read �� ������ �����ͷ� ����
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// vertex buffer�� ����ϴµ� �� ���̶�� �� �˷��ֱ�.
		vbDesc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * m_vertices.size());

		// ���� ���� ����
		D3D11_SUBRESOURCE_DATA vbData;
		ZeroMemory(&vbData, sizeof(vbData));
		vbData.pSysMem = m_vertices.data();
		HR_T(m_device->CreateBuffer(&vbDesc, &vbData, m_vertexBuffer.GetAddressOf()));
	}

	// IndexBuffer
	{
		// �ε��� ���� ���� ����
		D3D11_BUFFER_DESC ibDesc;
		ZeroMemory(&ibDesc, sizeof(ibDesc));
		ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.ByteWidth = static_cast<uint32>(sizeof(WORD) * m_indices.size());
		ibDesc.CPUAccessFlags = 0;

		// �ε��� ���� ����
		D3D11_SUBRESOURCE_DATA ibData;
		ZeroMemory(&ibData, sizeof(ibData));
		ibData.pSysMem = m_indices.data();		// �迭 ������ �Ҵ�.
		HR_T(m_device->CreateBuffer(&ibDesc, &ibData, m_indexBuffer.GetAddressOf()));
	}
}

void RenderTriangle::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

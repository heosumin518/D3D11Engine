#include "pch.h"
#include "RenderTriangle.h"

RenderTriangle::RenderTriangle(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

void RenderTriangle::Initialize()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();
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
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// �簢���� strip����

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		m_deviceContext->Draw(m_vertices.size(), 0);
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

		//// �ﰢ��
		//{
		//	m_vertices[0].position = Vector3(-0.5f, -0.5f, 0.f);
		//	m_vertices[0].color = Color(1.0f, 0.f, 0.f, 1.f);

		//	m_vertices[1].position = Vector3(0.f, 0.5f, 0.f);
		//	m_vertices[1].color = Color(0.f, 1.0f, 0.f, 1.f);

		//	m_vertices[2].position = Vector3(0.5f, -0.5f, 0.f);
		//	m_vertices[2].color = Color(0.f, 0.f, 1.0f, 1.f);
		//}

		//�簢��
		{
			m_vertices[0].position = Vector3(-0.5f, 0.5f, 0.f);
			m_vertices[0].color = Color(1.0f, 0.f, 0.f, 1.f);

			m_vertices[1].position = Vector3(0.5f, 0.5f, 0.f);
			m_vertices[1].color = Color(0.f, 1.0f, 0.f, 1.f);

			m_vertices[2].position = Vector3(-0.5f, -0.5f, 0.f);
			m_vertices[2].color = Color(0.f, 1.f, 1.0f, 1.f);

			m_vertices[3].position = Vector3(0.5f, -0.5f, 0.f);
			m_vertices[3].color = Color(0.f, 0.f, 1.0f, 1.f);
		}
	}


	// VertexBuffer
	GameProcessor::CreateGeometry();
}

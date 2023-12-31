#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<Vertex>& vertices)
{
	m_vertexBufferStride = sizeof(Vertex);
	m_vertexBufferOffset = 0;
	m_vertexCount = vertices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = m_vertexBufferStride * vertices.size();
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertices.data();
	HR_T(device->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf()));
}

void Mesh::CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<WORD>& indices)
{
	m_indexCount = indices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = sizeof(WORD) * m_indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();
	HR_T(device->CreateBuffer(&desc, &data, m_indexBuffer.GetAddressOf()));
}

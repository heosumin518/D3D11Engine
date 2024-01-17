#pragma once
#include "ModelLoader.h"
#include "Node.h"
#include "Struct.h"


// TODO: Mesh �� ���� �߻� Ŭ������ ������. Static �� Skeletal �� �и��ؼ� ����.
class Mesh
{
public:
	Mesh();
	~Mesh();

	template <typename T>
	void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<T>& vertices);

	void CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<UINT>& indices);

public:
	UINT GetIndexCount() const { return m_indexCount; }
	UINT GetMaterialIndex() const { return m_materialIndex; }
	const UINT* GetVertexBufferStride() { return &m_vertexBufferStride; }
	const UINT* GetVertexBufferOffset() { return &m_vertexBufferOffset; }

	ComPtr<ID3D11Buffer> GetIndexBuffer() { return m_indexBuffer; }
	ComPtr<ID3D11Buffer> GetVertexBuffer() { return m_vertexBuffer; }

	void UpdateMatrixPalette(vector<shared_ptr<Bone>> bones, Matrix* pMatrixPalette);

	Matrix GetNodeTransform() { return m_connectedNode->m_world; }

private:
	friend ModelLoader;
	friend Model;

	vector<BoneWeightVertex> m_boneWeightVertices;	// TODO : static �� skeletal ���� �ٸ��� ������ �ֵ��� �ϱ�.

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	string m_name;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;	// ���ؽ� �ϳ��� ũ��
	UINT m_vertexBufferOffset = 0;	// ���ؽ� ������ ������
	UINT m_indexCount = 0;			// �ε��� ����
	UINT m_materialIndex = 0;		// ���͸��� �ε���

	// ..
	shared_ptr<Node> m_connectedNode;
};

template <typename T>
void Mesh::CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<T>& vertices)
{
	m_vertexBufferStride = sizeof(T);
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

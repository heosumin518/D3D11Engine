#pragma once

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

static aiMesh;
class Mesh
{
public:
	Mesh(ComPtr<ID3D11Device> device);
	~Mesh();

	void CreateVertexBuffer(Vertex* vertices, UINT vertexCount);
	void CreateIndexBuffer(WORD* indices, UINT indexCount);
	void Create(aiMesh* mesh);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;	// ���ؽ� �ϳ��� ũ��
	UINT m_vertexBufferOffset = 0;	// ���ؽ� ������ ������
	UINT m_indexCount = 0;	// �ε��� ����
	UINT m_indexBufferStride = 0;
	UINT m_indexBufferOffset = 0;
	UINT m_materialIndex = 0;	// ���͸��� �ε���

};


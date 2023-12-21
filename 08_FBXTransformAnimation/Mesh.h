#pragma once
#include "ModelLoader.h"

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

class Model;
class Mesh
{
public:
	Mesh();
	~Mesh();

	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

	void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<Vertex>& vertices);
	void CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<WORD>& indices);

public:
	UINT GetIndexCount() const { return m_indexCount; }
	UINT GetMaterialIndex() const { return m_materialIndex; }
	const UINT* GetVertexBufferStride() { return &m_vertexBufferStride; }
	const UINT* GetVertexBufferOffset() { return &m_vertexBufferOffset; }

	ComPtr<ID3D11Buffer> GetIndexBuffer() { return m_indexBuffer; }
	ComPtr<ID3D11Buffer> GetVertexBuffer() { return m_vertexBuffer; }

	void SetMaterial(shared_ptr<Material> material) { m_material = material; }

private:
	friend ModelLoader;
	friend Model;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	shared_ptr<Model> m_owner;		// �޽����� ���� �θ�� ������ �ֵ��� �Ͽ� ���͸��� ������ ������ ���͸����� Render �Ѵ�.

	string m_name;

	shared_ptr<Material> m_material;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;	// ���ؽ� �ϳ��� ũ��
	UINT m_vertexBufferOffset = 0;	// ���ؽ� ������ ������
	UINT m_indexCount = 0;			// �ε��� ����
	UINT m_materialIndex = 0;		// ���͸��� �ε���

};


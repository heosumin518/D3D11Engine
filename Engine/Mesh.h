#pragma once
#include "ModelLoader.h"
#include "Node.h"

// static mesh
struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

// skeletal mesh
struct BoneWeightVertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;

	// �����ϴ� ���� �ε����� ������ �ִ� 128���� �ϴ� ó��.
	int blendIndices[4] = {};	// ����޴� ���� ���� �ִ� 4���� ����.

	// ���ؽ����� �����ϴ� ���� weight ��.
	// �ִ� 4���� �����ϴ� weight ���� 4������ �ְ� �ȴ�.
	float blendWeights[4] = {}; // ����ġ�� ������ 1�� �Ǿ�� �Ѵ�.

	void AddBoneData(int boneIndex, float weight)
	{
		// ��� �ϳ��� �����Ͱ� ����־�� �Ѵ�.
		assert(blendWeights[0] == 0.f || blendWeights[1] == 0.f ||
			blendWeights[2] == 0.f || blendWeights[3] == 0.f);

		for (UINT i = 0; i < 4; i++)
		{
			if (blendWeights[i] == 0.f)
			{
				blendIndices[i] = boneIndex;
				blendWeights[i] = weight;
				return;
			}
		}
	}
};

namespace Engine
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<BoneWeightVertex>& vertices);
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

		vector<BoneWeightVertex> m_boneWeightVertices;

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
}



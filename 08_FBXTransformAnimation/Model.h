#pragma once
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "ModelLoader.h"

/// Bone, Mesh, Material�� ������ �ִ�.

class Model
{
public:
	Model();
	~Model();

public:
	void Update(float deltaTime);
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	vector<shared_ptr<Mesh>> GetMeshes() { return m_meshes; }
	vector<shared_ptr<Material>> GetMaterials() { return m_materials; }
	Matrix GetTransform() { return m_transform; }
private:
	friend ModelLoader;

	shared_ptr<Node> m_root;	// ��Ʈ ���
	vector<shared_ptr<Node>> m_nodes;	// ���������� parentIndex�� ����.
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	vector<shared_ptr<Animation>> m_animations;

	Vector3 m_position = { 0, 0, 0 };
	Vector3 m_scale = { 1, 1, 1 };
	Vector3 m_rotation = { 0, 0, 0 };
	Matrix m_transform = {};

};


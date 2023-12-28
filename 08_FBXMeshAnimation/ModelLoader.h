#pragma once
#include <filesystem>
// FBX ���Ϸκ��� ������ �ε��Ͽ� Model ��ü�� �Ѱ��ִ� ������ �ϴ� Ŭ����

class Node;
class Model;
class Mesh;
class Material;
class Animation;

class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> m_device);
	~ModelLoader();
public:
	shared_ptr<Model> LoadModelFile(const string& file);
	void CreateNode(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> parent);
	void CreateMesh(aiNode* node, shared_ptr<Node> connectNode);
	void CreateMaterial();
	void CreateAnimation(aiAnimation* srcAnim);

private:
	ComPtr<ID3D11Device> m_device;

private:
	shared_ptr<Assimp::Importer> m_importer;
	const aiScene* m_scene = nullptr;

private:
	vector<shared_ptr<Node>> m_nodes;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	shared_ptr<Animation> m_animation;
};


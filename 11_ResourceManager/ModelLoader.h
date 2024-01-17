#pragma once
// FBX ���Ϸκ��� ������ �ε��Ͽ� Model ��ü�� �Ѱ��ִ� ������ �ϴ� Ŭ����
// ���ҽ� �Ŵ����� ���ؼ��� ����� �Ǹ� FBX �� �����͸� �ҷ��鿩 ���ҽ� �Ŵ����� ����Ѵ�.

class Mesh;
class StaticMesh;
class SkeletalMesh;
class Node;
class Bone;
class Model;
class Material;
class Animation;
class NodeAnimation;

class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> m_device);
	~ModelLoader();
public:
	shared_ptr<Model> LoadModelFile(const string& file);
	void CreateNode(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> parent);

	void CreateMesh(aiNode* node, shared_ptr<Node> connectNode);	// TODO: �׳� Mesh ���ְ� �ؿ� �ΰ��� ��ü
	shared_ptr<StaticMesh> CreateStaticMesh(aiNode* node, shared_ptr<Node> connectNode);
	shared_ptr<SkeletalMesh> CreateSkeletalMesh(aiNode* node, shared_ptr<Node> connectNode);

	void CreateMaterial();
	void CreateAnimation(aiAnimation* srcAnim);
	shared_ptr<NodeAnimation> ParseAnimationNode(shared_ptr<Animation> animation, aiNodeAnim* srcNodeAnim);

private:
	ComPtr<ID3D11Device> m_device;

private:
	shared_ptr<Assimp::Importer> m_importer;
	const aiScene* m_scene = nullptr;

private:
	// TODO: �Ʒ� �������� ��� ���ҽ� �Ŵ����� ������ �ֵ��� �ϱ�. �׳� ������ �־ �ɰ� ���⵵ �ϰ�..? ��
	vector<shared_ptr<Node>> m_nodes;
	vector<shared_ptr<Bone>> m_bones;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	shared_ptr<Animation> m_animation;
};


#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "ModelLoader.h"

Model::Model()
{
	m_animation = make_shared<Animation>();
	m_rootNode = make_shared<Node>();
}

Model::~Model()
{
	
}

//void Model::ReadFile(ComPtr<ID3D11Device> device, const char* filePath)
//{
//	Assimp::Importer importer;
//
//	m_scene = importer.ReadFile(filePath,
//		aiProcess_ConvertToLeftHanded |	// DX�� �޼���ǥ�� ��ȯ
//		aiProcess_Triangulate | // vertex �ﰢ�� ���� ���
//		aiProcess_GenUVCoords |	// �ؽ�ó ��ǥ ����
//		aiProcess_GenNormals |	// Normal ���� ����
//		aiProcess_CalcTangentSpace // ź��Ʈ ���� ����
//	);
//	assert(m_scene != nullptr);
//
//	m_meshes.resize(m_scene->mNumMeshes);
//	m_materials.resize(m_scene->mNumMaterials);
//
//	// fbx ���� �ؽ��ĸ� ���� ��������
//	for (UINT i = 0; i < m_scene->mNumMaterials; i++)
//	{
//		m_materials[i] = make_shared<Material>();
//		m_materials[i]->Create(device, m_scene->mMaterials[i]);
//	}
//
//	// �� �ִϸ��̼� ���Ϳ� �ִϸ��̼� Ŭ�� �����ϰ� �� �ȿ��� ������ ���ε�
//	for (UINT i = 0; i < m_scene->mNumAnimations; i++)
//	{
//		if (m_scene->mAnimations[i] != nullptr)
//			m_animation->Create(m_scene->mAnimations[i]);
//	}
//
//	// ��忡 �ִϸ��̼� ���ε�
//	m_rootNode->Create(shared_from_this(), m_scene->mRootNode, m_animation);
//
//	// vertex, index ���� ���ε�
//	for (UINT i = 0; i < m_scene->mNumMeshes; i++)
//	{
//		m_meshes[i] = make_shared<Mesh>();
//		m_meshes[i]->Create(device, m_scene->mMeshes[i]);
//	}
//}

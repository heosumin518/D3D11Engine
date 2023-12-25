#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"

Model::Model()
{
	
}

Model::~Model()
{
	
}

bool Model::ReadFile(ComPtr<ID3D11Device> device, const char* filePath)
{
	Assimp::Importer importer;

	m_scene = importer.ReadFile(filePath,
		aiProcess_ConvertToLeftHanded |	// DX�� �޼���ǥ�� ��ȯ
		aiProcess_Triangulate | // vertex �ﰢ�� ���� ���
		aiProcess_GenUVCoords |	// �ؽ�ó ��ǥ ����
		aiProcess_GenNormals |	// Normal ���� ����
		aiProcess_CalcTangentSpace // ź��Ʈ ���� ����
	);
	assert(m_scene != nullptr);

	m_meshes.resize(m_scene->mNumMeshes);
	m_materials.resize(m_scene->mNumMaterials);

	for (UINT i = 0; i < m_scene->mNumMaterials; i++)
		m_materials[i]->Create(device, m_scene->mMaterials[i]);



}

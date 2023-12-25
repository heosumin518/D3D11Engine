#include "pch.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Mesh.h"
#include "Node.h"
#include "Material.h"
#include "Animation.h"

ModelLoader::ModelLoader(ComPtr<ID3D11Device> device)
	: m_device(device)
{
	m_importer = make_shared<Assimp::Importer>();
}

ModelLoader::~ModelLoader()
{
}

shared_ptr<Model> ModelLoader::LoadModelFile(const string& file)
{
	shared_ptr<Model> model = make_shared<Model>();

	m_scene = m_importer->ReadFile(
		file.c_str(),
		aiProcess_ConvertToLeftHanded |	// DX�� �޼���ǥ�� ��ȯ
		aiProcess_Triangulate | // vertex �ﰢ�� ���� ���
		aiProcess_GenUVCoords |	// �ؽ�ó ��ǥ ����
		aiProcess_GenNormals |	// Normal ���� ����
		aiProcess_CalcTangentSpace | // ź��Ʈ ���� ����
		aiProcess_FlipWindingOrder
	);
	assert(m_scene != nullptr);

	if (m_scene->mMaterials != nullptr)
		CreateMaterial();

	model->m_materials = m_materials;

	m_root = CreateNode(m_scene->mRootNode, nullptr, model);

	model->m_root = m_root;
	model->m_nodes = m_nodes;
	model->m_meshes = m_meshes;

	if (m_scene->mAnimations != nullptr)
		ReadAnimationData(m_scene->mAnimations[0], model);

	model->m_animations = m_animations;

	// TODO : ���� ����ְ� �ٽ� �� �ε��Ҷ� ������ ��������
	m_root = nullptr;
	m_nodes.clear();
	m_meshes.clear();
	m_materials.clear();
	m_animations.clear();

	m_importer->FreeScene();

	return model;
}

void ModelLoader::ReadAnimationData(aiAnimation* srcAnimation, shared_ptr<Model> owner)
{
	if (!m_scene->HasAnimations())
		return;

	shared_ptr<Animation> animation = make_shared<Animation>();
	animation->m_name = srcAnimation->mName.C_Str();

	for (UINT c = 0; c < srcAnimation->mNumChannels; c++)
	{
		aiNodeAnim* srcNodeAnim = srcAnimation->mChannels[c];

		for (UINT p = 0; p < srcNodeAnim->mNumPositionKeys; p++)
		{
			AnimationKey key;

			aiVectorKey& position = srcNodeAnim->mPositionKeys[p];
			aiQuatKey& rotation = srcNodeAnim->mRotationKeys[p];
			aiVectorKey& scale = srcNodeAnim->mScalingKeys[p];

			assert(position.mTime == rotation.mTime);
			assert(rotation.mTime == scale.mTime);

			key.time = position.mTime;
			key.position = Vector3(position.mValue.x, position.mValue.y, position.mValue.z);
			key.rotation = Quaternion(rotation.mValue.x, rotation.mValue.y, rotation.mValue.z, rotation.mValue.w);
			key.scale = Vector3(scale.mValue.x, scale.mValue.y, scale.mValue.z);

			animation->m_animationKeys.push_back(key);
		}

		// �ִϸ��̼ǿ� ��� ����
		for (const auto& node : owner->m_nodes)
		{
			if (node->m_name == srcNodeAnim->mNodeName.C_Str())
				animation->m_node = node;
		}
	}

	m_animations.push_back(animation);
}

shared_ptr<Node> ModelLoader::CreateNode(aiNode* srcNode, shared_ptr<Node> parent, shared_ptr<Model> owner)
{
	shared_ptr<Node> node = make_shared<Node>();
	node->m_name = srcNode->mName.C_Str();
	node->m_parent = parent;
	node->m_owner = owner;

	// Relative Transform
	Matrix transform(srcNode->mTransformation[0]);
	node->m_matLocal = transform.Transpose();	// d3d�� �� �켱(column-major), 
												// Assimp�� ����� �� �켱(row-major) ����̱⿡ ��ġ�Ѵ�.
	// �޽� ���� �� ��忡 ����
	CreateMesh(srcNode, node, owner);

	m_nodes.push_back(node);

	// ���
	for (UINT i = 0; i < srcNode->mNumChildren; i++)
		node->m_children.push_back(CreateNode(srcNode->mChildren[i], node, owner));

	return node;
}

void ModelLoader::CreateMesh(aiNode* srcNode, shared_ptr<Node> node, shared_ptr<Model> owner)
{
	if (srcNode->mNumMeshes < 1)
		return;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->m_name = srcNode->mName.C_Str();
	mesh->m_owner = owner;

	for (UINT i = 0; i < srcNode->mNumMeshes; i++)
	{
		UINT index = srcNode->mMeshes[i];
		const aiMesh* srcMesh = m_scene->mMeshes[index];

		mesh->SetMaterial(m_materials[srcMesh->mMaterialIndex]);	// TODO : �޽��� ���͸��� ����

		// ���ؽ� ���� ����
		vector<Vertex> vertices;

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			Vertex vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(srcMesh->mVertices[v]));

			// UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			// Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(srcMesh->mNormals[v]));

			// Tangent
			if (srcMesh->HasTangentsAndBitangents())
				::memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(srcMesh->mTangents[v]));

			vertices.push_back(vertex);
		}
		mesh->CreateVertexBuffer(m_device, vertices);

		// �ε��� ���� ����
		vector<WORD> indices;
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			indices.push_back(srcMesh->mFaces[f].mIndices[0]);
			indices.push_back(srcMesh->mFaces[f].mIndices[1]);
			indices.push_back(srcMesh->mFaces[f].mIndices[2]);
		}
		mesh->CreateIndexBuffer(m_device, indices);

		mesh->m_materialIndex = srcMesh->mMaterialIndex;

		//// �ε��� ���� ����
		//vector<WORD> indices;
		//for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		//{
		//	aiFace& face = srcMesh->mFaces[f];

		//	for (uint32 k = 0; i < face.mNumIndices; k++)	// ���� ��(face)�� �����ϴ� �ε����� �� ��ŭ �ε����� ���Ͽ� �����Ѵ�. ������ �ﰢ���̴� 3�� �ݺ��ϰ� �ȴ�.
		//		indices.push_back(face.mIndices[k] + startVertex);
		//}
		//mesh->CreateIndexBuffer(m_device, indices);

		//mesh->m_materialIndex = srcMesh->mMaterialIndex;

	}

	node->m_meshs.push_back(mesh);
	m_meshes.push_back(mesh);
}


void ModelLoader::CreateMaterial()
{
	aiString texturePath;
	wstring basePath = L"../Resources/";
	filesystem::path path;
	wstring finalPath;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
	}// error

	for (UINT i = 0; i < m_scene->mNumMaterials; i++)
	{
		shared_ptr<Material> material = make_shared<Material>();
		aiMaterial* srcMaterial = m_scene->mMaterials[i];

		material->name = srcMaterial->GetName().C_Str();

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_textures[Diffuse].GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_textures[Normal].GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_textures[Specular].GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_textures[Emissive].GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_textures[Opacity].GetAddressOf()));
		}

		m_materials.push_back(material);	
	}

}

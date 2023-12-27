#include "pch.h"
#include "Mesh.h"
#include "Material.h"
#include "Node.h"
#include "Animation.h"
#include "NodeAnimation.h"
#include "Model.h"
#include "ModelLoader.h"

ModelLoader::ModelLoader(ComPtr<ID3D11Device> device)
	: m_device(device)
{
	m_importer = make_shared<Assimp::Importer>();
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
		aiProcess_CalcTangentSpace // ź��Ʈ ���� ����
	);
	assert(m_scene != nullptr);

	CreateMaterial();

	if (m_scene->mAnimations != nullptr)
		CreateAnimation(m_scene->mAnimations[0]);	// �ϴ� �ִϸ��̼� �ϳ���.

	m_rootNode = CreateNode(model, m_scene->mRootNode, nullptr);

	model->m_rootNode = m_rootNode;
	model->m_meshes = m_meshes;
	model->m_materials = m_materials;
	model->m_animation = m_animation;


	m_importer->FreeScene();

	return model;
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

		material->m_name = srcMaterial->GetName().C_Str();

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_diffuseRV.GetAddressOf()));
		}


		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_normalRV.GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_specularRV.GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_emissiveRV.GetAddressOf()));
		}

		if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));
			finalPath = basePath + path.filename().wstring();
			HR_T(CreateTextureFromFile(m_device.Get(), finalPath.c_str(), material->m_opacityRV.GetAddressOf()));
		}

		m_materials.push_back(material);
	}

}

void ModelLoader::CreateAnimation(aiAnimation* srcAnim)
{
	shared_ptr<Animation> animation = make_shared<Animation>();
	animation->m_name = srcAnim->mName.C_Str();
	animation->m_duration = srcAnim->mDuration;
	animation->m_ticksPerSecond = srcAnim->mTicksPerSecond;

	for (UINT i = 0; i < srcAnim->mNumChannels; i++)
	{
		shared_ptr<NodeAnimation> nodeAnim = make_shared<NodeAnimation>();

		nodeAnim->m_name = srcAnim->mChannels[i]->mNodeName.C_Str();
		aiNodeAnim* srcNodeAnim = srcAnim->mChannels[i];
		for (UINT j = 0; j < srcAnim->mChannels[i]->mNumPositionKeys; j++)
		{
			AnimationKey key;

			aiVectorKey& position = srcNodeAnim->mPositionKeys[j];
			aiQuatKey& rotation = srcNodeAnim->mRotationKeys[j];
			aiVectorKey& scale = srcNodeAnim->mScalingKeys[j];

			assert(position.mTime == rotation.mTime);
			assert(rotation.mTime == scale.mTime);

			key.time = position.mTime;
			key.position = Vector3(position.mValue.x, position.mValue.y, position.mValue.z);
			key.rotation = Quaternion(rotation.mValue.x, rotation.mValue.y, rotation.mValue.z, rotation.mValue.w);
			key.scale = Vector3(scale.mValue.x, scale.mValue.y, scale.mValue.z);

			nodeAnim->m_animationKeys.push_back(key);
		}

		animation->m_nodeAnimations.push_back(nodeAnim);
	}

	m_animation = animation;
}

shared_ptr<Node> ModelLoader::CreateNode(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> parent)
{
	shared_ptr<Node> node = make_shared<Node>();
	node->m_name = srcNode->mName.C_Str();
	node->m_parent = parent;

	node->m_meshIndices.resize(srcNode->mNumMeshes);

	// Relative Transform
	Matrix transform(srcNode->mTransformation[0]);
	node->m_local = transform.Transpose();		// d3d �� �� �켱 (column-major),
	// Assimp�� ����� �� �켱(row-major) ����̱⿡ ��ġ�Ѵ�.

	// �޽� ����
	CreateMesh(model, srcNode, node);

	// ��忡 �ִϸ��̼� ����
	if (m_animation != nullptr)
	{
		for (auto& nodeAnim : m_animation->m_nodeAnimations)
		{
			if (nodeAnim->m_name.compare(node->m_name) == 0)
			{
				node->m_pNodeAnimation = nodeAnim;
				break;
			}
		}
	}

	//node->m_children.resize(srcNode->mNumChildren);
	for (UINT i = 0; i < srcNode->mNumChildren; i++)
		node->m_children.push_back(CreateNode(model, srcNode->mChildren[i], node));

	return node;
}

void ModelLoader::CreateMesh(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> node)
{
	if (srcNode->mNumMeshes < 1)
		return;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->m_name = srcNode->mName.C_Str();

	for (UINT i = 0; i < srcNode->mNumMeshes; i++)
	{
		UINT index = srcNode->mMeshes[i];
		node->m_meshIndices[i] = index;

		const aiMesh* srcMesh = m_scene->mMeshes[index];

		// Mesh�� Node �� WorldMatrix �� ����
		mesh->m_nodeWorld = &node->m_world;

		mesh->m_materialIndex = srcMesh->mMaterialIndex;

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

		m_meshes.push_back(mesh);
	}

}

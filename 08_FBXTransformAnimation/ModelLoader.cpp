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
		aiProcess_CalcTangentSpace // ź��Ʈ ���� ����
	);
	assert(m_scene != nullptr);

	CreateMaterial();
	ReadModel(m_scene->mRootNode, -1, -1, model);

	if (m_scene->mAnimations != nullptr)
		ReadAnimationData(m_scene->mAnimations[0]);

	model->m_root = m_root;
	model->m_nodes = m_nodes;
	model->m_meshes = m_meshes;
	model->m_materials = m_materials;
	model->m_animation = m_animation;

	m_importer->FreeScene();

	return model;
}

void ModelLoader::ReadAnimationData(aiAnimation* srcAnimation)
{
	if (!m_scene->HasAnimations())
		return;

	shared_ptr<Animation> animation = make_shared<Animation>();
	animation->name = srcAnimation->mName.C_Str();
	animation->frameRate = (float)srcAnimation->mTicksPerSecond;
	animation->frameCount = (uint32)srcAnimation->mDuration + 1;

	for (uint32 i = 0; i < srcAnimation->mNumChannels; i++)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// �ִϸ��̼� ��� ������ �Ľ�
		shared_ptr<NodeAnimation> node = ParseAnimationNode(animation, srcNode);
		animation->nodeAnimations.push_back(node);

		// �̸��� �´� ��带 ã�Ƽ� �ش� ��忡 �ִϸ��̼� ���
		for (UINT i = 0; i < m_nodes.size(); i++)
		{
			if (m_nodes[i]->GetName() == node->name)
				m_nodes[i]->SetNodeAnimation(node);
		}

		// ���� ã�� ��� �߿� ���� �� �ð����� �ִϸ��̼� �ð� ����
		animation->duration = max(animation->duration, node->animationKeys.back().time);
	}

	m_animation = animation;
}

shared_ptr<NodeAnimation> ModelLoader::ParseAnimationNode(shared_ptr<Animation> animation, aiNodeAnim* srcNode)
{
	shared_ptr<NodeAnimation> node = make_shared<NodeAnimation>();
	node->name = srcNode->mNodeName.C_Str();
	node->duration = animation->duration;
	node->frameCount = animation->frameCount;
	node->frameRate = animation->frameRate;

	uint32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);

	for (uint32 k = 0; k < keyCount; k++)
	{
		AnimationKey frameData;

		bool found = false;
		uint32 t = node->animationKeys.size();

		// Position
		if (::fabsf((float)srcNode->mPositionKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Roatation
		if (::fabsf((float)srcNode->mRotationKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = (float)key.mTime;

			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}

		// Scale
		if (::fabsf((float)srcNode->mScalingKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
			node->animationKeys.push_back(frameData);
	}

	// Keyframe �÷��ֱ�
	if (node->animationKeys.size() < animation->frameCount)
	{
		uint32 count = animation->frameCount - node->animationKeys.size();
		AnimationKey keyFrame = node->animationKeys.back();

		for (uint32 n = 0; n < count; n++)
			node->animationKeys.push_back(keyFrame);
	}

	return node;
}

void ModelLoader::ReadModel(aiNode* srcNode, int32 index, int32 parentIndex, shared_ptr<Model> owner)
{
	shared_ptr<Node> node = make_shared<Node>();
	node->m_index = index;
	node->m_parentIndex = parentIndex;
	node->m_name = srcNode->mName.C_Str();

	// Relative Transform
	Matrix transform(srcNode->mTransformation[0]);
	node->m_matLocal = transform.Transpose();	// d3d�� �� �켱(column-major), 
												// Assimp�� ����� �� �켱(row-major) ����̱⿡ ��ġ�Ѵ�.

	if (parentIndex >= 0)
		node->SetParentNode(m_nodes[parentIndex]);
	else
		m_root = node;

	node->SetMesh(CreateMesh(srcNode, owner));
	m_nodes.push_back(node);

	// ��� �Լ�
	for (UINT i = 0; i < srcNode->mNumChildren; i++)
		ReadModel(srcNode->mChildren[i], m_nodes.size(), index, owner);
}

shared_ptr<Mesh> ModelLoader::CreateMesh(aiNode* node, shared_ptr<Model> owner)
{
	if (node->mNumMeshes < 1)
		return nullptr;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->m_name = node->mName.C_Str();
	mesh->m_owner = owner;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
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

	m_meshes.push_back(mesh);

	return mesh;
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

#include "pch.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Mesh.h"
#include "Node.h"
#include "Bone.h"
#include "Material.h"
#include "Animation.h"
#include "NodeAnimation.h"

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
	CreateNode(model, m_scene->mRootNode, nullptr);

	if (m_scene->HasAnimations())
		CreateAnimation(m_scene->mAnimations[0]);

	model->m_bones = m_bones;
	model->m_nodes = m_nodes;
	model->m_meshes = m_meshes;
	model->m_materials = m_materials;
	model->m_animation = m_animation;

	m_importer->FreeScene();

	return model;
}

void ModelLoader::CreateNode(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> parent)
{
	shared_ptr<Node> node = make_shared<Node>();
	node->m_name = srcNode->mName.C_Str();

	// Relative Transform
	Matrix transform(srcNode->mTransformation[0]);
	node->m_local = transform.Transpose();		// d3d �� �� �켱 (column-major),
												// Assimp�� ����� �� �켱(row-major) ����̱⿡ ��ġ�Ѵ�.

	if (parent != nullptr)
		node->SetParentNode(parent);
	else
		model->m_rootNode = node;

	// Mesh
	CreateMesh(srcNode, node);

	// ��� �Լ�
	for (UINT i = 0; i < srcNode->mNumChildren; i++)
		CreateNode(model, srcNode->mChildren[i], node);

	m_nodes.push_back(node);
}

void ModelLoader::CreateMesh(aiNode* node, shared_ptr<Node> connectNode)
{
	if (node->mNumMeshes < 1)
		return;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		const aiMesh* srcMesh = m_scene->mMeshes[index];
		mesh->m_name = srcMesh->mName.C_Str();

		// ���ؽ� ���� ����
		vector<BoneWeightVertex> vertices;
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			BoneWeightVertex vertex;
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

		// �޽� �� ��������
		if (srcMesh->HasBones())
		{
			// �޽��� ����� ������ ó��
			UINT boneIndexCounter = 0;
			map<string, int> boneMapping;

			// �� ������ŭ �� ����
			for (UINT i = 0; i < srcMesh->mNumBones; i++)
			{
				aiBone* srcBone = srcMesh->mBones[i];
				UINT boneIndex = 0;

				// ������ ���� �ȵǾ��ִٸ�
				if (boneMapping.find(srcBone->mName.C_Str()) == boneMapping.end())
				{
					shared_ptr<Bone> bone = make_shared<Bone>();

					// Map bone name to bone Index
					boneIndex = boneIndexCounter;
					boneIndexCounter++;

					bone->name = srcBone->mName.C_Str();
					bone->numWeights = srcBone->mNumWeights;
					bone->offsetMatrix = Matrix(&srcBone->mOffsetMatrix.a1).Transpose();

					for (auto& node : m_nodes)
					{
						if (node->m_name == bone->name)
						{
							/// TODO 23.01.03 ���� �ϴ�����.

						}
					}

				}
				else
					boneIndex = boneMapping[srcBone->mName.C_Str()];
			}

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
	}

	// ��忡 �޽� ����
	connectNode->m_mesh = mesh;

	mesh->m_connectedNode = connectNode;
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
	animation->m_frameRate = static_cast<float>(srcAnim->mTicksPerSecond);
	animation->m_frameCount = static_cast<unsigned int>(srcAnim->mDuration + 1);

	// ��� ������ŭ �ִϸ��̼� ��������
	for (UINT i = 0; i < srcAnim->mNumChannels; i++)
	{
		aiNodeAnim* node = srcAnim->mChannels[i];

		// �ִϸ��̼� ��� ������ �Ľ�
		shared_ptr<NodeAnimation> nodeAnim = ParseAnimationNode(animation, node);
		animation->m_nodeAnimations.push_back(nodeAnim);

		// �̸��� �´� ��带 ã�Ƽ� �ش� ��忡 �ִϸ��̼� ���
		for (UINT i = 0; i < m_nodes.size(); i++)
		{
			if (m_nodes[i]->m_name == nodeAnim->m_name)
				m_nodes[i]->m_nodeAnim = nodeAnim;
		}
	}

	m_animation = animation;
}

shared_ptr<NodeAnimation> ModelLoader::ParseAnimationNode(shared_ptr<Animation> animation, aiNodeAnim* srcNodeAnim)
{
	shared_ptr<NodeAnimation> nodeAnim = make_shared<NodeAnimation>();
	nodeAnim->m_frameCount = animation->m_frameCount;
	nodeAnim->m_frameRate = animation->m_frameRate;
	nodeAnim->m_name = srcNodeAnim->mNodeName.C_Str();

	for (UINT i = 0; i < srcNodeAnim->mNumPositionKeys; i++)
	{
		KeyFrameData key;

		aiVectorKey& position = srcNodeAnim->mPositionKeys[i];
		aiQuatKey& rotation = srcNodeAnim->mRotationKeys[i];
		aiVectorKey& scale = srcNodeAnim->mScalingKeys[i];

		assert(position.mTime == rotation.mTime);
		assert(rotation.mTime == scale.mTime);

		key.time = position.mTime;
		key.translation = Vector3(position.mValue.x, position.mValue.y, position.mValue.z);
		key.rotation = Quaternion(rotation.mValue.x, rotation.mValue.y, rotation.mValue.z, rotation.mValue.w);
		key.scale = Vector3(scale.mValue.x, scale.mValue.y, scale.mValue.z);

		nodeAnim->m_keyFrameBox.push_back(key);
	}

	// Keyframe �÷��ֱ�
	if (nodeAnim->m_keyFrameBox.size() < animation->m_frameCount)
	{
		unsigned int count = animation->m_frameCount - nodeAnim->m_keyFrameBox.size();
		KeyFrameData key = nodeAnim->m_keyFrameBox.back();

		for (UINT i = 0; i < count; i++)
			nodeAnim->m_keyFrameBox.push_back(key);
	}

	return nodeAnim;
}

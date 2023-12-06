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
		aiProcess_ConvertToLeftHanded |	// DX용 왼손좌표계 변환
		aiProcess_Triangulate | // vertex 삼각형 으로 출력
		aiProcess_GenUVCoords |	// 텍스처 좌표 생성
		aiProcess_GenNormals |	// Normal 정보 생성
		aiProcess_CalcTangentSpace // 탄젠트 벡터 생성
	);
	assert(m_scene != nullptr);

	ReadModel(m_scene->mRootNode, -1, -1);
	CreateMaterial();
	ReadAnimationData(m_scene->mAnimations[0]);

	model->m_meshes = m_meshes;
	model->m_materials = m_materials;
	model->m_nodes = m_bones;
	model->m_animations = m_animations;

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

	map<string, shared_ptr<NodeAnimation>> cacheAnimNodes;

	for (uint32 i = 0; i < srcAnimation->mNumChannels; i++)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// 애니메이션 노드 데이터 파싱
		shared_ptr<NodeAnimation> node = ParseAnimationNode(animation, srcNode);

		// 현재 찾은 노드 중에 제일 긴 시간으로 애니메이션 시간 갱신
		animation->duration = max(animation->duration, node->animationKeys.back().time);

		animation->nodeAnimations.push_back(node);
	}

	m_animations.push_back(animation);
}

shared_ptr<NodeAnimation> ModelLoader::ParseAnimationNode(shared_ptr<Animation> animation, aiNodeAnim* srcNode)
{
	shared_ptr<NodeAnimation> node = make_shared<NodeAnimation>();
	node->name = srcNode->mNodeName;

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

	// Keyframe 늘려주기
	if (node->animationKeys.size() < animation->frameCount)
	{
		uint32 count = animation->frameCount - node->animationKeys.size();
		AnimationKey keyFrame = node->animationKeys.back();

		for (uint32 n = 0; n < count; n++)
			node->animationKeys.push_back(keyFrame);
	}

	return node;
}

void ModelLoader::ReadModel(aiNode* node, int32 index, int32 parentIndex)
{
	shared_ptr<Node> bone = make_shared<Node>();
	bone->m_index = index;
	bone->m_parentIndex = parentIndex;
	bone->m_name = node->mName.C_Str();

	// Relative Transform
	Matrix transform(node->mTransformation[0]);
	bone->m_transform = transform.Transpose();	// d3d는 열 우선(column-major), 
												// Assimp의 행렬은 행 우선(row-major) 행렬이기에 전치한다.
	// 2) Root (Local)
	Matrix matParent = Matrix::Identity;
	if (parentIndex >= 0)	// 부모행렬이 존재한다면 부모 행렬을 구한 뒤 곱해준다.
		matParent = m_bones[parentIndex]->m_transform;

	// Local (Root) Transform
	bone->m_transform = bone->m_transform * matParent;

	m_bones.push_back(bone);

	// Mesh
	CreateMesh(node);

	// 재귀 함수
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadModel(node->mChildren[i], m_bones.size(), index);
}

void ModelLoader::CreateMesh(aiNode* node)
{
	if (node->mNumMeshes < 1)
		return;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->m_name = node->mName.C_Str();

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		const aiMesh* srcMesh = m_scene->mMeshes[index];

		// 버텍스 정보 생성
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

		// 인덱스 정보 생성
		vector<WORD> indices;
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			indices.push_back(srcMesh->mFaces[f].mIndices[0]);
			indices.push_back(srcMesh->mFaces[f].mIndices[1]);
			indices.push_back(srcMesh->mFaces[f].mIndices[2]);
		}
		mesh->CreateIndexBuffer(m_device, indices);

		mesh->m_materialIndex = srcMesh->mMaterialIndex;

		//// 인덱스 정보 생성
		//vector<WORD> indices;
		//for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		//{
		//	aiFace& face = srcMesh->mFaces[f];

		//	for (uint32 k = 0; i < face.mNumIndices; k++)	// 현재 면(face)을 구성하는 인덱스의 수 만큼 인덱스를 구하여 저장한다. 보통은 삼각형이니 3번 반복하게 된다.
		//		indices.push_back(face.mIndices[k] + startVertex);
		//}
		//mesh->CreateIndexBuffer(m_device, indices);

		//mesh->m_materialIndex = srcMesh->mMaterialIndex;
	}


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

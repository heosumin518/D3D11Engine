#pragma once

/// 온갖 리소스들을 관리한다.

#include "ResourceBase.h"

class Mesh;
class Material;
class Shader;
class Animation;
class Texture;

class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> device);

	void Init();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	template<typename T>
	ResourceType GetResourceType();

private:
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();

private:
	ComPtr<ID3D11Device> _device;

	// 키 값 하나에 해당하는 리소스 목록을 가지고 있는다.
	using KeyObjMap = map<wstring/*key*/, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template <typename T>
shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	auto objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);
	// 이미 존재한다면 그것을 로드

	// 없다면 새로 로드
	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template <typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;

	return true;
}

template <typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template <typename T>
ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;

	assert(false);
	return ResourceType::None;
}

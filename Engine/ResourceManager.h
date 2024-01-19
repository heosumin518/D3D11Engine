#pragma once
#include "ResourceBase.h"

namespace Engine
{
	class AnimationResource;
	class StaticMeshResource;
	class SkeletalMeshResource;
	class Texture;
	class Material;
	//class Animation;

	class ResourceManager
	{
		DECLARE_SINGLE(ResourceManager);

	public:

		void Init();

		template <typename T>
		void Load(const wstring& path);	// Mesh, Texture, Material, Animation �� ��� ���ҽ��� �� �Լ��� �ε��Ѵ�.

		template <typename T>
		void Add(const wstring& key);	// ���ҽ��� �����ϰ� ��Ͽ� �߰��Ѵ�.

		template <typename T>
		shared_ptr<T> Get(const wstring& key);

		template<typename T>
		ResourceType GetResourceType();

	private:
		using KeyObjMap = map<wstring/*key*/, shared_ptr<ResourceBase>>;
		array<KeyObjMap, RESOURCE_TYPE_COUNT> m_resources;
	};

	template <typename T>
	void ResourceManager::Load(const wstring& path)
	{
		auto objectType = GetResourceType<T>();
		KeyObjMap& keyObjMap = m_resources[static_cast<uint8>(objectType)];

		// path�� key ������ ���ҽ��� ã�´�.
		auto findIt = keyObjMap.find(path);
		if (findIt != keyObjMap.end())
			return static_pointer_cast<T>(findIt->second);

		// ã�� ���ߴٸ� ���ҽ��� �����ϰ� ��Ͽ� �߰�.
		shared_ptr<T> object = make_shared<T>();
		object->Load(path);
		keyObjMap[path] = object;
	}

	template <typename T>
	shared_ptr<T> ResourceManager::Get(const wstring& key)
	{
		ResourceType resourceType = GetResourceType<T>();
		KeyObjMap& keyObjMap = m_resources[static_cast<uint8>(resourceType)];

		auto findIt = keyObjMap.find(key);
		if (findIt != keyObjMap.end())
			return static_pointer_cast<T>(findIt->second);

		return nullptr;
	}

	template <typename T>
	ResourceType ResourceManager::GetResourceType()
	{
		if (std::is_same_v<T, Texture>)
			return ResourceType::Texture;
		if (std::is_same_v<T, StaticMeshResource>)
			return ResourceType::StaticMesh;
		if (std::is_same_v<T, SkeletalMeshResource>)
			return ResourceType::SkeletalMesh;
		if (std::is_same_v<T, Material>)
			return ResourceType::Material;
		if (std::is_same_v<T, AnimationResource>)
			return ResourceType::Animation;

		assert(false);
		return ResourceType::None;
	}
}



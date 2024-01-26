#include "pch.h"
#include "ResourceManager.h"

#include "TextureResource.h"

shared_ptr<Engine::StaticMeshSceneResource> Engine::ResourceManager::CreateStaticMeshSceneResource(string filePath)
{

}

shared_ptr<Engine::SkeletalMeshSceneResource> Engine::ResourceManager::CreateSkeletalMeshSceneResource(string filePath)
{

}

shared_ptr<Engine::TextureResource> Engine::ResourceManager::CreateTextureResource(wstring filePath)
{
	auto it = m_materialTextureMap.find(filePath);

	if (it != m_materialTextureMap.end())
	{
		shared_ptr<TextureResource> resource = it->second.lock();

		// UseCount�� 1�̻����� �޸𸮰� ���� ����ִٸ� resource �����͸� ��ȯ
		if (resource)
		{
			return resource;
		}
		// UseCount�� 0�̸� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����ϰ�
		else
		{
			m_materialTextureMap.erase(it);
			// resource �Ʒ����� ���� ���� ��ȯ�Ѵ�.
		}
	}

	filesystem::path path = filePath;
	if (!filesystem::exists(path))
	{
		LOG_MESSAGEA("Error file not found: %s", path.string().c_str());
		return nullptr;
	}

	shared_ptr<TextureResource> texture = make_shared<TextureResource>();
	texture->Create(filePath);
	m_materialTextureMap[filePath] = texture;

	return texture;
}

shared_ptr<Engine::AnimationResource> Engine::ResourceManager::CreateAnimationResource(string filePath)
{

}

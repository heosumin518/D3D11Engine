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

		// UseCount가 1이상으로 메모리가 아직 살아있다면 resource 포인터를 반환
		if (resource)
		{
			return resource;
		}
		// UseCount가 0이면 메모리가 이미 해제되었으므로 맵에서 제거하고
		else
		{
			m_materialTextureMap.erase(it);
			// resource 아래에서 새로 만들어서 반환한다.
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

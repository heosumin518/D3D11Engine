#pragma once

namespace Engine
{
	class TextureResource;
	class AnimationResource;
	class StaticMeshSceneResource;
	class SkeletalMeshSceneResource;
	class ResourceManager
	{
		DECLARE_SINGLE(ResourceManager);

		// FBX파일 경로로 스테틱 메시,머터리얼 정보 얻어오기
		shared_ptr<StaticMeshSceneResource> CreateStaticMeshSceneResource(string filePath);

		// FBX파일 경로로 스켈레탈 메시,머터리얼 정보,기본 포즈 애니메이션 얻어오기
		shared_ptr<SkeletalMeshSceneResource> CreateSkeletalMeshSceneResource(string filePath);

		// 같은 머터리얼 내에서도 같은 텍스처를 사용할수 있으므로 공유 목록에서 파일경로 확인해서 얻어오기
		shared_ptr<TextureResource> CreateTextureResource(wstring filePath);

		// FBX파일 경로로 애니메이션 정보 얻어오기
		shared_ptr<AnimationResource> CreateAnimationResource(string filePath);

	private:
		map<string, weak_ptr<StaticMeshSceneResource>> m_staticMeshSceneMap;
		map<string, weak_ptr<SkeletalMeshSceneResource>> m_skeletalMeshSceneMap;
		map<wstring, weak_ptr<TextureResource>> m_materialTextureMap;
		map<string, weak_ptr<AnimationResource>> m_animationMap;
	};
}



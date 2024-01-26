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

		// FBX���� ��η� ����ƽ �޽�,���͸��� ���� ������
		shared_ptr<StaticMeshSceneResource> CreateStaticMeshSceneResource(string filePath);

		// FBX���� ��η� ���̷�Ż �޽�,���͸��� ����,�⺻ ���� �ִϸ��̼� ������
		shared_ptr<SkeletalMeshSceneResource> CreateSkeletalMeshSceneResource(string filePath);

		// ���� ���͸��� �������� ���� �ؽ�ó�� ����Ҽ� �����Ƿ� ���� ��Ͽ��� ���ϰ�� Ȯ���ؼ� ������
		shared_ptr<TextureResource> CreateTextureResource(wstring filePath);

		// FBX���� ��η� �ִϸ��̼� ���� ������
		shared_ptr<AnimationResource> CreateAnimationResource(string filePath);

	private:
		map<string, weak_ptr<StaticMeshSceneResource>> m_staticMeshSceneMap;
		map<string, weak_ptr<SkeletalMeshSceneResource>> m_skeletalMeshSceneMap;
		map<wstring, weak_ptr<TextureResource>> m_materialTextureMap;
		map<string, weak_ptr<AnimationResource>> m_animationMap;
	};
}



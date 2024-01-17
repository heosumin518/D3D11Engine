#pragma once
#include "pch.h"

class Model;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);	// �� ������ ���� �Ÿ� ���� �̱������� ���� �ʾƵ� �� �� �ѵ�..

public:
	void Init();

	shared_ptr<Model> LoadModel(string path);

private:
	// StaticMesh Map
	// SkeletalMesh Map
	// Animation Map
	// Material
	// Model Map

	// Skeleton	// bone ����

};


#pragma once

// static mesh
struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

// skeletal mesh
struct BoneWeightVertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;

	// �����ϴ� ���� �ε����� ������ �ִ� 128���� �ϴ� ó��.
	int blendIndices[4] = {};	// ����޴� ���� ���� �ִ� 4���� ����.

	// ���ؽ����� �����ϴ� ���� weight ��.
	// �ִ� 4���� �����ϴ� weight ���� 4������ �ְ� �ȴ�.
	float blendWeights[4] = {}; // ����ġ�� ������ 1�� �Ǿ�� �Ѵ�.

	void AddBoneData(int boneIndex, float weight)
	{
		// ��� �ϳ��� �����Ͱ� ����־�� �Ѵ�.
		assert(blendWeights[0] == 0.f || blendWeights[1] == 0.f ||
			blendWeights[2] == 0.f || blendWeights[3] == 0.f);

		for (UINT i = 0; i < 4; i++)
		{
			if (blendWeights[i] == 0.f)
			{
				blendIndices[i] = boneIndex;
				blendWeights[i] = weight;
				return;
			}
		}
	}
};
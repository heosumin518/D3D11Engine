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


struct CB_Transform
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

static_assert((sizeof(CB_Transform) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_DirectionLight
{
	Vector4 direction = { 0.f, 0.f, 1.0f, 1.0f };
	Vector4 ambient = { 0.1f, 0.1f, 0.1f, 0.1f };
	Vector4 diffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 specular = { 0.6f, 0.6f, 0.6f, 0.6f };
	Vector3 eyePos;
	float pad0;
};

static_assert((sizeof(CB_DirectionLight) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Material
{
	Vector4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specularPower = 80.f;
	bool useDiffuseMap = true;
	bool pad1[3];
	bool useNormalMap = true;
	bool pad2[3];
	bool useSpecularMap = true;
	bool pad3[3];
	bool useEmissiveMap = true;
	bool pad4[3];
	bool useOpacityMap = true;
	bool pad5[3];
	Vector2 pad6;
};

static_assert((sizeof(CB_Material) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_MatrixPalette
{
	Matrix Array[128];
};

static_assert((sizeof(CB_MatrixPalette) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");
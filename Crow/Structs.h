#pragma once

struct Vertex
{
	Vector3 position;		// ���� ��ġ ����
	Vector3 normal;
	//Color color;			// ���� ���� ���� ������ �ϸ� �̰� ��� �ؽ�ó ����Ұ�.
	// Vector2 Tex;			// ������ �� �ϸ� �̰� ����

	Vertex() = default;		// �⺻������.  ���־��Ҽ���..?

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	//Vertex(Vector3 position, Color color)
	//	: position(position), color(color) { }

	Vertex(Vector3 position, Vector3 normal)
		: position(position), normal(normal) { }

	//Vertex(Vector3 position, Vector3 normal, Color color)
	//	: position(position), normal(normal), color(color) { }
};


struct TransformData
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;

	Vector4 lightDir[2];
	Vector4 lightColor[2];
	Vector4 outputColor;
};
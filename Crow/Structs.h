#pragma once

struct Vertex
{
	Vector3 position;		// ���� ��ġ ����
	Color color;			// ���� ���� ����

	Vertex() = default;		// �⺻������.  ���־��Ҽ���..?

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	Vertex(Vector3 position, Color color)
		: position(position), color(color) { }
};


struct TransformData
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};
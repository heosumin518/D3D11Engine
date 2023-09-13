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

struct ConstantBuffer
{
	Matrix world;
	Matrix view;
	Matrix projection;
};

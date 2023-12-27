#include "pch.h"
#include "Node.h"
#include "Mesh.h"
#include "Model.h"
#include "Animation.h"
#include "NodeAnimation.h"
#include "ModelLoader.h"

Node::Node()
{
}

Node::~Node()
{
}

void Node::Update(float deltaTime)
{
	// �ִϸ��̼��� �ִٸ� �����ϰ� Local Transform ������Ʈ
	if (m_pNodeAnimation != nullptr)
	{
		Vector3 position, scale;
		Quaternion rotation;

		m_pNodeAnimation->Evaluate(deltaTime, position, rotation, scale);

		m_local = Matrix::CreateScale(scale) * \
			Matrix::CreateFromQuaternion(rotation) * \
			Matrix::CreateTranslation(position);
	}

	// �θ� �ִٸ� �θ��� ���� Ʈ�������� �����ش�.
	if (m_parent != nullptr)
		m_world = m_local * m_parent->m_world;
	else
		m_world = m_local;

	for (auto& child : m_children)
		child->Update(deltaTime);
}


#include "pch.h"
#include "Node.h"
#include "Model.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "NodeAnimation.h"

void Node::Update(float deltaTime)
{
	// �ִϸ��̼��� �ִ� ��� �ִϸ��̼� ���� �� ���� Ʈ������ ������Ʈ
	if (m_nodeAnim != nullptr)
	{
		static float currentTime = 0.f;
		currentTime += deltaTime * 3;

		if (currentTime > m_nodeAnim->m_frameCount)
			currentTime -= m_nodeAnim->m_frameCount;

	}

	for (auto& child : m_children)
	{
		if (child != nullptr)
			child->Update(deltaTime);
	}
}
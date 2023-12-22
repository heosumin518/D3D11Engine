#include "pch.h"
#include "Node.h"

#include "Animation.h"
#include "Mesh.h"
#include "Model.h"

Node::Node()
{
}

Node::~Node()
{
}


void Node::Update()
{
	Matrix matParent = Matrix::Identity;

	// �θ� ��尡 �ִٸ� �θ� ����� WorldMatrix�� ���ؼ� �ڽ��� WorldMatrix�� �����.
	if (m_parent != nullptr)
	{
		matParent = m_parent->m_matWorld;
	}
	else
	{
		matParent = m_owner->GetTransform();
	}

	m_matWorld = m_matLocal * matParent;

	for (auto& child : m_children)
		child->Update();
}

void Node::Render(ComPtr<ID3D11DeviceContext> deviceContext)
{


	for (auto& child : m_children)
		child->Render(deviceContext);
}

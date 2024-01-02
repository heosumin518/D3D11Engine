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
		currentTime += DT;

		if (currentTime > m_nodeAnim->m_frameCount)
			currentTime -= m_nodeAnim->m_frameCount;

		Vector3 position, scale;
		Quaternion rotation;

		// �ִϸ��̼� ����
		Evaluate(currentTime, position, rotation, scale);

		m_local = Matrix::CreateScale(scale) *
			Matrix::CreateFromQuaternion(rotation) *
			Matrix::CreateTranslation(position);
	}

	// �θ� ������ �ش� �θ��� Ʈ������ ���ϱ�
	if (m_parent != nullptr)
	{
		m_world = m_local * m_parent->m_world;
	}
	else
	{
		m_world = m_local;
	}

	for (auto& child : m_children)
	{
		if (child != nullptr)
			child->Update(deltaTime);
	}
}

void Node::Evaluate(float currentTime, Vector3& position, Quaternion& rotation, Vector3& scale)
{
	int frameIndex = 0;

	// ���� �ð��� ���� ����� Ű���������� ����
	for (UINT i = 0; i < m_nodeAnim->m_frameCount - 1; i++)
	{
		if (currentTime < m_nodeAnim->m_keyFrameBox[frameIndex + 1].time)
			break;

		frameIndex = i;
	}

	const KeyFrameData& currentKey = m_nodeAnim->m_keyFrameBox[frameIndex];
	const KeyFrameData& nextKey = m_nodeAnim->m_keyFrameBox[frameIndex + 1];

	// ���� Ű�����Ӱ� ���� Ű�������� �ð� ���� ���
	float deltaTime = nextKey.time - currentKey.time;

	// ���� ����Ű�����Ӱ� ���� Ű�������� �ð� ���̰� ���ٸ� 1�� ���� ( ��� ������ �ִϸ��̼� ���� ������ ���ư��� ��� �����ϱ� ���� )
	if (deltaTime == 0)
		deltaTime = 1;

	// ���� �ð��� ���� Ű�����Ӱ� ���� Ű������ ������ ���� ���. �� ���� ���������� ����ϱ� ���� ����ġ�μ� ���δ�.
	float factor = 0;
	if (currentKey.time < currentTime)
		factor = (currentTime - currentKey.time) / deltaTime;

	// ���� ����
	position = Vector3::Lerp(currentKey.translation, nextKey.translation, factor);
	rotation = Quaternion::Slerp(currentKey.rotation, nextKey.rotation, factor);
	scale = Vector3::Lerp(currentKey.scale, nextKey.scale, factor);
}

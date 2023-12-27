#include "pch.h"
#include "Animation.h"
#include "Node.h"
#include "NodeAnimation.h"
#include "ModelLoader.h"

// �ִϸ��̼� ����
void NodeAnimation::Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scale)
{
	// �ִϸ��̼� ����ð� ����
	m_duration += progressTime;

	// ���� �������ε������� ������ Ű�����ӿ� �������� ���
	if (m_frameIndex + 1 >= m_animationKeys.size())
	{
		m_frameIndex = 0;
		m_duration = 0;
	}

	const AnimationKey& currentKey = m_animationKeys[m_frameIndex];
	const AnimationKey& nextKey = m_animationKeys[m_frameIndex + 1];

	// ���� �ִϸ��̼��� ����ð��� ����Ű�������� �ð��� �ʰ��ߴٸ� Ű������ ����
	// ��, ���� �������� ����Ű�����ӿ� �����ߴ��� Ȯ��
	if (m_duration > nextKey.time)
		m_frameIndex++;

	// ���� �ð��� ���� Ű�����Ӱ� ���� Ű������ ���̿��� ��� ������ ��ġ���ִ��� ���. �� ���� ���������� ����ϱ� ���� ����ġ�μ� ���δ�.
	float factor = (m_duration - currentKey.time) / (nextKey.time - currentKey.time);

	position = Vector3::Lerp(currentKey.position, nextKey.position, factor);
	rotation = Quaternion::Slerp(currentKey.rotation, nextKey.rotation, factor);
	scale = Vector3::Lerp(currentKey.scale, currentKey.scale, factor);
}

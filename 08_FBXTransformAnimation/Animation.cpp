#include "pch.h"
#include "Animation.h"
#include "Node.h"
#include "Model.h"
#include "ModelLoader.h"

int Animation::fps = 30;

void Animation::Update(float deltaTime)
{
	m_duration += deltaTime;

	// ���� Ű������ �ε��� ���.
	m_nextKey = (m_curKey + 1) % m_animationKeys.size();

	// ���� ���� ����� �ִϸ��̼� �ð��� ���� Ű�������� �ð��� �ʰ��Ѵٸ�,
	if (m_duration > m_animationKeys[m_nextKey].time / fps)
	{
		// ���� Ű�������� ���� Ű���������� ������Ʈ
		++m_curKey %= m_animationKeys.size();

		// ���� Ű�����ӵ� ������Ʈ
		++m_nextKey %= m_animationKeys.size();

		// ���� ���� Ű�������� ó�� Ű�������̶��, ������ Ű�������� �ð���ŭ ���� ����� �ִϸ��̼� �ð��� ���δ�.
		if (m_nextKey == 0)
			m_duration -= m_animationKeys.back().time / fps;
	}

	Vector3 scale, position;
	Quaternion rotation;

	// ����
	Evaluate(scale, rotation, position);
}

void Animation::Evaluate(Vector3& scale, Quaternion& rotation, Vector3& position)
{
	// ���� Ű�����Ӱ� ���� Ű������ ������ �ð� ���� ���
	float interval = (m_animationKeys[m_nextKey].time - m_animationKeys[m_curKey].time) / fps;	// fps �� ����� �ʴ� ������ �������� ��ȯ

	// ���� Ű�����ӿ��� �󸶳� �ð��� �귶����
	float curKeyframeProgress = m_duration - m_animationKeys[m_curKey].time / fps;
	
	// ���� �ð��� ���� Ű�����Ӱ� ���� Ű������ ���� �ð� ���ݿ��� �����ϴ� ���� ( 0 ~ 1 ) // ���� �ִϸ��̼��� ���൵�� ��Ÿ���⵵��.
	float ratio = curKeyframeProgress / interval;

	// ���� ����
	scale = Vector3::Lerp(m_animationKeys[m_curKey].scale, m_animationKeys[m_nextKey].scale, ratio);
	position = Vector3::Lerp(m_animationKeys[m_curKey].position, m_animationKeys[m_nextKey].position, ratio);
	rotation = Quaternion::Slerp(m_animationKeys[m_curKey].rotation, m_animationKeys[m_nextKey].rotation, ratio);

	m_node->SetLocalTransform(Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position));
}

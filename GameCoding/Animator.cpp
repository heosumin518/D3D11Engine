#include "pch.h"
#include "Animator.h"
#include "TimeManager.h"

Animator::Animator()
	: Super(ComponentType::Animator)
{
}

Animator::~Animator()
{
}

void Animator::Init()
{

}

void Animator::Update()
{
	// ���� �����ϰ� �ִ� �ִϸ��̼��� Ȯ���Ѵ�.
	shared_ptr<Animation> animation = GetCurrentAnimation();
	if (animation == nullptr)	// ���� ���� �ִϸ��̼��� ���ٸ� ������Ʈ ���� �ʴ´�.
		return;

	const Keyframe& keyframe = animation->GetKeyframe(_currentKeyframeIndex);

	float deltaTime = TIME->GetDeltaTime();
	_sumTime += deltaTime;

	// ���� �ð��� Ű������ �ð��� �ʰ��ߴٸ� ���� ���������� �Ѿ��.
	if (_sumTime >= keyframe.time)	
	{
		_currentKeyframeIndex++;
		int32 totalCount = animation->GetKeyframeCount();

		// ������ �����ӱ��� ����������
		if (_currentKeyframeIndex >= totalCount)
		{
			if (animation->IsLoop())	// �ݺ� ����̸� ó�� ���������� ���ư�
				_currentKeyframeIndex = 0;
			else
				_currentKeyframeIndex = totalCount - 1;		// �ε����� �ִ�ġ�� �д�. -> �ִϸ��̼� ��� ��
		}

		_sumTime = 0.f;
	}
}

shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}

const Keyframe& Animator::GetCurrentKeyframe()
{
	return _currentAnimation->GetKeyframe(_currentKeyframeIndex);
}


#include "pch.h"
#include "TimeManager.h"

void Engine::TimeManager::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prevCount)); // CPU Ŭ��
}

void Engine::TimeManager::Update()
{
	uint64_t currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	m_deltaTime = (currentCount - m_prevCount) / static_cast<float>(m_frequency);
	m_prevCount = currentCount;

	m_frameCount++;
	m_frameTime += m_deltaTime;

	if (m_frameTime > 1.f)
	{
		m_fps = static_cast<uint32_t>(m_frameCount / m_frameTime);

		m_frameTime = 0.f;
		m_frameCount = 0;
	}

	// ���࿡ �ߴ����� ��� �˻��ϴ� ����� ���¶�� �ð��� �帣�� ���� �����ϱ� ���� ������ ��ó����
#ifdef _DEBUG
	if (m_deltaTime > (1. / 60.))
		m_deltaTime = (1. / 60.);
#endif
}

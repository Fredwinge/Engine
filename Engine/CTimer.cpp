#include "CTimer.h"

using namespace std::chrono;

CTimer::CTimer()
{
	m_lastTime = steady_clock::now();
	m_startTime = steady_clock::now();
}

//Get time elapsed since this function was last called
float CTimer::Mark()
{
	const auto old = m_lastTime;
	m_lastTime = steady_clock::now();

	const duration<float> frameTime = m_lastTime - old;

	return frameTime.count();
}

//Get time since you last called Mark without resetting Mark
float CTimer::Peek() const
{
	return duration<float>(steady_clock::now() - m_lastTime).count();
}

float CTimer::TimeElapsed() const
{
	return duration<float>(steady_clock::now() - m_startTime).count();
}
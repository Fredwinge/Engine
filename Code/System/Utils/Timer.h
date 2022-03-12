#pragma once
#include <chrono>

class CTimer
{
public:

	CTimer();

	float Mark();				//Get amount of time elapsed since Mark was called last
	float Peek() const;			//Get amount of time elapsed since Mark was called last
	float TimeElapsed() const; //Get amount of time elapsed since CTimer was initialized

private:

	typedef std::chrono::steady_clock::time_point TimePoint;

	TimePoint m_lastTime;	//Last time mark was called
	TimePoint m_startTime;	//Time when initialized
};
#include "GameTime.hpp"

using namespace std::chrono;

namespace cubedemo
{
	// // //
	// GameTimePoint implementation
	// // //

	GameTimePoint::GameTimePoint()
		: m_deltaTime{ 0.0f }, m_totalTime{ 0.0f }
	{

	}

	GameTimePoint::GameTimePoint(float deltaTime, float totalTime)
		: m_deltaTime{ deltaTime }, m_totalTime{ totalTime }
	{

	}

	// // //
	// GameTimer implementation
	// // //

    // Use floats to capture fractions of seconds and milliseconds too
    typedef std::chrono::duration<float, std::milli> UpdateDuration;
    typedef std::chrono::duration<float, std::ratio<1>> TotalDuration;
    
	GameTimer::GameTimer()
		: m_startTime{ steady_clock::now() },
		m_lastUpdateTime{ steady_clock::now() }
	{

	}

	GameTimePoint GameTimer::currentTime() const
	{
		auto now = steady_clock::now();
		auto delta = duration_cast<UpdateDuration>(now - m_lastUpdateTime).count();
		auto total = duration_cast<TotalDuration>(now - m_startTime).count();
		return GameTimePoint(delta, total);
	}

	GameTimePoint GameTimer::nextTime()
	{
		auto now = steady_clock::now();
        auto delta = duration_cast<UpdateDuration>(now - m_lastUpdateTime).count();
        auto total = duration_cast<TotalDuration>(now - m_startTime).count();
		m_lastUpdateTime = now;
		return GameTimePoint(delta, total);
	}
}

#include "GameTime.hpp"

namespace cubedemo
{
	GameTime::GameTime()
		: totalTime{}, deltaTime{}, point{ GameTimeClock::now() }
	{
	}

	GameTime::GameTime(const GameTimePoint& now, const GTAccumDuration& total, const GTDeltaDuration& delta)
		: totalTime{ total }, deltaTime{ delta }, point{ now }
	{
	}

	GameTime GameTime::next(const GameTime& time)
	{
		auto now = GameTimeClock::now();
		auto newDelta = now - time.point;
		return GameTime{ now, chrono::duration_cast<GTAccumDuration>(time.totalTime + newDelta), chrono::duration_cast<GTDeltaDuration>(newDelta) };
	}
}

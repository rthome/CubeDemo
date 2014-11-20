#pragma once

#include <chrono>
namespace chrono = std::chrono;

namespace cubedemo
{
	typedef chrono::steady_clock GameTimeClock;
	typedef chrono::duration<float, std::milli> GTDeltaDuration; // Updates are measured in fractions of milliseconds
	typedef chrono::duration<long long, std::milli> GTAccumDuration;// Accumulated time is measured in full milliseconds to avoid increasing rounding error over time
	typedef GameTimeClock::time_point GameTimePoint;

	struct GameTime final
	{
		GTAccumDuration totalTime; // Total runtime in full milliseconds
		GTDeltaDuration deltaTime; // Time since last update in milliseconds
		GameTimePoint point; // Time of creation

		GameTime();
		GameTime(const GameTime&) = default;
		GameTime(const GameTimePoint& point, const GTAccumDuration& total, const GTDeltaDuration& delta);

		inline GTDeltaDuration timeSince() const { return chrono::duration_cast<GTDeltaDuration>(GameTimeClock::now() - point); }
		inline GTDeltaDuration timeSince(const GameTime& time) const { return chrono::duration_cast<GTDeltaDuration>(point - time.point); }

		static GameTime next(const GameTime& time);
	};
}
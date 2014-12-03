#pragma once

#include <chrono>

namespace cubedemo
{
    class GameTimePoint final
    {
    private:
        float m_deltaTime; // Time since the previous update, in milliseconds
        float m_totalTime; // Total elapsed time, in seconds

    public:
        GameTimePoint();
        GameTimePoint(const GameTimePoint&) = default;
        GameTimePoint(float deltaTime, float totalTime);

        // Return the time since the last update, in milliseconds
        inline float delta() const { return m_deltaTime; }

        // Return the total elapsed time, in seconds
        inline float total() const { return m_totalTime; }

        // Return the time since the given GTP, in milliseconds
        inline float timeSince(const GameTimePoint& time) const { return (m_totalTime - time.m_totalTime) * 1000.0f; }
    };
    
    class GameTimer
    {
    private:
        std::chrono::steady_clock::time_point m_startTime;
        std::chrono::steady_clock::time_point m_lastUpdateTime;

    public:
        GameTimer();

        // Return the current time point, but touching the last update time
        GameTimePoint currentTime() const;

        // Return the current time point, and set this as the new "last update time"
        GameTimePoint nextTime();
    };
}